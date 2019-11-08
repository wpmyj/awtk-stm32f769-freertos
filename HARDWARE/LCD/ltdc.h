//////////////////////////////////////////////////////////////////////////////////	 
//
//
//LCD驱动代码	   
//
//
//创建日期:2015/11/26
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C)  2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#ifndef _LCD_H
#define _LCD_H
#include "sys.h"

#define LCD_LED(n)              (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET))   //LCD背光PA8

//LCD LTDC重要参数集
typedef struct  
{							 
	u32 pwidth;			//LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
	u32 pheight;		//LCD面板的高度,固定参数,不随显示方向改变
	u16 hsw;			  //水平同步宽度
	u16 vsw;			  //垂直同步宽度
	u16 hbp;			  //水平后廊
	u16 vbp;			  //垂直后廊
	u16 hfp;			  //水平前廊
	u16 vfp;			  //垂直前廊 
	u8 activelayer; //当前层编号:0/1	
	u8 dir;				  //0,竖屏;1,横屏;
	u16 width;			//LCD宽度
	u16 height;			//LCD高度
	u32 pixsize;		//每个像素所占字节数
}_ltdc_dev;  

extern _ltdc_dev lcdltdc;		            //管理LCD LTDC参数
extern LTDC_HandleTypeDef LTDC_Handler;	    //LTDC句柄
extern DMA2D_HandleTypeDef DMA2D_Handler;   //DMA2D句柄

#define LCD_PIXEL_FORMAT_ARGB8888       0x00    
#define LCD_PIXEL_FORMAT_RGB888         0x01    
#define LCD_PIXEL_FORMAT_RGB565         0x02       
#define LCD_PIXEL_FORMAT_ARGB1555       0x03      
#define LCD_PIXEL_FORMAT_ARGB4444       0x04     
#define LCD_PIXEL_FORMAT_L8             0x05     
#define LCD_PIXEL_FORMAT_AL44           0x06     
#define LCD_PIXEL_FORMAT_AL88           0x07      

///////////////////////////////////////////////////////////////////////
//用户修改配置部分:

//定义颜色像素格式,一般用RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565	
//定义默认背景层颜色
#define LTDC_BACKLAYERCOLOR			0x00000000	
//LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define LCD_FRAME_BUF_ADDR			0xC0000000  


/*根据液晶数据手册的参数配置*/
//#define HBP  lcd_param[cur_lcd].hbp		//HSYNC后的无效像素
//#define VBP  lcd_param[cur_lcd].vbp		//VSYNC后的无效行数

//#define HSW  lcd_param[cur_lcd].hsw		//HSYNC宽度
//#define VSW  lcd_param[cur_lcd].vsw		//VSYNC宽度

//#define HFP  lcd_param[cur_lcd].hfp		//HSYNC前的无效像素
//#define VFP  lcd_param[cur_lcd].vfp		//VSYNC前的无效行数

//红色数据线
#define LTDC_R0_GPIO_PORT        	  GPIOI
#define LTDC_R0_GPIO_CLK_ENABLE()   __GPIOI_CLK_ENABLE()
#define LTDC_R0_GPIO_PIN         	 GPIO_PIN_15
#define LTDC_R0_AF			           GPIO_AF14_LTDC

#define LTDC_R1_GPIO_PORT        	 GPIOJ
#define LTDC_R1_GPIO_CLK_ENABLE()   __GPIOJ_CLK_ENABLE()
#define LTDC_R1_GPIO_PIN         	 GPIO_PIN_0
#define LTDC_R1_AF			           GPIO_AF14_LTDC

#define LTDC_R2_GPIO_PORT        	 GPIOJ
#define LTDC_R2_GPIO_CLK_ENABLE()	 __GPIOJ_CLK_ENABLE()
#define LTDC_R2_GPIO_PIN         	 GPIO_PIN_1
#define LTDC_R2_AF			           GPIO_AF14_LTDC

#define LTDC_R3_GPIO_PORT        	 GPIOJ
#define LTDC_R3_GPIO_CLK_ENABLE()	 __GPIOJ_CLK_ENABLE()
#define LTDC_R3_GPIO_PIN           GPIO_PIN_2
#define LTDC_R3_AF			           GPIO_AF14_LTDC

#define LTDC_R4_GPIO_PORT        	 GPIOJ
#define LTDC_R4_GPIO_CLK_ENABLE()	 __GPIOJ_CLK_ENABLE()
#define LTDC_R4_GPIO_PIN         	 GPIO_PIN_3
#define LTDC_R4_AF			           GPIO_AF14_LTDC

