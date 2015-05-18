/*
 * Beacon
 * (c) Copyright 2015 Atsushi Yokoyama, Firmlogics. All rights reserved.
 */

#include <Wire.h>
#include <math.h>
#include "Barometer.h"
#include "HTU21D.h"

enum {
	TIME_STEP = 10,			// Wakeup Period (ms)
	LED_PERIOD = 4000,		// LED Blinking Period (ms)

	LED = 13,			// LED Pin#

};

static unsigned long next_wakeup;
static int debug = 0;
Barometer myBarometer;
HTU21D myHumid;

// the setup routine runs once when you press reset:
void setup() {                
	// initialize the digital pin as an output.
	pinMode(LED, OUTPUT);
	Serial.begin(9600);  
	Wire.begin();
	myBarometer.init();
	myHumid.setResolution(1 << 7 | 1 << 0);
}

/*
 * Function to wait for the next period
 */
void wait(void) {
	while ((long) next_wakeup - (long) millis() > 0)
		;

	next_wakeup += TIME_STEP;
}

void loop() {
	int ct = 0;
	int led_ct = 0;
	boolean led_status = 0;
	char msb, lsb;
	float temp;
	float pres;
	float pres_sea;
	float temp2;
	float humid;

	next_wakeup = millis() + TIME_STEP;

	/*
	 * Putting a loop to remove dependency on Arduino framework
	 */
	for (;;) {
		if (++ led_ct >= LED_PERIOD / 2 / TIME_STEP) {
			// Serial.println(ct);
			ct ++;
			led_status = 1 - led_status;
			digitalWrite(LED, led_status);
			led_ct = 0;

#if 1
			temp = myBarometer.bmp085GetTemperature(
				myBarometer.bmp085ReadUT());
			pres = myBarometer.bmp085GetPressure(
				myBarometer.bmp085ReadUP()) / 100.0;
			pres_sea = pres / pow(1.0 - 92.0/44330, 5.255);
			
			Serial.println(temp, 1);
			Serial.println(pres, 1);
			Serial.println(pres_sea, 1);
#endif
			temp2 = myHumid.readTemperature();
			humid = myHumid.readHumidity();
			Serial.println(temp2, 1);
			Serial.println(humid, 1);
		}

		wait();
	}
}
