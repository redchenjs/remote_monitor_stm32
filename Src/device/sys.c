#include "stm32f7xx_hal.h"
#include "core_cm7.h"

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}

//进入待机模式	  
void sys_standby(void)
{ 
	SCB->SCR |= 1<<2;		//使能SLEEPDEEP位 (SYS->CTRL)
	RCC->APB1ENR |= 1<<28;	//使能电源时钟
	PWR->CSR2 |= 1<<8;		//设置PA0用于WKUP唤醒
	PWR->CR2 |= 0<<8;		//设置PA0为上升沿唤醒
	PWR->CR1 |= 1<<1;		//PDDS置位
	PWR->CR1 |= 1<<0;		//LPDS置位
	WFI_SET();				//执行WFI指令,进入待机模式
}

//系统软复位   
void sys_soft_reset(void)
{   
	SCB->AIRCR = 0x05FA0000 | (uint32_t)0x04;
}

//使能STM32F7的L1-Cache,同时开启D cache的强制透写
void sys_cache_enable(void)
{
    SCB_EnableICache();	//使能I-Cache,函数在core_cm7.h里面定义
    SCB_EnableDCache();	//使能D-Cache,函数在core_cm7.h里面定义
	SCB->CACR |= 1<<2;	//强制D-Cache透写,如不开启透写,实际使用中可能遇到各种问题
}

//时钟设置函数
//Fvco=Fs*(plln/pllm);
//Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco:VCO频率
//Fsys:系统时钟频率
//Fusb:USB,SDMMC,RNG等的时钟频率
//Fs:PLL输入时钟频率,可以是HSI,HSE等. 
//plln:主PLL倍频系数(PLL倍频),取值范围:50~432.
//pllm:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
//pllp:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
//pllq:USB/SDMMC/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.

//外部晶振为25M的时候,推荐值:plln=432,pllm=25,pllp=2,pllq=9.
//得到:Fvco=25*(432/25)=432Mhz
//     Fsys=432/2=216Mhz
//     Fusb=432/9=48Mhz
//返回值:0,成功;1,失败。
uint8_t  sys_set_clock(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq)
{ 
	uint16_t  retry = 0;
	uint8_t  status = 0;
	RCC->CR |= 1<<16;				//HSE 开启
	while(((RCC->CR & (1<<17)) == 0) && (retry < 0x1FFF)) {
		retry++;//等待HSE RDY
	}
	if (retry == 0x1FFF) {
		status = 1;	//HSE无法就绪
	}
	else {
		RCC->APB1ENR |= 1<<28;	//电源接口时钟使能
		PWR->CR1 |= 3<<14; 		//高性能模式,时钟可到180Mhz
		PWR->CR1 |= 1<<16; 		//使能过驱动,频率可到216Mhz
		PWR->CR1 |= 1<<17; 		//使能过驱动切换
		RCC->CFGR |= (0<<4) | (5<<10) | (4<<13);//HCLK 不分频;APB1 4分频;APB2 2分频.
		RCC->CR &= ~(1<<24);		//关闭主PLL
		RCC->PLLCFGR = pllm | (plln<<6) | (((pllp>>1)-1)<<16) | (pllq<<24) | (1<<22);//配置主PLL,PLL时钟源来自HSE
		RCC->CR |= 1<<24;			//打开主PLL
		while((RCC->CR & (1<<25)) == 0);//等待PLL准备好
		FLASH->ACR |= 1<<8;		//指令预取使能.
		FLASH->ACR |= 1<<9;		//使能ART Accelerator
		FLASH->ACR |= 7<<0;		//8个CPU等待周期.
		RCC->CFGR &= ~(3<<0);		//清零
		RCC->CFGR |= 2<<0;		//选择主PLL作为系统时钟
		while((RCC->CFGR & (3<<2)) != (2<<2));//等待主PLL作为系统时钟成功.
	} 
	return status;
}  
