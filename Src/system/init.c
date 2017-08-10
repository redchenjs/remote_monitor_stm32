/*
 * init.c
 *
 *  Created on: 2017-4-16
 *      Author: redchenjs
 */
#include "main.h"
#include "stm32f7xx_hal.h"

#include "device/usart.h"
#include "device/delay.h"

#include "driver/nt35510.h"
#include "driver/gt9147.h"
#include "driver/esp8266.h"

void device_init(void)
{
//	delay_init(216);
	usart3_receive_init();
}

void driver_init(void)
{
	nt35510_init();
	gt9147_init();
	esp8266_init();
}

void device_suspend(void)
{
//    HAL_Delay(1000);
}
