/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_eval.h"
#include "station.h"    
#include "7-seg.h"
#include "button.h"
#include "pid.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
 __IO int16_t Triac_angle = 0;

/**
  * @brief  Configure TIM4 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
  */
static void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124); //1000Hz interrupt
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  /* enable interrupts */
  //enableInterrupts();

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

uint16_t GetAdcValue(ADC1_Channel_TypeDef channel)
{
  ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, channel, ADC1_ALIGN_RIGHT);
  ADC1_StartConversion();
  while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);
  ADC1_ClearFlag(ADC1_FLAG_EOC);
  return ADC1_GetConversionValue();
}

void FLASH_Config(void)
{
 /* Define flash programming Time*/
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
/*
  FLASH_Unlock(FLASH_MEMTYPE_PROG);
  // Wait until Flash Program area unlocked flag is set
  while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
  {}
*/
  /* Unlock flash data eeprom memory */
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  /* Wait until Data EEPROM area unlocked flag is set*/
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
  {}
}

void main(void)
{
  //CFG->GCR |= 0x01; //disable swim pin
  
  /* Configure the Fcpu to DIV1*/
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  
   /* select Clock = 16 MHz */
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  
  /* Configure the system clock to use HSI clock source and to run at 16Mhz */
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
  
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
  CLK_HSICmd(ENABLE);
  
  FLASH_Config();
  
//You should add:
// Define FLASH programming time 
//FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

//optbyte1 = FLASH_ReadOptionByte(0x4803); //255 by def
//optbyte2 = FLASH_ReadOptionByte(0x4804); //255 by def
/*
if (optbyte != 765)
{
FLASH_Unlock(FLASH_MEMTYPE_DATA);     // unlock data memory by passing the RASS key in the proper order
FLASH_ProgramOptionByte(0x4803, 0xFD);     // byte OPT2 resides at address x4803, write a 1 to bit 7.  This will also write to the NOPT2 complement byte
FLASH_Lock(FLASH_MEMTYPE_DATA);       // re-lock data memory
}
  */
  
  //General purpose timer
  TIM4_Config(); 

#ifdef DFS_90
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
  
#endif
  
#ifndef DFS_90
  GPIO_Init(CONTROL_GPIO_PORT, CONTROL_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
#endif
  
  Soldering_ADC_Config();  
  
  STM_EVAL_SEGInit(SEG1);
  STM_EVAL_SEGInit(SEG2);
  STM_EVAL_SEGInit(SEG3);
  
  STM_EVAL_LEDInit(LEDA);
  STM_EVAL_LEDInit(LEDB);
  STM_EVAL_LEDInit(LEDC);
  STM_EVAL_LEDInit(LEDD);
  STM_EVAL_LEDInit(LEDE);
  STM_EVAL_LEDInit(LEDF);
  STM_EVAL_LEDInit(LEDG);
  STM_EVAL_LEDInit(LEDP);

#ifndef SOLDERING_VAR2
  //STM_EVAL_LEDInit(LEDP);
#endif
//  STM_EVAL_SEGOn(SEG1);
//  STM_EVAL_SEGOn(SEG2);
//  STM_EVAL_SEGOn(SEG3);
//  
//  STM_EVAL_LEDOn(LEDA);
//  STM_EVAL_LEDOn(LEDB);
//  STM_EVAL_LEDOn(LEDC);
//  STM_EVAL_LEDOn(LEDD);
//  STM_EVAL_LEDOn(LEDE);
//  STM_EVAL_LEDOn(LEDF);
//  STM_EVAL_LEDOn(LEDG);
//  
//  STM_EVAL_LEDOff(LEDA);
//  STM_EVAL_LEDOff(LEDB);
//  STM_EVAL_LEDOff(LEDC);
//  STM_EVAL_LEDOff(LEDD);
//  STM_EVAL_LEDOff(LEDE);
//  STM_EVAL_LEDOff(LEDF);
//  STM_EVAL_LEDOff(LEDG);
//  STM_EVAL_LEDOff(LEDP);
//  
//  STM_EVAL_SEGOff(SEG1);
//  STM_EVAL_SEGOff(SEG2);
//  STM_EVAL_SEGOff(SEG3);
  
  
  //STM_EVAL_LEDOn(LEDP);
  //GPIO_Init(ENC_DN_BUTTON_PORT, ENC_DN_BUTTON_PIN, GPIO_MODE_IN_PU_NO_IT);
  //GPIO_Init(ENC_UP_BUTTON_PORT, ENC_UP_BUTTON_PIN, GPIO_MODE_IN_PU_NO_IT);
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_REED, BUTTON_MODE_GPIO);
  
  ssegInit();
  
  enableInterrupts(); 

  ssegWriteStr("1.0.0.", 3, SEG1);
  
  Soldering_Main();
  
  while(1)
  {
    /*
    #if 0
     for (level = 0; level < calUpperLimit; level +=5)
     {
       Delayms(1);
     }
     
          for (level = calUpperLimit; level > 0; level -=5)
     {
       Delayms(1);
     }
    #else   
    RawData  = GetAdcValue(ADC_SOLDER_TEMP_CHANNEL);
    SmoothData = SmoothData - (LPF_Beta * (SmoothData - RawData));
    level = ((int)SmoothData)*26;
    #endif
    */
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
