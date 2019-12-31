#include "stm8s.h"
#include "stm8s_eval.h"
#include <stdlib.h>
#include "hotair.h" 
#include "pid.h"
#include "7-seg.h"
#include "button.h"
#include "thermo.h"
#include "eeprom.h"
#include "pid.h"

#define b0 1
#define b1 2
#define b2 4
#define b3 8
#define b4 16
#define b5 32
#define b6 64
#define b7 128

#define K_P     2.00
//! \xrefitem todo "Todo" "Todo list"
#define K_I     2.00
//! \xrefitem todo "Todo" "Todo list"
#define K_D     2.00

struct PID_DATA pidData;
static __IO int16_t Triac_angle = 0;
static uint32_t tempaccum = 0;
static uint16_t timedivider = 0;
static volatile uint16_t display_setpoint_timeout = DISPLAY_SETPOINT_TIMEOUT;
static volatile uint16_t display_type_timeout = DISPLAY_SETPOINT_TIMEOUT;
static uint16_t Temperature = 0;
static uint16_t tSet = 0;
static int16_t  Power = 0;
static uint16_t Setpoint = 0;
static uint8_t  RegulMode = 0;
static uint16_t FanSpeed = 100;
static uint8_t  tempcount = 0;
static uint16_t old_Temperature = 0;


unsigned char openloop;
unsigned char run;
unsigned char runstate;
unsigned char zcstate;
unsigned char cyclecounter;
unsigned char gstate;
unsigned char gateon;
unsigned char firstgate;
unsigned char potvalue;
unsigned char regulate;
unsigned char hallstate;

unsigned int halfperiod;
unsigned int usablehalfperiod;
unsigned int gatedelay;
unsigned int zctime;
unsigned int lineperiod;
unsigned int gatetime;

signed int propterm;
signed int intterm;
signed int speedest;
signed int rpmcmd;
signed int rpmref;
signed int rpm;

unsigned long int lineperiodsum;
unsigned long int position;

signed long int filter1int;
signed long errorintegral;

void initvars(void)
{
//PC_ODR &= ~(b6+b7);  // initialize gate low
zctime=0;
lineperiod=0;
zcstate=0;
cyclecounter=0;
lineperiodsum= 0;
halfperiod=0;
gstate=100;
gatedelay= 14000;
gateon=0;
firstgate=255;
position=0;
hallstate=0;
//positionest.us32 = 0;
speedest = 0;
propterm=0;
regulate=0;
filter1int = 0;
rpmcmd = 5000;
rpmref=0;
rpm = 0;
errorintegral=0;
}

void tim2isr(void)
{ 
unsigned char status;
unsigned int word0;
unsigned char byte0;
unsigned char byte1;
unsigned long long0;

status = TIM2->SR1;
TIM2->SR1 = 0;


if(status & b3) // if zero cross (channel 3 input capture)
{

switch(zcstate)
{

case 0: // wait 10 cycles for stability
cyclecounter++;
if(cyclecounter==30)
{
cyclecounter=0;
zcstate=4;
}
break;


case 4:
byte1=TIM2->CCR3H;
byte0=TIM2->CCR3L;
zctime = (byte1<<8)+byte0;
cyclecounter=0;
lineperiodsum=0;
zcstate=10;
break;


case 10:
byte1=TIM2->CCR3H;
byte0=TIM2->CCR3L;
word0 = (byte1<<8)+byte0;
lineperiod = word0 - zctime;
zctime = word0;
lineperiodsum = lineperiodsum + lineperiod;
cyclecounter++;
if(cyclecounter==16)
{
halfperiod = lineperiodsum>>5;
long0 = halfperiod;
long0 = long0 * 218;
long0 = long0>>8;
usablehalfperiod = long0; // usable range for gating is 85% of half period
gatedelay= usablehalfperiod;
zcstate=15;
}
break;


case 15: 
byte1=TIM2->CCR3H;
byte0=TIM2->CCR3L;
zctime = (byte1<<8)+byte0;
gatetime = zctime + halfperiod - 500;
TIM2->CCR1H = gatetime>>8;
TIM2->CCR1L = gatetime;
gstate=100;
zcstate=20;
break;



case 20: 
byte1=TIM2->CCR3H;
byte0=TIM2->CCR3L;
zctime = (byte1<<8)+byte0;
gatetime = zctime + gatedelay; 
TIM2->CCR1H = gatetime>>8;
TIM2->CCR1L = gatetime;
gstate=0;
break;


} // end of switch statement


} // end of zero cross ISR


if( (status & b1) && (zcstate > 10) ) // if gate fire ( channel 1 compare )
{
	
switch(gstate)
{
case 0: // first gating pulse
GPIO_WriteLow(TRIAC_PORT, TRIAC_PIN); // gate low
gateon=255;
gatetime = gatetime + 1000; // schedule turn off 
TIM2->CCR1H = gatetime>>8;
TIM2->CCR1L = gatetime;
gstate=10; // first turn off
regulate = 255;
break;

case 10: // first turn off
GPIO_WriteHigh(TRIAC_PORT, TRIAC_PIN); // take gate high
gatetime = zctime + halfperiod + gatedelay; // setup for second gate
TIM2->CCR1H = gatetime>>8;
TIM2->CCR1L = gatetime;
gateon = 0; // stop toggling
gstate=20;
break;

case 20: // second gate
GPIO_WriteLow(TRIAC_PORT, TRIAC_PIN); // gate low
gatetime = gatetime + 1000; // schedule turn off 
TIM2->CCR1H = gatetime>>8;
TIM2->CCR1L = gatetime;
gstate=30; // second turn off
gateon=255; // enable toggling
regulate = 255;
break;

case 30: // second turn off
GPIO_WriteHigh(TRIAC_PORT, TRIAC_PIN); // take gate high
gateon=0; // stop toggling
gstate=0; // setup for first gating
break;


case 100: // very first gating pulse (get output high safely)
GPIO_WriteHigh(TRIAC_PORT, TRIAC_PIN);
gateon=0;
firstgate=0;
//gatetime = gatetime + 1000; // schedule turn off 100uS before zc
gatetime = gatetime + 100; // schedule turn off 100uS before zc
TIM2->CCR1H = gatetime>>8;
TIM2->CCR1L = gatetime;
gstate=10; // first turn off
break;

	
	
} // end of switch statement	
	
}

} // end of tim1isr function

