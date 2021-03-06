#include "relay_box_modbus.h"

#define RELAY_OFF HIGH
#define RELAY_ON LOW

#ifdef USE_SOFTWARE_SERIAL
#include <ModbusSerial.h>
ModbusSerial mb;

#define SSerialGND       10
#define SSerialRX        9  //Serial Receive pin
#define SSerialTX        8  //Serial Transmit pin
#define SSerialVCC       7
#define SSerialTxControl 6   //RS485 Direction control
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

Atm_timer smoke_timer;

// Action handler. Add all your actions mapped by action_id in rs485_node of Lua script
void process_actions() {
	if (mb.Hreg(ACTIONS) == 0)
		return;

	switch (mb.Hreg(ACTIONS)) {
		case 1 : // Put here code for Reset
			Serial.println("[Reset] action fired");
			digitalWrite(LED_BUILTIN, HIGH);

			ceiling_lights1.off();
			ceiling_lights2.off();
			exit_door.off();
			smoke_machine.off();

			digitalWrite(OUT5, RELAY_OFF);
			digitalWrite(OUT6, RELAY_OFF);
			digitalWrite(OUT7, RELAY_OFF);
			digitalWrite(OUT8, RELAY_OFF);
			break;
		case 2 : // Put here code for Enable_top_lights1
			Serial.println("[Enable_top_lights1] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			ceiling_lights1.on();
			break;
		case 3 : // Put here code for Enable_top_lights2
			Serial.println("[Enable_top_lights2] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			ceiling_lights2.on();
			break;
		case 4 : // Put here code for Disable_top_lights1
			Serial.println("[Disable_top_lights1] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			ceiling_lights1.off();
			break;
		case 5 : // Put here code for Disable_top_lights2
			Serial.println("[Disable_top_lights2] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			ceiling_lights2.off();
			break;
		case 6 : // Put here code for Unlock_exit_door
			Serial.println("[Unlock_exit_door] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			exit_door.off();
			break;
		case 7 : // Put here code for Lock_exit_door
			Serial.println("[Lock_exit_door] action fired");
			digitalWrite(LED_BUILTIN, LOW);
			exit_door.on();
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

#ifdef EMULATE_RS3485_POWER_PINS
	pinMode(SSerialVCC, OUTPUT);
	digitalWrite(SSerialVCC, HIGH);
	pinMode(SSerialGND, OUTPUT);
	digitalWrite(SSerialGND, LOW);
	delay(10);
#endif

#ifndef USE_ESP8266_TCP
	mb.config(&RS485Serial, 31250, SSerialTxControl);
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
