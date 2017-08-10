#include "stm32f7xx_hal.h"

#include "stdlib.h"
#include "device/gpio.h"
#include "system/font.h"
#include "system/fonts.h"
#include "driver/nt35510.h"

_nt35510_dev nt35510;	//管理LCD重要参数
//LCD的画笔颜色和背景色
extern uint16_t POINT_COLOR;//默认红色
extern uint16_t BACK_COLOR; //背景颜色.默认为白色

//LCD地址结构体
typedef struct {
	volatile uint16_t LCD_REG;
	volatile uint16_t LCD_RAM;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector1,地址位HADDR[27,26]=00 A18作为数据命令区分线
//注意设置时STM32内部会右移一位对其!
#define LCD_BASE        ((uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
/////////////////////////////////////////////////////////////////b/////////////////

//扫描方向定义
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上
#define R2L_U2D  2 		//从右到左,从上到下
#define R2L_D2U  3 		//从右到左,从下到上

#define U2D_L2R  4 		//从上到下,从左到右
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

#define NT35510_GPIO_PORT_BL    	GPIOB
#define NT35510_GPIO_PIN_BL    	GPIO_PIN_11

extern TIM_HandleTypeDef htim2;
#define NT35510_BL()		do {\
									HAL_TIM_Base_Start(&htim2);\
									HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);\
							} while (0)
//HAL_GPIO_WritePin(NT35510_GPIO_PORT_BL, NT35510_GPIO_PIN_BL, __X)		//LCD背光	PB5

//LCD的画笔颜色和背景色	   
uint16_t POINT_COLOR=0xffff;		//画笔颜色
uint16_t BACK_COLOR =0x0000;  	//背景色

