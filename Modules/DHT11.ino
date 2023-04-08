#include "DHT.h"
#define DHTPIN 2 // D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);  
 
void setup(){  
  Serial.begin(9600); // Set serial port speed
  dht.begin();
}

void loop(){  
  Serial.print("Temperature: ");  
  Serial.print(dht.readTemperature());
  Serial.println("°C");

  Serial.print("Humidity: ");  
  Serial.print(dht.readHumidity());
  Serial.println(" RH");

  Serial.println("----------------------");

  delay(1000);  
}  