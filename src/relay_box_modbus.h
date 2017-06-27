#ifndef RELAY_BOX_MODBUS_H
#define RELAY_BOX_MODBUS_H

#define USE_HOLDING_REGISTERS_ONLY
#include <Arduino.h>
#include <Modbus.h>

//////////////// registers of RELAY_BOX ///////////////////
enum
{
  // The first register starts at address 0
  ACTIONS,      // Always present, used for incoming actions

  // Any registered events, denoted by 'triggered_by_register' in rs485_node of Lua script, 1 and up
  
  TOTAL_ERRORS     // leave this one, error counter
};

extern void modbus_setup();
extern void modbus_loop();
extern void modbus_set(word event, word value);

#define OUT1 PIN2
#define OUT2 PIN3
#define OUT3 PIN4
#define OUT4 PIN5
#define OUT5 PIN_A3
#define OUT6 PIN_A2
#define OUT7 PIN_A1
#define OUT8 PIN_A0

#define CEILING_LIGHTS1 OUT1
#define CEILING_LIGHTS2 OUT2
#define EXIT_DOOR OUT3

#endif //RELAY_BOX_MODBUS_H
