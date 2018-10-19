#include "stm8s.h"
#include "stm8s_eval.h"
#include <stdlib.h>
#include "hotair.h" 
#include "pid.h"
#include "7-seg.h"
#include "button.h"
#include "thermo.h"
#include "eeprom.h"

static __IO int16_t Triac_angle = 0;
static uint32_t tempaccum;
static uint16_t timedivider;
static volatile uint16_t display_setpoint_timeout=DISPLAY_SETPOINT_TIMEOUT;
static volatile uint16_t display_type_timeout=DISPLAY_SETPOINT_TIMEOUT;
static uint16_t Temperature = 0;
static uint16_t tSet = 0;
extern uint16_t GetAdcValue(ADC1_Channel_TypeDef channel);
static int16_t  Power = 0;  
static uint16_t Setpoint = 0;
static uint8_t  RegulMode = 0;
static uint16_t FanSpeed = 100;
static uint8_t  tempcount = 0;

void HotAir_Config(void)
{
  /* Initialize I/Os in Output Mode */
  GPIO_Init(TRIAC_PORT, (GPIO_Pin_TypeDef)TRIAC_PIN, GPIO_MODE_OUT_PP_LOW_FAST);  
  GPIO_Init(ZERO_CROSS_PORT, ZERO_CROSS_PIN, GPIO_MODE_IN_PU_IT);
  
/* Initialize ext. interrput pin for zero cross deccation  */
  EXTI_SetExtIntSensitivity(ZERO_EXTI_PORT, EXTI_SENSITIVITY_RISE_ONLY);
  //  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_ONLY);
  EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_RISE_ONLY); 
  
//---------------------------------------------------  
  /* Configure TIMER2 for AC dimming */
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_8, 20000);
  TIM2_OC1Init(TIM2_OCMODE_PWM1,TIM2_OUTPUTSTATE_ENABLE,0,TIM2_OCPOLARITY_HIGH);
  
  /*        
  TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 5, TIM2_OCPOLARITY_HIGH);
  TIM2_OC2PreloadConfig(ENABLE);
  */
  
  TIM2_ARRPreloadConfig(ENABLE);
  
  TIM2_Cmd(ENABLE);
  //Delayms(1);
  //Calc_AC_Freqency();

//---------------------------------------------------  
 /* Time base configuration */      
   TIM1_TimeBaseInit(1, TIM1_COUNTERMODE_UP, 99, 0);

  /* Output Compare Active Mode configuration: Channel1 */
  /*
    TIM2_OCMode = TIM2_OCMODE_ACTIVE
    TIM2_OutputState = TIM2_OUTPUTSTATE_ENABLE
    TIM2_Pulse = CCR1_Val
    TIM2_OCPolarity = TIM2_OCPOLARITY_HIGH
  */
//  TIM1_OC4Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE,
//               0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_LOW, 
//               TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);           
  
  TIM1_OC4Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, 
               0, TIM1_OCPOLARITY_HIGH,  
               TIM1_OCIDLESTATE_RESET);
   
  TIM1_OC4PreloadConfig(ENABLE);

  TIM1_ARRPreloadConfig(ENABLE);
  
  TIM1_CtrlPWMOutputs(ENABLE);
  
  /* TIM1 enable counter */
  TIM1_Cmd(ENABLE); 
  
  //reed input
  GPIO_Init(REED_GPIO_PORT, REED_GPIO_PIN, GPIO_MODE_IN_PU_NO_IT);
//---------------------------------------------------  
}

