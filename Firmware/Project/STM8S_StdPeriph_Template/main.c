/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_eval.h"
#include "thermo.h" 
#include "station.h"
#include "hotair.h"
#include "7-seg.h"
#include "button.h"
#include "pid.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

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
  
  EXTI_DeInit();
  
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
  HotAir_Config();
  #endif
  
  #ifndef DFS_90
  GPIO_Init(CONTROL_GPIO_PORT, CONTROL_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  #endif
  
  Temperature_ADC_Config();  
  
#ifndef SOLDERING_VAR2
  //STM_EVAL_LEDInit(LEDP);
#endif

  GPIO_Init(ENC_KEY_BUTTON_PORT, ENC_KEY_BUTTON_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(ENC_DN_BUTTON_PORT, ENC_DN_BUTTON_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(ENC_UP_BUTTON_PORT, ENC_UP_BUTTON_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(REED_GPIO_PORT, REED_GPIO_PIN, GPIO_MODE_IN_PU_NO_IT);  
      
  ssegInit();
  
  ssegWriteStr("1.0.1", 5, SEG1);
  
  enableInterrupts();
  
  #ifdef DFS_90
  HotAir_Main();
  #endif
  
  #ifdef SOLDERING_HAKKO_A1321
  Soldering_Main();
  #endif
  
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
