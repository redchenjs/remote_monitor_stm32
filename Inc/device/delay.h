/*
 * delay.h
 *
 *  Created on: 2017年8月4日
 *      Author: redchenjs
 */

#ifndef DEVICE_DELAY_H_
#define DEVICE_DELAY_H_

extern void delay_init(uint8_t SYSCLK);
extern void delay_us(uint32_t nus);
extern void delay_ms(uint16_t nms);

#endif /* DEVICE_DELAY_H_ */
