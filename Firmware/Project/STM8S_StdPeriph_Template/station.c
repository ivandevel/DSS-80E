#include "stm8s.h"
#include "stm8s_eval.h"
#include <stdlib.h>
#include "station.h" 
#include "pid.h"
#include "7-seg.h"
#include "button.h"
#include "thermo.h"

#pragma location=FLASH_DATA_START_PHYSICAL_ADDRESS 
__no_init volatile uint16_t eeSetpoint;

static uint32_t tempaccum;
static uint16_t timedivider;
static volatile uint16_t display_setpoint_timeout=DISPLAY_SETPOINT_TIMEOUT;
static volatile uint16_t display_type_timeout=DISPLAY_SETPOINT_TIMEOUT;
static uint16_t Temperature = 0;
static uint16_t tSet = 0;
extern uint16_t GetAdcValue(ADC1_Channel_TypeDef channel);
static int16_t Power = 0;  
static uint32_t SecondTick = 0;
static uint16_t Setpoint=0;
static volatile uint16_t *lcddata;
static uint8_t StbyMode=MODE_WORKING;

static uint8_t RegulMode = 0;
static uint16_t FanSpeed = 100;

#ifdef DFS_90

__IO uint16_t ADC_Val;
__IO uint16_t calUpperLimit;
__IO uint16_t levelMemory = UPPER_LIMIT;

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

void Soldering_ADC_Config (void)
{
  /*  Init GPIO for ADC2 */
  GPIO_Init(ADC_GPIO_PORT, ADC_GPIO_PIN, GPIO_MODE_IN_FL_NO_IT);
  
  // Deinit ADC
  ADC1_DeInit();
  
  ADC1_Init(
    ADC1_CONVERSIONMODE_CONTINUOUS,
    ADC1_CHANNEL_4,
    ADC1_PRESSEL_FCPU_D4,
    ADC1_EXTTRIG_TIM, DISABLE,
    ADC1_ALIGN_RIGHT,
    ADC1_SCHMITTTRIG_CHANNEL4, DISABLE);

  ADC1_Cmd(ENABLE);
}

