#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid =  "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

// Replace with your OpenWeatherMap API key
const char* apiKey = "Your_API_Key";

// Replace with the desired latitude and longitude
const char* lat = "29.8543";
const char* lon = "77.8880";

// OpenWeatherMap server URL
const char* server = "api.openweathermap.org";

// Debugging Mode
int debug = 0; // Set to 1 for debugging messages

void setup() {
    Serial.begin(115200); // Set serial port speed
    Serial.println();
    delay(10);

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

    // Make a HTTP request to the OpenWeatherMap API
    if (client.connect(server, 80)) {
        String url = "/data/2.5/weather?lat=";
        url += lat;
        url += "&lon=";
        url += lon;
        url += "&units=metric&appid=";
        url += apiKey;

        Serial.print("Requesting URL: ");
        Serial.println(url);

        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + server + "\r\n" +
                     "Connection: close\r\n\r\n");
        delay(500);

        StaticJsonDocument<1024> doc;

        while(client.available()) {
            String line = client.readStringUntil('\r');

            if(debug){
                Serial.print(line);
            }

            line.trim();

            if (line.startsWith("{\"coord\":")){
                DeserializationError error = deserializeJson(doc, line);

                if(error) {
                    Serial.println(F("Failed to parse response"));
                    return;
                }

                break;
            }
        }

        // Extract the temperature and humidity values from the JSON response
        float temperature = doc["main"]["temp"];
        float humidity = doc["main"]["humidity"];

        // Print the temperature and humidity values to the serial monitor
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");

        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" RH");

        Serial.println("----------------------");
    }
    else {
        Serial.println("Connection failed");
    }

    // Wait for 60 seconds before making another request
    delay(60000);
}
