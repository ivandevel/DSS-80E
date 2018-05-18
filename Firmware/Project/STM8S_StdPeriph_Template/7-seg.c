#include "stm8s.h"
#include "stm8s_eval.h"
#include "7-seg.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/** Utilities
  *
  */
void WriteDigit(char digit);

#define DEFAULT_UPDATE_PERIOD_US	1000
#define DEFAULT_BRIGHTNESS_PERCENT	100

/* We can assign a 7-seg symbol code to any ASCII symbol */
#pragma location=FLASH_DATA_START_PHYSICAL_ADDRESS+2
static const char charmap[128] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//		0...7
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//		8...15
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//		16..23
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//		24..31
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//		32..39
	0x00,0x00,0x00,0x00,0x00,0x40,0x80,0x00,	//     -.	40..47
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,	//01234567	48..55
	0x7F,0x6F,0x00,0x00,0x18,0x48,0x0C,0x00,	//89  <=> 	56..63
	0x00,0x77,0x00,0x39,0x00,0x79,0x71,0x3D,	// A C EFG	64..71
	0x76,0x06,0x1F,0x00,0x38,0x00,0x00,0x3F,	//HIJ L  O	72..79
	0x73,0x00,0x00,0x6D,0x78,0x3E,0x1C,0x00,	//P  STUV 	80..87
	0x00,0x6E,0x00,0x39,0x00,0x0F,0x09,0x08,	// Y [ ]^_ 	88..95
	0x00,0x00,0x7C,0x58,0x5E,0x00,0x71,0x00,	//  bcd f 	96..103
	0x74,0x00,0x0E,0x00,0x00,0x00,0x54,0x5C,	//h j   no	104..111
	0x00,0x00,0x50,0x00,0x78,0x1C,0x1C,0x00,	//  r tuv	112..119
	0x00,0x00,0x00,0x46,0x00,0x70,0x00,0x00	        //   { }  	120..127
};


volatile uint8_t Buffer[3] = {0x00,0x00,0x00};
Seg_TypeDef CurrentSeg = SEG1;
uint8_t RefreshTimerThreshold = (uint8_t)(-1);
uint8_t BrightnessPercent = DEFAULT_BRIGHTNESS_PERCENT;
unsigned short RefreshTimerPrescaler = 1;	//it means divided by 2


/* 7-segment dynamic indication timer init */
void ssegInit(void) 
{
  
   
 
}


/**
  * @brief  This function handles SSegTimer global interrupt request.
  * @param  None
  * @retval None
  */
void ssegTimerCC1IRQHandler(void)
{
  		/* Brightness control */
		/* Clear TIM Capture compare interrupt pending bit */
		TIM1_ClearITPendingBit(TIM1_IT_CC1);
		/* Disable current digit */
#ifdef USE_BRIGHTNESS            
#ifdef SEGMENT_POLARITY_HIGH
			/* Disable current digit */
			STM_EVAL_SEGOff(CurrentSeg);
#endif

#ifdef SEGMENT_POLARITY_LOW
			/* Disable current digit */
			STM_EVAL_SEGOn(CurrentSeg);
#endif 
#endif                    
}

void ssegTimerIRQHandler(void) 
{
			/* Switch to the next digit */
			/* Clear TIM Update interrupt pending bit */
			//TIM1_ClearITPendingBit(TIM1_IT_UPDATE);

#ifdef SEGMENT_POLARITY_HIGH
			/* Disable current digit */
			STM_EVAL_SEGOff(CurrentSeg);
#endif

#ifdef SEGMENT_POLARITY_LOW
			/* Disable current digit */
			STM_EVAL_SEGOn(CurrentSeg);
#endif                        
                        
			/* Switch to the next digit */
			CurrentSeg++;
			if (CurrentSeg == SEGn)
				CurrentSeg = SEG1;

			/* Send next digit value */
			WriteDigit(Buffer[CurrentSeg]);

                        
#ifdef SEGMENT_POLARITY_HIGH                       
			/* Enable digit */
			STM_EVAL_SEGOn(CurrentSeg);
#endif

#ifdef SEGMENT_POLARITY_LOW                       
			/* Enable digit */
			STM_EVAL_SEGOff(CurrentSeg);
#endif
                        
                        
}


