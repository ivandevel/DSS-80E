/**
  ******************************************************************************
  * @file    stm8_128_eval.c
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
#include "stm8_128_eval.h"
#include "stm8s_spi.h"
#include "stm8s_i2c.h"
#include "stm8s_clk.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM8S_EVAL
  * @{
  */

/** @addtogroup STM8_128_EVAL
  * @{
  */

/** @defgroup STM8_128_EVAL_LOW_LEVEL
  * @brief This file provides firmware functions to manage Leds, push-buttons,
  *        COM ports, SD card on SPI and EEPROM (sEE) available on STM8_128-EVAL
  *        evaluation board from STMicroelectronics.
  * @{
  */

/* Private types definition --------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/


/** @defgroup STM8_128_EVAL_LOW_LEVEL_Private_Variables
  * @{
  */
GPIO_TypeDef* LED_PORT[LEDn] =	{LED_A_GPIO_PORT, LED_B_GPIO_PORT, LED_C_GPIO_PORT,
									 LED_D_GPIO_PORT, LED_E_GPIO_PORT, LED_F_GPIO_PORT,
									 LED_G_GPIO_PORT, LED_H_GPIO_PORT
									 };

const uint8_t LED_PIN[LEDn] = {LED_A_GPIO_PIN, LED_B_GPIO_PIN, LED_C_GPIO_PIN,
									 LED_D_GPIO_PIN, LED_E_GPIO_PIN, LED_F_GPIO_PIN,
									 LED_G_GPIO_PIN, LED_H_GPIO_PIN
                                                                         };

GPIO_TypeDef* SEG_PORT[LEDn] =	{SEG1_GPIO_PORT, SEG2_GPIO_PORT, SEG3_GPIO_PORT};

const uint8_t SEG_PIN[LEDn] = {SEG1_GPIO_PIN, SEG2_GPIO_PIN, SEG3_GPIO_PIN};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] =
  {
    ENC_KEY_BUTTON_PORT, ENC_UP_BUTTON_PORT, ENC_DN_BUTTON_PORT
  };
const uint8_t BUTTON_PIN[BUTTONn] =
  {
    ENC_KEY_BUTTON_PIN, ENC_UP_BUTTON_PIN, ENC_DN_BUTTON_PIN
  };

const uint8_t BUTTON_EXTI[BUTTONn] =
  {
    KEY_BUTTON_EXTI_PORT, UP_BUTTON_EXTI_PORT, DN_BUTTON_EXTI_PORT
  };

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/** @defgroup STM8_128_EVAL_LOW_LEVEL_Private_Functions
  * @{
  */
/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_SEGInit(Seg_TypeDef Seg)
{
  /* Configure the GPIO_LED pin */
  GPIO_Init(SEG_PORT[Seg], (GPIO_Pin_TypeDef)SEG_PIN[Seg], GPIO_MODE_OUT_PP_HIGH_FAST);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_SEGOn(Seg_TypeDef Seg)
{
  SEG_PORT[Seg]->ODR &= (uint8_t)~SEG_PIN[Seg];
  //SEG_PORT[Seg]->ODR |= (uint8_t)SEG_PIN[Seg];
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_SEGOff(Seg_TypeDef Seg)
{
  SEG_PORT[Seg]->ODR |= (uint8_t)SEG_PIN[Seg];
  //SEG_PORT[Seg]->ODR &= (uint8_t)~SEG_PIN[Seg];
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
  /* Configure the GPIO_LED pin */
  GPIO_Init(LED_PORT[Led], (GPIO_Pin_TypeDef)LED_PIN[Led], GPIO_MODE_OUT_PP_HIGH_FAST);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
  LED_PORT[Led]->ODR &= (uint8_t)~LED_PIN[Led];
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
  LED_PORT[Led]->ODR |= (uint8_t)LED_PIN[Led];
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
  LED_PORT[Led]->ODR ^= (uint8_t)LED_PIN[Led];
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_KEY: Key Push Button
  *     @arg BUTTON_RIGHT: Joystick Right Push Button
  *     @arg BUTTON_LEFT: Joystick Left Push Button
  *     @arg BUTTON_UP: Joystick Up Push Button
  *     @arg BUTTON_DOWN: Joystick Down Push Button
  *     @arg BUTTON_SEL: Joystick Sel Push Button
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI pin (sensitive to falling trigger)
  * @retval None
  */
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{

  if (Button_Mode == BUTTON_MODE_EXTI)
  { /* Pin configured in input floating mode with interrupt enabled
                       --> connected to EXTIx Interrupt, where x:0..7 */
    GPIO_Init(BUTTON_PORT[Button], (GPIO_Pin_TypeDef)BUTTON_PIN[Button], GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity((EXTI_Port_TypeDef)BUTTON_EXTI[Button], EXTI_SENSITIVITY_RISE_FALL);
  }
  else
  { /* Pin configured in input floating mode with interrupt disabled */
    GPIO_Init(BUTTON_PORT[Button], (GPIO_Pin_TypeDef)BUTTON_PIN[Button], GPIO_MODE_IN_PU_NO_IT);
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_KEY: Key Push Button
  *     @arg BUTTON_RIGHT: Joystick Right Push Button
  *     @arg BUTTON_LEFT: Joystick Left Push Button
  *     @arg BUTTON_UP: Joystick Up Push Button
  *     @arg BUTTON_DOWN: Joystick Down Push Button
  *     @arg BUTTON_SEL: Joystick Sel Push Button
  * @retval The Button GPIO pin value.
  */
uint8_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
  return GPIO_ReadInputPin(BUTTON_PORT[Button], (GPIO_Pin_TypeDef)BUTTON_PIN[Button]) ? 1 : 0;
}

/**
  * @}
  */

/**
  * @}
  */

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
