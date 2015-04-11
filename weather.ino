/*
 * Beacon
 * (c) Copyright 2015 Atsushi Yokoyama, Firmlogics. All rights reserved.
 */

enum {
	TIME_STEP = 10,			// Wakeup Period (ms)
	LED_PERIOD = 4000,		// LED Blinking Period (ms)

	LED = 13,			// LED Pin#

};

static unsigned long next_wakeup;
static int debug = 0;

// the setup routine runs once when you press reset:
void setup() {                
	// initialize the digital pin as an output.
	pinMode(LED, OUTPUT);
	Serial.begin(9600);  
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

	next_wakeup = millis() + TIME_STEP;

	/*
	 * Putting a loop to remove dependency on Arduino framework
	 */
	for (;;) {
		if (++ led_ct >= LED_PERIOD / 2 / TIME_STEP) {
			Serial.println(ct);
			ct ++;
			led_status = 1 - led_status;
			digitalWrite(LED, led_status);
			led_ct = 0;
		}

		wait();
	}
}
