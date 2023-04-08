#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"

// Use RTC_DS3231 if you are using it want to print temperature as well
RTC_DS1307 RTC;  // Setting up an instance of DS1307 naming it RTC
 
void setup() {
  Serial.begin(57600); // Set serial port speed
  RTC.begin();  // Initialize RTC
  RTC.adjust(DateTime(__DATE__, __TIME__));  // Time and date is expanded to date and time on your computer at compile time
  Serial.println("Time and date is set on Real Time Clock");
  delay(3000);
}
 
void loop() {
  DateTime now = RTC.now();

  Serial.print("Date: ");
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.println(now.year(), DEC);

  Serial.print("Time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);

  Serial.print("Temperature: ");
  Serial.print(RTC.getTemperature());
  Serial.println(" °C");

  Serial.println("------------------");

  delay(1000);
}