/*
 * task_unpack.c
 *
 *  Created on: 2017年8月10日
 *      Author: redchenjs
 */
#include <stdio.h>
#include <string.h>

#include "device/usart.h"
#include "driver/nt35510.h"
#include "system/fonts.h"
#include "tasks/task_data.h"

power_param_t power_param;
harmonic_param_t harmonic_param;
elec_param_t elec_param[8];

extern ring_buffer_t usart3_rx_buffer;

extern uint16_t POINT_COLOR;
extern uint16_t BACK_COLOR;

void data_unpack(void)
{
	uint8_t i = 0;
	char str_temp[32] = {0};

	if (usart3_rx_buffer.head < usart3_rx_buffer.tail) {
		while (usart3_rx_buffer.data[usart3_rx_buffer.head] != '\n' && usart3_rx_buffer.data[usart3_rx_buffer.head] != '\0') {
			str_temp[i++] = usart3_rx_buffer.data[usart3_rx_buffer.head++];
		}
	}

	sscanf(str_temp, "%*[^+]+IPD:%*d,%*d:voltage=%lf%*[\n]", &power_param.voltage);

  	POINT_COLOR = Red;
  	BACK_COLOR  = White;
	snprintf(str_temp, 32, "voltage:%lfV", power_param.voltage);
	nt35510_ShowString(32,32,400,32,32,str_temp);

  	POINT_COLOR = Green;
  	BACK_COLOR  = White;
	snprintf(str_temp, 32, "current:%lfA", power_param.current);
	nt35510_ShowString(30,64,400,32,32,str_temp);

  	POINT_COLOR = Red;
  	BACK_COLOR  = White;
	snprintf(str_temp, 32, "phase:%lfdegree", power_param.phase);
	nt35510_ShowString(30,96,400,32,32,str_temp);

  	POINT_COLOR = Red;
  	BACK_COLOR  = White;
	snprintf(str_temp, 32, "frequency:%lfHz", power_param.frequency);
	nt35510_ShowString(30,128,400,32,32,str_temp);

  	POINT_COLOR = Red;
  	BACK_COLOR  = White;
	snprintf(str_temp, 32, "duty:%lf%%", power_param.duty);
	nt35510_ShowString(30,160,400,32,32,str_temp);

  	POINT_COLOR = Red;
  	BACK_COLOR  = White;
	snprintf(str_temp, 32, "apparent_power:%lfVA", power_param.apparent_power);
	nt35510_ShowString(30,192,400,32,32,str_temp);

  	POINT_COLOR = Red;
  	BACK_COLOR  = White;
	snprintf(str_temp, 32, "active_power:%lfW", power_param.active_power);
	nt35510_ShowString(30,224,400,32,32,str_temp);

  	POINT_COLOR = Red;
  	BACK_COLOR  = White;
	snprintf(str_temp, 32, "reactive_power:%lfvar", power_param.reactive_power);
	nt35510_ShowString(30,256,400,32,32,str_temp);

  	POINT_COLOR = Red;
  	BACK_COLOR  = White;
	snprintf(str_temp, 32, "power_factor:%lf", power_param.power_factor);
	nt35510_ShowString(30,288,400,32,32,str_temp);
}

void data_clear(void)
{
    memset(&power_param, 0x00, sizeof(power_param));
    memset(&harmonic_param, 0x00, sizeof(harmonic_param));
    memset(elec_param, 0x00, sizeof(elec_param));
}