enum nt35510_panel_value{
	NT35510_HEGHT
};
//写寄存器函数
//regval:寄存器值
void nt35510_WR_REG(volatile uint16_t regval)
{   
	LCD->LCD_REG=regval;//写入要写的寄存器序号	 
}
//写LCD数据
//data:要写入的值
void nt35510_WR_DATA(volatile uint16_t data)
{	  
	LCD->LCD_RAM=data;		 
}
//读LCD数据
//返回值:读到的值
uint16_t nt35510_RD_DATA(void)
{
//	volatile uint16_t ram;			//防止被优化
	return LCD->LCD_RAM;
//	return ram;
}					   
//写寄存器
//nt35510_Reg:寄存器地址
//nt35510_RegValue:要写入的数据
void nt35510_WriteReg(uint16_t nt35510_Reg, uint16_t nt35510_RegValue)
{	
	LCD->LCD_REG = nt35510_Reg;		//写入要写的寄存器序号
	LCD->LCD_RAM = nt35510_RegValue;//写入数据
}	   
//读寄存器
//nt35510_Reg:寄存器地址
//返回值:读到的数据
uint16_t nt35510_ReadReg(uint16_t nt35510_Reg)
{										   
	nt35510_WR_REG(nt35510_Reg);		//写入要读的寄存器序号
	return nt35510_RD_DATA();		//返回读到的值
}   
//开始写GRAM
void nt35510_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=nt35510.wramcmd;
}	 
//LCD写GRAM
//RGB_Code:颜色值
void nt35510_WriteRAM(uint16_t RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
uint16_t nt35510_BGR2RGB(uint16_t c)
{
	uint16_t  r,g,b,rgb;
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(uint8_t i)
{
	while(i--);
}

void nt35510_init(void)
{ 	 
	nt35510_WriteReg(0xF000,0x55);
	nt35510_WriteReg(0xF001,0xAA);
	nt35510_WriteReg(0xF002,0x52);
	nt35510_WriteReg(0xF003,0x08);
	nt35510_WriteReg(0xF004,0x01);
	//AVDD Set AVDD 5.2V
	nt35510_WriteReg(0xB000,0x0D);
	nt35510_WriteReg(0xB001,0x0D);
	nt35510_WriteReg(0xB002,0x0D);
	//AVDD ratio
	nt35510_WriteReg(0xB600,0x34);
	nt35510_WriteReg(0xB601,0x34);
	nt35510_WriteReg(0xB602,0x34);
	//AVEE -5.2V
	nt35510_WriteReg(0xB100,0x0D);
	nt35510_WriteReg(0xB101,0x0D);
	nt35510_WriteReg(0xB102,0x0D);
	//AVEE ratio
	nt35510_WriteReg(0xB700,0x34);
	nt35510_WriteReg(0xB701,0x34);
	nt35510_WriteReg(0xB702,0x34);
	//VCL -2.5V
	nt35510_WriteReg(0xB200,0x00);
	nt35510_WriteReg(0xB201,0x00);
	nt35510_WriteReg(0xB202,0x00);
	//VCL ratio
	nt35510_WriteReg(0xB800,0x24);
	nt35510_WriteReg(0xB801,0x24);
	nt35510_WriteReg(0xB802,0x24);
	//VGH 15V (Free pump)
	nt35510_WriteReg(0xBF00,0x01);
	nt35510_WriteReg(0xB300,0x0F);
	nt35510_WriteReg(0xB301,0x0F);
	nt35510_WriteReg(0xB302,0x0F);
	//VGH ratio
	nt35510_WriteReg(0xB900,0x34);
	nt35510_WriteReg(0xB901,0x34);
	nt35510_WriteReg(0xB902,0x34);
	//VGL_REG -10V
	nt35510_WriteReg(0xB500,0x08);
	nt35510_WriteReg(0xB501,0x08);
	nt35510_WriteReg(0xB502,0x08);
	nt35510_WriteReg(0xC200,0x03);
	//VGLX ratio
	nt35510_WriteReg(0xBA00,0x24);
	nt35510_WriteReg(0xBA01,0x24);
	nt35510_WriteReg(0xBA02,0x24);
	//VGMP/VGSP 4.5V/0V
	nt35510_WriteReg(0xBC00,0x00);
	nt35510_WriteReg(0xBC01,0x78);
	nt35510_WriteReg(0xBC02,0x00);
	//VGMN/VGSN -4.5V/0V
	nt35510_WriteReg(0xBD00,0x00);
	nt35510_WriteReg(0xBD01,0x78);
	nt35510_WriteReg(0xBD02,0x00);
	//VCOM
	nt35510_WriteReg(0xBE00,0x00);
	nt35510_WriteReg(0xBE01,0x64);
	//Gamma Setting
	nt35510_WriteReg(0xD100,0x00);
	nt35510_WriteReg(0xD101,0x33);
	nt35510_WriteReg(0xD102,0x00);
	nt35510_WriteReg(0xD103,0x34);
	nt35510_WriteReg(0xD104,0x00);
	nt35510_WriteReg(0xD105,0x3A);
	nt35510_WriteReg(0xD106,0x00);
	nt35510_WriteReg(0xD107,0x4A);
	nt35510_WriteReg(0xD108,0x00);
	nt35510_WriteReg(0xD109,0x5C);
	nt35510_WriteReg(0xD10A,0x00);
	nt35510_WriteReg(0xD10B,0x81);
	nt35510_WriteReg(0xD10C,0x00);
	nt35510_WriteReg(0xD10D,0xA6);
	nt35510_WriteReg(0xD10E,0x00);
	nt35510_WriteReg(0xD10F,0xE5);
	nt35510_WriteReg(0xD110,0x01);
	nt35510_WriteReg(0xD111,0x13);
	nt35510_WriteReg(0xD112,0x01);
	nt35510_WriteReg(0xD113,0x54);
	nt35510_WriteReg(0xD114,0x01);
	nt35510_WriteReg(0xD115,0x82);
	nt35510_WriteReg(0xD116,0x01);
	nt35510_WriteReg(0xD117,0xCA);
	nt35510_WriteReg(0xD118,0x02);
	nt35510_WriteReg(0xD119,0x00);
	nt35510_WriteReg(0xD11A,0x02);
	nt35510_WriteReg(0xD11B,0x01);
	nt35510_WriteReg(0xD11C,0x02);
	nt35510_WriteReg(0xD11D,0x34);
	nt35510_WriteReg(0xD11E,0x02);
	nt35510_WriteReg(0xD11F,0x67);
	nt35510_WriteReg(0xD120,0x02);
	nt35510_WriteReg(0xD121,0x84);
	nt35510_WriteReg(0xD122,0x02);
	nt35510_WriteReg(0xD123,0xA4);
	nt35510_WriteReg(0xD124,0x02);
	nt35510_WriteReg(0xD125,0xB7);
	nt35510_WriteReg(0xD126,0x02);
	nt35510_WriteReg(0xD127,0xCF);
	nt35510_WriteReg(0xD128,0x02);
	nt35510_WriteReg(0xD129,0xDE);
	nt35510_WriteReg(0xD12A,0x02);
	nt35510_WriteReg(0xD12B,0xF2);
	nt35510_WriteReg(0xD12C,0x02);
	nt35510_WriteReg(0xD12D,0xFE);
	nt35510_WriteReg(0xD12E,0x03);
	nt35510_WriteReg(0xD12F,0x10);
	nt35510_WriteReg(0xD130,0x03);
	nt35510_WriteReg(0xD131,0x33);
	nt35510_WriteReg(0xD132,0x03);
	nt35510_WriteReg(0xD133,0x6D);
	nt35510_WriteReg(0xD200,0x00);
	nt35510_WriteReg(0xD201,0x33);
	nt35510_WriteReg(0xD202,0x00);
	nt35510_WriteReg(0xD203,0x34);
	nt35510_WriteReg(0xD204,0x00);
	nt35510_WriteReg(0xD205,0x3A);
	nt35510_WriteReg(0xD206,0x00);
	nt35510_WriteReg(0xD207,0x4A);
	nt35510_WriteReg(0xD208,0x00);
	nt35510_WriteReg(0xD209,0x5C);
	nt35510_WriteReg(0xD20A,0x00);

	nt35510_WriteReg(0xD20B,0x81);
	nt35510_WriteReg(0xD20C,0x00);
	nt35510_WriteReg(0xD20D,0xA6);
	nt35510_WriteReg(0xD20E,0x00);
	nt35510_WriteReg(0xD20F,0xE5);
	nt35510_WriteReg(0xD210,0x01);
	nt35510_WriteReg(0xD211,0x13);
	nt35510_WriteReg(0xD212,0x01);
	nt35510_WriteReg(0xD213,0x54);
	nt35510_WriteReg(0xD214,0x01);
	nt35510_WriteReg(0xD215,0x82);
	nt35510_WriteReg(0xD216,0x01);
	nt35510_WriteReg(0xD217,0xCA);
	nt35510_WriteReg(0xD218,0x02);
	nt35510_WriteReg(0xD219,0x00);
	nt35510_WriteReg(0xD21A,0x02);
	nt35510_WriteReg(0xD21B,0x01);
	nt35510_WriteReg(0xD21C,0x02);
	nt35510_WriteReg(0xD21D,0x34);
	nt35510_WriteReg(0xD21E,0x02);
	nt35510_WriteReg(0xD21F,0x67);
	nt35510_WriteReg(0xD220,0x02);
	nt35510_WriteReg(0xD221,0x84);
	nt35510_WriteReg(0xD222,0x02);
	nt35510_WriteReg(0xD223,0xA4);
	nt35510_WriteReg(0xD224,0x02);
	nt35510_WriteReg(0xD225,0xB7);
	nt35510_WriteReg(0xD226,0x02);
	nt35510_WriteReg(0xD227,0xCF);
	nt35510_WriteReg(0xD228,0x02);
	nt35510_WriteReg(0xD229,0xDE);
	nt35510_WriteReg(0xD22A,0x02);
	nt35510_WriteReg(0xD22B,0xF2);
	nt35510_WriteReg(0xD22C,0x02);
	nt35510_WriteReg(0xD22D,0xFE);
	nt35510_WriteReg(0xD22E,0x03);
	nt35510_WriteReg(0xD22F,0x10);
	nt35510_WriteReg(0xD230,0x03);
	nt35510_WriteReg(0xD231,0x33);
	nt35510_WriteReg(0xD232,0x03);
	nt35510_WriteReg(0xD233,0x6D);
	nt35510_WriteReg(0xD300,0x00);
	nt35510_WriteReg(0xD301,0x33);
	nt35510_WriteReg(0xD302,0x00);
	nt35510_WriteReg(0xD303,0x34);
	nt35510_WriteReg(0xD304,0x00);
	nt35510_WriteReg(0xD305,0x3A);
	nt35510_WriteReg(0xD306,0x00);
	nt35510_WriteReg(0xD307,0x4A);
	nt35510_WriteReg(0xD308,0x00);
	nt35510_WriteReg(0xD309,0x5C);
	nt35510_WriteReg(0xD30A,0x00);

	nt35510_WriteReg(0xD30B,0x81);
	nt35510_WriteReg(0xD30C,0x00);
	nt35510_WriteReg(0xD30D,0xA6);
	nt35510_WriteReg(0xD30E,0x00);
	nt35510_WriteReg(0xD30F,0xE5);
	nt35510_WriteReg(0xD310,0x01);
	nt35510_WriteReg(0xD311,0x13);
	nt35510_WriteReg(0xD312,0x01);
	nt35510_WriteReg(0xD313,0x54);
	nt35510_WriteReg(0xD314,0x01);
	nt35510_WriteReg(0xD315,0x82);
	nt35510_WriteReg(0xD316,0x01);
	nt35510_WriteReg(0xD317,0xCA);
	nt35510_WriteReg(0xD318,0x02);
	nt35510_WriteReg(0xD319,0x00);
	nt35510_WriteReg(0xD31A,0x02);
	nt35510_WriteReg(0xD31B,0x01);
	nt35510_WriteReg(0xD31C,0x02);
	nt35510_WriteReg(0xD31D,0x34);
	nt35510_WriteReg(0xD31E,0x02);
	nt35510_WriteReg(0xD31F,0x67);
	nt35510_WriteReg(0xD320,0x02);
	nt35510_WriteReg(0xD321,0x84);
	nt35510_WriteReg(0xD322,0x02);
	nt35510_WriteReg(0xD323,0xA4);
	nt35510_WriteReg(0xD324,0x02);
	nt35510_WriteReg(0xD325,0xB7);
	nt35510_WriteReg(0xD326,0x02);
	nt35510_WriteReg(0xD327,0xCF);
	nt35510_WriteReg(0xD328,0x02);
	nt35510_WriteReg(0xD329,0xDE);
	nt35510_WriteReg(0xD32A,0x02);
	nt35510_WriteReg(0xD32B,0xF2);
	nt35510_WriteReg(0xD32C,0x02);
	nt35510_WriteReg(0xD32D,0xFE);
	nt35510_WriteReg(0xD32E,0x03);
	nt35510_WriteReg(0xD32F,0x10);
	nt35510_WriteReg(0xD330,0x03);
	nt35510_WriteReg(0xD331,0x33);
	nt35510_WriteReg(0xD332,0x03);
	nt35510_WriteReg(0xD333,0x6D);
	nt35510_WriteReg(0xD400,0x00);
	nt35510_WriteReg(0xD401,0x33);
	nt35510_WriteReg(0xD402,0x00);
	nt35510_WriteReg(0xD403,0x34);
	nt35510_WriteReg(0xD404,0x00);
	nt35510_WriteReg(0xD405,0x3A);
	nt35510_WriteReg(0xD406,0x00);
	nt35510_WriteReg(0xD407,0x4A);
	nt35510_WriteReg(0xD408,0x00);
	nt35510_WriteReg(0xD409,0x5C);
	nt35510_WriteReg(0xD40A,0x00);
	nt35510_WriteReg(0xD40B,0x81);

	nt35510_WriteReg(0xD40C,0x00);
	nt35510_WriteReg(0xD40D,0xA6);
	nt35510_WriteReg(0xD40E,0x00);
	nt35510_WriteReg(0xD40F,0xE5);
	nt35510_WriteReg(0xD410,0x01);
	nt35510_WriteReg(0xD411,0x13);
	nt35510_WriteReg(0xD412,0x01);
	nt35510_WriteReg(0xD413,0x54);
	nt35510_WriteReg(0xD414,0x01);
	nt35510_WriteReg(0xD415,0x82);
	nt35510_WriteReg(0xD416,0x01);
	nt35510_WriteReg(0xD417,0xCA);
	nt35510_WriteReg(0xD418,0x02);
	nt35510_WriteReg(0xD419,0x00);
	nt35510_WriteReg(0xD41A,0x02);
	nt35510_WriteReg(0xD41B,0x01);
	nt35510_WriteReg(0xD41C,0x02);
	nt35510_WriteReg(0xD41D,0x34);
	nt35510_WriteReg(0xD41E,0x02);
	nt35510_WriteReg(0xD41F,0x67);
	nt35510_WriteReg(0xD420,0x02);
	nt35510_WriteReg(0xD421,0x84);
	nt35510_WriteReg(0xD422,0x02);
	nt35510_WriteReg(0xD423,0xA4);
	nt35510_WriteReg(0xD424,0x02);
	nt35510_WriteReg(0xD425,0xB7);
	nt35510_WriteReg(0xD426,0x02);
	nt35510_WriteReg(0xD427,0xCF);
	nt35510_WriteReg(0xD428,0x02);
	nt35510_WriteReg(0xD429,0xDE);
	nt35510_WriteReg(0xD42A,0x02);
	nt35510_WriteReg(0xD42B,0xF2);
	nt35510_WriteReg(0xD42C,0x02);
	nt35510_WriteReg(0xD42D,0xFE);
	nt35510_WriteReg(0xD42E,0x03);
	nt35510_WriteReg(0xD42F,0x10);
	nt35510_WriteReg(0xD430,0x03);
	nt35510_WriteReg(0xD431,0x33);
	nt35510_WriteReg(0xD432,0x03);
	nt35510_WriteReg(0xD433,0x6D);
	nt35510_WriteReg(0xD500,0x00);
	nt35510_WriteReg(0xD501,0x33);
	nt35510_WriteReg(0xD502,0x00);
	nt35510_WriteReg(0xD503,0x34);
	nt35510_WriteReg(0xD504,0x00);
	nt35510_WriteReg(0xD505,0x3A);
	nt35510_WriteReg(0xD506,0x00);
	nt35510_WriteReg(0xD507,0x4A);
	nt35510_WriteReg(0xD508,0x00);
	nt35510_WriteReg(0xD509,0x5C);
	nt35510_WriteReg(0xD50A,0x00);
	nt35510_WriteReg(0xD50B,0x81);

	nt35510_WriteReg(0xD50C,0x00);
	nt35510_WriteReg(0xD50D,0xA6);
	nt35510_WriteReg(0xD50E,0x00);
	nt35510_WriteReg(0xD50F,0xE5);
	nt35510_WriteReg(0xD510,0x01);
	nt35510_WriteReg(0xD511,0x13);
	nt35510_WriteReg(0xD512,0x01);
	nt35510_WriteReg(0xD513,0x54);
	nt35510_WriteReg(0xD514,0x01);
	nt35510_WriteReg(0xD515,0x82);
	nt35510_WriteReg(0xD516,0x01);
	nt35510_WriteReg(0xD517,0xCA);
	nt35510_WriteReg(0xD518,0x02);
	nt35510_WriteReg(0xD519,0x00);
	nt35510_WriteReg(0xD51A,0x02);
	nt35510_WriteReg(0xD51B,0x01);
	nt35510_WriteReg(0xD51C,0x02);
	nt35510_WriteReg(0xD51D,0x34);
	nt35510_WriteReg(0xD51E,0x02);
	nt35510_WriteReg(0xD51F,0x67);
	nt35510_WriteReg(0xD520,0x02);
	nt35510_WriteReg(0xD521,0x84);
	nt35510_WriteReg(0xD522,0x02);
	nt35510_WriteReg(0xD523,0xA4);
	nt35510_WriteReg(0xD524,0x02);
	nt35510_WriteReg(0xD525,0xB7);
	nt35510_WriteReg(0xD526,0x02);
	nt35510_WriteReg(0xD527,0xCF);
	nt35510_WriteReg(0xD528,0x02);
	nt35510_WriteReg(0xD529,0xDE);
	nt35510_WriteReg(0xD52A,0x02);
	nt35510_WriteReg(0xD52B,0xF2);
	nt35510_WriteReg(0xD52C,0x02);
	nt35510_WriteReg(0xD52D,0xFE);
	nt35510_WriteReg(0xD52E,0x03);
	nt35510_WriteReg(0xD52F,0x10);
	nt35510_WriteReg(0xD530,0x03);
	nt35510_WriteReg(0xD531,0x33);
	nt35510_WriteReg(0xD532,0x03);
	nt35510_WriteReg(0xD533,0x6D);
	nt35510_WriteReg(0xD600,0x00);
	nt35510_WriteReg(0xD601,0x33);
	nt35510_WriteReg(0xD602,0x00);
	nt35510_WriteReg(0xD603,0x34);
	nt35510_WriteReg(0xD604,0x00);
	nt35510_WriteReg(0xD605,0x3A);
	nt35510_WriteReg(0xD606,0x00);
	nt35510_WriteReg(0xD607,0x4A);
	nt35510_WriteReg(0xD608,0x00);
	nt35510_WriteReg(0xD609,0x5C);
	nt35510_WriteReg(0xD60A,0x00);
	nt35510_WriteReg(0xD60B,0x81);

	nt35510_WriteReg(0xD60C,0x00);
	nt35510_WriteReg(0xD60D,0xA6);
	nt35510_WriteReg(0xD60E,0x00);
	nt35510_WriteReg(0xD60F,0xE5);
	nt35510_WriteReg(0xD610,0x01);
	nt35510_WriteReg(0xD611,0x13);
	nt35510_WriteReg(0xD612,0x01);
	nt35510_WriteReg(0xD613,0x54);
	nt35510_WriteReg(0xD614,0x01);
	nt35510_WriteReg(0xD615,0x82);
	nt35510_WriteReg(0xD616,0x01);
	nt35510_WriteReg(0xD617,0xCA);
	nt35510_WriteReg(0xD618,0x02);
	nt35510_WriteReg(0xD619,0x00);
	nt35510_WriteReg(0xD61A,0x02);
	nt35510_WriteReg(0xD61B,0x01);
	nt35510_WriteReg(0xD61C,0x02);
	nt35510_WriteReg(0xD61D,0x34);
	nt35510_WriteReg(0xD61E,0x02);
	nt35510_WriteReg(0xD61F,0x67);
	nt35510_WriteReg(0xD620,0x02);
	nt35510_WriteReg(0xD621,0x84);
	nt35510_WriteReg(0xD622,0x02);
	nt35510_WriteReg(0xD623,0xA4);
	nt35510_WriteReg(0xD624,0x02);
	nt35510_WriteReg(0xD625,0xB7);
	nt35510_WriteReg(0xD626,0x02);
	nt35510_WriteReg(0xD627,0xCF);
	nt35510_WriteReg(0xD628,0x02);
	nt35510_WriteReg(0xD629,0xDE);
	nt35510_WriteReg(0xD62A,0x02);
	nt35510_WriteReg(0xD62B,0xF2);
	nt35510_WriteReg(0xD62C,0x02);
	nt35510_WriteReg(0xD62D,0xFE);
	nt35510_WriteReg(0xD62E,0x03);
	nt35510_WriteReg(0xD62F,0x10);
	nt35510_WriteReg(0xD630,0x03);
	nt35510_WriteReg(0xD631,0x33);
	nt35510_WriteReg(0xD632,0x03);
	nt35510_WriteReg(0xD633,0x6D);
	//LV2 Page 0 enable
	nt35510_WriteReg(0xF000,0x55);
	nt35510_WriteReg(0xF001,0xAA);
	nt35510_WriteReg(0xF002,0x52);
	nt35510_WriteReg(0xF003,0x08);
	nt35510_WriteReg(0xF004,0x00);
	//Display control
	nt35510_WriteReg(0xB100, 0xCC);
	nt35510_WriteReg(0xB101, 0x00);
	//Source hold time
	nt35510_WriteReg(0xB600,0x05);
	//Gate EQ control
	nt35510_WriteReg(0xB700,0x70);
	nt35510_WriteReg(0xB701,0x70);
	//Source EQ control (Mode 2)
	nt35510_WriteReg(0xB800,0x01);
	nt35510_WriteReg(0xB801,0x03);
	nt35510_WriteReg(0xB802,0x03);
	nt35510_WriteReg(0xB803,0x03);
	//Inversion mode (2-dot)
	nt35510_WriteReg(0xBC00,0x02);
	nt35510_WriteReg(0xBC01,0x00);
	nt35510_WriteReg(0xBC02,0x00);
	//Timing control 4H w/ 4-delay
	nt35510_WriteReg(0xC900,0xD0);
	nt35510_WriteReg(0xC901,0x02);
	nt35510_WriteReg(0xC902,0x50);
	nt35510_WriteReg(0xC903,0x50);
	nt35510_WriteReg(0xC904,0x50);
	nt35510_WriteReg(0x3500,0x00);
	nt35510_WriteReg(0x3A00,0x55);  //16-bit/pixel
	nt35510_WR_REG(0x1100);
//	delay_us(120);
	nt35510_WR_REG(0x2900);

	nt35510_Display_Dir(0);		//默认为竖屏
	nt35510_clear(White);
	HAL_Delay(150);
	NT35510_BL();				//点亮背光
}

uint16_t nt35510_read_id(void)
{
	return 0x5510;
}

void nt35510_display_on(void)
{
	nt35510_WR_REG(0X2900);	//开启显示
}

void nt35510_display_off(void)
{
	nt35510_WR_REG(0X2800);	//关闭显示
}

void nt35510_set_cursor(uint16_t Xpos, uint16_t Ypos)
{
	nt35510_WR_REG(nt35510.setxcmd);
	nt35510_WR_DATA(Xpos>>8);
	nt35510_WR_REG(nt35510.setxcmd+1);
	nt35510_WR_DATA(Xpos&0XFF);
	nt35510_WR_REG(nt35510.setycmd);
	nt35510_WR_DATA(Ypos>>8);
	nt35510_WR_REG(nt35510.setycmd+1);
	nt35510_WR_DATA(Ypos&0XFF);
}

void nt35510_write_pixel(uint16_t x,uint16_t y,uint16_t color)
{
	nt35510_WR_REG(nt35510.setxcmd);
	nt35510_WR_DATA(x>>8);
	nt35510_WR_REG(nt35510.setxcmd+1);
	nt35510_WR_DATA(x&0XFF);
	nt35510_WR_REG(nt35510.setycmd);
	nt35510_WR_DATA(y>>8);
	nt35510_WR_REG(nt35510.setycmd+1);
	nt35510_WR_DATA(y&0XFF);
	LCD->LCD_REG=nt35510.wramcmd;
	LCD->LCD_RAM=color;
}

uint16_t nt35510_read_pixel(uint16_t x,uint16_t y)
{
 	uint16_t r=0,g=0,b=0;
	if(x>=nt35510.width||y>=nt35510.height) {
		return 0;	//超过了范围,直接返回
	}
	nt35510_set_cursor(x,y);
	nt35510_WR_REG(0X2E00);	//5510 发送读GRAM指令
 	r=nt35510_RD_DATA();	//dummy Read
 	r=nt35510_RD_DATA();  		  						//实际坐标颜色
	//9341/NT35310/NT35510要分2次读出
	b=nt35510_RD_DATA();
	g=r & 0xff;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
	g<<=8;
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//ILI9341/NT35310/NT35510需要公式转换一下
}

void nt35510_set_display_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	uint16_t twidth,theight;
	twidth=x+width-1;
	theight=y+height-1;

	nt35510_WR_REG(nt35510.setxcmd);
	nt35510_WR_DATA(x>>8);
	nt35510_WR_REG(nt35510.setxcmd+1);
	nt35510_WR_DATA(x&0XFF);
	nt35510_WR_REG(nt35510.setxcmd+2);
	nt35510_WR_DATA(twidth>>8);
	nt35510_WR_REG(nt35510.setxcmd+3);
	nt35510_WR_DATA(twidth&0xff);
	nt35510_WR_REG(nt35510.setycmd);
	nt35510_WR_DATA(y>>8);
	nt35510_WR_REG(nt35510.setycmd+1);
	nt35510_WR_DATA(y&0XFF);
	nt35510_WR_REG(nt35510.setycmd+2);
	nt35510_WR_DATA(theight>>8);
	nt35510_WR_REG(nt35510.setycmd+3);
	nt35510_WR_DATA(theight&0xff);
}

void nt35510_draw_h_line(uint16_t color, uint16_t x, uint16_t y, uint16_t length)
{

}
void nt35510_draw_v_line(uint16_t color, uint16_t x, uint16_t y, uint16_t length)
{

}

//void     ST7789H2_SetOrientation(uint32_t orientation);
//void     ST7789H2_WriteReg(uint8_t Command, uint8_t *Parameters, uint8_t NbParameters);
//uint8_t  ST7789H2_ReadReg(uint8_t Command);

void nt35510_draw_bitmap(uint16_t x, uint16_t y, uint8_t *pbmp)
{
	;
}

void nt35510_draw_rgb_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pimg)
{
	;
}

