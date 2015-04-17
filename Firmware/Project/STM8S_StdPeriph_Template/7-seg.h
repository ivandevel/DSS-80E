
#ifndef __7_SEG_H__
#define __7_SEG_H__
#include "stm8s_eval.h"

#define DIGIT_POLARITY_HIGH

/* Defines */

/* Need to use a timer for segment dynamic switching */
#define SSEG_TIMER				TIM4
#define SSEG_TIMER_IRQn				TIM4_IRQn
#define SSEG_TIMER_CLK				RCC_APB1Periph_TIM4
#define SSEG_TIMER_APB_CLOCK_CMD	        RCC_APB1PeriphClockCmd

typedef enum {
	WITHOUT_DOT = 0,
	WITH_DOT,
} eDot_t;

/* Public functions declaration */
void ssegInit(void);
void ssegSetUpdateRate(unsigned short us);
void ssegSetBrightness(unsigned char percent);
void ssegWriteStr(char *str, uint8_t len, Seg_TypeDef seg);
void ssegWriteInt(uint16_t value);
void ssegWriteFloat(float value);
void ssegWriteTime(uint8_t min, uint8_t sec, eDot_t dot);
void ssegClear(void);
void ssegTimerIRQHandler(void);
void ssegTimerCC1IRQHandler(void);

#endif //__7_SEG_H__
