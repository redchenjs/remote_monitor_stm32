/*
 * gpio.c
 *
 *  Created on: 2017年8月4日
 *      Author: redchenjs
 */
#include "stm32f7xx_hal.h"

//GPIO复用设置
//GPIOx:GPIOA~GPIOI.
//BITx:0~15,代表IO引脚编号.
//AFx:0~15,代表AF0~AF15.
//AF0~15设置情况(这里仅是列出常用的,详细的请见429/746数据手册,Table 12):
//AF0:MCO/SWD/SWCLK/RTC   		AF1:TIM1/TIM2;            		AF2:TIM3~5;               		AF3:TIM8~11
//AF4:I2C1~I2C4;          		AF5:SPI1~SPI6;            		AF6:SPI3/SAI1;            	 	AF7:SPI2/3/USART1~3/UART5/SPDIFRX;
//AF8:USART4~8/SPDIFRX/SAI2; 	AF9;CAN1~2/TIM12~14/LCD/QSPI; 	AF10:USB_OTG/USB_HS/SAI2/QSPI  	AF11:ETH
//AF12:FMC/SDMMC/OTG/HS   		AF13:DCIM                 		AF14:LCD;                  		AF15:EVENTOUT
void gpio_set_af(GPIO_TypeDef* GPIOx, uint8_t BITx, uint8_t AFx)
{
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(uint32_t)AFx<<((BITx&0X07)*4);
}

//GPIO通用设置
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,位设置,每个位代表一个IO,第0位代表Px0,第1位代表Px1,依次类推.比如0X0101,代表同时设置Px0和Px8.
//MODE:0~3;模式选择,0,输入(系统复位默认状态);1,普通输出;2,复用功能;3,模拟输入.
//OTYPE:0/1;输出类型选择,0,推挽输出;1,开漏输出.
//OSPEED:0~3;输出速度设置,0,低速;1,中速;2,快速;3,高速.
//PUPD:0~3:上下拉设置,0,不带上下拉;1,上拉;2,下拉;3,保留.
//注意:在输入模式(普通输入/模拟输入)下,OTYPE和OSPEED参数无效!!
void gpio_set_pin(GPIO_TypeDef* GPIOx, uint32_t BITx, uint32_t MODE, uint32_t OTYPE, uint32_t OSPEED, uint32_t PUPD)
{
	uint32_t pinpos = 0, pos = 0, curpin = 0;
	for (pinpos=0; pinpos<16; pinpos++) {
		pos = 1<<pinpos;	//一个个位检查
		curpin = BITx & pos;//检查引脚是否要设置
		if (curpin == pos) {	//需要设置
			GPIOx->MODER &= ~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->MODER |= MODE<<(pinpos*2);	//设置新的模式
			if ((MODE==0X01) || (MODE==0X02)) {	//如果是输出模式/复用功能模式
				GPIOx->OSPEEDR &= ~(3<<(pinpos*2));	//清除原来的设置
				GPIOx->OSPEEDR |= (OSPEED<<(pinpos*2));//设置新的速度值
				GPIOx->OTYPER &= ~(1<<pinpos) ;		//清除原来的设置
				GPIOx->OTYPER |= OTYPE<<pinpos;		//设置新的输出模式
			}
			GPIOx->PUPDR &= ~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->PUPDR |= PUPD<<(pinpos*2);	//设置新的上下拉
		}
	}
}
