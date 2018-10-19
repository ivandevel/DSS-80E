#include "stm8s.h"
#include "stm8s_eval.h"
#include "eeprom.h"

#pragma location=FLASH_DATA_START_PHYSICAL_ADDRESS 
__no_init volatile uint16_t eeSetpoint;

#pragma location=FLASH_DATA_START_PHYSICAL_ADDRESS+2
__no_init volatile uint16_t eeFanSpeed;

uint16_t ee_Setpoint_Get(void)
{
  if ((eeSetpoint > 450) || (eeSetpoint < 150)) eeSetpoint = 150;
  
  return eeSetpoint;
}

void ee_Setpoint_Set(uint16_t ee_setpoint)
{
  eeSetpoint = ee_setpoint;
}

uint16_t ee_Fanspeed_Get(void)
{
  
  if ((eeFanSpeed > 100) || (eeFanSpeed < FANSPEED_MIN)) eeFanSpeed = 100;
  
  return eeFanSpeed;
}

void ee_Fanspeed_Set(uint16_t ee_fanspeed)
{
  eeFanSpeed = ee_fanspeed;
}