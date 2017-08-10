#ifndef __NT35510_H
#define __NT35510_H

typedef struct {
	uint16_t width;			//LCD 宽度
	uint16_t height;		//LCD 高度
	uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。
	uint16_t wramcmd;		//开始写gram指令
	uint16_t setxcmd;		//设置x坐标指令
	uint16_t setycmd;		//设置y坐标指令
} _nt35510_dev;

extern void nt35510_init(void);													   	//初始化
extern uint16_t nt35510_read_id(void);	//设置窗口
extern void nt35510_display_on(void);													//开显示
extern void nt35510_display_off(void);													//关显示
extern void nt35510_set_cursor(uint16_t x, uint16_t y);										//设置光标
extern void nt35510_write_pixel(uint16_t x, uint16_t y, uint16_t color);								//快速画点
extern uint16_t nt35510_read_pixel(uint16_t x, uint16_t y); 											//读点
extern void nt35510_set_display_window(uint16_t x,uint16_t y,uint16_t width,uint16_t height);	//设置窗口
extern void nt35510_draw_h_line(uint16_t color, uint16_t x, uint16_t y, uint16_t length);
extern void nt35510_draw_v_line(uint16_t color, uint16_t x, uint16_t y, uint16_t length);
extern void nt35510_draw_bitmap(uint16_t x, uint16_t y, uint8_t *pbmp);
extern void nt35510_draw_rgb_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pimg);
extern uint16_t nt35510_get_lcd_pixel_width(void);
extern uint16_t nt35510_get_lcd_pixel_height(void);

extern void nt35510_set_front_color(uint16_t color);

extern void nt35510_clear(uint16_t color);	 												//清屏
extern void nt35510_draw_point(uint16_t x,uint16_t y);											//画点
extern void nt35510_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);						 			//画圆
extern void nt35510_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);							//画线
extern void nt35510_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   				//画矩形
extern void nt35510_fill(uint16_t color, uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1);		   				//填充单色
extern void nt35510_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);				//填充指定颜色
extern void nt35510_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);						//显示一个字符
extern void nt35510_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  						//显示一个数字
extern void nt35510_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);				//显示 数字
extern void nt35510_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p);		//显示一个字符串,12/16字体

extern void nt35510_WriteReg(uint16_t nt35510_Reg, uint16_t nt35510_RegValue);
extern uint16_t nt35510_ReadReg(uint16_t nt35510_Reg);
extern void nt35510_WriteRAM_Prepare(void);
extern void nt35510_WriteRAM(uint16_t RGB_Code);
extern void nt35510_Scan_Dir(uint8_t dir);									//设置屏扫描方向
extern void nt35510_Display_Dir(uint8_t dir);								//设置屏幕显示方向

#endif  
	 
	 



