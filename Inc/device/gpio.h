/*
 * gpio.h
 *
 *  Created on: 2017年8月4日
 *      Author: redchenjs
 */

#ifndef DEVICE_GPIO_H_
#define DEVICE_GPIO_H_

#define GPIO_MODE_IN    	0		//普通输入模式
#define GPIO_MODE_OUT		1		//普通输出模式
#define GPIO_MODE_AF		2		//AF功能模式
#define GPIO_MODE_AIN		3		//模拟输入模式

#define GPIO_SPEED_2M		0		//GPIO速度2Mhz(低速)
#define GPIO_SPEED_25M		1		//GPIO速度25Mhz(中速)
#define GPIO_SPEED_50M		2		//GPIO速度50Mhz(快速)
#define GPIO_SPEED_100M		3		//GPIO速度100Mhz(高速)

#define GPIO_PUPD_NONE		0		//不带上下拉
#define GPIO_PUPD_PU		1		//上拉
#define GPIO_PUPD_PD		2		//下拉
#define GPIO_PUPD_RES		3		//保留

#define GPIO_OTYPE_PP		0		//推挽输出
#define GPIO_OTYPE_OD		1		//开漏输出

extern void gpio_set_af(GPIO_TypeDef* GPIOx,uint8_t BITx, uint8_t AFx);
extern void gpio_set_pin(GPIO_TypeDef* GPIOx, uint32_t BITx, uint32_t MODE, uint32_t OTYPE, uint32_t OSPEED, uint32_t PUPD);

#endif /* DEVICE_GPIO_H_ */
