#include "stm8s.h"
#include "stm8s_eval.h"
#include <stdlib.h>
#include "station.h" 
#include "pid.h"
#include "7-seg.h"
#include "button.h"
#include "thermo.h"

static uint32_t adc_accum=0;
static uint16_t timedivider;
static volatile uint16_t display_setpoint_timeout=DISPLAY_SETPOINT_TIMEOUT;
static volatile uint16_t display_type_timeout=DISPLAY_SETPOINT_TIMEOUT;
static uint16_t Temperature = 0;
static uint16_t tSet = 0;
extern uint16_t GetAdcValue(ADC1_Channel_TypeDef channel);
static int16_t Power = 0;  
static uint16_t Setpoint=0;
static uint8_t StbyMode=MODE_WORKING;
static uint8_t RegulMode = 0;
static uint16_t FanSpeed = 100;
static uint32_t SecondTick = 0;
static int16_t iron_wait_timeout=2000;
static uint8_t tempcount = 0;
static uint16_t microvolts;

#ifdef DFS_90

static __IO uint16_t ADC_Val;
static __IO uint16_t calUpperLimit;
static __IO uint16_t levelMemory = UPPER_LIMIT;

void Calc_AC_Freqency(void)
{
uint16_t AcPeriod=0;

  while( AcPeriod < 19500 ||  AcPeriod > 20500 ){
    while(!GPIO_ReadInputPin(ZERO_CROSS_PORT,ZERO_CROSS_PIN)); // wait for AC low
    while(GPIO_ReadInputPin(ZERO_CROSS_PORT,ZERO_CROSS_PIN)); // wait for AC falling high
    TIM2_SetCounter(0);                                            // reset counter
    while(!GPIO_ReadInputPin(ZERO_CROSS_PORT,ZERO_CROSS_PIN)); // wait for AC low
    while(GPIO_ReadInputPin(ZERO_CROSS_PORT,ZERO_CROSS_PIN)); // wait for AC falling high
    AcPeriod =  TIM2_GetCounter();
  }
  
  TIM2_SetAutoreload(AcPeriod );
  calUpperLimit =(uint16_t)( AcPeriod *0.80);
  levelMemory = calUpperLimit;
}
#endif

//void Soldering_TIM2_Config(void)
//{
//  /* Time base configuration */
//  TIM2_TimeBaseInit(TIM2_PRESCALER_32, 512);
//
//  /* PWM1 Mode configuration: Channel3 */         
//  TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0, TIM2_OCPOLARITY_HIGH);
//  TIM2_OC3PreloadConfig(ENABLE);
//
////    /* PWM1 Mode configuration: Channel3 */         
////  TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0, TIM2_OCPOLARITY_HIGH);
////  TIM2_OC2PreloadConfig(ENABLE);
//  
//  TIM2_ARRPreloadConfig(ENABLE);
//
//  /* TIM2 enable counter */
//  TIM2_Cmd(ENABLE);
//}

void Soldering_Main(void)
{
  if ((eeSetpoint > 450) || (eeSetpoint < 150)) eeSetpoint = 150;
  
  //¬ытаскиваем значение уставки из EEPROM
  Setpoint = eeSetpoint;
 
  tSet = Setpoint;
  
  #ifdef DFS_90
  FAN_SET_PWM_DUTY(FanSpeed);
  #endif

  while(1) 
  {  
    //Pressing encoder button
  if (eButtonGetEvent(BUTTON_KEY) == eButtonEventPress ) {
      switch(StbyMode)
      {
      case MODE_WORKING:
        StbyMode = MODE_POWEROFF;
        break;
      case MODE_STANDBY:        
        StbyMode = MODE_WORKING;
        break;
      case MODE_POWEROFF:
        StbyMode = MODE_WORKING;
        break; 
      }
    }
  
    uint8_t state = ENC_GetStateEncoder();
  
  //Encoder is rotated 
if (state != 0) {
                          StbyMode = MODE_WORKING;
                          
                          display_setpoint_timeout = DISPLAY_SETPOINT_TIMEOUT;
                          
				if (state == RIGHT_SPIN) {
					Setpoint+=5;
                                        if (Setpoint >= 450) Setpoint = 450;
				}
				if (state == LEFT_SPIN) {
                                        Setpoint-=5;
                                        if (Setpoint <= 150) Setpoint = 150;
				}
			}  
  }
}







