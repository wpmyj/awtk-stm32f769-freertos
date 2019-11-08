//////////////////////////////////////////////////////////////////////////////////  
//
//
//使用SysTick的普通计数模式对延迟进行管理(支持ucosii)
//包括delay_us,delay_ms
//
//
//创建日期:2015/6/10
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C)  2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//////////////////////////////////////////////////////////////////////////////////
#ifndef _DELAY_H
#define _DELAY_H
#include <sys.h>	   

void delay_init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u32 nms);
void delay_xms(u32 nms);
#endif

