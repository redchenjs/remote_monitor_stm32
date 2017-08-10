/*
 * tim.c
 *
 *  Created on: 2017年8月7日
 *      Author: redchenjs
 */
#include "stm32f7xx_hal.h"

extern TIM_HandleTypeDef htim2;

TIM_OC_InitTypeDef htim2_oc_config = {
	.OCMode = TIM_OCMODE_PWM1,
	.Pulse  = 150,
	.OCPolarity = TIM_OCPOLARITY_HIGH,
	.OCFastMode = TIM_OCFAST_ENABLE
};

uint32_t htim2_millduty = 750;

void tim2_set_pwm_duty(uint32_t millduty)
{
	if (millduty > 1000 || millduty < 0) {
		return;
	}
	htim2_millduty = millduty;
	htim2_oc_config.Pulse = htim2_millduty * htim2.Init.Period / 1000;

	if (HAL_TIM_PWM_ConfigChannel(&htim2, &htim2_oc_config, TIM_CHANNEL_4) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
}
