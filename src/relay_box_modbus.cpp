#include "relay_box_modbus.h"

#define RELAY_OFF HIGH
#define RELAY_ON LOW

#ifdef USE_SOFTWARE_SERIAL
#include <ModbusSerial.h>
ModbusSerial mb;

#define SSerialTxControl 6   //RS485 Direction control
#define SSerialRX        8  //Serial Receive pin
#define SSerialTX        9  //Serial Transmit pin
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX
#endif

#ifdef USE_ALT_SOFT_SERIAL

#include <ModbusSerial.h>

ModbusSerial mb;

#define SSerialTxControl 6   //RS485 Direction control
#define SSerialRX        8  //Serial Receive pin
#define SSerialTX        9  //Serial Transmit pin
AltSoftSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX
#endif

#ifdef USE_SERIAL1
#include <ModbusSerial.h>
ModbusSerial mb;

#define SSerialRX        19  //Serial3 Receive pin (just a reference, can't be changed)
#define SSerialTX        18  //Serial3 Transmit pin (just a reference, can't be changed)
#define SSerialTxControl 20   //RS485 Direction control

#define RS485Serial Serial1
#endif

#ifdef USE_ESP8266_TCP
#include <ESP8266WiFi.h>
#include <ModbusIP_ESP8266.h>
ModbusIP mb;
#endif

// Action handler. Add all your actions mapped by action_id in rs485_node of Lua script
void process_actions() {
	if (mb.Hreg(ACTIONS) == 0)
		return;

	switch (mb.Hreg(ACTIONS)) {
		case 1 : // Put here code for Reset
			Serial.println("[Reset] action fired");
			digitalWrite(LED_BUILTIN, HIGH);

			digitalWrite(OUT1, RELAY_OFF);
			digitalWrite(OUT2, RELAY_OFF);
			digitalWrite(OUT3, RELAY_OFF);
			digitalWrite(OUT4, RELAY_OFF);
			digitalWrite(OUT5, RELAY_OFF);
			digitalWrite(OUT6, RELAY_OFF);
			digitalWrite(OUT7, RELAY_OFF);
			digitalWrite(OUT8, RELAY_OFF);
			break;
		case 2 : // Put here code for Enable_top_lights1
			Serial.println("[Enable_top_lights1] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			digitalWrite(CEILING_LIGHTS1, RELAY_OFF);
			break;
		case 3 : // Put here code for Enable_top_lights2
			Serial.println("[Enable_top_lights2] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			digitalWrite(CEILING_LIGHTS1, RELAY_ON);
			break;
		case 4 : // Put here code for Disable_top_lights1
			Serial.println("[Disable_top_lights1] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			digitalWrite(CEILING_LIGHTS2, RELAY_OFF);
			break;
		case 5 : // Put here code for Disable_top_lights2
			Serial.println("[Disable_top_lights2] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			digitalWrite(CEILING_LIGHTS2, RELAY_ON);
			break;
		case 6 : // Put here code for Unlock_exit_door
			Serial.println("[Unlock_exit_door] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			digitalWrite(EXIT_DOOR, RELAY_OFF);
			break;
		case 7 : // Put here code for Lock_exit_door
			Serial.println("[Lock_exit_door] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			digitalWrite(EXIT_DOOR, RELAY_ON);
			break;
		default:
			break;
	}

	// Signal that action was processed
	mb.Hreg(ACTIONS, 0);
}

/////////////////////////////////////////////////////////////////

void modbus_setup() {
	Serial.println("ModBus Slave RELAY_BOX:4 for lua/Aliens.lua");

#ifndef USE_ESP8266_TCP
	mb.config(&RS485Serial, 57600, SSerialTxControl);
	mb.setSlaveId(4);
#else
	mb.config("Aliens", "123123");
	WiFi.config(IPAddress(4), IPAddress(), IPAddress(), IPAddress(), IPAddress());

	Serial.print("Connecting to Aliens ");
	while (WiFi.status() != WL_CONNECTED) {
	  delay(500);
	  Serial.print(".");
	}

	Serial.println(" CONNECTED!");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	Serial.print("Netmask: ");
	Serial.println(WiFi.subnetMask());

	Serial.print("Gateway: ");
	Serial.println(WiFi.gatewayIP());
#endif

	mb.addHreg(ACTIONS, 0);

	pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output (D4)
}


void modbus_loop() {
	mb.task();              // not implemented yet: mb.Hreg(TOTAL_ERRORS, mb.task());
	process_actions();
}