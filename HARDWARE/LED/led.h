#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//
//
//LED驱动代码	   
//
//技术论坛:w
//创建日期:2015/6/10
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C)  2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//LED端口定义
#define LED(n)		  (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET))
#define LED_Toggle  (HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12)) //LED0输出电平翻转


void LED_Init(void); //LED初始化函数
#endif