/* Set switching period between 7-seg digits indication */
void ssegSetUpdateRate(unsigned short us) {
//RCC_ClocksTypeDef RCC_Clocks;
//	RCC_GetClocksFreq(&RCC_Clocks);
//
//	/* Calculating new timer prescaler value */
//	RefreshTimerPrescaler= us/(RCC_Clocks.SYSCLK_Frequency / 65536);
//
//	/* Calculating timer top value */
//	RefreshTimerThreshold= us*((RCC_Clocks.SYSCLK_Frequency / (RefreshTimerPrescaler+1)) / 1000000UL);
//
//	/* Set new timer divider value */
//	TIM_PrescalerConfig(SSEG_TIMER, RefreshTimerPrescaler, TIM_PSCReloadMode_Update);
//
//	/* Set new timer top value */
//	TIM_SetAutoreload(SSEG_TIMER, RefreshTimerThreshold);
//
//	/* Update brightness according to the new update rate */
//	ssegSetBrightness(BrightnessPercent);
}


/* Set brightness of the 7-seg digits indication */
void ssegSetBrightness(unsigned char percent) {
uint16_t threshold;
	BrightnessPercent = percent;

	/* Calculating timer compare value */
	threshold = ( (uint32_t)RefreshTimerThreshold * BrightnessPercent ) / 100;

	//set timer compare register
        TIM1_SetCompare1(threshold);
}


void WriteDigit(char digit) {
Led_TypeDef Led;
	/* Write new DIGIT port bits */
	for ( Led = LEDA; Led < LEDn; Led++ ) {
		if (digit & 0x01)
                #ifdef ELEMENT_POLARITY_LOW
			STM_EVAL_LEDOff(Led);
                #endif
                #ifdef ELEMENT_POLARITY_HIGH
			STM_EVAL_LEDOn(Led);
                #endif
		else
                #ifdef ELEMENT_POLARITY_LOW
			STM_EVAL_LEDOn(Led);
                #endif
                #ifdef ELEMENT_POLARITY_HIGH
			STM_EVAL_LEDOff(Led);
                #endif
                
		digit >>= 1;
	}
}

void ssegWriteStr(char *str, uint8_t len, Seg_TypeDef seg) {
uint8_t i;
char newchar;
	assert_param(IS_SSEG(seg));
	for ( i=0; i<len; i++, seg++ ) {
		if ( seg == SEGn )
			break;
		newchar = str[i];
		Buffer[seg] = charmap[newchar];
		if ( str[i+1] == '.' ) {
			Buffer[seg] += charmap['.'];
			i++;
		}
	}
}

 /* reverse:  переворачиваем строку s на месте */
 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

/* itoa:  конвертируем n в символы в s */
void itoa(int n, char * s)
 {
     int i, sign;
     
     if ((sign = n) < 0)  /* записываем знак */
         n = -n;          /* делаем n положительным числом */
     i = 0;
     do {       /* генерируем цифры в обратном порядке */
         s[i++] = n % 10 + '0';   /* берем следующую цифру */
     } while ((n /= 10) > 0);     /* удаляем */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     
 }

void ssegWriteInt(uint16_t value) {
  ssegClear();
 static char buf[3];
  
 memset(buf, 0x00, sizeof(buf));
  
 itoa(value, buf);
  
 reverse(buf);

  if (value < 10) 
  ssegWriteStr(buf, 3, SEG3);
  else if (value <= 99)  
   ssegWriteStr(buf, 3, SEG2);  
  else if (value > 99) 
    ssegWriteStr(buf, 3, SEG1);
}


void ssegClear(void) {
uint8_t i;
	for (i=0; i<sizeof(Buffer); i++)
		Buffer[i] = 0x00;
}


