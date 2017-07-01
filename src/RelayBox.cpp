#include <Arduino.h>
#include "relay_box_modbus.h"

// #define MY_TEST_MODE

void setup() {
	Serial.begin(115200);
	modbus_setup();

	pinMode(OUT1, OUTPUT);
	pinMode(OUT2, OUTPUT);
	pinMode(OUT3, OUTPUT);
	pinMode(OUT4, OUTPUT);
	pinMode(OUT5, OUTPUT);
	pinMode(OUT6, OUTPUT);
	pinMode(OUT7, OUTPUT);
	pinMode(OUT8, OUTPUT);

	digitalWrite(OUT1, HIGH);
	digitalWrite(OUT2, HIGH);
	digitalWrite(OUT3, HIGH);
	digitalWrite(OUT4, HIGH);
	digitalWrite(OUT5, HIGH);
	digitalWrite(OUT6, HIGH);
	digitalWrite(OUT7, HIGH);
	digitalWrite(OUT8, HIGH);
}

void loop() {
	modbus_loop();

#ifdef MY_TEST_MODE
	digitalWrite(OUT1, HIGH);
	delay(500);
	digitalWrite(OUT2, HIGH);
	delay(500);
	digitalWrite(OUT3, HIGH);
	delay(500);
	digitalWrite(OUT4, HIGH);
	delay(500);
	digitalWrite(OUT5, HIGH);
	delay(500);
	digitalWrite(OUT6, HIGH);
	delay(500);
	digitalWrite(OUT7, HIGH);
	delay(500);
	digitalWrite(OUT8, HIGH);
	delay(500);

	digitalWrite(OUT1, LOW);
	delay(500);
	digitalWrite(OUT2, LOW);
	delay(500);
	digitalWrite(OUT3, LOW);
	delay(500);
	digitalWrite(OUT4, LOW);
	delay(500);
	digitalWrite(OUT5, LOW);
	delay(500);
	digitalWrite(OUT6, LOW);
	delay(500);
	digitalWrite(OUT7, LOW);
	delay(500);
	digitalWrite(OUT8, LOW);
	delay(500);
#endif
}
