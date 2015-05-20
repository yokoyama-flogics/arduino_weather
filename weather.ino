#define USE_XBEE

#include <avr/sleep.h>
#include <DHT22.h>
#include <Wire.h>
#include <math.h>
#include "Barometer.h"
#include "HTU21D.h"

Barometer myBarometer;
HTU21D myHumid;

extern volatile unsigned long timer0_millis;

#define LED_PIN		13
#define XBEE_CTS_PIN	4

void setup(void)
{
	// start serial port
	Serial.begin(9600);

	// CTS may be de-asserted
	//Serial.println("# DHT22 Application");

	pinMode(LED_PIN, OUTPUT);
	pinMode(XBEE_CTS_PIN, INPUT);

	Wire.begin();
	myBarometer.init();
	myHumid.setResolution(1 << 7 | 1 << 0);

	delay(3000);		// To ensure DHT22 is correctly initialized
}

SIGNAL(PCINT2_vect)
{
	sleep_disable();

	// Resume to the main loop.
}

void loop(void)
{
	int i;
	int cts;
	float temp;
	float pres;
	float pres_sea;
	float temp2;
	float humid;

#ifdef USE_XBEE
	/*
	 * Enable Pin Change Interrupts
	 */
	PCICR = 0;		// Disable all Pin Change Interrupts
	PCMSK0 = 0;
	PCMSK1 = 0;
	PCMSK2 = 1 << (20 - 16);
	PCICR = 1 << 2;		// Enable PCINT2 only

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
#endif

	/*
	 * Enter main loop
	 */
	for (;;) {
#ifdef USE_XBEE
		sleep_enable();
		sleep_cpu();			// NOT sleep_mode();
#else
		delay(2000);
#endif

		/*
		 * Quick hack to cheat millis().  With out this, DHT22 library
		 * says it is called to quickly.
		 */
#ifdef USE_XBEE
		timer0_millis += 60 * 1000;	// Quick hack to cheat millis()

		if ((cts = digitalRead(XBEE_CTS_PIN)) == 1) {
			// To ensure CTS status becomes stable
			delay(10);
			continue;
		};
#endif

		digitalWrite(LED_PIN, HIGH);

		loop_dht22();
		delay(50);		// testing to avoild mojibake

		temp = myBarometer.bmp085GetTemperature(
			myBarometer.bmp085ReadUT());
		pres = myBarometer.bmp085GetPressure(
			myBarometer.bmp085ReadUP()) / 100.0;
		pres_sea = pres / pow(1.0 - 92.0/44330, 5.255);
			
		Serial.print(temp, 1);
		Serial.print(" C, ");
		Serial.print(pres, 1);
		Serial.print(" hPa, ");
		Serial.print(pres_sea, 1);
		Serial.println(" hPa_sea BMP180");

		temp2 = myHumid.readTemperature();
		humid = myHumid.readHumidity();
		Serial.print(temp2, 1);
		Serial.print(" C, ");
		Serial.print(humid, 1);
		Serial.println(" %  HTU21D");

		digitalWrite(LED_PIN, LOW);

#ifdef USE_XBEE
		while ((cts = digitalRead(XBEE_CTS_PIN)) == 0) {
			// To ensure serial data is sent
			delay(1);
		}
#endif
	}
}
