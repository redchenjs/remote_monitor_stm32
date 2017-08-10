#include "stm32f7xx_hal.h"

#include "device/usart.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define AT_OK_STRING            "OK\r\n"
#define AT_IPD_OK_STRING        "OK\r\n\r\n"
#define AT_SEND_OK_STRING       "SEND OK\r\n"
#define AT_SEND_PROMPT_STRING   "OK\r\n>"
#define AT_ERROR_STRING         "ERROR\r\n"
#define AT_IPD_STRING           "+IPD,"

#define AT_CIPMUX_STRING        "AT+CIPMUX=1\r\n"
#define AT_CIPSERVER_STRING     "AT+CIPSERVER=1,10001\r\n"
#define AT_CIPSTO_STRING        "AT+CIPSTO=10\r\n"

extern ring_buffer_t usart3_rx_buffer;
extern UART_HandleTypeDef huart3;

void esp8266_init(void)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&AT_CIPMUX_STRING, sizeof(AT_CIPMUX_STRING)-1, 1000);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart3, (uint8_t *)&AT_CIPSERVER_STRING, sizeof(AT_CIPSERVER_STRING)-1, 1000);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart3, (uint8_t *)&AT_CIPSTO_STRING, sizeof(AT_CIPSTO_STRING)-1, 1000);
	usart3_rx_buffer.head = 0;
	usart3_rx_buffer.tail = 0;
}
