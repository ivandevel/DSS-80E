/**
  ******************************************************************************
  * @file    UART1_Interrupt\main.h
  * @author  MCD Application Team
  * @version V2.0.1
  * @date    18-November-2011
  * @brief   This file contains the external declaration of main.c file.
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
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)
#define RxBufferSize1   TxBufferSize2
#define RxBufferSize2   TxBufferSize1

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Exported functions ------------------------------------------------------- */
uint8_t GetVar_TxCounter1(void);
uint8_t IncrementVar_TxCounter1(void);
uint8_t GetVar_TxCounter2(void);
uint8_t IncrementVar_TxCounter2(void);
uint8_t GetVar_RxCounter1(void);
uint8_t IncrementVar_RxCounter1(void);
uint8_t GetVar_RxCounter2(void);
uint8_t IncrementVar_RxCounter2(void);
uint8_t GetVar_NbrOfDataToTransfer1(void);
uint8_t GetVar_NbrOfDataToTransfer2(void);
uint8_t GetVar_NbrOfDataToRead1(void);
uint8_t GetVar_NbrOfDataToRead2(void);


#endif /* __MAIN_H__ */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
