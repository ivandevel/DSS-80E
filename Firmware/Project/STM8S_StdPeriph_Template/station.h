#ifndef __STATION_H__
#define __STATION_H__
#include "stm8s_eval.h"

#define MODE_WORKING 0
#define MODE_POWEROFF 2
#define MODE_STANDBY 1
#define MEASURING_INTERVAL_TICKS 50
#define N_MEASUREMENTS_OF_TEMPERATURE 3

#define STANDBY_TIME_MIN 60
#define POWEROFF_TIME_MIN 120

#define DISPLAY_SETPOINT_TIMEOUT 750

void Soldering_TIM2_Config(void);
void Soldering_ADC_Config(void);
void Soldering_Main(void);
void Soldering_ISR(void);

#endif