//////////////////////////////////////////////////////////////////////////////////	 
//
//
//MPU驱动代码	   
//
//
//创建日期:2015/11/30
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C)  2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#ifndef __MPU_H
#define __MPU_H
#include "sys.h"	

u8 MPU_Set_Protection(u32 baseaddr,u32 size,u32 rnum,u32 ap,u8 sen,u8 cen,u8 ben);
void MPU_Memory_Protection(void);
#endif