void Soldering_Main(void)
{
  
  if ((eeSetpoint > 450) || (eeSetpoint < 150)) eeSetpoint = 150;
  
  //Вытаскиваем значение уставки из EEPROM
  Setpoint = eeSetpoint;
  
  tSet = Setpoint;
  
  FAN_SET_PWM_DUTY(FanSpeed);
  /*
  pid_s.KP = 60; //8
  pid_s.KI = 49; //22
  pid_s.KD = 20; //4
  pid_s.KT = 5; //30
  */
//uint8_t now_button=0;

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
#ifndef DFS_90
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
#endif 
      
#ifdef DFS_90
      StbyMode = MODE_WORKING;
      SecondTick = 0;
      
      switch(RegulMode)
      {
      case PARAM_TEMPERATURE:
        RegulMode = PARAM_FANSPEED;
        break;
      case PARAM_FANSPEED:        
        RegulMode = PARAM_COOLDOWN;
        break;
      case PARAM_HEATPOWER:
        RegulMode = PARAM_COOLDOWN;
        break;
      case PARAM_COOLDOWN:
        RegulMode = PARAM_TEMPERATURE;
        FanSpeed = 100;
        FAN_SET_PWM_DUTY(FanSpeed);
        break;
//      case PARAM_STANDBY:
//        RegulMode = PARAM_TEMPERATURE;
//        FanSpeed = 100;
//        FAN_SET_PWM_DUTY(FanSpeed);
//        break;
      }
      display_type_timeout = DISPLAY_SETPOINT_TIMEOUT;
#endif      
      
    }
  
    uint8_t state = ENC_GetStateEncoder();
  
  //Encoder is rotated
  if (state != 0) 
  {
                          StbyMode = MODE_WORKING;
                          SecondTick = 0;
                          
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
                                        if (FanSpeed <= 30) FanSpeed = 30;
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

int32_t varVolt = 112;
int32_t varProcess = 100;
int32_t Pc = 0;
int32_t G = 0;
int32_t P=100000;
int32_t Xp = 0.0;
int32_t Zp = 0;
int32_t Xe=0;

int32_t Kalman(int32_t voltage)
{
Pc = P+varProcess;
G = Pc/(Pc+varVolt);
P = (100000-G)*Pc;
Xp = Xe;
Zp = Xp;
Xe = G * (voltage - Zp) + Xp;

return Xe;
}

uint8_t tempcount = 0;
uint16_t microvolts;

void Soldering_ISR (void)
{
   ssegWriteStr("   ", 3, SEG1);
   
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
     #ifndef DFS_90
     GPIO_WriteLow(ADC_GPIO_PORT, ADC_GPIO_PIN);
     GPIO_Init(ADC_GPIO_PORT, ADC_GPIO_PIN, GPIO_MODE_IN_FL_NO_IT);
     //TIM2_SetCompare3(0);
     GPIO_WriteLow(CONTROL_GPIO_PORT, CONTROL_GPIO_PIN);
     ADC1_Cmd(ENABLE);
     #endif
   }
   
   
   if (timedivider == 2) {
     tempaccum += GetAdcValue(ADC_SOLDER_TEMP_CHANNEL);
     //TIM2_SetCompare3(Power);
     tempcount++;
   }  
  
  
     if (timedivider == (MEASURING_INTERVAL_TICKS-Power+1)) {
     #ifndef DFS_90
     GPIO_WriteHigh(CONTROL_GPIO_PORT, CONTROL_GPIO_PIN);
     #endif
   }
  
#ifndef DFS_90
  if (display_setpoint_timeout)
    {
      display_setpoint_timeout--;
      //Уставку - на экран
      lcddata = &Setpoint;
      //Если кончилось время отображения значения уставки
      if (!display_setpoint_timeout) {
        //Температуру - на экран
        lcddata = &Temperature;
        //lcddata = &Power;
        eeSetpoint = Setpoint;
      }
    }
#endif
  
#ifdef DFS_90 
if (display_type_timeout)
    {
      display_type_timeout--;
      //Уставку - на экран
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
        ssegWriteStr("COL", 3, SEG1);
        break;
//        case PARAM_STANDBY:
//        ssegWriteStr("Stb", 3, SEG1);
//        break;
      }
      //Если кончилось время отображения значения уставки
      if (!display_type_timeout) {
        //Температуру - на экран
        lcddata = &Temperature;
        //lcddata = &Power;
        eeSetpoint = Setpoint;
      }
    }  
  
//While spinning encoder - show changing parameter value
if (display_setpoint_timeout)
    {
      display_setpoint_timeout--;
      //Уставку - на экран
      switch (RegulMode)
      {
        case PARAM_TEMPERATURE:
        lcddata = &Setpoint;
        break;
        case PARAM_FANSPEED:
        lcddata = &FanSpeed;
        break;
        case PARAM_HEATPOWER:
        lcddata = &Power;
        break;
        case PARAM_COOLDOWN:
        lcddata = &FanSpeed;
        break;
      }
      //Если кончилось время отображения значения уставки
      if (!display_setpoint_timeout) {
        //Температуру - на экран
        lcddata = &Temperature;
        //lcddata = &Power;
        eeSetpoint = Setpoint;
      }
    }
#endif

#ifndef DFS_90  
     switch(StbyMode)
      {
      case MODE_WORKING:       
        if ((Temperature > 480)) {
        ssegWriteStr("---", 3, SEG1); 
        }
        else
          {
      if (!display_type_timeout) 
        ssegWriteInt(*lcddata); 
          }
      break;
      case MODE_STANDBY:
      ssegWriteStr("Stb", 3, SEG1);
      break;     
      case MODE_POWEROFF:
      ssegWriteStr("OFF", 3, SEG1);
      break; 
      }
