/*
 * nvic.c
 *
 *  Created on: 2017年8月4日
 *      Author: redchenjs
 */
#include "stm32f7xx_hal.h"

//外部中断配置函数
//只针对GPIOA~I;不包括PVD,RTC,USB_OTG,USB_HS,以太网唤醒等
//参数:
//GPIOx:0~8,代表GPIOA~I
//BITx:需要使能的位;
//TRIM:触发模式,1,下升沿;2,上降沿;3，任意电平触发
//该函数一次只能配置1个IO口,多个IO口,需多次调用
//该函数会自动开启对应中断,以及屏蔽线
void nvic_config(uint8_t GPIOx, uint8_t BITx,uint8_t TRIM)
{
	uint8_t  EXTOFFSET = (BITx % 4) * 4;
	RCC->APB2ENR |= 1<<14;  						//使能SYSCFG时钟
	SYSCFG->EXTICR[BITx/4] &= ~(0x000F<<EXTOFFSET);	//清除原来设置！！！
	SYSCFG->EXTICR[BITx/4] |= GPIOx<<EXTOFFSET;		//EXTI.BITx映射到GPIOx.BITx
	//自动设置
	EXTI->IMR |= 1<<BITx;				//开启line BITx上的中断(如果要禁止中断，则反操作即可)
	if(TRIM&0x01)EXTI->FTSR |= 1<<BITx;	//line BITx上事件下降沿触发
	if(TRIM&0x02)EXTI->RTSR |= 1<<BITx;	//line BITx上事件上升降沿触发
}
