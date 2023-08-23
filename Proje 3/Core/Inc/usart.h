/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
typedef struct{
	uint16_t start;
	uint16_t lenght;
	uint8_t type;
	uint8_t *data;
	uint8_t reserved;
	uint16_t stop;
	uint8_t led_On_State:1;
	uint8_t led_Off_State:1;
	uint8_t led_Blink_State:1;
	uint8_t led_PWM_State:1;
}Message_InitTypeDef;

typedef struct{
	uint8_t flag_start1stbyte:1;
	uint8_t flag_start2ndbyte:1;
	uint8_t flag_lenght1stbyte:1;
	uint8_t flag_lenght2ndbyte:1;
	uint8_t flag_type:1;
	uint8_t flag_data:1;
	uint8_t flag_reserved:1;
	uint8_t flag_stop1stbyte:1;
	uint8_t flag_stop2ndbyte:1;
}Message_FlagTypeDef;
/* USER CODE END Private defines */

void MX_USART2_UART_Init(void);
void Flag_Clear(Message_FlagTypeDef *msgFlag);
void Message_Clear(Message_InitTypeDef *msg);
void Message_Check(Message_InitTypeDef *msg, Message_FlagTypeDef *msgFlag);
void USART_Rx_Callback(void);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

/* USER CODE BEGIN Macros */
#define FLAG_SET				1
#define FLAG_RESET				0
/* USER CODE END Macros */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