uint16_t nt35510_get_lcd_pixel_width(void)
{
	return 480;
}

uint16_t nt35510_get_lcd_pixel_height(void)
{
	return 800;
}

void nt35510_set_front_color(uint16_t color)
{
	POINT_COLOR = color;
}

//读取个某点的颜色值
//x,y:坐标
//返回值:此点的颜色

//LCD开启显示

//设置光标位置(对RGB屏无效)
//Xpos:横坐标
//Ypos:纵坐标

//设置LCD的自动扫描方向(对RGB屏无效)
//注意:其他函数可能会受到此函数设置的影响(尤其是9341),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9341/5310/5510/1963等IC已经实际测试
void nt35510_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint16_t dirreg=0;
	uint16_t temp;
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5);
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5);
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5);
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5);
				break;
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5);
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5);
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5);
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5);
				break;
		}
		dirreg=0X3600;
 		nt35510_WriteReg(dirreg,regval);
			if(regval&0X20)
			{
				if(nt35510.width<nt35510.height)//交换X,Y
				{
					temp=nt35510.width;
					nt35510.width=nt35510.height;
					nt35510.height=temp;
				}
			}else
			{
				if(nt35510.width>nt35510.height)//交换X,Y
				{
					temp=nt35510.width;
					nt35510.width=nt35510.height;
					nt35510.height=temp;
				}
			}

			nt35510_WR_REG(nt35510.setxcmd);nt35510_WR_DATA(0);
			nt35510_WR_REG(nt35510.setxcmd+1);nt35510_WR_DATA(0);
			nt35510_WR_REG(nt35510.setxcmd+2);nt35510_WR_DATA((nt35510.width-1)>>8);
			nt35510_WR_REG(nt35510.setxcmd+3);nt35510_WR_DATA((nt35510.width-1)&0XFF);
			nt35510_WR_REG(nt35510.setycmd);nt35510_WR_DATA(0);
			nt35510_WR_REG(nt35510.setycmd+1);nt35510_WR_DATA(0);
			nt35510_WR_REG(nt35510.setycmd+2);nt35510_WR_DATA((nt35510.height-1)>>8);
			nt35510_WR_REG(nt35510.setycmd+3);nt35510_WR_DATA((nt35510.height-1)&0XFF);
}
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void nt35510_draw_point(uint16_t x,uint16_t y)
{
	nt35510_set_cursor(x,y);		//设置光标位置
	nt35510_WriteRAM_Prepare();	//开始写入GRAM
	LCD->LCD_RAM=POINT_COLOR;
}
//快速画点
//x,y:坐标
//color:颜色

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void nt35510_Display_Dir(uint8_t dir)
{
	nt35510.dir=dir;		//竖屏/横屏
	if(dir==0)			//竖屏
	{
		nt35510.width=240;
		nt35510.height=320;

			nt35510.wramcmd=0X2C00;
	 		nt35510.setxcmd=0X2A00;
			nt35510.setycmd=0X2B00;
			nt35510.width=480;
			nt35510.height=800;

	}else 				//横屏
	{
			nt35510.wramcmd=0X2C00;
	 		nt35510.setxcmd=0X2A00;
			nt35510.setycmd=0X2B00;
			nt35510.width=800;
			nt35510.height=480;

	}
	nt35510_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}
