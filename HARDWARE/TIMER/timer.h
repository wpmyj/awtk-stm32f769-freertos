//////////////////////////////////////////////////////////////////////////////////	 
//
//
//定时器驱动代码	   
//
//
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C)  2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern TIM_HandleTypeDef TIM3_Handler;      //定时器3句柄 
extern volatile unsigned long long FreeRTOSRunTimeTicks;

void TIM3_Init(u16 arr,u16 psc); 
void ConfigureTimeForRunTimeStats(void);
#endif

