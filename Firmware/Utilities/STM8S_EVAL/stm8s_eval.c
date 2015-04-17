/**
  ******************************************************************************
  * @file    stm8s_eval.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    25-February-2011
  * @brief   This file provides firmware functions to manage Leds, push-buttons
  *          and COM ports available on STM8S Evaluation Boards from STMicroelectronics.
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
#include "stm8s_eval.h"

/** @addtogroup Utilities
  * @{
  */

/** @defgroup STM8S_EVAL
  * @brief This file provides firmware functions to manage Leds, push-buttons
  *        and COM ports available on STM8 Evaluation Boards from STMicroelectronics.
  * @{
  */

/** @defgroup STM8S_EVAL_Abstraction_Layer
  * @{
  */

#ifdef USE_STM8_128_EVAL
 #include "stm8-128_eval/stm8_128_eval.c"
#else
 #error "Please select first the STM8 EVAL board to be used (in stm8s_eval.h)"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function ----------------------------------------------------------*/

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