/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include <string.h>
#include "keyled.h"

uint8_t proBuffer[10] = "#H12;";
uint8_t rxBuffer[10] = "#H12;";

uint8_t rxCompleted = RESET;
uint8_t isUploadTime = 1;
//#include <stdio.h>
//#if 1
//#pragma import(__use_no_semihosting)
////标准库需要的支持函数                 
//struct __FILE {
//int handle;
//};
//FILE __stdout;
////定义_sys_exit()以避免使用半主机模式    
//void _sys_exit(int x) {
//	x = x;
//}
////重定义fputc函数 
//int fputc(int ch, FILE *f) {
//	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//		USART1->DR = (uint8_t) ch;
//	return ch;
//}
//#endif
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_8;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
	HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		rxCompleted = SET;



//		for (uint16_t i = 0; i < RX_CMD_LEN; i++) {
//			proBuffer[i] = rxBuffer[i];
//		}
		int same = 1;
		for (uint16_t i = 0; i < RX_CMD_LEN; i++) {
			if (proBuffer[i] != rxBuffer[i]) {
				same = 0;
			}
		}
		if (same == 1) {
			LED0_Toggle();
		}
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		LED1_Toggle();
	}
}

//void USART_SendData(USART_TypeDef* USARTx, uint16_t Data) {
//	USARTx->DR = (Data & (uint16_t)0x01FF);
//	while(__HAL_UART_GET_FLAG(USARTx, UART_FLAG_TXE) == RESET){} //等待发送缓冲区空才能发送下一个字符
//}

/* USER CODE BEGIN 4 */
void debugPrint(UART_HandleTypeDef *huart, char _out[]){
 	HAL_UART_Transmit(huart, (uint8_t *) _out, strlen(_out), 1000);
}

void on_UART_IDLE(UART_HandleTypeDef *huart) {

	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)) {
		return;
	}

	__HAL_UART_CLEAR_IDLEFLAG(huart);
	__HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);

	if (rxCompleted) {
		LED1_ON();
//		HAL_UART_Transmit(huart, proBuffer, sizeof(proBuffer), 5000);
//		printf("正点原子@ALIENTEK\r\n\r\n\r\n");

//		while(__HAL_UART_GET_FLAG(huart, UART_FLAG_TC)==RESET);
//		uint8_t sendBuffer[1] = "A";
//		HAL_StatusTypeDef result = HAL_UART_Transmit(huart, sendBuffer, 1, 5000);
//		if (result == HAL_OK) {
//			LED1_OFF();
//		}
//		USART_SendData(huart, proBuffer);
		debugPrint(huart, "hello");
		LED1_OFF();

//		HAL_Delay(1000);

		rxCompleted = RESET;
		HAL_UART_Receive_IT(huart, rxBuffer, RX_CMD_LEN);
	}
}

/* USER CODE END 1 */
