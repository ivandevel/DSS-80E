/**
  ******************************************************************************
  * @file    stm8_128_eval.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    25-February-2011
  * @brief   This file contains definitions for STM8_128_EVAL's Leds, push-buttons
  *          and COM ports hardware resources.
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
#ifndef __STM8_128_EVAL_H
#define __STM8_128_EVAL_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s_eval.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM8S_EVAL
  * @{
  */

/** @addtogroup STM8_128_EVAL
  * @{
  */
/** @addtogroup STM8_128_EVAL_LOW_LEVEL
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup STM8_128_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */
/** @addtogroup STM8_128_EVAL_LOW_LEVEL_LED
  * @{
  */
#ifdef SOLDERING_VAR1
#define LEDn                         8
#define LED_A_GPIO_PORT              GPIOA
#define LED_A_GPIO_PIN               GPIO_PIN_2

#define LED_B_GPIO_PORT              GPIOC
#define LED_B_GPIO_PIN               GPIO_PIN_4
    
#define LED_C_GPIO_PORT              GPIOD
#define LED_C_GPIO_PIN               GPIO_PIN_5
    
#define LED_D_GPIO_PORT              GPIOC
#define LED_D_GPIO_PIN               GPIO_PIN_5
    
#define LED_E_GPIO_PORT              GPIOC
#define LED_E_GPIO_PIN               GPIO_PIN_6
    
#define LED_F_GPIO_PORT              GPIOA
#define LED_F_GPIO_PIN               GPIO_PIN_1
    
#define LED_G_GPIO_PORT              GPIOD
#define LED_G_GPIO_PIN               GPIO_PIN_4
    
#define LED_H_GPIO_PORT              GPIOD
#define LED_H_GPIO_PIN               GPIO_PIN_6

#define SEG1_GPIO_PORT               GPIOA
#define SEG1_GPIO_PIN                GPIO_PIN_3
    
#define SEG2_GPIO_PORT               GPIOC
#define SEG2_GPIO_PIN                GPIO_PIN_3
    
#define SEG3_GPIO_PORT               GPIOC
#define SEG3_GPIO_PIN                GPIO_PIN_7

#define CONTROL_GPIO_PORT            GPIOD
#define CONTROL_GPIO_PIN             GPIO_PIN_2

#define ADC_GPIO_PORT                GPIOD
#define ADC_GPIO_PIN                 GPIO_PIN_3

#define ADC_SOLDER_TEMP_CHANNEL      ADC1_CHANNEL_4
/**
  * @}
  */

/** @addtogroup STM8_128_EVAL_LOW_LEVEL_BUTTON
  * @{
  */
#define BUTTONn                     3

/**
 * @brief Key push-button
 */
#define ENC_KEY_BUTTON_PORT            GPIOD
#define ENC_KEY_BUTTON_PIN             GPIO_PIN_1

#define ENC_UP_BUTTON_PORT             GPIOB
#define ENC_UP_BUTTON_PIN              GPIO_PIN_4

#define ENC_DN_BUTTON_PORT             GPIOB
#define ENC_DN_BUTTON_PIN              GPIO_PIN_5

/**
 * @brief EXTI push-button definition
 */
#define KEY_BUTTON_EXTI_PORT       EXTI_PORT_GPIOD
#define UP_BUTTON_EXTI_PORT        EXTI_PORT_GPIOB
#define DN_BUTTON_EXTI_PORT        EXTI_PORT_GPIOB

//----------------------------------------------------------------------------------
#elif SOLDERING_VAR2

#define DIGIT_POLARITY_HIGH
    
#define LEDn                         7
#define LED_A_GPIO_PORT              GPIOC
#define LED_A_GPIO_PIN               GPIO_PIN_4

#define LED_B_GPIO_PORT              GPIOC
#define LED_B_GPIO_PIN               GPIO_PIN_3
    
#define LED_C_GPIO_PORT              GPIOC
#define LED_C_GPIO_PIN               GPIO_PIN_7
    
