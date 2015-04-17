/**
  ******************************************************************************
  * @file    ITC_SoftwarePriority\main.c
  * @author  MCD Application Team
  * @version  V2.0.1
  * @date     18-November-2011
  * @brief   This file contains the main function for the ITC software priority example.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_eval.h"

/**
  * @addtogroup ITC_SoftwarePriority
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void GPIO_Config(void);
void ITC_Config(void);
void Delay (uint16_t nCount);
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief Example main entry point.
  * @param  None
  * @retval None
  */
void main(void)
{
  
    /* GPIO Configuration  -----------------------------------------*/
    GPIO_Config();  
    
    /* ITC Configuration  -----------------------------------------*/
    ITC_Config();  

  while (1)
  {}
}

/**
  * @brief  Configure GPIO for buttons and LEDs available on the evaluation board
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
  
  /* Configure GPIO to drive LEDs on Eval board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  
  /* LED1 & LED2 are on oer default */
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  
  /* Configure GPIO used to drive the joystick Down and the Key button */
    STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);
    STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_EXTI);
}

/**
  * @brief  Change the software priority between Key button and joystick Down
  * @param  None
  * @retval None
  */
void ITC_Config(void)
{
  /*
    Change the software priority between button and joystick:
    - By hardware the PORTB (joystick) has a higher priority than PORTC (button)
    - We configure the PORTC (button) to have a higher sw priority than PORTB (joystick)
  */
  ITC_SetSoftwarePriority(ITC_IRQ_PORTB, ITC_PRIORITYLEVEL_1); /* joystick Down*/
  ITC_SetSoftwarePriority(ITC_IRQ_PORTC, ITC_PRIORITYLEVEL_2); /* button = higher sw priority */
  
  enableInterrupts();
}


/**
  * @brief Delay.
  * @param nCount
  * @retval None
   */
void Delay(uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