void HotAir_Config(void)
{
  /* Initialize I/Os in Output Mode */
  GPIO_Init(TRIAC_PORT, (GPIO_Pin_TypeDef)TRIAC_PIN, GPIO_MODE_OUT_PP_LOW_FAST);  
  
  //TIM2_CH3
  GPIO_Init(ZERO_CROSS_PORT, ZERO_CROSS_PIN, GPIO_MODE_IN_PU_NO_IT);
  
  /* Initialize ext. interrput pin for zero cross deccation  */
  //EXTI_SetExtIntSensitivity(ZERO_EXTI_PORT, EXTI_SENSITIVITY_RISE_ONLY);
  //EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_RISE_ONLY); 
  
////---------------------------------------------------  
//  /* Configure TIMER2 for AC dimming */
//  TIM2_DeInit();
//  TIM2_TimeBaseInit(TIM2_PRESCALER_8, 20000);
//  TIM2_OC1Init(TIM2_OCMODE_PWM1,TIM2_OUTPUTSTATE_ENABLE,0,TIM2_OCPOLARITY_HIGH);
//  
//  /*        
//  TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 5, TIM2_OCPOLARITY_HIGH);
//  TIM2_OC2PreloadConfig(ENABLE);
//  */
//  
//  TIM2_ARRPreloadConfig(ENABLE);
//  
//  TIM2_Cmd(ENABLE);
//  //Delayms(1);
//  //Calc_AC_Freqency();
////---------------------------------------------------

//New TIM2 Routine
//---------------------------------------------------
// configure tim1
TIM2_DeInit();
TIM2->CR1 = 1; // enable counter
TIM2->PSCR = TIM2_PRESCALER_8; // divide by 8

TIM2->CCMR3 = 0xF1; // enable input capture for channel 3

TIM2->CCER2=1; // enable IC3

TIM2->IER = b3 + b1;

//TIM1_CCMR1 = 0x10; // enable output compare for channel 1
TIM2->CCR1H = 0x80;
TIM2->CCR1L = 0x00;
 
TIM2_Cmd(ENABLE);
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
  
  pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR , K_D * SCALING_FACTOR , &pidData);
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
//#pragma optimize=none
void HotAir_ISR (void)  
{
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
        //Power = pid(tSet, Temperature);
        Power = pid_Controller(tSet, Temperature, &pidData);
        if (Temperature > 450) Power = 0;   
        Triac_angle = Power/2;
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
        Triac_angle = 0;
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
     
//     if ((!display_setpoint_timeout) && (!display_type_timeout)) {
//       if (old_Temperature != Temperature) {
//       ssegWriteInt(Temperature); 
//       old_Temperature = Temperature;
//       }      
//     }
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
        ssegWriteInt(100);
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
     if ((!display_setpoint_timeout) && (!display_type_timeout)) {
       if (old_Temperature != Temperature) {
       ssegWriteInt(Temperature); 
       old_Temperature = Temperature;
       }      
     }
    
    
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
        tSet = 0;
        Power = 0;
        Triac_angle = 0;
        FAN_SET_PWM_DUTY(100);
        pid_Reset_Integrator(&pidData);
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