#define LED_D_GPIO_PORT              GPIOD
#define LED_D_GPIO_PIN               GPIO_PIN_5
    
#define LED_E_GPIO_PORT              GPIOD
#define LED_E_GPIO_PIN               GPIO_PIN_4
    
#define LED_F_GPIO_PORT              GPIOC
#define LED_F_GPIO_PIN               GPIO_PIN_6
    
#define LED_G_GPIO_PORT              GPIOC
#define LED_G_GPIO_PIN               GPIO_PIN_5
    
//#define LED_H_GPIO_PORT              GPIOC
//#define LED_H_GPIO_PIN               GPIO_PIN_5

#define SEG1_GPIO_PORT               GPIOA
#define SEG1_GPIO_PIN                GPIO_PIN_3
    
#define SEG2_GPIO_PORT               GPIOD
#define SEG2_GPIO_PIN                GPIO_PIN_2
    
#define SEG3_GPIO_PORT               GPIOD
#define SEG3_GPIO_PIN                GPIO_PIN_3
   
#define CONTROL_GPIO_PORT            GPIOA
#define CONTROL_GPIO_PIN             GPIO_PIN_1

#define ADC_GPIO_PORT                GPIOD
#define ADC_GPIO_PIN                 GPIO_PIN_6

#define ADC_SOLDER_TEMP_CHANNEL ADC1_CHANNEL_6
/**
  * @}
  */

/** @addtogroup STM8_128_EVAL_LOW_LEVEL_BUTTON
  * @{
  */
#define BUTTONn                     3

/**
 * @brief Key push-button
 */
#define ENC_KEY_BUTTON_PORT            GPIOA
#define ENC_KEY_BUTTON_PIN             GPIO_PIN_2

#define ENC_DN_BUTTON_PORT             GPIOB
#define ENC_DN_BUTTON_PIN              GPIO_PIN_4

#define ENC_UP_BUTTON_PORT             GPIOB
#define ENC_UP_BUTTON_PIN              GPIO_PIN_5

/**
 * @brief EXTI push-button definition
 */
#define KEY_BUTTON_EXTI_PORT       EXTI_PORT_GPIOA
#define UP_BUTTON_EXTI_PORT        EXTI_PORT_GPIOB
#define DN_BUTTON_EXTI_PORT        EXTI_PORT_GPIOB

//----------------------------------------------------------------------------------
#elif SOLDERING_HAKKO_A1321_MINI

#define DIGIT_POLARITY_LOW
    
#define LEDn                         7
   
#define LED_A_GPIO_PORT              GPIOA
#define LED_A_GPIO_PIN               GPIO_PIN_1

#define LED_B_GPIO_PORT              GPIOC
#define LED_B_GPIO_PIN               GPIO_PIN_3
    
#define LED_C_GPIO_PORT              GPIOC
#define LED_C_GPIO_PIN               GPIO_PIN_7
    
#define LED_D_GPIO_PORT              GPIOD
#define LED_D_GPIO_PIN               GPIO_PIN_3
    
#define LED_E_GPIO_PORT              GPIOA
#define LED_E_GPIO_PIN               GPIO_PIN_3
    
#define LED_F_GPIO_PORT              GPIOA
#define LED_F_GPIO_PIN               GPIO_PIN_2
    
#define LED_G_GPIO_PORT              GPIOC
#define LED_G_GPIO_PIN               GPIO_PIN_6
    
//#define LED_H_GPIO_PORT              GPIOD
//#define LED_H_GPIO_PIN               GPIO_PIN_2

#define SEG1_GPIO_PORT               GPIOD
#define SEG1_GPIO_PIN                GPIO_PIN_6
    
#define SEG2_GPIO_PORT               GPIOC
#define SEG2_GPIO_PIN                GPIO_PIN_5
    
#define SEG3_GPIO_PORT               GPIOC
#define SEG3_GPIO_PIN                GPIO_PIN_4

#define CONTROL_GPIO_PORT            GPIOD
#define CONTROL_GPIO_PIN             GPIO_PIN_4

