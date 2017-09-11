/*
 * task_unpack.c
 *
 *  Created on: 2017年8月10日
 *      Author: redchenjs
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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

char str_temp[1024] = {0};

void data_unpack(void)
{
	static uint32_t count = 0;
	uint16_t i = 0;
	uint16_t j = 0;
//	char *buff_ptr = (char *)&usart3_rx_buffer.data[usart3_rx_buffer.head];
//	char *match_ptr = NULL;

//	if (usart3_buffer_read == 0) {
//			usart3_rx_buffer.head;
	uint16_t start_idx = 0;
	uint16_t stop_idx = 0;
	char str_buff[2048] = {0};
	char str_temp[64] = {0};

	for (i=0; i<usart3_rx_buffer.tail; i++) {
		if (usart3_rx_buffer.data[i] == '\xff') {
			start_idx = i;
			break;
		}
	}

	if (start_idx != 0) {
		for (i=start_idx, j=0; i<usart3_rx_buffer.tail; i++, j++) {
			if (usart3_rx_buffer.data[i] != '\xfe') {
				str_buff[j] = usart3_rx_buffer.data[i];
			} else {
				stop_idx = i;
				break;
			}
		}
	}

	if (stop_idx != 0) {
		count++;
		memset(&usart3_rx_buffer, 0x00, sizeof(usart3_rx_buffer));

		POINT_COLOR = Black;
	  	BACK_COLOR  = SkyBlue;

	  	sscanf(str_buff, "%*[^A]A=%16s", str_temp);
	  	power_param.voltage = atof(str_temp);
	  	sscanf(str_buff, "%*[^B]B=%16s", str_temp);
	  	power_param.current = atof(str_temp);
	  	sscanf(str_buff, "%*[^C]C=%16s", str_temp);
	  	power_param.phase = atof(str_temp);
	  	sscanf(str_buff, "%*[^D]D=%16s", str_temp);
	  	power_param.frequency = atof(str_temp);
	  	sscanf(str_buff, "%*[^E]E=%16s", str_temp);
	  	power_param.duty = atof(str_temp);
	  	sscanf(str_buff, "%*[^F]F=%16s", str_temp);
	  	power_param.apparent_power = atof(str_temp);
	  	sscanf(str_buff, "%*[^G]G=%16s", str_temp);
	  	power_param.active_power = atof(str_temp);
	  	sscanf(str_buff, "%*[^H]H=%16s", str_temp);
	  	power_param.reactive_power = atof(str_temp);
	  	sscanf(str_buff, "%*[^I]I=%16s", str_temp);
	  	power_param.power_factor = atof(str_temp);

	  	sscanf(str_buff, "%*[^J]J=%1u", (unsigned int *)&elec_param[1].status);
	  	sscanf(str_buff, "%*[^K]K=%1u", (unsigned int *)&elec_param[2].status);
	  	sscanf(str_buff, "%*[^L]L=%1u", (unsigned int *)&elec_param[3].status);
	  	sscanf(str_buff, "%*[^M]M=%1u", (unsigned int *)&elec_param[4].status);
	  	sscanf(str_buff, "%*[^N]N=%1u", (unsigned int *)&elec_param[5].status);
	  	sscanf(str_buff, "%*[^O]O=%1u", (unsigned int *)&elec_param[6].status);
	  	sscanf(str_buff, "%*[^P]P=%1u", (unsigned int *)&elec_param[7].status);

	    char str_temp[32] = {0};
	    i = 0;
	    snprintf(str_temp, 32, "Voltage: %17.3lf V  ", round(power_param.voltage * 1e3) / 1e3);
	  	nt35510_ShowString(0,i,480,32,32,str_temp);
	  	i += 32;
	    snprintf(str_temp, 32, "Current: %17.3lf A  ", round(power_param.current * 1e3) / 1e3);
	  	nt35510_ShowString(0,i,480,32,32,str_temp);
	  	i += 32;
	    snprintf(str_temp, 32, "Phase: %19.2lf deg", round(power_param.phase * 1e2) / 1e2);
	  	nt35510_ShowString(0,i,480,32,32,str_temp);
	  	i += 32;
	    snprintf(str_temp, 32, "Frequency: %15.3lf Hz ", round(power_param.frequency * 1e3) / 1e3);
	  	nt35510_ShowString(0,i,480,32,32,str_temp);
	  	i += 32;
	    snprintf(str_temp, 32, "Duty: %20.2lf %%  ", round(power_param.duty * 1e2) / 1e2);
	  	nt35510_ShowString(0,i,480,32,32,str_temp);
	  	i += 32;
	    snprintf(str_temp, 32, "Apparent power: %10.3lf VA ", round(power_param.apparent_power * 1e3) / 1e3);
	  	nt35510_ShowString(0,i,480,32,32,str_temp);
	  	i += 32;
	    snprintf(str_temp, 32, "Active power: %12.3lf W  ", round(power_param.active_power * 1e3) / 1e3);
	  	nt35510_ShowString(0,i,480,32,32,str_temp);
	  	i += 32;
	    snprintf(str_temp, 32, "Reactive power: %10.3lf var", round(power_param.reactive_power * 1e3) / 1e3);
	  	nt35510_ShowString(0,i,480,32,32,str_temp);
	  	i += 32;
	    snprintf(str_temp, 32, "Power factor: %12.3lf    ", round(power_param.power_factor * 1e3) / 1e3);
	  	nt35510_ShowString(0,i,480,32,32,str_temp);

	  	nt35510_DrawRectangle(118, 334+32*2, 153+16*13, 401+32*7);
	  	nt35510_DrawRectangle(119, 335+32*2, 152+16*13, 400+32*7);

	  	i = 400;
	  	for (j=1; j<8; j++) {
	  		if (elec_param[j].status != 0) {
	  			POINT_COLOR = Black;
	  		  	BACK_COLOR  = Lime;
	  			snprintf(str_temp, 32, " Status %d: ON  ", j);
	  		} else {
	  			POINT_COLOR = Black;
	  		  	BACK_COLOR  = Red;
	  			snprintf(str_temp, 32, " Status %d: OFF ", j);
	  		}
			nt35510_ShowString(120,i,480,32,32,str_temp);
			i += 32;
	  	}

		POINT_COLOR = Black;
		BACK_COLOR  = White;
		snprintf(str_temp, 32, "Received: %20lu", count);
		nt35510_ShowString(0,767,480,32,32,str_temp);
	}

//	if ((match_ptr = strstr(str_temp, "voltage=")) != NULL) {
//		power_param.voltage = atof(str_temp + 8);
//		POINT_COLOR = Black;
//	  	BACK_COLOR  = White;
//	  	strncpy(str_temp, buff_ptr, 233);
//		snprintf(str_temp, 32, "voltage:%.2lfV", power_param.voltage);
//		nt35510_ShowString(32,32,400,400,32,str_temp);
//	  	nt35510_ShowString(0,0,480,800,32,(char *)&usart3_rx_buffer.data[usart3_rx_buffer.head]);
//	  	usart3_buffer_read = 1;
//	}
//
//  	POINT_COLOR = Green;
//  	BACK_COLOR  = White;
//	snprintf(str_temp, 32, "current:%lfA", power_param.current);
//	nt35510_ShowString(30,64,400,32,32,str_temp);
//
//  	POINT_COLOR = Red;
//  	BACK_COLOR  = White;
//	snprintf(str_temp, 32, "phase:%lfdegree", power_param.phase);
//	nt35510_ShowString(30,96,400,32,32,str_temp);
//
//  	POINT_COLOR = Red;
//  	BACK_COLOR  = White;
//	snprintf(str_temp, 32, "frequency:%lfHz", power_param.frequency);
//	nt35510_ShowString(30,128,400,32,32,str_temp);
//
//  	POINT_COLOR = Red;
//  	BACK_COLOR  = White;
//	snprintf(str_temp, 32, "duty:%lf%%", power_param.duty);
//	nt35510_ShowString(30,160,400,32,32,str_temp);
//
//  	POINT_COLOR = Red;
//  	BACK_COLOR  = White;
//	snprintf(str_temp, 32, "apparent_power:%lfVA", power_param.apparent_power);
//	nt35510_ShowString(30,192,400,32,32,str_temp);
//
//  	POINT_COLOR = Red;
//  	BACK_COLOR  = White;
//	snprintf(str_temp, 32, "active_power:%lfW", power_param.active_power);
//	nt35510_ShowString(30,224,400,32,32,str_temp);
//
//  	POINT_COLOR = Red;
//  	BACK_COLOR  = White;
//	snprintf(str_temp, 32, "reactive_power:%lfvar", power_param.reactive_power);
//	nt35510_ShowString(30,256,400,32,32,str_temp);
//
//  	POINT_COLOR = Red;
//  	BACK_COLOR  = White;
//	snprintf(str_temp, 32, "power_factor:%lf", power_param.power_factor);
//	nt35510_ShowString(30,288,400,32,32,str_temp);
}

void data_clear(void)
{
    memset(&power_param, 0x00, sizeof(power_param));
    memset(&harmonic_param, 0x00, sizeof(harmonic_param));
    memset(elec_param, 0x00, sizeof(elec_param));
}