#define LTDC_R5_GPIO_PORT        	 GPIOJ
#define LTDC_R5_GPIO_CLK_ENABLE()	 __GPIOJ_CLK_ENABLE()
#define LTDC_R5_GPIO_PIN         	 GPIO_PIN_4
#define LTDC_R5_AF			           GPIO_AF14_LTDC

#define LTDC_R6_GPIO_PORT        	 GPIOJ
#define LTDC_R6_GPIO_CLK_ENABLE()	 __GPIOJ_CLK_ENABLE()
#define LTDC_R6_GPIO_PIN         	 GPIO_PIN_5
#define LTDC_R6_AF			           GPIO_AF14_LTDC

#define LTDC_R7_GPIO_PORT        	 GPIOG
#define LTDC_R7_GPIO_CLK_ENABLE()  __GPIOG_CLK_ENABLE()
#define LTDC_R7_GPIO_PIN         	 GPIO_PIN_6
#define LTDC_R7_AF			           GPIO_AF14_LTDC
//绿色数据线
#define LTDC_G0_GPIO_PORT        	 GPIOE
#define LTDC_G0_GPIO_CLK_ENABLE()	 __GPIOE_CLK_ENABLE()
#define LTDC_G0_GPIO_PIN         	 GPIO_PIN_5
#define LTDC_G0_AF			           GPIO_AF14_LTDC

#define LTDC_G1_GPIO_PORT        	 GPIOE
#define LTDC_G1_GPIO_CLK_ENABLE()	 __GPIOE_CLK_ENABLE()
#define LTDC_G1_GPIO_PIN         	 GPIO_PIN_6
#define LTDC_G1_AF			           GPIO_AF14_LTDC

#define LTDC_G2_GPIO_PORT        	 GPIOA
#define LTDC_G2_GPIO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()
#define LTDC_G2_GPIO_PIN         	 GPIO_PIN_6
#define LTDC_G2_AF			           GPIO_AF14_LTDC

#define LTDC_G3_GPIO_PORT        	 GPIOC
#define LTDC_G3_GPIO_CLK_ENABLE()	 __GPIOC_CLK_ENABLE()
#define LTDC_G3_GPIO_PIN         	 GPIO_PIN_9
#define LTDC_G3_AF			           GPIO_AF10_LTDC

#define LTDC_G4_GPIO_PORT        	 GPIOB
#define LTDC_G4_GPIO_CLK_ENABLE()	 __GPIOB_CLK_ENABLE()
#define LTDC_G4_GPIO_PIN         	 GPIO_PIN_10
#define LTDC_G4_AF			           GPIO_AF14_LTDC

#define LTDC_G5_GPIO_PORT        	 GPIOB
#define LTDC_G5_GPIO_CLK_ENABLE()	 __GPIOB_CLK_ENABLE()
#define LTDC_G5_GPIO_PIN         	 GPIO_PIN_11
#define LTDC_G5_AF			           GPIO_AF14_LTDC

#define LTDC_G6_GPIO_PORT        	 GPIOI
#define LTDC_G6_GPIO_CLK_ENABLE()	 __GPIOI_CLK_ENABLE()
#define LTDC_G6_GPIO_PIN         	 GPIO_PIN_11
#define LTDC_G6_AF			           GPIO_AF9_LTDC

#define LTDC_G7_GPIO_PORT        	 GPIOB
#define LTDC_G7_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define LTDC_G7_GPIO_PIN         	 GPIO_PIN_5
#define LTDC_G7_AF			           GPIO_AF14_LTDC

//蓝色数据线
#define LTDC_B0_GPIO_PORT        	 GPIOJ
#define LTDC_B0_GPIO_CLK_ENABLE()  __GPIOJ_CLK_ENABLE()
#define LTDC_B0_GPIO_PIN         	 GPIO_PIN_12
#define LTDC_B0_AF			           GPIO_AF14_LTDC

#define LTDC_B1_GPIO_PORT        	 GPIOJ
#define LTDC_B1_GPIO_CLK_ENABLE()  __GPIOJ_CLK_ENABLE()
#define LTDC_B1_GPIO_PIN         	 GPIO_PIN_13
#define LTDC_B1_AF			           GPIO_AF14_LTDC