#define ADC_GPIO_PORT                GPIOD
#define ADC_GPIO_PIN                 GPIO_PIN_5

#define ADC_SOLDER_TEMP_CHANNEL      ADC1_CHANNEL_5
/**
  * @}
  */

/** @addtogroup STM8_128_EVAL_LOW_LEVEL_BUTTON
  * @{
  */
#define BUTTONn                     3

/**
 * @brief Key push-button
 */
#define ENC_KEY_BUTTON_PORT            GPIOD
#define ENC_KEY_BUTTON_PIN             GPIO_PIN_1

#define ENC_DN_BUTTON_PORT             GPIOB
#define ENC_DN_BUTTON_PIN              GPIO_PIN_4

#define ENC_UP_BUTTON_PORT             GPIOB
#define ENC_UP_BUTTON_PIN              GPIO_PIN_5

/**
 * @brief EXTI push-button definition
 */
#define KEY_BUTTON_EXTI_PORT       EXTI_PORT_GPIOD
#define UP_BUTTON_EXTI_PORT        EXTI_PORT_GPIOB
#define DN_BUTTON_EXTI_PORT        EXTI_PORT_GPIOB

//----------------------------------------------------------------------------------
#elif DFS_90

extern __IO int16_t Triac_angle;
    
#define SEGMENT_POLARITY_HIGH
#define ELEMENT_POLARITY_LOW

#define LEDn                         8
   
#define LED_A_GPIO_PORT              GPIOA
#define LED_A_GPIO_PIN               GPIO_PIN_1

#define LED_B_GPIO_PORT              GPIOC
#define LED_B_GPIO_PIN               GPIO_PIN_4
    
#define LED_C_GPIO_PORT              GPIOD
#define LED_C_GPIO_PIN               GPIO_PIN_5
    
#define LED_D_GPIO_PORT              GPIOC
#define LED_D_GPIO_PIN               GPIO_PIN_5
    
#define LED_E_GPIO_PORT              GPIOC
#define LED_E_GPIO_PIN               GPIO_PIN_6
    
#define LED_F_GPIO_PORT              GPIOC
#define LED_F_GPIO_PIN               GPIO_PIN_3
    
#define LED_G_GPIO_PORT              GPIOD
#define LED_G_GPIO_PIN               GPIO_PIN_7
    
#define LED_H_GPIO_PORT              GPIOD
#define LED_H_GPIO_PIN               GPIO_PIN_6

#define SEG1_GPIO_PORT               GPIOA
#define SEG1_GPIO_PIN                GPIO_PIN_2
    
#define SEG2_GPIO_PORT               GPIOF
#define SEG2_GPIO_PIN                GPIO_PIN_4
    
#define SEG3_GPIO_PORT               GPIOC
#define SEG3_GPIO_PIN                GPIO_PIN_7

#define CONTROL_GPIO_PORT            GPIOB
#define CONTROL_GPIO_PIN             GPIO_PIN_3

#define ADC_GPIO_PORT                GPIOB
#define ADC_GPIO_PIN                 GPIO_PIN_1

#define ADC_SOLDER_TEMP_CHANNEL      ADC1_CHANNEL_1

#define BUTTONn                     4

#define ENC_KEY_BUTTON_PORT            GPIOB
#define ENC_KEY_BUTTON_PIN             GPIO_PIN_3

#define ENC_DN_BUTTON_PORT             GPIOD
#define ENC_DN_BUTTON_PIN              GPIO_PIN_0

#define ENC_UP_BUTTON_PORT             GPIOD
#define ENC_UP_BUTTON_PIN              GPIO_PIN_2

/**
 * @brief EXTI push-button definition
 */
#define KEY_BUTTON_EXTI_PORT            EXTI_PORT_GPIOD
#define UP_BUTTON_EXTI_PORT             EXTI_PORT_GPIOD
#define DN_BUTTON_EXTI_PORT             EXTI_PORT_GPIOD
#define REED_EXTI_PORT                  EXTI_PORT_GPIOB

