/**
  ******************************************************************************
  * @file    stm8s_eval.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    25-February-2011
  * @brief   Header file for stm8sl_eval.c module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_EVAL_H
#define __STM8S_EVAL_H

/* Includes ------------------------------------------------------------------*/

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM8S_EVAL
  * @{
  */

/** @defgroup STM8S_EVAL_Abstraction_Layer
  * @{
  */

/** @defgroup STM8S_EVAL_HARDWARE_RESOURCES
  * @{
  */

/**
@code
 The table below gives an overview of the hardware resources supported by each
 STM8S EVAL board.
     - LCD: Mono LCD (Glass and Serial (SPI))
     - sFLASH: serial SPI FLASH (M25Pxxx)
     - sEE: serial I2C EEPROM (M24C32, M24C64)
     - SD: SD Card memory (SPI (SD Card MODE))
  =============================================================================================+
    STM8S EVAL     | LED | Buttons  | Com Ports |    LCD    | sFLASH | sEE | TSENSOR | SD (SPI) |
  -============================================================================================+
   STM8-128-EVAL   |  5  |    6     |     é     | YES (SPI) |  YES   | YES |   No    |    YES   |
  =============================================================================================+
@endcode
*/
/**
  * @}
  */


/** @defgroup STM8S_EVAL_Exported_Types
  * @{
  */
typedef enum {
	LEDA = 0,
	LEDB,
	LEDC,
	LEDD,
	LEDE,
	LEDF,
	LEDG,
	LEDP,
	LEDn
} Led_TypeDef;

typedef enum {
	SEG1 = 0,
	SEG2,
	SEG3,
	SEGn
} Seg_TypeDef;

#define IS_SSEG(SEG)	(((SEG) == SEG1) || \
						 ((SEG) == SEG2) || \
						 ((SEG) == SEG3))

typedef enum {
	ROW1 = 0,
	ROWn
} SegRow_TypeDef;

#define IS_SSEG_ROW(ROW) (((ROW) == ROW1) || \
                           ((ROW) == ROW2) || \
                           ((ROW) == ROW3))

typedef enum
{
  BUTTON_KEY = 0,
  BUTTON_UP = 1,
  BUTTON_DOWN = 2
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum
{
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOYState_TypeDef;

/**
  * @}
  */

/** @defgroup STM8S_EVAL_Exported_Constants
  * @{
  */
/**
  * @brief  Uncomment the line corresponding to the STMicroelectronics evaluation
  *   board used in your application.
  *
  *  Tip: To avoid modifying this file each time you need to switch between these
  *       boards, you can define the board in your toolchain compiler preprocessor.
  */
#if !defined (USE_STM8_128_EVAL)
//#define USE_STM8_128_EVAL
#endif

#ifdef USE_STM8_128_EVAL
 #include "stm8s.h"
 #include "stm8-128_eval/stm8_128_eval.h"
#else
 #error "Please select first the STM8S EVAL board to be used (in stm8s_eval.h)"
#endif

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __STM8S_EVAL_H */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