void TriacAngle_ISR (void) 
{
TIM2_SetCounter(2000 + Triac_angle);
  //monitor the level value 
  if (Triac_angle == LOWER_LIMIT)
    TIM2_SetCompare1(0);
  else 
    TIM2_SetCompare1(TRIAC_FIRE_WIDTH);
}
#pragma optimize=none
void HotAir_ISR (void)  
{
  static uint16_t old_Temperature;
  
  timedivider++;
   
   if (timedivider == 2) {
     tempaccum += GetAdcValue(ADC_SOLDER_TEMP_CHANNEL);
     tempcount++;
   } 
   
   if (timedivider == MEASURING_INTERVAL_TICKS) { 
     timedivider = 0;
   }
   
   if (tempcount == N_MEASUREMENTS_OF_TEMPERATURE) {
        
     Temperature = Get_Temperature_Filtered(tempaccum/N_MEASUREMENTS_OF_TEMPERATURE);    
     
      switch (RegulMode)
      {
        case PARAM_TEMPERATURE:
        case PARAM_FANSPEED:
        tSet = Setpoint;
        Power = PIDcal(tSet, Temperature);
        if (Temperature > 450) Power = 0;   
        Triac_angle = Power*16;
        if (FanSpeed < FANSPEED_MIN)
        {
          FanSpeed = FANSPEED_MIN;
          FAN_SET_PWM_DUTY(FanSpeed);
        }
        break;
        case PARAM_HEATPOWER:
          if (Temperature > 450) 
          {
            Power = 0;
            //FAN_SET_PWM_DUTY(FanSpeed);
          }
        Triac_angle = Power*16;
        if (FanSpeed < FANSPEED_MIN)
        {
          FanSpeed = FANSPEED_MIN;
          FAN_SET_PWM_DUTY(FanSpeed);
        }
        break;
        case PARAM_COOLDOWN:
        tSet = 0;
        Power = 0;
        Triac_angle = Power*16;
        if  (Temperature < 32)
        {
          //RegulMode = PARAM_STANDBY;
          FanSpeed = 0;
          FAN_SET_PWM_DUTY(FanSpeed);
        }
        
         if  (Temperature > 45)
        {
          //FanSpeed = 100;
          FAN_SET_PWM_DUTY(100);
        }
        break;
//       case PARAM_STANDBY:
//        tSet = 0;
//        Power = 0;
//        if  (Temperature < 31) 
//        {
//          //RegulMode = PARAM_STANDBY;
//          FanSpeed = 0;
//          FAN_SET_PWM_DUTY(FanSpeed);
//        }
//        if  (Temperature > 40)
//        {
//          FanSpeed = 100;
//          FAN_SET_PWM_DUTY(FanSpeed);
//        }
//       break;
      }
     
     tempaccum = 0;
     tempcount = 0;
     
     if ((!display_setpoint_timeout) && (!display_type_timeout)) {
       if (old_Temperature != Temperature) {
       ssegWriteInt(Temperature); 
       old_Temperature = Temperature;
       }      
     }
   }
  
if (display_type_timeout)
    {
      if (display_type_timeout == DISPLAY_SETPOINT_TIMEOUT)
      {
      switch (RegulMode)
      {
        case PARAM_TEMPERATURE:
        ssegWriteStr("SEt", 3, SEG1);
        break;
        case PARAM_FANSPEED:
        ssegWriteStr("FAn", 3, SEG1);
        break;
        case PARAM_HEATPOWER:
        ssegWriteStr("HEA", 3, SEG1);
        break;
        case PARAM_COOLDOWN:
        ssegWriteStr("COL", 4, SEG1);
        break;
//        case PARAM_STANDBY:
//        ssegWriteStr("Stb", 3, SEG1);
//        break;
      }
      }
      
      display_type_timeout--;
      
      //Если кончилось время отображения значения уставки
      if (display_type_timeout == 0) {
        display_setpoint_timeout = DISPLAY_SETPOINT_TIMEOUT;
      }
    }

if (display_setpoint_timeout)
    {
      //Уставку - на экран
  if (display_setpoint_timeout == DISPLAY_SETPOINT_TIMEOUT) {
      switch (RegulMode)
      {
        case PARAM_TEMPERATURE:
        ssegWriteInt(Setpoint);
        break;
        case PARAM_FANSPEED:
        ssegWriteInt(FanSpeed);
        break;
        case PARAM_HEATPOWER:
        ssegWriteInt(Power);
        break;
        case PARAM_COOLDOWN:
        ssegWriteInt(FanSpeed);
        break;
      }
      }
      
      display_setpoint_timeout--;
      //Если кончилось время отображения значения уставки
      if (display_setpoint_timeout == 0) {
      switch (RegulMode)
      {
        case PARAM_TEMPERATURE:
        ee_Setpoint_Set(Setpoint);
        break;
        case PARAM_FANSPEED:
        //eeFanSpeed = FanSpeed;
        break;
        case PARAM_HEATPOWER:
        //ssegWriteInt(Power);
        break;
        case PARAM_COOLDOWN:
        //ssegWriteInt(FanSpeed);
        break;
      }
        ee_Setpoint_Set(Setpoint);
        ee_Fanspeed_Set(FanSpeed);
      }
    }

}