#define ZERO_EXTI_PORT                  EXTI_PORT_GPIOE
#define ZERO_CROSS_PORT                 GPIOE
#define ZERO_CROSS_PIN                  GPIO_PIN_5

#define TRIAC_PORT                      GPIOD
#define TRIAC_PIN                       GPIO_PIN_4

#define REED_GPIO_PORT                  GPIOB
#define REED_GPIO_PIN                   GPIO_PIN_0

#define TRIAC_FIRE_WIDTH 500
#define UPPER_LIMIT 16500
#define LOWER_LIMIT 0
#define LEVEL_STEP  1
#define RAMP_STEP   2

//----------------------------------------------------------------------------------
#elif SOLDERING_HAKKO_A1321

#define DIGIT_POLARITY_LOW
    
#define LEDn                         8
   
#define LED_A_GPIO_PORT              GPIOA
#define LED_A_GPIO_PIN               GPIO_PIN_1

#define LED_B_GPIO_PORT              GPIOC
#define LED_B_GPIO_PIN               GPIO_PIN_4
    
#define LED_C_GPIO_PORT              GPIOD
#define LED_C_GPIO_PIN               GPIO_PIN_5
    
#define LED_D_GPIO_PORT              GPIOC
#define LED_D_GPIO_PIN               GPIO_PIN_5
    
#define LED_E_GPIO_PORT              GPIOC
#define LED_E_GPIO_PIN               GPIO_PIN_6
    
#define LED_F_GPIO_PORT              GPIOC
#define LED_F_GPIO_PIN               GPIO_PIN_3
    
#define LED_G_GPIO_PORT              GPIOD
#define LED_G_GPIO_PIN               GPIO_PIN_4
    
#define LED_H_GPIO_PORT              GPIOD
#define LED_H_GPIO_PIN               GPIO_PIN_6

#define SEG1_GPIO_PORT               GPIOA
#define SEG1_GPIO_PIN                GPIO_PIN_2
    
#define SEG2_GPIO_PORT               GPIOA
#define SEG2_GPIO_PIN                GPIO_PIN_3
    
#define SEG3_GPIO_PORT               GPIOC
#define SEG3_GPIO_PIN                GPIO_PIN_7

#define CONTROL_GPIO_PORT            GPIOD
#define CONTROL_GPIO_PIN             GPIO_PIN_2

#define ADC_GPIO_PORT                GPIOD
#define ADC_GPIO_PIN                 GPIO_PIN_3

#define ADC_SOLDER_TEMP_CHANNEL      ADC1_CHANNEL_4
/**
  * @}
  */

/** @addtogroup STM8_128_EVAL_LOW_LEVEL_BUTTON
  * @{
  */
#define BUTTONn                     3

/**
 * @brief Key push-button
 */
#define ENC_KEY_BUTTON_PORT            GPIOD
#define ENC_KEY_BUTTON_PIN             GPIO_PIN_1

#define ENC_DN_BUTTON_PORT             GPIOB
#define ENC_DN_BUTTON_PIN              GPIO_PIN_4

#define ENC_UP_BUTTON_PORT             GPIOB
#define ENC_UP_BUTTON_PIN              GPIO_PIN_5

/**
 * @brief EXTI push-button definition
 */
#define KEY_BUTTON_EXTI_PORT       EXTI_PORT_GPIOD
#define UP_BUTTON_EXTI_PORT        EXTI_PORT_GPIOB
#define DN_BUTTON_EXTI_PORT        EXTI_PORT_GPIOB

#else
 #error "Please select first the STM8S SOLDERING board to be used (in stm8s_eval.h)"
#endif
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup STM8_128_EVAL_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_EVAL_SEGInit(Seg_TypeDef Seg);
void STM_EVAL_SEGOn(Seg_TypeDef Seg);
void STM_EVAL_SEGOff(Seg_TypeDef Seg);
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint8_t STM_EVAL_PBGetState(Button_TypeDef Button);

/**
  * @}
  */

#endif /* __STM8_128_EVAL_H */

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