//设置窗口(对RGB屏无效),并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height.

//初始化lcd
//该初始化函数可以初始化各种型号的LCD(详见本.c文件最前面的描述)

//清屏函数
//color:要清屏的填充色
void nt35510_clear(uint16_t color)
{
	uint32_t index=0;
	uint32_t totalpoint=nt35510.width;
	totalpoint*=nt35510.height; 			//得到总点数
	nt35510_set_cursor(0x00,0x0000);			//设置光标位置
	nt35510_WriteRAM_Prepare();     		//开始写入GRAM
	for (index=0;index<totalpoint;index++) {
		LCD->LCD_RAM=color;
	}
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void nt35510_fill(uint16_t color, uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1)
{          
	uint16_t i,j;
	uint16_t xlen=0;

	xlen=x1-x0+1;
	for (i=y0;i<=y1;i++) {
		nt35510_set_cursor(x0,i);      				//设置光标位置
		nt35510_WriteRAM_Prepare();     			//开始写入GRAM
		for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//显示颜色
	}

}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void nt35510_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;

	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
	for (i=0;i<height;i++) {
		nt35510_set_cursor(sx,sy+i);   	//设置光标位置
		nt35510_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*width+j];//写入数据
	}

}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void nt35510_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t;
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
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
	for (t=0;t<=distance+1;t++) {//画线输出
		nt35510_draw_point(uRow,uCol);//画点
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) {
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) {
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void nt35510_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	nt35510_DrawLine(x1,y1,x2,y1);
	nt35510_DrawLine(x1,y1,x1,y2);
	nt35510_DrawLine(x1,y2,x2,y2);
	nt35510_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void nt35510_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while (a<=b) {
		nt35510_draw_point(x0+a,y0-b);             //5
 		nt35510_draw_point(x0+b,y0-a);             //0
		nt35510_draw_point(x0+b,y0+a);             //4
		nt35510_draw_point(x0+a,y0+b);             //6
		nt35510_draw_point(x0-a,y0+b);             //1
 		nt35510_draw_point(x0-b,y0+a);
		nt35510_draw_point(x0-a,y0-b);             //2
  		nt35510_draw_point(x0-b,y0-a);             //7
		a++;
		//使用Bresenham算法画圆     
		if (di<0) {
			di += 4*a+6;
		}
		else {
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void nt35510_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for (t=0;t<csize;t++) {
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else if(size==32)temp=asc2_3216[num][t];	//调用3216字体
		else return;								//没有的字库
		for (t1=0;t1<8;t1++) {
			if(temp&0x80) {
				nt35510_write_pixel(x,y,POINT_COLOR);
			}
			else if(mode==0) {
				nt35510_write_pixel(x,y,BACK_COLOR);
			}
			temp<<=1;
			y++;
			if(y>=nt35510.height)return;		//超区域了
			if ((y-y0)==size) {
				y=y0;
				x++;
				if(x>=nt35510.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
uint32_t nt35510_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void nt35510_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	for (t=0;t<len;t++) {
		temp=(num/nt35510_Pow(10,len-t-1))%10;
		if (enshow==0&&t<(len-1)) {
			if (temp==0) {
				nt35510_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else {
				enshow=1;
			}
		}
	 	nt35510_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void nt35510_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;
	for (t=0;t<len;t++) {
		temp=(num/nt35510_Pow(10,len-t-1))%10;
		if (enshow==0&&t<(len-1)) {
			if (temp==0) {
				if(mode&0X80)nt35510_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);
				else nt35510_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
 				continue;
			}else {
				enshow=1;
			}
		 	 
		}
	 	nt35510_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01);
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void nt35510_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p)
{
	uint8_t x0=x;
	width+=x;
	height+=y;
    while ((*p<='~')&&(*p>=' ')) {//判断是不是非法字符!
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        nt35510_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
