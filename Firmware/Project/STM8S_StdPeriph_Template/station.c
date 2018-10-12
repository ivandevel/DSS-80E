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

#pragma location=FLASH_DATA_START_PHYSICAL_ADDRESS+2
__no_init volatile uint16_t eeFanSpeed;

static uint32_t tempaccum;
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

void HotAir_Main(void)
{
  
  if ((eeSetpoint > 450) || (eeSetpoint < 150)) eeSetpoint = 150;
  if ((eeFanSpeed > 100) || (eeFanSpeed < FANSPEED_MIN)) eeFanSpeed = 100;
  
  //Вытаскиваем значение уставки из EEPROM
  Setpoint = eeSetpoint;
  FanSpeed = eeFanSpeed;
    
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

      StbyMode = MODE_WORKING;
      SecondTick = 0;
      
      switch(RegulMode)
      {
      case PARAM_TEMPERATURE:
        RegulMode = PARAM_FANSPEED;
        FanSpeed = eeFanSpeed;
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
        FanSpeed = eeFanSpeed;
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
     tempaccum += GetAdcValue(ADC_SOLDER_TEMP_CHANNEL);
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
      //Если кончилось время отображения значения уставки
      if (!display_setpoint_timeout) {
        
        if ((Temperature > MAX_TEMPERATURE))
        ssegWriteStr("---", 3, SEG1); 
         else ssegWriteInt(Temperature);
        
        eeSetpoint = Setpoint;
      }
    }
  
     if (tempcount == N_MEASUREMENTS_OF_TEMPERATURE) {
        
     Temperature = Kalman(Convert(tempaccum/N_MEASUREMENTS_OF_TEMPERATURE, 1)) + 20;//Convert(tempaccum/3,1);//kalman_get_x(Convert(tempaccum/3,1));     

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
      
       tempaccum = 0;
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

#ifdef DFS_90
void HotAir_ISR (void)  
{
  static uint16_t old_Temperature;

  //Triac_angle = Power*16;
  
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
        if (Temperature > 450) Power = 0;
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
        eeFanSpeed = FanSpeed;
      }
    }

}
#endif