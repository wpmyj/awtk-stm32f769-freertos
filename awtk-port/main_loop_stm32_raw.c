/**
 * file:   main_loop_stm32_raw.c
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  main loop for stm32
 *
 * copyright (c) 2018 - 2018 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-05-11 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "sys.h"
#include "delay.h"
#include "led.h"
#include "ltdc.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "string.h"
#include "ltdc.h"
#include "bsp_touch_gtxx.h"

#include "base/g2d.h"
#include "base/idle.h"
#include "base/timer.h"
#include "main_loop/main_loop_simple.h"


uint8_t platform_disaptch_input(main_loop_t* loop)
{
//	int x = 0;
//	int y = 0;
////	int Touch_Pres_Ponit = 0;

////  Touch_Pres_Ponit = GTP_Execu(&x,&y);
//	
//	GT911_OnePiontScan();
//	
//	x = g_GT911_Point.x;
//	y = g_GT911_Point.y;
//	
//	if (g_GT911_Point.Pressed) 
//	{
//		main_loop_post_pointer_event(loop, TRUE, x, y);
//	} 
//	else
//	{
//		main_loop_post_pointer_event(loop, FALSE, x, y);
//	}
	return 0;
}

extern lcd_t* stm32f769_create_lcd(wh_t w, wh_t h);

lcd_t* platform_create_lcd(wh_t w, wh_t h)
{
  return stm32f769_create_lcd(w, h);
}

#include "main_loop/main_loop_raw.inc"
