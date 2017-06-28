# arduino_weather
Wireless (XBee) "Stevenson screen" software for Arduino

This software runs on Arduino Fio and XBee to capture temperature, humidity and also barometer (air pressure) periodically.

## Hardware Requirements

- 2 XBee modules (Digi XBee ZigBee)
- Arduino Fio (the model I used looks discontinued)
- Grove Temperature and Humidity Sensor Pro (SEEED-SEN51035P), other DHT-22 or RHT03 module may be used alternatively
- HTU21D Humidity Sensor (SEN-12064), unfortunately discontinued product
- SparkFun Barometric Pressure Sensor Breakout - BMP180 (SEN-11824)

You may omit some sensors by modifying code.

My Arduino Fio is retired version.  Please refer https://www.arduino.cc/en/Main/ArduinoBoardFio .

## Software (Library) Requirement

- Arduino-DHT22 library.  Downloadable at https://github.com/nethoncho/Arduino-DHT22 .

- HTU21D library.  Downloadable at https://github.com/sparkfun/SparkFun_HTU21D_Breakout_Arduino_Library

- Barometer Sensor library.  Downloadable at https://github.com/Seeed-Studio/Grove_Barometer_Sensor .

## XBee Configurations

Sample XBee configurations are in the xbee directory.
