#include "sdram.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//+-------------------+--------------------+--------------------+--------------------+
//+                       SDRAM pins assignment                                      +
//+-------------------+--------------------+--------------------+--------------------+
//| PD0  <-> FMC_D2   | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0    | PG0 <-> FMC_A10    |
//| PD1  <-> FMC_D3   | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1    | PG1 <-> FMC_A11    |
//| PD8  <-> FMC_D13  | PE7  <-> FMC_D4    | PF2  <-> FMC_A2    | PG4 <-> FMC_A14    |
//| PD9  <-> FMC_D14  | PE8  <-> FMC_D5    | PF3  <-> FMC_A3    | PG5 <-> FMC_A15    |
//| PD10 <-> FMC_D15  | PE9  <-> FMC_D6    | PF4  <-> FMC_A4    | PG8 <-> FC_SDCLK   |
//| PD14 <-> FMC_D0   | PE10 <-> FMC_D7    | PF5  <-> FMC_A5    | PG15 <-> FMC_NCAS  |
//| PD15 <-> FMC_D1   | PE11 <-> FMC_D8    | PF11 <-> FC_NRAS   |--------------------+
//+-------------------| PE12 <-> FMC_D9    | PF12 <-> FMC_A6    |
//									 | PE13 <-> FMC_D10   | PF13 <-> FMC_A7    |
//									 | PE14 <-> FMC_D11   | PF14 <-> FMC_A8    |
//									 | PE15 <-> FMC_D12   | PF15 <-> FMC_A9    |
//+-------------------+--------------------+--------------------+
//| PH2 <-> FMC_SDCKE0| PI4 <-> FMC_NBL2   |
//| PH3 <-> FMC_SDNE0 | PI5 <-> FMC_NBL3   |
//| PH5 <-> FMC_SDNW  |--------------------+
//+-------------------+
//+-------------------+------------------+
//+   32-bits Mode: D31-D16              +
//+-------------------+------------------+
//| PH8  <-> FMC_D16  | PI0  <-> FMC_D24 |
//| PH9  <-> FMC_D17  | PI1  <-> FMC_D25 |
//| PH10 <-> FMC_D18  | PI2  <-> FMC_D26 |
//| PH11 <-> FMC_D19  | PI3  <-> FMC_D27 |
//| PH12 <-> FMC_D20  | PI6  <-> FMC_D28 |
//| PH13 <-> FMC_D21  | PI7  <-> FMC_D29 |
//| PH14 <-> FMC_D22  | PI9  <-> FMC_D30 |
//| PH15 <-> FMC_D23  | PI10 <-> FMC_D31 |
//+-------------------+------------------+

//+-------------------+
//+  Pins remapping   +
//+-------------------+
//| PC0 <-> FMC_SDNWE |
//| PC2 <-> FMC_SDNE0 |
//| PC3 <-> FMC_SDCKE0|
//+-------------------+	 							  
////////////////////////////////////////////////////////////////////////////////// 	
SDRAM_HandleTypeDef SDRAM_Handler;   //SDRAM句柄

//SDRAM初始化
void SDRAM_Init(void)
{
	u32 temp = 0;
	FMC_SDRAM_TimingTypeDef SDRAM_Timing;

	SDRAM_Handler.Instance                = FMC_Bank5_6;                       //SDRAM在BANK5,6  
	SDRAM_Handler.Init.SDBank             = FMC_SDRAM_BANK1;                   //SDRAM接在BANK1上
	SDRAM_Handler.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;       //列数量
	SDRAM_Handler.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;         //行数量
	SDRAM_Handler.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_32;        //数据宽度为32位
	SDRAM_Handler.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;      //一共4个BANK
	SDRAM_Handler.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;           //CAS为2
	SDRAM_Handler.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;//失能写保护
	SDRAM_Handler.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;          //SDRAM时钟为HCLK/2=216M/2=108M=9.3ns
	SDRAM_Handler.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;           //使能突发
	SDRAM_Handler.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;           //读通道延时

	SDRAM_Timing.LoadToActiveDelay    = 2;                                     //加载模式寄存器到激活时间的延迟为2个时钟周期
	SDRAM_Timing.ExitSelfRefreshDelay = 7;                                     //退出自刷新延迟为8个时钟周期
	SDRAM_Timing.SelfRefreshTime      = 5;                                     //自刷新时间为6个时钟周期                                 
	SDRAM_Timing.RowCycleDelay        = 6;                                     //行循环延迟为6个时钟周期
	SDRAM_Timing.WriteRecoveryTime    = 2;                                     //恢复延迟为2个时钟周期
	SDRAM_Timing.RPDelay              = 2;                                     //行预充电延迟为2个时钟周期
	SDRAM_Timing.RCDDelay             = 2;                                     //行到列延迟为2个时钟周期
	HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);  

	//SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM
	SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_CLK_ENABLE,1,0); //时钟配置使能
	delay_us(500);                                  //至少延时200us
	SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_PALL,1,0);       //对所有存储区预充电
	SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);//设置自刷新次数 
	//配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度，
	//bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
	//bit9为指定的写突发模式，bit10和bit11位保留位
	temp = (u32)SDRAM_MODEREG_BURST_LENGTH_1     |	  //设置突发长度:1(可以是1/2/4/8)
	       SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |	  //设置突发类型:连续(可以是连续/交错)
	       SDRAM_MODEREG_CAS_LATENCY_2           |	  //设置CAS值:3(可以是2/3)
	       SDRAM_MODEREG_OPERATING_MODE_STANDARD |    //设置操作模式:0,标准模式
	       SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;      //设置突发写模式:1,单点访问
	SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_LOAD_MODE,1,temp); //设置SDRAM的模式寄存器

	//刷新频率计数器(以SDCLK频率计数),计算方法:
	//COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
	//我们使用的SDRAM刷新周期为64ms,SDCLK=216/2=108Mhz,行数为8192(2^13).
	//所以,COUNT=64*1000*108/8192-20=823
	HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler,0x056A);
}

