/*
 * tasks.c
 *
 *  Created on: 2017-4-16
 *      Author: redchenjs
 */
#include "tasks/task_data.h"
#include "tasks/task_lcd.h"

void tasks_loop(void)
{
	data_unpack();
}

void tasks_init(void)
{
//	lcd_test();
	data_clear();
}
