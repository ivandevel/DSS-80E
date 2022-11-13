#ifndef __EEPROM_H__
#define __EEPROM_H__
#include "stm8s_eval.h"

//#define FANSPEED_MIN 5

uint16_t ee_Setpoint_Get(void);
void ee_Setpoint_Set(uint16_t ee_setpoint);
uint16_t ee_Fanspeed_Get(void);
void ee_Fanspeed_Set(uint16_t ee_fanspeed);

#endif