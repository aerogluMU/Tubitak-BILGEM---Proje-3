/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
static Message_InitTypeDef msg;
static Message_FlagTypeDef msgFlag;
static uint16_t count = 0;
/* USER CODE END 0 */

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX
  */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART2_IRQn);

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_Enable(USART2);
  /* USER CODE BEGIN USART2_Init 2 */
  Flag_Clear(&msgFlag);
  /* USER CODE END USART2_Init 2 */

}

/* USER CODE BEGIN 1 */
void Flag_Clear(Message_FlagTypeDef *msgFlag){
	msgFlag->flag_data = FLAG_RESET;
	msgFlag->flag_lenght1stbyte = FLAG_RESET;
	msgFlag->flag_lenght2ndbyte = FLAG_RESET;
	msgFlag->flag_reserved = FLAG_RESET;
	msgFlag->flag_start1stbyte = FLAG_RESET;
	msgFlag->flag_start2ndbyte = FLAG_RESET;
	msgFlag->flag_stop1stbyte = FLAG_RESET;
	msgFlag->flag_stop2ndbyte = FLAG_RESET;
	msgFlag->flag_type = FLAG_RESET;
}

void Message_Clear(Message_InitTypeDef *msg){
	msg->start = '\0';
	msg->lenght = '\0';
	msg->type = '\0';
	msg->reserved = '\0';
	msg->stop = '\0';
	free(msg->data);
	msg->led_On_State = FLAG_RESET;
	msg->led_Off_State = FLAG_RESET;
	msg->led_Blink_State = FLAG_RESET;
	msg->led_PWM_State = FLAG_RESET;
}

void USART_Rx_Callback(void){
	uint8_t receiveData;
	receiveData = LL_USART_ReceiveData8(USART2);
	if (msgFlag.flag_start1stbyte == FLAG_RESET && receiveData == 0xFE) {
		msg.start = 0xFFFF & (receiveData << 8);
		msgFlag.flag_start1stbyte = FLAG_SET;
	}
	else if (msgFlag.flag_start2ndbyte == FLAG_RESET && receiveData == 0xFD) {
		msg.start = msg.start | (0xFFFF & receiveData);
		msgFlag.flag_start2ndbyte = FLAG_SET;
	}
	else if (msgFlag.flag_start1stbyte == FLAG_SET && \
			msgFlag.flag_start2ndbyte == FLAG_SET && \
			msgFlag.flag_lenght1stbyte == FLAG_RESET) {
		msg.lenght = 0xFFFF & (receiveData << 8);
		msgFlag.flag_lenght1stbyte = FLAG_SET;
	}
	else if (msgFlag.flag_lenght1stbyte == FLAG_SET && \
			msgFlag.flag_lenght2ndbyte == FLAG_RESET) {
		msg.lenght = msg.lenght | (0xFFFF & receiveData);
		msgFlag.flag_lenght2ndbyte = FLAG_SET;
	}
	else if (msgFlag.flag_lenght2ndbyte == FLAG_SET && \
			msgFlag.flag_type == FLAG_RESET) {
		msg.type = receiveData;
		msgFlag.flag_type = FLAG_SET;
		if(msg.lenght == 0){
			msgFlag.flag_data = FLAG_SET;
		}
	}
	else if (msgFlag.flag_type == FLAG_SET && \
			msgFlag.flag_data == FLAG_RESET) {
		if (count == 0) {
			msg.data = (uint8_t*)calloc(msg.lenght,sizeof(uint8_t));
			msg.data[count++] = receiveData;
		}
		else if(msg.lenght >= count) {
			msg.data[count++] = receiveData;
		}
		if (msg.lenght == count) {
			count = 0;
			msgFlag.flag_data = FLAG_SET;
		}
	}
	else if (msgFlag.flag_data == FLAG_SET && \
			receiveData == 0x00 && \
			msgFlag.flag_reserved == FLAG_RESET) {
		msg.reserved = receiveData;
		msgFlag.flag_reserved = FLAG_SET;
	}
	else if (msgFlag.flag_reserved == FLAG_SET && \
			receiveData == 0xFD && \
			msgFlag.flag_stop1stbyte == FLAG_RESET) {
		msg.stop = 0xFFFF & (receiveData << 8);
		msgFlag.flag_stop1stbyte = FLAG_SET;
	}
	else if (msgFlag.flag_stop1stbyte == FLAG_SET && \
			receiveData == 0xFE && \
			msgFlag.flag_stop2ndbyte == FLAG_RESET) {
		msg.stop = msg.stop | (0xFFFF & receiveData);
		msgFlag.flag_stop2ndbyte = FLAG_SET;
	}
	else{
		Flag_Clear(&msgFlag);
		Message_Clear(&msg);
	}
	Message_Check(&msg, &msgFlag);
	LL_USART_TransmitData8(USART2, receiveData);
}

void Message_Check(Message_InitTypeDef *msg, Message_FlagTypeDef *msgFlag){
	if(msgFlag->flag_start1stbyte == FLAG_SET && \
			msgFlag->flag_start2ndbyte == FLAG_SET && \
			msgFlag->flag_lenght1stbyte == FLAG_SET && \
			msgFlag->flag_lenght2ndbyte == FLAG_SET && \
			msgFlag->flag_type == FLAG_SET && \
			msgFlag->flag_data == FLAG_SET && \
			msgFlag->flag_reserved == FLAG_SET && \
			msgFlag->flag_stop1stbyte == FLAG_SET && \
			msgFlag->flag_stop2ndbyte == FLAG_SET){
		if(msg->type == 0x01){
			if(msg->data == 0x00){
				msg->led_Off_State = FLAG_SET;
				msg->led_On_State = FLAG_RESET;
				msg->led_Blink_State = FLAG_RESET;
				msg->led_PWM_State = FLAG_RESET;
			}
			else if(msg->data[0] == 0x01){
				msg->led_Off_State = FLAG_RESET;
				msg->led_On_State = FLAG_SET;
				msg->led_Blink_State = FLAG_RESET;
				msg->led_PWM_State = FLAG_RESET;
			}
		}
		else if(msg->type == 0x02){
			msg->led_Off_State = FLAG_RESET;
			msg->led_On_State = FLAG_RESET;
			msg->led_Blink_State = FLAG_SET;
			msg->led_PWM_State = FLAG_RESET;
		}
		else if(msg->type == 0x03){
			msg->led_Off_State = FLAG_RESET;
			msg->led_On_State = FLAG_RESET;
			msg->led_Blink_State = FLAG_RESET;
			msg->led_PWM_State = FLAG_SET;
		}
		uint8_t temp[sizeof(msg)];
		memcpy(temp,&msg,sizeof(msg));
		for(int i = 0 ; i < sizeof(msg) ; i++) {
			LL_USART_TransmitData8(USART2, temp[i]);
		}
		Flag_Clear(msgFlag);
		Message_Clear(msg);
	}
}
/* USER CODE END 1 */
