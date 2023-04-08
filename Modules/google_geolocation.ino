#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "ESP8266WiFi.h"

// Replace with your network credentials
char myssid[] = "Your_WiFi_SSID";
char mypass[] = "Your_WiFi_Password";

//Credentials for Google GeoLocation API...
const char* Host = "www.googleapis.com";
String thisPage = "/geolocation/v1/geolocate?key=";
String key = "Your_Geolocation_API_Key";

int status = WL_IDLE_STATUS;

// Variables to store the required data
double latitude = 0.0;
double longitude = 0.0;
double accuracy = 0.0;

// Debugging Mode
int debug = 1; // set to 1 for more debug output

void setup()   {
  Serial.begin(9600); // Set serial port speed
  Serial.println("Start");

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(myssid);
  WiFi.begin(myssid, mypass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("connected");
}

void loop(){

  //Scan the nearby WiFi networks
  Serial.println("scan start");
  int n = WiFi.scanNetworks(); // It will return the number of networks found
  Serial.println("scan done");

  if(n == 0){
    Serial.println("no networks found");
  }
  else{
    Serial.print(n);
    Serial.println(" networks found...");
  }

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

  if(debug){
    Serial.println(requestBody);
  }

  //Connect to the client and make the api call
  Serial.print("Requesting URL: ");
  Serial.println("https://" + (String)Host + thisPage + key);
  Serial.println(" ");
  delay(500);

  WiFiClientSecure client;
  client.setInsecure();
  if (client.connect(Host, 443)) {
    Serial.println("Connected to port");
    client.println("POST " + thisPage + key + " HTTP/1.1");
    client.println("Host: " + (String)Host);
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
  }

  // wait for response from server
  while (!client.available());

  //Read and process line by line the reply from server
  while (client.available()) {
    String line = client.readStringUntil('\r');

    if(debug){
      Serial.print(line);
    }

    if(line.indexOf("location") != -1){

      // Allocate the memory for the document
      StaticJsonDocument<128> doc;

      // Deserialize the JSON data
      DeserializationError error = deserializeJson(doc, line);

      if(error) {
        Serial.print(F("deserializeJson() failed: "));
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

  Serial.print("Latitude = ");
  Serial.println(latitude, 8);
  Serial.print("Longitude = ");
  Serial.println(longitude, 8);
  Serial.print("Accuracy = ");
  Serial.println(accuracy, 3);

  delay(600000);
}
