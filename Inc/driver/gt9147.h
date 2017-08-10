#ifndef __GT9147_H
#define __GT9147_H	

#define GT9147_MAX_TOUCH  5    //电容屏支持的点数,固定为5点

typedef struct {
	uint16_t x[GT9147_MAX_TOUCH]; 	//当前坐标
	uint16_t y[GT9147_MAX_TOUCH];	//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
								//x[4],y[4]存储第一次按下时的坐标.
	uint8_t sta;				//笔的状态
								//b7:按下1/松开0;
	                            //b6:0,没有按键按下;1,有按键按下.
								//b5:保留
								//b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
	uint8_t touchtype;			//b0:0,竖屏(适合左右为X坐标,上下为Y坐标的TP)
								//   1,横屏(适合左右为Y坐标,上下为X坐标的TP)
								//b1~7:保留.

} _gt9147_dev;

extern uint8_t gt9147_send_cfg(uint8_t mode);
extern uint8_t gt9147_write_reg(uint16_t reg,uint8_t *buf,uint8_t len);
extern void gt9147_read_reg(uint16_t reg, uint8_t *buf,uint8_t len);
extern uint8_t gt9147_init(void);
extern uint8_t gt9147_scan(uint8_t mode);

#endif













