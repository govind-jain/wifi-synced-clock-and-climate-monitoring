#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid =  "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

// Replace with the desired latitude and longitude
const char* latitude = "29.8543";
const char* longitude = "77.8880";

// May replace with the URL of your time API
const char* time_api_url = "api.timezonedb.com";

// Replace with your API key for the time API service
const char* api_key = "Your_API_Key";

// Debugging Mode
int debug = 0; // Set to 1 for debugging messages

void setup() {
  Serial.begin(115200); // Set serial port speed

  // We start by connecting to a Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Connected to WiFi.");
}

void loop() {
   WiFiClient client;

   // make the HTTP request to the time API
   if (client.connect(time_api_url, 80)) {
        client.print(String("GET /v2.1/get-time-zone?key=") + api_key
                        + "&format=json&by=position&lat=" + latitude
                        + "&lng=" + longitude + " HTTP/1.1\r\n"
                        + "Host: " + time_api_url + "\r\n"
                        + "Connection: close\r\n\r\n"
                );

       // wait for response from server
       while (!client.available());

       while (client.available()) {
           String line = client.readStringUntil('\r');

           if(debug){
               Serial.print(line);
           }

           line.trim();

           if(line.indexOf("formatted") != -1){
               StaticJsonDocument<512> doc;

               DeserializationError error = deserializeJson(doc, line);

               if(error) {
                   Serial.print(F("deserializeJson() failed: "));
                   Serial.println(error.f_str());
                   return;
               }

               const char* formattedTime = doc["formatted"]; // "2023-04-08 19:54:11"
               Serial.print("Formatted Time: ");
               Serial.println(formattedTime);
           }
       }
   }
   else {
       Serial.println("Failed to connect to time API.");
   }

    delay(600000);
}
