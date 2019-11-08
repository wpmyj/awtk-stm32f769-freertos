#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//
//
//LED驱动代码	   
//
//
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C)  2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化PC12为输出.并使能这个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOC_CLK_ENABLE();			        //开启GPIOC时钟

	GPIO_Initure.Pin   = GPIO_PIN_12;         //PC12
	GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP; //推挽输出
	GPIO_Initure.Pull  = GPIO_PULLUP;         //上拉
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //高速
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);       //初始化GPIOC.12

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);	//PB1置0
}

