#include <DHT22.h>

// Data wire is plugged into port 7 on the Arduino
// Connect a 4.7K resistor between VCC and the data pin (strong pullup)
#define DHT22_PIN 2

// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);

void loop_dht22(void)
{ 
  DHT22_ERROR_t errorCode;
  
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      Serial.print(myDHT22.getTemperatureC());
      Serial.print(" C, ");
      Serial.print(myDHT22.getHumidity());
      Serial.println(" % DHT22");
      break;
    case DHT_ERROR_CHECKSUM:
      Serial.print("# check sum error ");
      Serial.print(myDHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(myDHT22.getHumidity());
      Serial.println("% DHT22");
      break;
    case DHT_BUS_HUNG:
      Serial.println("# BUS Hung ");
      break;
    case DHT_ERROR_NOT_PRESENT:
      Serial.println("# Not Present ");
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      Serial.println("# ACK time out ");
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      Serial.println("# Sync Timeout ");
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      Serial.println("# Data Timeout ");
      break;
    case DHT_ERROR_TOOQUICK:
      Serial.println("# Polled to quick ");
      break;
  }
}
