/************************************************

************************************************/
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "bsp_touch_gtxx.h"
#include "sdram.h"
#include "ltdc.h"
#include "mpu.h"
#include "qspi.h"


#include "lcd/lcd_mem_bgr565.h"
#include "lcd/lcd_mem_bgra8888.h"
#include "main_loop/main_loop_simple.h"

#include "ff_gen_drv.h"
#include "sd_diskio_dma_rtos.h"

//任务优先级
#define START_TASK_PRIO			1
//任务堆栈大小	
#define START_STK_SIZE 			256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LOW_TASK_PRIO			2
//任务堆栈大小	
#define LOW_STK_SIZE 			256  
//任务句柄
TaskHandle_t LowTask_Handler;
//任务函数
void low_task(void *pvParameters);

//任务优先级
#define MIDDLE_TASK_PRIO 		3
//任务堆栈大小	
#define MIDDLE_STK_SIZE  		256 
//任务句柄
TaskHandle_t MiddleTask_Handler;
//任务函数
void middle_task(void *pvParameters);

//任务优先级
#define HIGH_TASK_PRIO 			4
//任务堆栈大小	
#define HIGH_STK_SIZE  			10 * 1024 
//任务句柄
TaskHandle_t HighTask_Handler;
//任务函数
void high_task(void *pvParameters);

//二值信号量句柄
SemaphoreHandle_t BinarySemaphore;	//二值信号量

FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL   MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */
uint8_t workBuffer[2*_MAX_SS];


extern int gui_app_start(int lcd_w, int lcd_h);

#define FB_ADDR (uint8_t*)0xC0000000

static uint8_t* s_framebuffers[2];

lcd_t* stm32f769_create_lcd(wh_t w, wh_t h) {
  lcd_t* lcd = NULL;
  uint32_t size = w * h * lcdltdc.pixsize;
  s_framebuffers[0] = FB_ADDR;
  s_framebuffers[1] = FB_ADDR + size;

#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 
  lcd = lcd_mem_bgra8888_create_double_fb(w, h, s_framebuffers[0], s_framebuffers[1]);
#else
  lcd = lcd_mem_bgr565_create_double_fb(w, h, s_framebuffers[0], s_framebuffers[1]);
#endif /*LCD_PIXFORMAT*/
	
  return lcd;
}


void SDMMC_Init_Mount(void)
{
  /*##-1- Link the micro SD disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {  
    /*##-2- Register the file system object to the FatFs module ##############*/
    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      while(1);
    }
	}		
}


int main(void)
{
	MPU_Memory_Protection();        //保护相关存储区域
	Cache_Enable();                 //打开L1-Cache
	HAL_Init();				              //初始化HAL库
	Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
	delay_init(216);                //延时初始化
	BSP_QSPI_Init();                //初始化QSPI FLASH
	BSP_QSPI_EnableMemoryMappedMode();	//映射QSPI内存地址	
	LED_Init();                     //初始化LED
  SDRAM_Init();                   //SDRAM初始化	
  GTP_Init_Panel();				        //触摸屏初始化	
  LTDC_Init();                    //初始化LCD		
	SDMMC_Init_Mount();             //SDMMC初始化

	//创建开始任务
	xTaskCreate((TaskFunction_t )start_task,            //任务函数
							(const char*    )"start_task",          //任务名称
							(uint16_t       )START_STK_SIZE,        //任务堆栈大小
							(void*          )NULL,                  //传递给任务函数的参数
							(UBaseType_t    )START_TASK_PRIO,       //任务优先级
							(TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
	vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();           //进入临界区
	
	//创建二值信号量
	BinarySemaphore=xSemaphoreCreateBinary();
	//二值信号量创建成功以后要先释放一下
	if(BinarySemaphore!=NULL)xSemaphoreGive(BinarySemaphore);	
	
	//创建高优先级任务
	xTaskCreate((TaskFunction_t )high_task,             
							(const char*    )"high_task",           
							(uint16_t       )HIGH_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )HIGH_TASK_PRIO,        
							(TaskHandle_t*  )&HighTask_Handler);   
	//创建中等优先级任务
	xTaskCreate((TaskFunction_t )middle_task,     
							(const char*    )"middle_task",   
							(uint16_t       )MIDDLE_STK_SIZE,
							(void*          )NULL,
							(UBaseType_t    )MIDDLE_TASK_PRIO,
							(TaskHandle_t*  )&MiddleTask_Handler); 
//创建低优先级任务
	xTaskCreate((TaskFunction_t )low_task,     
							(const char*    )"low_task",   
							(uint16_t       )LOW_STK_SIZE,
							(void*          )NULL,
							(UBaseType_t    )LOW_TASK_PRIO,
							(TaskHandle_t*  )&LowTask_Handler);
	vTaskDelete(StartTask_Handler); //删除开始任务
	taskEXIT_CRITICAL();            //退出临界区
}

//高优先级任务的任务函数
void high_task(void *pvParameters)
{
	gui_app_start(lcdltdc.pwidth, lcdltdc.pheight);
	while(1)
	{
		vTaskDelay(10);	//延时500ms，也就是500个时钟节拍  
	}
}

//中等优先级任务的任务函数
void middle_task(void *pvParameters)
{
	while(1)
	{    
		GT911_OnePiontScan();
		
		if (g_GT911_Point.Pressed) 
		{
			main_loop_post_pointer_event(main_loop(), TRUE, g_GT911_Point.x, g_GT911_Point.y);
		} 
		else
		{
			main_loop_post_pointer_event(main_loop(), FALSE, g_GT911_Point.x, g_GT911_Point.y);
		}		
		vTaskDelay(20);	//延时1s，也就是1000个时钟节拍	
	}
}

//低优先级任务的任务函数
void low_task(void *pvParameters)
{
	while(1)
	{
		LED(0);
		vTaskDelay(300);	//延时1s，也就是1000个时钟节拍	
		LED(1);
		vTaskDelay(1000);	//延时1s，也就是1000个时钟节拍
	}
}