#endif
  
#ifdef DFS_90
  if ((Temperature > 480)) {
        ssegWriteStr("---", 3, SEG1); 
        }
        else
          {
      if (!display_type_timeout) 
        ssegWriteInt(*lcddata); 
          }
#endif
  
     if (tempcount == N_MEASUREMENTS_OF_TEMPERATURE) {
        
     Temperature = Kalman(Convert(tempaccum/N_MEASUREMENTS_OF_TEMPERATURE, 1));//Convert(tempaccum/3,1);//kalman_get_x(Convert(tempaccum/3,1));     

#ifndef DFS_90
      switch(StbyMode)
      {
      case MODE_WORKING:    
      tSet = Setpoint;
      break;
      case MODE_STANDBY:
      tSet = Setpoint/2;
      break;
      case MODE_POWEROFF:
      tSet = 0;
      break;      
      }
#endif     
      
#ifdef DFS_90
      switch (RegulMode)
      {
        case PARAM_TEMPERATURE:
        tSet = Setpoint;
        Power = PIDcal(tSet, Temperature);
        Triac_angle = Power*16;
        
        break;
        case PARAM_FANSPEED:
        tSet = Setpoint;
        Power = PIDcal(tSet, Temperature);
        Triac_angle = Power*16;
        break;
        case PARAM_HEATPOWER:
        Triac_angle = Power*16;
        break;
        case PARAM_COOLDOWN:
        tSet = 0;
        Power = 0;
        break;
      }
#endif
      
     #ifndef DFS_90
       Power = PIDcal(tSet, Temperature);
    #endif

     tempaccum = 0;
     tempcount = 0;

   }
     
    if (timedivider == MEASURING_INTERVAL_TICKS) { //20
     timedivider = 0;
     #ifndef DFS_90
     ADC1_Cmd(DISABLE);
     GPIO_Init(ADC_GPIO_PORT, ADC_GPIO_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
     GPIO_WriteHigh(ADC_GPIO_PORT, ADC_GPIO_PIN);
     #endif
   }
}

void HotAir_ISR (void)  
{
  static uint16_t old_Temperature;
  
  Triac_angle = Power*16;
  
  timedivider++;
   
   if (timedivider == 2) {
     tempaccum += GetAdcValue(ADC_SOLDER_TEMP_CHANNEL);
     tempcount++;
   } 
   
   if (timedivider == MEASURING_INTERVAL_TICKS) { 
     timedivider = 0;
   }
   
   if (tempcount == N_MEASUREMENTS_OF_TEMPERATURE) {
        
     Temperature = Kalman(Convert(tempaccum/N_MEASUREMENTS_OF_TEMPERATURE, 1));    
     
      switch (RegulMode)
      {
        case PARAM_TEMPERATURE:
        case PARAM_FANSPEED:
        tSet = Setpoint;
        Power = pid(tSet, Temperature);
        Triac_angle = Power*16;
        if (FanSpeed < 30)
        {
          FanSpeed = 30;
          FAN_SET_PWM_DUTY(FanSpeed);
        }
        break;
        case PARAM_HEATPOWER:
        Triac_angle = Power*16;
        if (FanSpeed < 30)
        {
          FanSpeed = 30;
          FAN_SET_PWM_DUTY(FanSpeed);
        }
        break;
        case PARAM_COOLDOWN:
        tSet = 0;
        Power = 0;
        if  (Temperature < 32)
        {
          //RegulMode = PARAM_STANDBY;
          FanSpeed = 0;
          FAN_SET_PWM_DUTY(FanSpeed);
        }
        
         if  (Temperature > 45)
        {
          FanSpeed = 100;
          FAN_SET_PWM_DUTY(FanSpeed);
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
        eeSetpoint = Setpoint;
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
        eeSetpoint = Setpoint;
      }
    }

}
