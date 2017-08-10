#include "stm32f7xx_hal.h"
#include "driver/nt35510.h"
#include "driver/gt9147.h"
#include "system/fonts.h"
#include "driver/esp8266.h"
#include "string.h"
#include "GUI.h"
#include "device/tim.h"

extern _gt9147_dev gt9147;
extern _nt35510_dev nt35510;

extern uint16_t POINT_COLOR;
extern uint16_t BACK_COLOR;

extern uint32_t htim2_millduty;
//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
	char temp[32] = {0};
	nt35510_clear(Black);//清屏
 	POINT_COLOR=White;//设置字体为蓝色
	nt35510_ShowString(nt35510.width-48,0,200,32,32,"RST");//显示清屏区域
	POINT_COLOR=Red;//设置画笔蓝色
	nt35510_ShowString(nt35510.width-48,32,200,32,32,"+++");
	POINT_COLOR=Lime;//设置画笔蓝色
	nt35510_ShowString(nt35510.width-48,64,200,32,32,"---");
	snprintf(temp, 32, "%3lu%%", htim2_millduty/10);
	POINT_COLOR=Yellow;//设置画笔蓝色
	nt35510_ShowString(0,0,200,32,32,temp);
}

void increase_brightness(void)
{
	char temp[32] = {0};
	if ((htim2_millduty + 50) <= 1000) {
		tim2_set_pwm_duty(htim2_millduty + 50);
	}
	snprintf(temp, 32, "%3lu%%", htim2_millduty/10);
	POINT_COLOR=Yellow;//设置画笔蓝色
	nt35510_ShowString(0,0,200,32,32,temp);
}