//SDRAM底层驱动，引脚配置，时钟使能
//此函数会被HAL_SDRAM_Init()调用
//hsdram:SDRAM句柄
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
	GPIO_InitTypeDef gpio_init_structure;

	__HAL_RCC_FMC_CLK_ENABLE();     //使能FMC时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();   //使能GPIOD时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();   //使能GPIOE时钟
	__HAL_RCC_GPIOF_CLK_ENABLE();   //使能GPIOF时钟
	__HAL_RCC_GPIOG_CLK_ENABLE();   //使能GPIOG时钟
  __HAL_RCC_GPIOH_CLK_ENABLE();   //使能GPIOH时钟
  __HAL_RCC_GPIOI_CLK_ENABLE();	  //使能GPIOI时钟

  /* GPIOD configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8| GPIO_PIN_9 | GPIO_PIN_10 |\
                                  GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC; 
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 |\
                                  GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                                  GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;    
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 |\
                                  GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                                  GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;    
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4|\
                                  GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;															
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOH configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 |\
                                  GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                                  GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;															
  HAL_GPIO_Init(GPIOH, &gpio_init_structure); 
  
  /* GPIOI configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |\
                                  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;															
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);  
}

//向SDRAM发送命令
//bankx:0,向BANK5上面的SDRAM发送指令
//      1,向BANK6上面的SDRAM发送指令
//cmd:指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
//refresh:自刷新次数
//regval:模式寄存器的定义
//返回值:0,正常;1,失败.
u8 SDRAM_Send_Cmd(u8 bankx,u8 cmd,u8 refresh,u16 regval)
{
	u32 target_bank = 0;
	FMC_SDRAM_CommandTypeDef Command;

	if(bankx == 0)
	{
		target_bank = FMC_SDRAM_CMD_TARGET_BANK1;  
	}		
	else if(bankx == 1)
	{
		target_bank = FMC_SDRAM_CMD_TARGET_BANK2;   
	}
	Command.CommandMode            = cmd;         //命令
	Command.CommandTarget          = target_bank; //目标SDRAM存储区域
	Command.AutoRefreshNumber      = refresh;     //自刷新次数
	Command.ModeRegisterDefinition = regval;      //要写入模式寄存器的值
	if(HAL_SDRAM_SendCommand(&SDRAM_Handler,&Command,0x1000)==HAL_OK) //向SDRAM发送命令
	{
		return 0;  
	}
	else
	{
		return 1;    
	}
}

//在指定地址(WriteAddr+Bank5_SDRAM_ADDR)开始,连续写入n个字节.
//pBuffer:字节指针
//WriteAddr:要写入的地址
//n:要写入的字节数
void FMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n)
{
	for(;n != 0;n--)
	{
		*(vu8*)(Bank5_SDRAM_ADDR + WriteAddr) = *pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//在指定地址((WriteAddr+Bank5_SDRAM_ADDR))开始,连续读出n个字节.
//pBuffer:字节指针
//ReadAddr:要读出的起始地址
//n:要写入的字节数
void FMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n)
{
	for(;n != 0;n--)
	{
		*pBuffer++ = *(vu8*)(Bank5_SDRAM_ADDR + ReadAddr);
		ReadAddr++;
	}
}
