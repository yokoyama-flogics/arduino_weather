#include <avr/sleep.h>
#include <DHT22.h>

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

	/*
	 * Enable Pin Change Interrupts
	 */
	PCICR = 0;		// Disable all Pin Change Interrupts
	PCMSK0 = 0;
	PCMSK1 = 0;
	PCMSK2 = 1 << (20 - 16);
	PCICR = 1 << 2;		// Enable PCINT2 only

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	/*
	 * Enter main loop
	 */
	for (;;) {
		sleep_enable();
		sleep_cpu();			// NOT sleep_mode();

		/*
		 * Quick hack to cheat millis().  With out this, DHT22 library
		 * says it is called to quickly.
		 */
		timer0_millis += 60 * 1000;	// Quick hack to cheat millis()

		if ((cts = digitalRead(XBEE_CTS_PIN)) == 1) {
			// To ensure CTS status becomes stable
			delay(10);
			continue;
		};

		digitalWrite(LED_PIN, HIGH);
		loop_dht22();
		digitalWrite(LED_PIN, LOW);

		while ((cts = digitalRead(XBEE_CTS_PIN)) == 0) {
			// To ensure serial data is sent
			delay(1);
		}
	}
}
