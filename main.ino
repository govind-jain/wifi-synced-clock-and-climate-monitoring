#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include "ESP8266WiFi.h"
#include "Ubidots.h"
#include "DHT.h"
#include "RTClib.h"

// API Keys and Passwords
const char* UBIDOTS_TOKEN = "";  // Put here your Ubidots TOKEN
String GEOLOCATION_API_KEY = "";
const char* TIMEZONEDB_API_KEY = "";
const char* OPEN_WEATHER_MAP_API_KEY = "";
const char* WIFI_SSID = "";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "";      // Put here your Wi-Fi password

// Variables and Constants
int DHT_PIN = 2; // PIN D4
DHT dht(DHT_PIN, DHT11);

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

RTC_DS1307 RTC;  // Setting up an instance of DS1307 naming it RTC

float temperatureLocal = 0.0;
float temperatureServer = 0.0;
float humidityLocal = 0.0;
float humidityServer = 0.0;
double latitude = 0.0;
double longitude = 0.0;
double accuracy = 0.0;
String timeRTC = "";

/****************************************
 * Auxiliar Functions
 ****************************************/

void fetch_location_server(){

  const char* Geolocation_Host = "www.googleapis.com";
  String requestPage = "/geolocation/v1/geolocate?key=";

  //Scan the nearby WiFi networks
  Serial.println("WiFi scan start");
  int n = WiFi.scanNetworks(); // It will return the number of networks found
  Serial.println("WiFi scan done");

  // Build the request body in the required format
  String requestBody = "{\n";
  requestBody += "\"wifiAccessPoints\": [\n";

  for(int j = 0; j < n; ++j){
    requestBody += "{\n";
    requestBody += "\"macAddress\" : \"";
    requestBody += (WiFi.BSSIDstr(j));
    requestBody += "\",\n";
    requestBody += "\"signalStrength\": ";
    requestBody += WiFi.RSSI(j);
    requestBody += "\n";

    if (j < n - 1){
      requestBody += "},\n";
    }
    else{
      requestBody += "}\n";
    }
  }

  requestBody += ("]\n");
  requestBody += ("}\n");

  //Connect to the client and make the api call
  Serial.print("Requesting for Location to URL: ");
  Serial.println("https://" + (String)Geolocation_Host + requestPage + GEOLOCATION_API_KEY);
  Serial.println(" ");
  delay(500);

  WiFiClientSecure client;
  client.setInsecure();
  if (client.connect(Geolocation_Host, 443)) {
    Serial.println("Connected to Geolocation port");
    client.println("POST " + requestPage + GEOLOCATION_API_KEY + " HTTP/1.1");
    client.println("Host: " + (String)Geolocation_Host);
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(requestBody.length());
    client.println();
    client.print(requestBody);
    delay(500);
  }
  else{
    Serial.println("Connection failed to Geolocation port");
    return;
  }

  // wait for response from server
  while (!client.available());

  //Read and process line by line the reply from server
  while (client.available()) {
    String line = client.readStringUntil('\r');

    if(line.indexOf("location") != -1){

      // Allocate the memory for the document
      StaticJsonDocument<128> doc;

      // Deserialize the JSON data
      DeserializationError error = deserializeJson(doc, line);

      if(error) {
        Serial.print(F("deserializeJson() for Geolocation failed: "));
        Serial.println(error.f_str());
      }

      // Extract the required data
      latitude = doc["location"]["lat"];
      longitude = doc["location"]["lng"];
      accuracy = doc["accuracy"];

      break;
    }
  }

  Serial.println("closing connection with Geolocation Server");
  Serial.println();
  client.stop();
}

