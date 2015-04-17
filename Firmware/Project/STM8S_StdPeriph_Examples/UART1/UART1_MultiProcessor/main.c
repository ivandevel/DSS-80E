/**
  ******************************************************************************
  * @file UART1_MultiProcessor\main.c
  * @brief This file contains the main function for:use the UART1 in multi-processor mode example.
  * @author  MCD Application Team
  * @version  V2.0.1
  * @date     18-November-2011
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
  * @addtogroup UART1_MultiProcessor
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void GPIO_Config(void);
static void UART_Config(void);
void Delay(uint32_t nCount);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* GPIO configuration -----------------------------------------*/
  GPIO_Config();

  /* UART configuration -----------------------------------------*/
  UART_Config();  
    
  while (1)
  {
      /* Send one byte from UART1 to UART3 */
      UART1_SendData9(0x33);
      
      /* Wait while UART1 TXE = 0 */
      while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
      {
      }
      if (UART3_GetFlagStatus(UART3_FLAG_RXNE) != RESET)
      {
          if (UART3_ReceiveData9() == 0x33)
          {
            STM_EVAL_LEDToggle(LED1);
            STM_EVAL_LEDToggle(LED2);
            STM_EVAL_LEDToggle(LED3);
            STM_EVAL_LEDToggle(LED4);
            Delay(0x5FF);
          }
      }
  }
}

/**
  * @brief  Configure GPIO for Leds and buttons available on the evaluation board  
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* Initialize I/Os in Output Mode for LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* Put all Leds On*/
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
  
  /* Initialize I/O for Key button*/
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);
  
  /* Initialize I/O for Joystick Up*/
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_EXTI);  
  
  enableInterrupts();  
}

/**
  * @brief  UART1 and UART3 Configuration for multi processor communication
  * @param  None
  * @retval None
  */
static void UART_Config(void)
{
  UART1_DeInit();
  /* UART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 9 Bits
        - One Stop Bit
        - No parity
        - Receive and Transmit enabled
        - UART1 Clock disabled
  */
  UART1_Init((uint32_t)9600, UART1_WORDLENGTH_9D, UART1_STOPBITS_1, UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  /* Set the UART1 Address */
  UART1_SetAddress(0x1);
  
  UART3_DeInit();
  /* UART3 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 9 Bits
        - One Stop Bit
        - No parity
        - Receive and transmit enabled
  */
  UART3_Init((uint32_t)9600, UART3_WORDLENGTH_9D, UART3_STOPBITS_1, UART3_PARITY_NO,
              UART3_MODE_TXRX_ENABLE);

  /* Set the UART3 Address */
  UART3_SetAddress(0x2);

  /* Select the UART3 WakeUp Method */
  UART3_WakeUpConfig(UART3_WAKEUP_ADDRESSMARK);
}

/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay(uint32_t nCount)
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