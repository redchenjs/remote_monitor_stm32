/*
 * usart.c
 *
 *  Created on: 2017年8月10日
 *      Author: redchenjs
 */
#include "stm32f7xx_hal.h"

#include "string.h"

#include "device/usart.h"

ring_buffer_t usart3_rx_buffer;
extern UART_HandleTypeDef huart3;

void usart3_receive_init(void)
{
	/* Once the WiFi UART is intialized, start an asynchrounous recursive
	listening. the HAL_UART_Receive_IT() call below will wait until one char is
	received to trigger the HAL_UART_RxCpltCallback(). The latter will recursively
	call the former to read another char.  */
	usart3_rx_buffer.head = 0;
	usart3_rx_buffer.tail = 0;

	HAL_UART_IRQHandler(&huart3);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_ERR);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
	HAL_UART_Receive_IT(&huart3, (uint8_t *)&usart3_rx_buffer.data[usart3_rx_buffer.tail], 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	/* If ring buffer end is reached reset tail pointer to start of buffer */
	if(++usart3_rx_buffer.tail >= RING_BUFFER_SIZE) {
		usart3_rx_buffer.tail = 0;
	}

	HAL_UART_Receive_IT(UartHandle, (uint8_t *)&usart3_rx_buffer.data[usart3_rx_buffer.tail], 1);
}