void decrease_brightness(void)
{
	char temp[32] = {0};
	if ((htim2_millduty - 50) >= 0) {
		tim2_set_pwm_duty(htim2_millduty - 50);
	}
	snprintf(temp, 32, "%3lu%%", htim2_millduty/10);
	POINT_COLOR=Yellow;//设置画笔蓝色
	nt35510_ShowString(0,0,200,32,32,temp);
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(uint16_t  x0,uint16_t  y0,uint16_t  len,uint16_t  color)
{
	if(len==0)return;
	if((x0+len-1)>=nt35510.width)x0=nt35510.width-len-1;	//限制坐标范围
	if(y0>=nt35510.height)y0=nt35510.height-1;			//限制坐标范围
	nt35510_fill(color,x0,y0,x0+len-1,y0);
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(uint16_t  x0,uint16_t  y0,uint16_t  r,uint16_t  color)
{											  
	uint32_t i;
	uint32_t imax = ((uint32_t)r*707)/1000+1;
	uint32_t sqmax = (uint32_t)r*(uint32_t)r+(uint32_t)r/2;
	uint32_t x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
uint16_t  my_abs(uint16_t  x1,uint16_t  x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2,uint8_t  size,uint16_t  color)
{
	uint16_t  t;
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   
////////////////////////////////////////////////////////////////////////////////
//5个触控点的颜色(电容触摸屏用)												 
const uint16_t  POINT_COLOR_TBL[5]={White, Lime, Blue, Red, Cyan};
//电容触摸屏测试函数
void ctp_test(void)
{
	uint8_t  t=0;
	uint8_t  i=0;
 	uint16_t  lastpos[5][2];		//最后一次的数据
	while(1)
	{
		gt9147_scan(0);
		for(t=0;t<5;t++)
		{
			if((gt9147.sta)&(1<<t))
			{
				if(gt9147.x[t]<nt35510.width&&gt9147.y[t]<nt35510.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = gt9147.x[t];
						lastpos[t][1] = gt9147.y[t];
					}
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],gt9147.x[t],gt9147.y[t],2,POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=gt9147.x[t];
					lastpos[t][1]=gt9147.y[t];
					if (gt9147.x[t]>(nt35510.width-48)&&gt9147.y[t]<32) {
						Load_Drow_Dialog();//清除
					} else if (gt9147.x[t]>(nt35510.width-48)&&gt9147.y[t]<64) {
						increase_brightness();
					} else if (gt9147.x[t]>(nt35510.width-48)&&gt9147.y[t]<96) {
						decrease_brightness();
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
		HAL_Delay(1);i++;
	}	
}

void lcd_test(void)
{   
//	nt35510_clear(Black);
//	HAL_Delay(1000);
//	nt35510_clear(Blue);
//	HAL_Delay(1000);
//	nt35510_clear(Brown);
//	HAL_Delay(1000);
//	nt35510_clear(Green);
//	HAL_Delay(1000);
//	nt35510_clear(Cyan);
//	HAL_Delay(1000);
//	nt35510_clear(Magenta);
//	HAL_Delay(1000);
//	nt35510_clear(Lime);
//	HAL_Delay(1000);
//	nt35510_clear(Orchid);
//	HAL_Delay(1000);
//	nt35510_clear(Yellow);
//	HAL_Delay(1000);
//	nt35510_clear(Gray);
//	HAL_Delay(Navy);
//	nt35510_clear(Snow);
//	HAL_Delay(1000);
//	nt35510_clear(Purple);
//	HAL_Delay(1000);
//	nt35510_clear(Red);
//	HAL_Delay(1000);
//	nt35510_clear(White);
//	HAL_Delay(1000);
  	POINT_COLOR=Red;
  	BACK_COLOR=White;
	nt35510_ShowString(30,50,400,32,32,"Apollo STM32F4/F7");
	nt35510_ShowString(30,100,400,32,32,"TOUCH TEST");
	nt35510_ShowString(30,150,400,32,32,"ATOM@ALIENTEK");
	nt35510_ShowString(30,200,400,32,32,"2016/7/19");
// 	Load_Drow_Dialog();
// 	nt35510_ShowString(0, 0, 400, 16, 16, "WiFi IAP Demo");
// 	HAL_Delay(500);
// 	nt35510_ShowString(0, 16, 400, 16, 16, "Initializing Wifi module...\n");

// 	  /* Initialize the WiFi module ESP8266 */
// 	  Status = ESP8266_Init();
//
 	  /* Check if initialization passed */
// 	  if (Status != ESP8266_OK) {
// 		 nt35510_ShowString(0, 16, 400, 16, 16, "Initializing Wifi module  Failed!\n\n");
// 	  }
// 	  else {
// 		 nt35510_ShowString(0, 16, 400, 16, 16, "OK!\n\n");
// 	  }
//
// 	  /* Join Access Point defined by user */
// 	 nt35510_ShowString(0, 16, 400, 16, 16, "Joining Access Point  WIFI_SSID...\n");
// 	  while(ESP8266_JoinAccessPoint((uint8_t *)WIFI_SSID, (uint8_t *)WIFI_PASSWORD) != ESP8266_OK)
// 	  {
// 		 nt35510_ShowString(0, 16, 400, 16, 16, "Retrying ( %d )to Join Access Point  WIFI_SSID\n\n");
// 		Trial++;
// 	    if (Trial == MAX_NUM_TRIAL)
// 	      break;
// 	  }
//
// 	  /* Check if maximum number of trials has been reached */
// 	  if (Trial == MAX_NUM_TRIAL)
// 	  {
// 		 nt35510_ShowString(0, 16, 400, 16, 16, "Joining Access Point  WIFI_SSID  Failed!\n\n");
// 	  }
// 	  else
// 	  {
// 		/* Reset the IP address field to 0 */
// 	    memset(IpAddress, '\0', 15);
//
// 		/* Access point joined: start getting IP address */
// 	    ESP8266_GetIPAddress(ESP8266_STATION_MODE, IpAddress);
// 	   nt35510_ShowString(0, 16, 400, 16, 16, "OK!\nGot IP Address: %s\n\n");
// 	    HAL_Delay(1000);
// 	  }
// 	ctp_test();//电容屏测试
}

void MainTask(void)
{
 	GUI_Init();
 	GUI_Clear();
 	GUI_SetFont(&GUI_Font20_1);
 	GUI_DispStringAt("Hello world!", (LCD_GetXSize()-100)/2, (LCD_GetYSize()-20)/2);
}