void HotAir_Main(void)
{
  Setpoint = ee_Setpoint_Get();
  FanSpeed = ee_Fanspeed_Get();
    
  tSet = Setpoint;
  
  FAN_SET_PWM_DUTY(FanSpeed);

  while(1) 
  {
    
//   if (eButtonGetEvent(BUTTON_REED) == eButtonEventHold)
//    {
//      RegulMode = PARAM_COOLDOWN;
//      FanSpeed = 90;
//      FAN_SET_PWM_DUTY(FanSpeed);
//      display_type_timeout = DISPLAY_SETPOINT_TIMEOUT;
//    } 
    
    //Pressing encoder button
  if (eButtonGetEvent(BUTTON_KEY) == eButtonEventPress ) {

      //StbyMode = MODE_WORKING;
      //SecondTick = 0;
      
      switch(RegulMode)
      {
      case PARAM_TEMPERATURE:
        RegulMode = PARAM_FANSPEED;
        FanSpeed = ee_Fanspeed_Get();
        FAN_SET_PWM_DUTY(FanSpeed);
        break;
      case PARAM_FANSPEED:
        RegulMode = PARAM_COOLDOWN;
        //FanSpeed = eeFanSpeed;
        //FAN_SET_PWM_DUTY(FanSpeed);
        break;
      case PARAM_HEATPOWER:
        RegulMode = PARAM_COOLDOWN;
        break;
      case PARAM_COOLDOWN:
        RegulMode = PARAM_TEMPERATURE;
        FanSpeed = ee_Fanspeed_Get();
        FAN_SET_PWM_DUTY(FanSpeed);
        break;
//      case PARAM_STANDBY:
//        RegulMode = PARAM_TEMPERATURE;
//        FanSpeed = 100;
//        FAN_SET_PWM_DUTY(FanSpeed);
//        break;
      }
      
      display_type_timeout = DISPLAY_SETPOINT_TIMEOUT;
      
    }
  
    uint8_t state = ENC_GetStateEncoder();
  
  //Encoder is rotated
  if (state != 0) 
  {
                          //StbyMode = MODE_WORKING;
                          //SecondTick = 0;
                          
                          display_setpoint_timeout = DISPLAY_SETPOINT_TIMEOUT;
                          display_type_timeout = 0;
                          
				if (state == RIGHT_SPIN) {
                                  switch (RegulMode)
                                  {
                                  case PARAM_TEMPERATURE:
					Setpoint+=5;
                                        if (Setpoint >= 500) Setpoint = 500;
                                        tSet = Setpoint; 
                                        break;
                                  case PARAM_FANSPEED:
                                  //case PARAM_COOLDOWN:
                                    FanSpeed+=5;
                                        if (FanSpeed >= 100) FanSpeed = 100;
                                        FAN_SET_PWM_DUTY(FanSpeed); 
                                    break;
                                  case PARAM_HEATPOWER:
                                    Power+=5;
                                    if (Power >= 995) Power = 995;
                                    break;
                                  }
				}
                                
				if (state == LEFT_SPIN) {
                                  switch (RegulMode)
                                  {
                                  case PARAM_TEMPERATURE:
                                        Setpoint-=5;
                                        if (Setpoint <= 150) Setpoint = 150;
                                        tSet = Setpoint;
                                        break;
                                  case PARAM_FANSPEED:
                                  //case PARAM_COOLDOWN:
                                    FanSpeed-=5;
                                        if (FanSpeed <= FANSPEED_MIN) FanSpeed = FANSPEED_MIN;
                                        FAN_SET_PWM_DUTY(FanSpeed);
                                    break;
                                    case PARAM_HEATPOWER:
                                    Power-=5;
                                        if (Power <= 5) Power = 5;
                                    break;
                                  }
				}
			} 
 
  }
}
