#ifndef __SYS_H
#define __SYS_H	 

//uint8_t sys_set_clock(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq);	//系统时钟设置
extern void sys_soft_reset(void);      							//系统软复位
extern void sys_cache_enable(void);								//使能catch
extern void sys_standby(void);         							//待机模式

extern void INTX_DISABLE(void);	//关闭所有中断
extern void INTX_ENABLE(void);	//开启所有中断

#endif
