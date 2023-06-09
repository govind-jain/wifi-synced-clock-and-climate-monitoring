
# Wi-Fi Synced Clock and Climate Monitoring

- The project uses an ESP8266 NodeMCU board to periodically collect the internal and external weather for the nearby device location via local sensors and API calls to Weather servers respectively.

- We will send these data to an IoT cloud platform, plot the data, and process the data there to perform the desired actions (such as ringing an alarm in case internal and external temperature difference is higher than a threshold).

- Getting the current location (required to determine current weather) is implemented without using a GPS sensor. Instead, we have used Google Geolocation API. This saved the cost, power, and physical complexity of the project.

## Architecture and Connections

![Architecture](https://user-images.githubusercontent.com/106254063/232279503-4e8e1df1-c47c-45db-acc8-03b695ab1f7b.png)

![Circuit Connections](https://user-images.githubusercontent.com/106254063/232307870-eef04f9d-8323-4ddc-af51-e16b45ca36c9.png)

## IoT Dashboard Plots
The fluctuations in local climate was intentionally generated by blowing wind to the DHT11 sensor.

![Temperature](https://user-images.githubusercontent.com/106254063/232279872-c907f088-78df-4be6-9d9a-7d16e10b26a9.png)

![Humidity](https://user-images.githubusercontent.com/106254063/232279874-5f224a34-c7e9-4129-97b4-e001dd6055ab.png)

## Hardware's and API's used

- **ESP8266 NodeMCU Board:** Main microcontroller unit of the project.

- **Google Geolocation API:** For fetching the current *'Location'* of the device.

- **DHT11 Sensor:** For fetching the local *'Temperature'* and *'Humidity'*.

- **[OpenWeatherMap API](https://openweathermap.org/api):** Fetching 'Temperature' and 'Humidity' from server.

- **[TimeZoneDB API](https://timezonedb.com/):** Fetching *'Time'* for the current location from server.

- **DS1307 RTC Module:** Keep track of *'Correct Time'* and fetch it when required.

- **RG1602A I2C LCD Screen:** To display data locally.

## Libraries used
Prefer installing from library manager first. In case you cannot install, or if not working then use the links.

 - [For Uploading Data From ESP8266 to Ubidots](https://github.com/ubidots/ubidots-esp8266)
 - [LiquidCrystal_I2C-1.1.2](https://downloads.arduino.cc/libraries/github.com/marcoschwartz/LiquidCrystal_I2C-1.1.2.zip)
 - [RTC Library AdaFruit](https://github.com/adafruit/RTClib)
 - [RTC Library](http://www.mediafire.com/download/w2vnjcmiv377hpj/RTClib.rar)
 - [ArduinoJSON V6](https://arduinojson.org/v6/doc/installation/)
 - DHTLib
 - Wire

## Relevant Links

 - [Logic Behind Working of Google Geolocation](https://www.youtube.com/watch?v=Ymd71ML41vo)
 - [Steps to Get Google Geolocation API Key](https://www.getambee.com/blogs/lat-long-without-gps-hardware-sounds-interesting)
 - [How to Use an I2C LCD Display With ESP8266](https://lastminuteengineers.com/esp8266-i2c-lcd-tutorial/)
 - [ESP8266 and DS1307 Based Real Time Clock (RTC)](https://how2electronics.com/esp8266-ds3231-real-time-clock/)
 - [Interface DHT11 (Humidity Sensor) Using NodeMCU](https://www.instructables.com/Interface-DHT11-Humidity-Sensor-Using-NodeMCU/)
