/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <string.h>
#include "stm32f0xx_hal_def.h"
#include "stm32f0xx_hal_uart.h"
#include "../Inc/stx_etx.h"
uint8_t aRxBuffer[1];
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

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
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */
  //__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  //HAL_UART_Receive_IT(&huart1,(uint8_t *)aRxBuffer, 1);
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);

	/* NOTE : This function should not be modified, when the callback is needed,
	 * the HAL_UART_RxCpltCallback can be implemented in the user file.
	 */

	uint8_t data = (huart->Instance->RDR & (uint8_t)0x00FF);

	stx_etx_queue_get_data(data);

	HAL_UART_Receive_IT(huart,(uint8_t *)aRxBuffer, 1);
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);

	/* NOTE: This function should not be modified, when the callback is needed,
		   the HAL_UART_RxHalfCpltCallback can be implemented in the user file.
	*/
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);

	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_UART_ErrorCallback can be implemented in the user file.
	*/
}

void MX_USART1_UART_String(char *txt)
{
	uint16_t size = strlen(txt);
	HAL_UART_Transmit(&huart1,(uint8_t*)txt,size,10);
	//HAL_UART_Transmit_IT(&huart1,(uint8_t*)txt,size);
}

void MX_USART1_UART_EnableIRQ(void)
{
	uint8_t aRxBuffer[1];
	HAL_UART_Receive_IT(&huart1,(uint8_t *)aRxBuffer, 1);
}

void MX_USART1_UART_Transmit(uint8_t *data,uint8_t size)
{
	HAL_UART_Transmit(&huart1,data,size,10);
}

//void MX_USART1_UART_Callback(UART_HandleTypeDef *huart)
//{
//	/* Prevent unused argument(s) compilation warning */
//	UNUSED(huart);
//
//	/* USER CODE BEGIN USART1_IRQn 0 */
//	uint8_t data;
//	uint32_t tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
//	uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);
//
//	if((tmp_flag != RESET) && (tmp_it_source != RESET))
//	{
//		data = (huart->Instance->RDR & (uint8_t)0x00FF);
//
//		__HAL_UART_CLEAR_PEFLAG(huart);
//
//	    /* Disable the UART Parity Error Interrupt and RXNE interrupt*/
//	    CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
//
//	    /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
//	    CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);
//
//		__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
//		return;
//	}
//
//	if((__HAL_UART_GET_FLAG(&huart, UART_FLAG_TXE) != RESET) &&
//	  (__HAL_UART_GET_IT_SOURCE(&huart, UART_IT_TXE) != RESET))
//	{
//		//pxMBFrameCBTransmitterEmpty();
//		return ;
//	}
//}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