#define LTDC_B2_GPIO_PORT        	 GPIOJ
#define LTDC_B2_GPIO_CLK_ENABLE()  __GPIOJ_CLK_ENABLE()
#define LTDC_B2_GPIO_PIN           GPIO_PIN_14
#define LTDC_B2_AF			           GPIO_AF14_LTDC

#define LTDC_B3_GPIO_PORT        	 GPIOJ
#define LTDC_B3_GPIO_CLK_ENABLE()  __GPIOJ_CLK_ENABLE()
#define LTDC_B3_GPIO_PIN         	 GPIO_PIN_15
#define LTDC_B3_AF			           GPIO_AF14_LTDC

#define LTDC_B4_GPIO_PORT        	 GPIOK
#define LTDC_B4_GPIO_CLK_ENABLE()  __GPIOK_CLK_ENABLE()
#define LTDC_B4_GPIO_PIN         	 GPIO_PIN_3
#define LTDC_B4_AF			           GPIO_AF14_LTDC

#define LTDC_B5_GPIO_PORT        	 GPIOK
#define LTDC_B5_GPIO_CLK_ENABLE()	 __GPIOK_CLK_ENABLE()
#define LTDC_B5_GPIO_PIN         	 GPIO_PIN_4
#define LTDC_B5_AF			           GPIO_AF14_LTDC

#define LTDC_B6_GPIO_PORT        	 GPIOK
#define LTDC_B6_GPIO_CLK_ENABLE()  __GPIOK_CLK_ENABLE()
#define LTDC_B6_GPIO_PIN         	 GPIO_PIN_5
#define LTDC_B6_AF			           GPIO_AF14_LTDC

#define LTDC_B7_GPIO_PORT        	 GPIOK
#define LTDC_B7_GPIO_CLK_ENABLE()  __GPIOK_CLK_ENABLE()
#define LTDC_B7_GPIO_PIN         	 GPIO_PIN_6
#define LTDC_B7_AF			           GPIO_AF14_LTDC

//控制信号线
/*像素时钟CLK*/
#define LTDC_CLK_GPIO_PORT              GPIOI
#define LTDC_CLK_GPIO_CLK_ENABLE()      __GPIOI_CLK_ENABLE()
#define LTDC_CLK_GPIO_PIN               GPIO_PIN_14
#define LTDC_CLK_AF			                GPIO_AF14_LTDC
/*水平同步信号HSYNC*/
#define LTDC_HSYNC_GPIO_PORT            GPIOI
#define LTDC_HSYNC_GPIO_CLK_ENABLE()    __GPIOI_CLK_ENABLE()
#define LTDC_HSYNC_GPIO_PIN             GPIO_PIN_12
#define LTDC_HSYNC_AF			              GPIO_AF14_LTDC
/*垂直同步信号VSYNC*/
#define LTDC_VSYNC_GPIO_PORT            GPIOI
#define LTDC_VSYNC_GPIO_CLK_ENABLE()    __GPIOI_CLK_ENABLE()
#define LTDC_VSYNC_GPIO_PIN             GPIO_PIN_13
#define LTDC_VSYNC_AF			              GPIO_AF14_LTDC

/*数据使能信号DE*/
#define LTDC_DE_GPIO_PORT               GPIOK
#define LTDC_DE_GPIO_CLK_ENABLE()       __GPIOK_CLK_ENABLE()
#define LTDC_DE_GPIO_PIN                GPIO_PIN_7
#define LTDC_DE_AF			                GPIO_AF14_LTDC

/*液晶屏背光信号，高电平使能*/
#define LTDC_BL_GPIO_PORT               GPIOA
#define LTDC_BL_GPIO_CLK_ENABLE()       __GPIOA_CLK_ENABLE()
#define LTDC_BL_GPIO_PIN                GPIO_PIN_8

void LTDC_Switch(u8 sw);					//LTDC开关
void LTDC_Layer_Switch(u8 layerx,u8 sw);	//层开关
void LTDC_Select_Layer(u8 layerx);			//层选择
void LTDC_Display_Dir(u8 dir);				//显示方向控制
void LTDC_Draw_Point(u16 x,u16 y,u32 color);//画点函数
u32 LTDC_Read_Point(u16 x,u16 y);			//读点函数
void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);			//矩形单色填充函数
void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);	//矩形彩色填充函数
void LTDC_Clear(u32 color);					//清屏函数
u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr);//LTDC时钟配置
void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height);//LTDC层窗口设置
void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u32 bkcolor);//LTDC基本参数设置
void LTDC_Init(void);						//LTDC初始化函数
#endif 
