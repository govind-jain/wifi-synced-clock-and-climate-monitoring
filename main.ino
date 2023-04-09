#include "Ubidots.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "ESP8266WiFi.h"

// API Keys and Passwords
const char* UBIDOTS_TOKEN = "";  // Put here your Ubidots TOKEN
String GEOLOCATION_API_KEY = "";
const char* TIMEZONEDB_API_KEY = "";
const char* OPENWEATHERMAP_API_KEY = "Your_API_Key";
const char* WIFI_SSID = "";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "";      // Put here your Wi-Fi password

// Variables and Constants
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);
float temperatureLocal = 0.0;
float temperatureServer = 0.0;
float humidityLocal = 0.0;
float humidityServer = 0.0;
double latitude = 0.0;
double longitude = 0.0;
double accuracy = 0.0;

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
  Serial.print("Requesting URL: ");
  Serial.println("https://" + (String)Geolocation_Host + requestPage + GEOLOCATION_API_KEY);
  Serial.println(" ");
  delay(500);

  WiFiClientSecure client;
  client.setInsecure();
  if (client.connect(Geolocation_Host, 443)) {
    Serial.println("Connected to port");
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
    Serial.println("Connection failed to port");
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

  Serial.println("closing connection");
  Serial.println();
  client.stop();
}

void fetch_weather_local(){

}

void fetch_weather_server(){

}

void adjust_time(){

}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  ubidots.setDebug(true);  // Uncomment this line for printing debug messages
}

void loop() {

  

  delay(5000);
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
