/*
 * nvic.h
 *
 *  Created on: 2017年8月4日
 *      Author: redchenjs
 */

#ifndef DEVICE_NVIC_H_
#define DEVICE_NVIC_H_

#define FTIR   				1  		//下降沿触发
#define RTIR   				2  		//上升沿触发

extern void nvic_config(uint8_t GPIOx, uint8_t BITx,uint8_t TRIM);

#endif /* DEVICE_NVIC_H_ */
