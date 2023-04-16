
# Wi-Fi Synced Clock and Climate Monitoring

In this project, we will periodically collect the temprature and humidity for the nearby location of the device via local sensors as well as via API calls to Weather servers. We will send these data to an IoT cloud platform and plot the data. We will also process the data there to perform the desired actions.


## Hardware's and API's used

- **ESP8266 NodeMCU Board:** Main microcontroller unit of the project.

- **Google Geolocation API:** For fetching the current *'Location'* of the device.

- **DHT11 Sensor:** For fetching the local *'Temprature'* and *'Humidity'*.

- **[OpenWeatherMap API](https://openweathermap.org/api):** Fetching 'Temprature' and 'Humidity' from server.

- **[TimeZoneDB API](https://timezonedb.com/):** Fetching *'Time'* for the current location from server.

- **DS1307 RTC Module:** Keep track of *'Correct Time'* and fetch it when required.


## Libraries used

 - [For Uploading Data From ESP8266 to Ubidots](https://github.com/ubidots/ubidots-esp8266)
## Relevant Links

 - [Logic Behind Working of Google Geolocation](https://www.youtube.com/watch?v=Ymd71ML41vo)
 - [Steps to Get Google Geolocation API Key](https://www.getambee.com/blogs/lat-long-without-gps-hardware-sounds-interesting)
 - [How to Use an I2C LCD Display With ESP8266](https://lastminuteengineers.com/esp8266-i2c-lcd-tutorial/)
 - [ESP8266 and DS1307 Based Real Time Clock (RTC)](https://how2electronics.com/esp8266-ds3231-real-time-clock/)