#ifndef DFS_90
void Soldering_ISR (void)
{
   //ssegWriteStr("   ", 3, SEG1);
   static uint16_t old_Temperature;
   
   timedivider++;
  
  if ((StbyMode == MODE_WORKING) || (StbyMode == MODE_STANDBY)) SecondTick++;

  if (SecondTick ==  STANDBY_TIME_MIN * 60000UL) // 5 minutes
   {
     StbyMode = MODE_STANDBY;
     //SecondTick = 0;
   }     
    
  if (SecondTick == POWEROFF_TIME_MIN * 60000UL) // 30 minutes
   {
     StbyMode = MODE_POWEROFF;
     SecondTick = 0;
   } 
  
  
   if (timedivider == 1) {
     GPIO_WriteLow(ADC_GPIO_PORT, ADC_GPIO_PIN);
     GPIO_Init(ADC_GPIO_PORT, ADC_GPIO_PIN, GPIO_MODE_IN_FL_NO_IT);
     //TIM2_SetCompare3(0);
     GPIO_WriteLow(CONTROL_GPIO_PORT, CONTROL_GPIO_PIN);
     ADC1_Cmd(ENABLE);
   }
   
   
   if (timedivider == 2) {
     adc_accum += GetAdcValue(ADC_SOLDER_TEMP_CHANNEL);
     //TIM2_SetCompare3(Power);
     tempcount++;
   }  
  
  
     if (timedivider == (MEASURING_INTERVAL_TICKS-Power+1)) {
     GPIO_WriteHigh(CONTROL_GPIO_PORT, CONTROL_GPIO_PIN);
   }
  

  if (display_setpoint_timeout)
    {
      if (display_setpoint_timeout == DISPLAY_SETPOINT_TIMEOUT) ssegWriteInt(Setpoint);
      
      display_setpoint_timeout--;
      //≈сли кончилось врем€ отображени€ значени€ уставки
      if (!display_setpoint_timeout) {
        
        if ((Temperature > MAX_TEMPERATURE))
        ssegWriteStr("---", 3, SEG1); 
         else ssegWriteInt(Temperature);
        
        eeSetpoint = Setpoint;
      }
    }
  
     if (tempcount == N_MEASUREMENTS_OF_TEMPERATURE) {
        
     Temperature = Get_Temperature_Filtered(adc_accum/N_MEASUREMENTS_OF_TEMPERATURE);//Kalman(Convert(adc_accum/N_MEASUREMENTS_OF_TEMPERATURE, 1)) + 20;   

      switch(StbyMode)
      {
      case MODE_WORKING:    
      tSet = Setpoint;
      break;
      case MODE_STANDBY:
      tSet = Setpoint/2;
      break;
      case MODE_WAIT_IRON:
      case MODE_POWEROFF:
      tSet = 0;
      break;      
      }    
      
       Power = PIDcal(tSet, Temperature) / 20;

      if (Power <  0) 
        Power = 0;
      
       adc_accum = 0;
       tempcount = 0;

       switch(StbyMode)
      {
      case MODE_WORKING:
       if ((!display_setpoint_timeout)) {
       if (old_Temperature != Temperature) {
       
         if ((Temperature > MAX_TEMPERATURE)) 
         {
           ssegWriteStr("---", 3, SEG1); 
           StbyMode = MODE_WAIT_IRON;
         }
         else 
         {
           ssegWriteInt(Temperature);
         }
         
       old_Temperature = Temperature;
       }
     }
      break;
      case MODE_STANDBY:        
        ssegWriteStr("Stb", 3, SEG1);      
      break;     
      case MODE_POWEROFF:
      ssegWriteStr("OFF", 3, SEG1);
      break; 
      
      case MODE_WAIT_IRON:
      //if (old_Temperature != Temperature) {
      if ((Temperature > MAX_TEMPERATURE)) {
        iron_wait_timeout = 20;
        ssegWriteStr("---", 3, SEG1);
      }
      else
      {
        iron_wait_timeout --;
        ssegWriteInt(Temperature);
      }
      //old_Temperature = Temperature;
      //}
      
      if (iron_wait_timeout == 0) StbyMode = MODE_WORKING;
      
      break;
      }     
       
       

     
   }
     
    if (timedivider == MEASURING_INTERVAL_TICKS) {
     timedivider = 0;

     ADC1_Cmd(DISABLE);
     GPIO_Init(ADC_GPIO_PORT, ADC_GPIO_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
     GPIO_WriteHigh(ADC_GPIO_PORT, ADC_GPIO_PIN);

   }
}
#endif

