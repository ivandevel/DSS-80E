/**
  ******************************************************************************
  * @file    SPI_SimplexLCD\main.c
  * @author  MCD Application Team
  * @version  V2.0.1
  * @date     18-November-2011
  * @brief   This file contains the main function for SPI simplex communication with LCD example.
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
#include "stm8s_eval_lcd.h"

/**
  * @addtogroup SPI_SimplexLCD
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void PrintLogo(void);
void Delay(uint32_t nCount);
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint8_t i = 0;
  
  /* Select fCPU = 16MHz */
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  
  /* Init the Eval board LCD */
  STM8S_EVAL_LCD_Init();

  /* Clear LCD */
  LCD_Clear();

  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print("SPI-LCD example");		
  
  /* Display "moving" ST logo */
  for (i = 1; i < 8; i++)
  {
    PrintLogo();
    Delay(0xFFFF);
  }
  
  LCD_SetCursorPos(LCD_LINE2, 0);
  LCD_Print("               ");

  LCD_DisplayLogo(0x93);
  Delay(600);
  while(1)
  {}

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

/**
  * @brief  Print ST logo on the LCD
  * @param  None
  * @retval None
  */
void PrintLogo(void)
{
  static uint8_t logo_pos = 0x90;
  static int8_t logo_pos_index = 1;

  LCD_SetCursorPos(LCD_LINE2, 0);
  LCD_Print("               ");

  LCD_DisplayLogo(logo_pos);
  logo_pos += logo_pos_index;
  if (logo_pos > 0x95)
  {
    logo_pos -= 2;
    logo_pos_index = -1;
  }
  if (logo_pos < 0x90)
  {
    logo_pos += 2;
    logo_pos_index = 1;
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