void fetch_weather_server(){

  const char* weatherServer = "api.openweathermap.org";

  WiFiClient client;

  // Make a HTTP request to the OpenWeatherMap API
  if (client.connect(weatherServer, 80)) {
      String url = "/data/2.5/weather?lat=";
      url += latitude;
      url += "&lon=";
      url += longitude;
      url += "&units=metric&appid=";
      url += OPEN_WEATHER_MAP_API_KEY;

      Serial.print("Requesting for Weather to URL: ");
      Serial.println(url);

      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                    "Host: " + weatherServer + "\r\n" +
                    "Connection: close\r\n\r\n");
      delay(500);

      StaticJsonDocument<1024> doc;

      while(client.available()) {
          String line = client.readStringUntil('\r');
          line.trim();

          if (line.startsWith("{\"coord\":")){
              DeserializationError error = deserializeJson(doc, line);

              if(error) {
                  Serial.println(F("Failed to parse response from Weather Server"));
                  return;
              }

              break;
          }
      }

      // Extract the temperature and humidity values from the JSON response
      temperatureServer = doc["main"]["temp"];
      humidityServer = doc["main"]["humidity"];
  }
  else {
      Serial.println("Connection failed to Weather Server");
  }

}

void fetch_weather_local(){
  temperatureLocal = dht.readTemperature();
  humidityLocal = dht.readHumidity();
}

void fetch_time_server(){

  const char* time_api_url = "api.timezonedb.com";

  WiFiClient client;

  // Make the HTTP request to the time API
  if (client.connect(time_api_url, 80)) {
    client.print(String("GET /v2.1/get-time-zone?key=") + TIMEZONEDB_API_KEY
                  + "&format=json&by=position&lat=" + latitude
                  + "&lng=" + longitude
                  + " HTTP/1.1\r\n"
                  + "Host: " + time_api_url + "\r\n"
                  + "Connection: close\r\n\r\n"
    );

    // wait for response from server
    while (!client.available());

    while (client.available()) {
      String line = client.readStringUntil('\r');
      line.trim();

      if(line.indexOf("formatted") != -1){
        StaticJsonDocument<512> doc;

        DeserializationError error = deserializeJson(doc, line);

        if(error) {
          Serial.print(F("deserializeJson() failed while fetching time: "));
          Serial.println(error.f_str());
          return;
        }

        const char* iso8601dateTime = doc["formatted"]; // "2023-04-08 19:54:11"
        RTC.adjust(DateTime(iso8601dateTime));
        Serial.println("Time and Date is set on Real Time Clock");
      }
    }
  }
  else {
    Serial.println("Failed to connect to time API.");
  }
}

void fetch_time_local(){
  
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  dht.begin();
  RTC.begin();  // Initialize RTC
}

void loop() {
  fetch_location_server();

  Serial.print("Latitude = ");
  Serial.println(latitude, 8);
  Serial.print("Longitude = ");
  Serial.println(longitude, 8);
  Serial.print("Accuracy = ");
  Serial.println(accuracy, 3);

  fetch_weather_server();

  Serial.print("Temperature Server: ");
  Serial.print(temperatureServer);
  Serial.println(" °C");

  Serial.print("Humidity Server: ");
  Serial.print(humidityServer);
  Serial.println(" RH");

  fetch_weather_local();

  Serial.print("Temperature Local: ");
  Serial.print(temperatureLocal);
  Serial.println(" °C");

  Serial.print("Humidity Local: ");
  Serial.print(humidityLocal);
  Serial.println(" RH");

  fetch_time_server(); // Fetch time from server and adjust this time on RTC
  fetch_time_local(); // Fetch time from RTC

  delay(200000);
}

// double value1 = random(0, 9) * 10;
// double value2 = random(0, 9) * 100;
// double value3 = random(0, 9) * 1000;
// ubidots.add("Variable_Name_One", value1);  // Change for your variable name
// ubidots.add("Variable_Name_Two", value2);
// ubidots.add("Variable_Name_Three", value3);

// bool bufferSent = false;
// bufferSent = ubidots.send("node_mcu");  // Will send data to a device label that matches the device Id

// if (bufferSent) {
//   // Do something if values were sent properly
//   Serial.println("Values sent by the device");
// }

// const char* DEVICE_LABEL_TO_RETRIEVE_VALUES_FROM = "nodemcu";  // Replace with your device label
// const char* VARIABLE_LABEL_TO_RETRIEVE_VALUES_FROM = "LED";       // Replace with your variable label
// int value = ubidots.get(DEVICE_LABEL_TO_RETRIEVE_VALUES_FROM, VARIABLE_LABEL_TO_RETRIEVE_VALUES_FROM); //To Read Values from the Control Variable
