/**
  ******************************************************************************
  * @file    gtxx.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2c电容屏驱动函数gt9157/gt911/gt5688芯片
  ******************************************************************************
  */ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bsp_touch_gtxx.h"
#include "bsp_i2c_touch.h"
#include "sys.h"

// 5寸屏GT9157驱动配置
const uint8_t CTP_CFG_GT9157[] ={ 
	0x00,0x20,0x03,0xE0,0x01,0x05,0x3C,0x00,0x01,0x08,
	0x28,0x0C,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x17,0x19,0x1E,0x14,0x8B,0x2B,0x0D,
	0x33,0x35,0x0C,0x08,0x00,0x00,0x00,0x9A,0x03,0x11,
	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x32,0x00,0x00,
	0x00,0x20,0x58,0x94,0xC5,0x02,0x00,0x00,0x00,0x04,
	0xB0,0x23,0x00,0x93,0x2B,0x00,0x7B,0x35,0x00,0x69,
	0x41,0x00,0x5B,0x4F,0x00,0x5B,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
	0x12,0x14,0x16,0x18,0x1A,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0F,
	0x10,0x12,0x13,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,
	0x21,0x22,0x24,0x26,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
	0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x48,0x01
};

// 7寸屏GT911驱动配置
const uint8_t CTP_CFG_GT911[] =  {
	0xFF,              // 寄存器地址：0x8047，功能：配置版本号
	0x00,0x04,         // 寄存器地址：0x8048~0x8049，功能：X坐标最大值，低位在前
	0x58,0x02,         // 寄存器地址：0x804A~0x804B，功能：Y坐标最大值，低位在前
	0x05,              // 寄存器地址：0x804C，功能：设置最大触摸点数，1~5点
	0x0D,              // 寄存器地址：0x804D，功能：设置INT触发方式、XY坐标交换
	0x00,              // 寄存器地址：0x804E，该寄存器无需配置
	0x03,              // 寄存器地址：0x804F，功能：按下或松开去抖次数
	0x0F,              // 寄存器地址：0x8050，功能：原始坐标窗口滤波值

	0x1E,              // 寄存器地址：0x8051，功能：大面积触摸点个数
	0x0F,              // 寄存器地址：0x8052，功能：噪声消除值（系数为1,0-15有效）
	0x5A,              // 寄存器地址：0x8053，功能：屏上触摸点从无到有的阈值
	0x32,              // 寄存器地址：0x8054，功能：屏上触摸点从有到无的阈值
	0x03,              // 寄存器地址：0x8055，功能：进低功耗时间(0~15s)
	0x05,              // 寄存器地址：0x8056，功能：坐标上报率(周期为5+N ms)
	0x00,0x00,         // 寄存器地址：0x8057-0x8058，功能：该寄存器无需配置	
	0x00,0x00,         // 寄存器地址：0x8059-0x805A，功能：该寄存器无需配置

	0x00,              // 寄存器地址：0x805B，功能：高4bit上边框的空白区（以32为系数）
	                   //                           低4bit下边框的空白区（以32为系数）
	0x00,              // 寄存器地址：0x805C，功能：高4bit左边框的空白区（以32为系数） 
	                   //                           低4bit右边框的空白区（以32为系数）
	0x0F,              // 寄存器地址：0x805D，功能：弱拉伸的拉伸程度（拉伸 x/16 Pitch）
                     //	（beta版占用，发布版无效）
	0x18,              // 寄存器地址：0x805E，功能：区间1系数
	0x1A,              // 寄存器地址：0x805F，功能：区间2系数
	0x1E,              // 寄存器地址：0x8060，功能：区间3系数
	0x14,              // 寄存器地址：0x8061，功能：各区间基数
	0x8C,              // 寄存器地址：0x8062，功能：A组驱动数量
	0x2E,              // 寄存器地址：0x8063，功能：B组驱动数量
	0x0E,              // 寄存器地址：0x8064，功能：高4bit Sensor_Group_B_Number 
	                   //                           低4bit Sensor_Group_A_Number
	
	0x20,              // 寄存器地址：0x8065，功能：驱动组A的驱动频率倍频系数 
	                   // GroupA_Frequence = 倍频系数 * 基频
	0x22,              // 寄存器地址：0x8066，功能：驱动组B的驱动频率倍频系数
                     //	GroupA_Frequence = 倍频系数 * 基频
	0x70,0x11,         // 寄存器地址：0x8067-0x8068，功能：驱动组A、B的基频(1526HZ<基频<14600Hz)
	0x00,0x00,         // 寄存器地址：0x8069-0x806A，功能：相邻两次驱动信号输出时间间隔
	                   // （以us为单位）,Reserved（beta版占用，发布版无效）
	0x02,              // 寄存器地址：0x806B，功能：A组驱动数量
	0x02,              // 寄存器地址：0x806C，功能：A组驱动数量
	0x03,              // 寄存器地址：0x806D，功能：A组驱动数量
	0x1D,              // 寄存器地址：0x806E，功能：A组驱动数量
	
	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		// 0x806F ~ 0x8078
	0x00,              // 寄存器地址：0x8079
	0x23,              // 寄存器地址：0x807A，跳频范围的起点频率 
	0x53,							 // 寄存器地址：0x807B，跳频范围的终点频率
	0x94,							 // 寄存器地址：0x807C，多次噪声检测后确定噪声量，1-63有效
	0xC5,							 // 寄存器地址：0x807D，噪声检测超时时间
	0x02,							 // 寄存器地址：0x807E，判别有干扰的门限
	0x07,							 // 寄存器地址：0x807F
	0x00,0x00,				 // 寄存器地址：0x8080 - 0x8081
	0x04,							 // 寄存器地址：0x8082，跳频检测区间频段1中心点基频（适用于驱动A、B）
	0x6D,							 // 寄存器地址：0x8083
	0x26,							 // 寄存器地址：0x8084  跳频检测区间频段1中心点倍频系数
	0x00, 						 // 寄存器地址：0x8085  跳频检测区间频段2中心点基频(驱动A、B在此基础上换算) 
	0x5F,							 // 寄存器地址：0x8086  
	0x2D,							 // 寄存器地址：0x8087  跳频检测区间频段2中心点倍频系数
	0x00,							 // 寄存器地址：0x8088  跳频检测区间频段3中心点基频（适用于驱动A、B） 
	0x52,							 // 寄存器地址：0x8089
	0x36,							 // 寄存器地址：0x808A  跳频检测区间频段3中心点倍频系数
	0x00,							 // 寄存器地址：0x808B  跳频检测区间频段4中心点基频（适用于驱动A、B）
	0x4B,							 // 寄存器地址：0x808C
	0x40,							 // 寄存器地址：0x808D  跳频检测区间频段4中心点倍频系数
	0x00,							 // 寄存器地址：0x808E  跳频检测区间频段5中心点基频（适用于驱动A、B）
	0x43,							 // 寄存器地址：0x808F
	0x4C,							 // 寄存器地址：0x8090  跳频检测区间频段5中心点倍频系数
	0x00,0x43,         // 寄存器地址：0x8091 - 0x8092
	0x00,							 // 寄存器地址：0x8093  key 1:位置 0:无按键 
	0x00,							 // 寄存器地址：0x8094  key 1:位置 0:无按键 
	0x00,							 // 寄存器地址：0x8095  key 1:位置 0:无按键 
	0x00, 						 // 寄存器地址：0x8096  key 1:位置 0:无按键 
	0x00,							 // 寄存器地址：0x8097
	0x00,							 // 寄存器地址：0x8098
	0x00,							 // 寄存器地址：0x8099
	0x00,							 // 寄存器地址：0x809A
	0x00,							 // 寄存器地址：0x809B
	0x00,							 // 寄存器地址：0x809C
	0x00,0x00,0x00,0x00,// 寄存器地址：0x809D - 0x80A0
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		// 0x80A1 - 0x80AA
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,              // 0x80AB - 0x80B2
	0x00,              // 寄存器地址：0x80B3
	0x00,		           // 寄存器地址：0x80B4
	0x00,0x00,         // 寄存器地址：0x80B5 - 0x80B6

	/******************************************************************************************
	*	寄存器地址:	0x80B7~0X80C4
	*	功能说明  :	修改感应通道对应的芯片通道号，可以改变触摸面板的垂直扫描方向
	*******************************************************************************************/

	0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,      // 扫描方向从 上 到 下
	0x16,0x18,0x1A,0x1C,

	/******************************************************************************************/

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		  // 无作用寄存器，无需配置
	0x00,0x00,0x00,0x00,0x00,0x00,                    		  // 无作用寄存器，无需配置

	/*******************************************************************************************
	*	寄存器地址:	0x80D5~0x80EE
	*	功能说明  :	修改驱动通道对应的芯片通道号，可以改变触摸面板的水平扫描方向
	********************************************************************************************/

	0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0F,0x10,0x12,		  // 扫描方向从 左 到 右
	0x13,0x14,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
	0x22,0x24,0x26,0x28,0x29,0x2A,

	/*******************************************************************************************/	

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	    // 通道调整系数寄存器，无需修改
	0x00,0x00,0x00,0x00,0x00,0x00,                          // 通道调整系数寄存器，无需修改
};

// 4.5寸屏GT5688驱动配置

const uint8_t CTP_CFG_GT5688[] =  {
0x97,0xE0,0x01,0x10,0x01,0x05,0x0D,0x00,0x01,0x00,
0x00,0x05,0x50,0x3C,0x53,0x11,0x00,0x00,0x11,0x11,
0x14,0x14,0x14,0x22,0x0A,0x04,0x00,0x00,0x00,0x00,
0x00,0x00,0x53,0x00,0x14,0x00,0x00,0x84,0x00,0x00,
0x3C,0x00,0x00,0x64,0x1E,0x28,0x87,0x27,0x08,0x32,
0x34,0x05,0x0D,0x20,0x33,0x60,0x13,0x02,0x24,0x00,
0x00,0x64,0x80,0x80,0x14,0x02,0x00,0x00,0x54,0xC5,
0x26,0xA5,0x2E,0x90,0x36,0x80,0x3F,0x75,0x47,0x6C,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x50,0x3C,
0xFF,0xFF,0x07,0x00,0x00,0x00,0x02,0x14,0x14,0x03,
0x04,0x00,0x21,0x64,0x0A,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x32,0x20,0x50,0x3C,0x3C,0x00,0x00,0x00,0x00,0x00,
0x0D,0x06,0x0C,0x05,0x0B,0x04,0x0A,0x03,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x02,0x03,0x04,0x05,
0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x3C,0x00,0x05,0x1E,0x00,0x02,
0x2A,0x1E,0x19,0x14,0x02,0x00,0x03,0x0A,0x05,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0x86,
0x22,0x03,0x00,0x00,0x33,0x00,0x0F,0x00,0x00,0x00,
0x50,0x3C,0x50,0x00,0x00,0x00,0x2F,0xE1,0x01
};


//uint8_t config[GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH]
//                = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};

/* 触摸IC类型默认为5寸屏的ic */
TOUCH_IC touchIC = GT911;		

const TOUCH_PARAM_TypeDef touch_param[3] = 
{
  /* GT9157,5寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047,
  },
  /* GT911,7寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047,
  },
  /* GT5688,4.3寸屏 */
  {
    .max_width = 480,
    .max_height = 272,
    .config_reg_addr = 0x8050,
  }
};

uint8_t Touch_Pres_Down = 0;
GT911_T g_GT911;
GT911_Point g_GT911_Point;

static int8_t GTP_I2C_Test(void);
//static void GT91xx_Config_Read_Proc(void);

//static void Delay(__IO uint32_t nCount)	 //简单的延时函数
//{
//	for(; nCount != 0; nCount--);
//}


/**
  * @brief   使用IIC进行数据传输
  * @param
  *		@arg i2c_msg:数据传输结构体
  *		@arg num:数据传输结构体的个数
  * @retval  正常完成的传输结构个数，若不正常，返回0xff
  */
static int I2C_Transfer( struct i2c_msg *msgs,int num)
{
	int im = 0;
	int ret = 0;

	for (im = 0; ret == 0 && im != num; im++)
	{
		if ((msgs[im].flags&I2C_M_RD))																//根据flag判断是读数据还是写数据
		{
			ret = I2C_ReadBytes(msgs[im].addr, msgs[im].buf, msgs[im].len);		//IIC读取数据
		} else
		{
			ret = I2C_WriteBytes(msgs[im].addr,  msgs[im].buf, msgs[im].len);	//IIC写入数据
		}
	}

	if(ret)
		return ret;

	return im;   													//正常完成的传输结构个数
}

/**
  * @brief   从IIC设备中读取数据
  * @param
  *		@arg client_addr:设备地址
  *		@arg  buf[0~1]: 读取数据寄存器的起始地址
  *		@arg buf[2~len-1]: 存储读出来数据的缓冲buffer
  *		@arg len:    GTP_ADDR_LENGTH + read bytes count（寄存器地址长度+读取的数据字节数）
  * @retval  i2c_msgs传输结构体的个数，2为成功，其它为失败
  */
static int32_t GTP_I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct i2c_msg msgs[2];
    int32_t ret=-1;
    int32_t retries = 0;
    /*一个读数据的过程可以分为两个传输过程:
     * 1. IIC  写入 要读取的寄存器地址
     * 2. IIC  读取  数据
     * */

    msgs[0].flags = !I2C_M_RD;					//写入
    msgs[0].addr  = client_addr;					//IIC设备地址
    msgs[0].len   = GTP_ADDR_LENGTH;	//寄存器地址为2字节(即写入两字节的数据)
    msgs[0].buf   = &buf[0];						//buf[0~1]存储的是要读取的寄存器地址
    
    msgs[1].flags = I2C_M_RD;					//读取
    msgs[1].addr  = client_addr;					//IIC设备地址
    msgs[1].len   = len - GTP_ADDR_LENGTH;	//要读取的数据长度
    msgs[1].buf   = &buf[GTP_ADDR_LENGTH];	//buf[GTP_ADDR_LENGTH]之后的缓冲区存储读出的数据

    while(retries < 5)
    {
        ret = I2C_Transfer( msgs, 2);					//调用IIC数据传输过程函数，有2个传输过程
        if(ret == 2)break;
        retries++;
    }
    if((retries >= 5))
    {
      
    }
    return ret;
}



/**
  * @brief   向IIC设备写入数据
  * @param
  *		@arg client_addr:设备地址
  *		@arg  buf[0~1]: 要写入的数据寄存器的起始地址
  *		@arg buf[2~len-1]: 要写入的数据
  *		@arg len:    GTP_ADDR_LENGTH + write bytes count（寄存器地址长度+写入的数据字节数）
  * @retval  i2c_msgs传输结构体的个数，1为成功，其它为失败
  */
static int32_t GTP_I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
{
    struct i2c_msg msg;
    int32_t ret = -1;
    int32_t retries = 0;
    /*一个写数据的过程只需要一个传输过程:
     * 1. IIC连续 写入 数据寄存器地址及数据
     * */
    msg.flags = !I2C_M_RD;			//写入
    msg.addr  = client_addr;			//从设备地址
    msg.len   = len;							//长度直接等于(寄存器地址长度+写入的数据字节数)
    msg.buf   = buf;						//直接连续写入缓冲区中的数据(包括了寄存器地址)

    while(retries < 5)
    {
        ret = I2C_Transfer(&msg, 1);	//调用IIC数据传输过程函数，1个传输过程
        if (ret == 1)break;
        retries++;
    }
    if((retries >= 5))
    {

    }
    return ret;
}



/**
  * @brief   使用IIC读取再次数据，检验是否正常
  * @param
  *		@arg client:设备地址
  *		@arg  addr: 寄存器地址
  *		@arg rxbuf: 存储读出的数据
  *		@arg len:    读取的字节数
  * @retval
  * 	@arg FAIL
  * 	@arg SUCCESS
  */
 int32_t GTP_I2C_Read_dbl_check(uint8_t client_addr, uint16_t addr, uint8_t *rxbuf, int len)
{
    uint8_t buf[16] = {0};
    uint8_t confirm_buf[16] = {0};
    uint8_t retry = 0;

    while (retry++ < 3)
    {
        memset(buf, 0xAA, 16);
        buf[0] = (uint8_t)(addr >> 8);
        buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, buf, len + 2);
        
        memset(confirm_buf, 0xAB, 16);
        confirm_buf[0] = (uint8_t)(addr >> 8);
        confirm_buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, confirm_buf, len + 2);

      
        if (!memcmp(buf, confirm_buf, len+2))
        {
            memcpy(rxbuf, confirm_buf+2, len);
            return SUCCESS;
        }
    }    
    return FAIL;
}


/**
  * @brief   关闭GT91xx中断
  * @param 无
  * @retval 无
  */
void GTP_IRQ_Disable(void)
{
    I2C_GTP_IRQDisable();
}

/**
  * @brief   使能GT91xx中断
  * @param 无
  * @retval 无
  */
void GTP_IRQ_Enable(void)
{
	I2C_GTP_IRQEnable();    
}


/**
  * @brief   用于处理或报告触屏检测到按下
  * @param
  *    @arg     id: 触摸顺序trackID
  *    @arg     x:  触摸的 x 坐标
  *    @arg     y:  触摸的 y 坐标
  *    @arg     w:  触摸的 大小
  * @retval 无
  */
/*用于记录连续触摸时(长按)的上一次触摸位置，负数值表示上一次无触摸按下*/
//static int16_t pre_x[GTP_MAX_TOUCH] ={-1,-1,-1,-1,-1};
//static int16_t pre_y[GTP_MAX_TOUCH] ={-1,-1,-1,-1,-1};

static void GTP_Touch_Down(int32_t id,int32_t x,int32_t y,int32_t w)
{

}


/**
  * @brief   用于处理或报告触屏释放
  * @param 释放点的id号
  * @retval 无
  */
static void GTP_Touch_Up( int32_t id)
{

}


/**
  * @brief   触屏处理函数，轮询或者在触摸中断调用
  * @param 无
  * @retval 无
  */
static void Goodix_TS_Work_Func(void)
{
    uint8_t  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
    uint8_t  point_data[2 + 1 + 8 * GTP_MAX_TOUCH + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
    uint8_t  touch_num = 0;
    uint8_t  finger = 0;
    static uint16_t pre_touch = 0;
    static uint8_t pre_id[GTP_MAX_TOUCH] = {0};

    uint8_t client_addr=GTP_ADDRESS;
    uint8_t* coor_data = NULL;
    int32_t input_x = 0;
    int32_t input_y = 0;
    int32_t input_w = 0;
    uint8_t id = 0;
 
    int32_t i  = 0;
    int32_t ret = -1;

    ret = GTP_I2C_Read(client_addr, point_data, 12);//10字节寄存器加2字节地址
    if (ret < 0)
    {
        return;
    }
    
    finger = point_data[GTP_ADDR_LENGTH];//状态寄存器数据

    if (finger == 0x00)		//没有数据，退出
    {
        return;
    }

    if((finger & 0x80) == 0)//判断buffer status位
    {
        goto exit_work_func;//坐标未就绪，数据无效
    }

    touch_num = finger & 0x0f;//坐标点数
    if (touch_num > GTP_MAX_TOUCH)
    {
        goto exit_work_func;//大于最大支持点数，错误退出
    }

    if (touch_num > 1)//不止一个点
    {
        uint8_t buf[8 * GTP_MAX_TOUCH] = {(GTP_READ_COOR_ADDR + 10) >> 8, (GTP_READ_COOR_ADDR + 10) & 0xff};

        ret = GTP_I2C_Read(client_addr, buf, 2 + 8 * (touch_num - 1));
        memcpy(&point_data[12], &buf[2], 8 * (touch_num - 1));			//复制其余点数的数据到point_data
    }

    
    
    if (pre_touch>touch_num)				//pre_touch>touch_num,表示有的点释放了
    {
        for (i = 0; i < pre_touch; i++)						//一个点一个点处理
         {
            uint8_t j;
           for(j=0; j<touch_num; j++)
           {
               coor_data = &point_data[j * 8 + 3];
               id = coor_data[0] & 0x0F;									//track id
              if(pre_id[i] == id)
                break;

              if(j >= touch_num-1)											//遍历当前所有id都找不到pre_id[i]，表示已释放
              {
                 GTP_Touch_Up( pre_id[i]);
              }
           }
       }
    }


    if (touch_num)
    {
        for (i = 0; i < touch_num; i++)						//一个点一个点处理
        {
            coor_data = &point_data[i * 8 + 3];

            id = coor_data[0] & 0x0F;									//track id
            pre_id[i] = id;

            input_x  = coor_data[1] | (coor_data[2] << 8);	//x坐标
            input_y  = coor_data[3] | (coor_data[4] << 8);	//y坐标
            input_w  = coor_data[5] | (coor_data[6] << 8);	//size
        
            {
                GTP_Touch_Down( id, input_x, input_y, input_w);//数据处理
            }
        }
    }
    else if (pre_touch)		//touch_ num=0 且pre_touch！=0
    {
      for(i=0;i<pre_touch;i++)
      {
          GTP_Touch_Up(pre_id[i]);
      }
    }


    pre_touch = touch_num;


exit_work_func:
    {
        ret = GTP_I2C_Write(client_addr, end_cmd, 3);
        if (ret < 0)
        {
					
        }
    }

}


/**
  * @brief   给触屏芯片重新复位
  * @param 无
  * @retval 无
  */
int8_t GTP_Reset_Guitar(void)
{
#if 1
    I2C_ResetChip();
    return 0;
#else 		//软件复位
    int8_t ret = -1;
    int8_t retry = 0;
    uint8_t reset_command[3]={(uint8_t)GTP_REG_COMMAND>>8,(uint8_t)GTP_REG_COMMAND&0xFF,2};

    //写入复位命令
    while(retry++ < 5)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, reset_command, 3);
        if (ret > 0)
        {
            return ret;
        }

    }
    return ret;
#endif

}



 /**
   * @brief   进入睡眠模式
   * @param 无
   * @retval 1为成功，其它为失败
   */
//int8_t GTP_Enter_Sleep(void)
//{
//    int8_t ret = -1;
//    int8_t retry = 0;
//    uint8_t reset_comment[3] = {(uint8_t)(GTP_REG_COMMENT >> 8), (uint8_t)GTP_REG_COMMENT&0xFF, 5};//5
//
//    GTP_DEBUG_FUNC();
//
//    while(retry++ < 5)
//    {
//        ret = GTP_I2C_Write(GTP_ADDRESS, reset_comment, 3);
//        if (ret > 0)
//        {
//            GTP_INFO("GTP enter sleep!");
//
//            return ret;
//        }
//
//    }
//    GTP_ERROR("GTP send sleep cmd failed.");
//    return ret;
//}


int8_t GTP_Send_Command(uint8_t command)
{
    int8_t ret = -1;
    int8_t retry = 0;
    uint8_t command_buf[3] = {(uint8_t)(GTP_REG_COMMAND >> 8), (uint8_t)GTP_REG_COMMAND&0xFF, GTP_COMMAND_READSTATUS};

    while(retry++ < 5)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, command_buf, 3);
        if (ret > 0)
        {
            return ret;
        }

    }
    return ret;
}

/**
  * @brief   唤醒触摸屏
  * @param 无
  * @retval 0为成功，其它为失败
  */
int8_t GTP_WakeUp_Sleep(void)
{
    uint8_t retry = 0;
    int8_t ret = -1;

    while(retry++ < 10)
    {
        ret = GTP_I2C_Test();
        if (ret > 0)
        {
            return ret;
        }
        GTP_Reset_Guitar();
    }
    return ret;
}

static int32_t GTP_Get_Info(void)
{
    uint8_t opr_buf[10] = {0};
    int32_t ret = 0;

//    uint16_t abs_x_max = GTP_MAX_WIDTH;
//    uint16_t abs_y_max = GTP_MAX_HEIGHT;
//    uint8_t int_trigger_type = GTP_INT_TRIGGER;
        
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+1) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+1) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 10);
    if (ret < 0)
    {
        return FAIL;
    }
    
//    abs_x_max = (opr_buf[3] << 8) + opr_buf[2];
//    abs_y_max = (opr_buf[5] << 8) + opr_buf[4];

    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+6) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+6) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 3);
    if (ret < 0)
    {
        return FAIL;
    }
//    int_trigger_type = opr_buf[2] & 0x03;
    
    return SUCCESS;    
}

/*******************************************************
Function:
    Initialize gtp.
Input:
    ts: goodix private data
Output:
    Executive outcomes.
        0: succeed, otherwise: failed
*******************************************************/
 int32_t GTP_Init_Panel(void)
{
    int32_t ret = -1;

    int32_t i = 0;
    uint16_t check_sum = 0;
    int32_t retry = 0;

    const uint8_t* cfg_info;
    uint8_t cfg_info_len  ;
		uint8_t* config;

    uint8_t cfg_num =0 ;		//需要配置的寄存器个数

    I2C_Touch_Init();

    ret = GTP_I2C_Test();
    if (ret < 0)
    {
				return ret;
    } 
		
		//获取触摸IC的型号
    GTP_Read_Version(); 

		config = (uint8_t *)malloc (GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH);

		config[0] = GTP_REG_CONFIG_DATA >> 8;
		config[1] =  GTP_REG_CONFIG_DATA & 0xff;
		
		//根据IC的型号指向不同的配置
		if(touchIC == GT9157)
		{
			cfg_info =  CTP_CFG_GT9157; //指向寄存器配置
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT9157);//计算配置表的大小
		}
		else if(touchIC == GT911)
		{
			cfg_info =  CTP_CFG_GT911;//指向寄存器配置
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT911) ;//计算配置表的大小
		}
		else if(touchIC == GT5688)			
		{
			cfg_info =  CTP_CFG_GT5688; //指向寄存器配置
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT5688);//计算配置表的大小
		}
		
    memset(&config[GTP_ADDR_LENGTH], 0, GTP_CONFIG_MAX_LENGTH);
    memcpy(&config[GTP_ADDR_LENGTH], cfg_info, cfg_info_len);
		
		cfg_num = cfg_info_len;
		
		/*根据LCD的扫描方向设置分辨率*/
		config[GTP_ADDR_LENGTH+1] = LCD_PIXEL_WIDTH & 0xFF;
		config[GTP_ADDR_LENGTH+2] = LCD_PIXEL_WIDTH >> 8;
		config[GTP_ADDR_LENGTH+3] = LCD_PIXEL_HEIGHT & 0xFF;
		config[GTP_ADDR_LENGTH+4] = LCD_PIXEL_HEIGHT >> 8;
		
		/*根据模式设置X2Y交换*/

    //不交换
    //				config[GTP_ADDR_LENGTH+6] &= ~(X2Y_LOC);

    //交换
    config[GTP_ADDR_LENGTH+6] |= (X2Y_LOC);

    //计算要写入checksum寄存器的值
    check_sum = 0;

    /* 计算check sum校验值 */
    if(touchIC == GT911 || touchIC == GT9157)
    {
        for (i = GTP_ADDR_LENGTH; i < cfg_num+GTP_ADDR_LENGTH; i++)
        {
            check_sum += (config[i] & 0xFF);
        }
        config[ cfg_num+GTP_ADDR_LENGTH] = (~(check_sum & 0xFF)) + 1; 	//checksum
        config[ cfg_num+GTP_ADDR_LENGTH+1] =  1; 						//refresh 配置更新标志
    }
    else if(touchIC == GT5688) 
    {
      for (i = GTP_ADDR_LENGTH; i < (cfg_num+GTP_ADDR_LENGTH -3); i += 2) 
      {
        check_sum += (config[i] << 8) + config[i + 1];
      }
      
      check_sum = 0 - check_sum;
      //更新checksum
      config[(cfg_num+GTP_ADDR_LENGTH -3)] = (check_sum >> 8) & 0xFF;
      config[(cfg_num+GTP_ADDR_LENGTH -2)] = check_sum & 0xFF;
      config[(cfg_num+GTP_ADDR_LENGTH -1)] = 0x01;
    }

    //写入配置信息
    for (retry = 0; retry < 5; retry++)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, config , cfg_num + GTP_ADDR_LENGTH+2);
        if (ret > 0)
        {
            break;
        }
    }
    //Delay(0xffffff);				//延迟等待芯片更新
		HAL_Delay(50);

		
#if 1	//读出写入的数据，检查是否正常写入
    //检验读出的数据与写入的是否相同
	{
		uint16_t i;
		uint8_t buf[300];
		buf[0] = config[0];
		buf[1] =config[1];    //寄存器地址

		ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));

		//不对比版本号
		for(i=3;i<cfg_num+GTP_ADDR_LENGTH-3;i++)
		{

			if(config[i] != buf[i])
			{
				free(config);
				return -1;
			}
		}
		if(i==cfg_num+GTP_ADDR_LENGTH-3){}
}
#endif
	
		
	 /* emXGUI示例中不使能中断 */
		I2C_GTP_IRQDisable();
	
    GTP_Get_Info();
		
		free(config);

    return 0;
}


/*******************************************************
Function:
    Read chip version.
Input:
    client:  i2c device
    version: buffer to keep ic firmware version
Output:
    read operation return.
        2: succeed, otherwise: failed
*******************************************************/
int32_t GTP_Read_Version(void)
{
	int32_t ret = -1;
	uint8_t buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff};    //寄存器地址

	ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
	if (ret < 0)
	{
			return ret;
	}

	if (buf[2] == '9')
	{				
		//GT911芯片
		if(buf[2] == '9' && buf[3] == '1' && buf[4] == '1')
		{
			touchIC = GT911;
		}
		//GT9157芯片
		else if( buf[2] == '9' && buf[3] == '1' && buf[4] == '5' && buf[5] == '7')
		{
			touchIC = GT9157;
		}

	}    
	else if (buf[2] == '5')
	{	
		//GT5688芯片
		if(buf[2] == '5' && buf[3] == '6' && buf[4] == '8' && buf[5] == '8')
		{
			touchIC = GT5688;
		}
	}
	return ret;
}

/*******************************************************
Function:
    I2c test Function.
Input:
    client:i2c client.
Output:
    Executive outcomes.
        2: succeed, otherwise failed.
*******************************************************/
static int8_t GTP_I2C_Test( void)
{
	uint8_t test[3] = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};
	uint8_t retry = 0;
	int8_t ret = -1;

	while(retry++ < 5)
	{
		ret = GTP_I2C_Read(GTP_ADDRESS, test, 3);
		if (ret > 0)
		{
				return ret;
		}
	}
	return ret;
}

//检测到触摸中断时调用，
void GTP_TouchProcess(void)
{
  Goodix_TS_Work_Func();
}

#if 0//没有到的测试函数
/*******************************************************
Function:
    Request gpio(INT & RST) ports.
Input:
    ts: private data.
Output:
    Executive outcomes.
        >= 0: succeed, < 0: failed
*******************************************************/
static int8_t GTP_Request_IO_Port(struct goodix_ts_data *ts)
{
}

/*******************************************************
Function:
    Request interrupt.
Input:
    ts: private data.
Output:
    Executive outcomes.
        0: succeed, -1: failed.
*******************************************************/
static int8_t GTP_Request_IRQ(struct goodix_ts_data *ts)
{
}

//输出要初始化的数据及芯片中的真实数据
static void GT91xx_Config_Read_Proc(void)
{
    char temp_data[GTP_CONFIG_MAX_LENGTH + 2] = {0x80, 0x47};
    int i;

    GTP_INFO("==== GT9XX config init value====\n");

    for (i = 0 ; i < GTP_CONFIG_MAX_LENGTH ; i++)
    {
        printf("reg0x%x = 0x%02X ", i+0x8047, config[i + 2]);

        if (i % 10 == 9)
            printf("\n");
    }


    GTP_INFO("==== GT9XX config real value====\n");
    GTP_I2C_Read(GTP_ADDRESS, (uint8_t *)temp_data, GTP_CONFIG_MAX_LENGTH + 2);
    for (i = 0 ; i < GTP_CONFIG_MAX_LENGTH ; i++)
    {
        printf("reg0x%x = 0x%02X ", i+0x8047,temp_data[i+2]);

        if (i % 10 == 9)
            printf("\n");
    }

}

//向芯片写入配置数据
static int32_t GT91xx_Config_Write_Proc(void)
{
    int32_t ret = -1;

    int32_t i = 0;
    uint8_t check_sum = 0;
    int32_t retry = 0;
    uint8_t cfg_num =0x80FE-0x8047+1 ;		//需要配置的寄存器个数

    uint8_t cfg_info[] = CTP_CFG_GROUP1;
    uint8_t cfg_info_len =CFG_GROUP_LEN(cfg_info) ;

    GTP_INFO("==== GT9XX send config====\n");

    memset(&config[GTP_ADDR_LENGTH], 0, GTP_CONFIG_MAX_LENGTH);
    memcpy(&config[GTP_ADDR_LENGTH], cfg_info,cfg_info_len);

    //计算要写入checksum寄存器的值
    check_sum = 0;
    for (i = GTP_ADDR_LENGTH; i < cfg_num+GTP_ADDR_LENGTH; i++)
    {
        check_sum += config[i];
    }
    config[ cfg_num+GTP_ADDR_LENGTH] = (~check_sum) + 1; 	//checksum
    config[ cfg_num+GTP_ADDR_LENGTH+1] =  1; 						//refresh 配置更新标志

    //写入配置信息
    for (retry = 0; retry < 5; retry++)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, config , cfg_num + GTP_ADDR_LENGTH+2);
        if (ret > 0)
        {
            break;
        }
    }


    return ret;
}

#endif

/**
  * @brief  触屏中断服务函数，emXGUI示例中没有使用中断
  * @param 无
  * @retval 无
  */
void GTP_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GTP_INT_GPIO_PIN) != RESET) //确保是否产生了EXTI Line中断
	{
		//LED2_TOGGLE;
		GTP_TouchProcess();    
		__HAL_GPIO_EXTI_CLEAR_IT(GTP_INT_GPIO_PIN);     //清除中断标志位
	}  
}

/**
  * @brief  触屏检测函数，本函数作为emXGUI的定制检测函数，
   *        参考Goodix_TS_Work_Func修改而来， 只读取单个触摸点坐标
  * @param x[out] y[out] 读取到的坐标
  * @retval 坐标有效返回1，否则返回0
  */
int	GTP_Execu( int *x,int *y)
{
	uint8_t  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
	//2-寄存器地址 1-状态寄存器 8*1-每个触摸点使用8个寄存器 
	uint8_t  point_data[2 + 1 + 8 * 1 + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
	uint8_t  touch_num = 0;
	uint8_t  finger = 0;

	uint8_t client_addr=GTP_ADDRESS;
	int32_t input_x = 0;
	int32_t input_y = 0;

	int32_t ret = -1;

	ret = GTP_I2C_Read(client_addr, point_data, 12);//10字节寄存器加2字节地址
	if (ret < 0)
	{
			return 0;
	}

	finger = point_data[GTP_ADDR_LENGTH];//状态寄存器数据

	if (finger == 0x00)		//没有数据，退出
	{
			return 0;
	}

	if((finger & 0x80) == 0)//判断buffer status位
	{
			goto exit_work_func;//坐标未就绪，数据无效
	}

	touch_num = finger & 0x0f;//坐标点数
	if (touch_num > GTP_MAX_TOUCH)
	{
			goto exit_work_func;//大于最大支持点数，错误退出
	}    

	if (touch_num)
	{
		input_x  = point_data[3+1] | (point_data[3+2] << 8);	//x坐标
		input_y  = point_data[3+3] | (point_data[3+4] << 8);	//y坐标

		if(input_x < GTP_MAX_WIDTH && input_y < GTP_MAX_HEIGHT)  
		{
			*x = input_x;
			*y = input_y;
			Touch_Pres_Down = 1;
		}
		else
		{
				//超出范围，错误退出
			 Touch_Pres_Down = 0;
			 goto exit_work_func;
		}
	}

exit_work_func:
	{
			
		  //清空标志
			ret = GTP_I2C_Write(client_addr, end_cmd, 3);
			if (ret < 0)
			{
					return 0;
			}
	}
	return touch_num;
}

/*
*********************************************************************************************************
*	函 数 名: GT911_OnePiontScan
*	功能说明: 读取GT911触摸数据，这里仅读取一个触摸点。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void GT911_OnePiontScan(void)
{
	static uint8_t s_tp_down = 0;
	uint16_t x, y;
	uint8_t res;
	static uint32_t count = 0;
	uint8_t  point_data[2 + 1 + 8 * 1 + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
	uint8_t  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
	
	res = GTP_I2C_Read(GTP_ADDRESS, point_data, 12);//10字节寄存器加2字节地址
	//GTP_I2C_Write(GTP_ADDRESS, end_cmd, 3);
	if(res != 2)
	{
			return;
	}
	/* 判断是否按下，没有按下，直接退出 */
	if((point_data[GTP_ADDR_LENGTH] & 0x0F) == 0)
	{
		GTP_I2C_Write(GTP_ADDRESS, end_cmd, 3);
		
		if(s_tp_down == 1)
		{
			count++;
			/* State.x和State.y的数值无需更新，State是全局变量，保存的就是最近一次的数值 */
			/* 必须检测到2次以上才可以松手 */
			if(count > 2)
			{   
				s_tp_down = 0;
				count = 0;
				g_GT911_Point.x = 0xFFFF;
		    g_GT911_Point.y = 0XFFFF;
				g_GT911_Point.Pressed = 0;
			}
		}
		return;
	}
	if((point_data[GTP_ADDR_LENGTH] & 0x80) && ((point_data[GTP_ADDR_LENGTH] & 0x0F) < 6))
	{
		GTP_I2C_Write(GTP_ADDRESS, end_cmd, 3);
		
		g_GT911.X0  = point_data[3+1] | (point_data[3+2] << 8);	//x坐标
		g_GT911.Y0  = point_data[3+3] | (point_data[3+4] << 8);	//y坐标

		/* 检测按下 */
		/* 坐标转换 :
			电容触摸板左下角是 (0，0);  右上角是 (479，799)
			需要转到LCD的像素坐标 (左上角是 (0，0), 右下角是 (799，479)
		*/
		x = g_GT911.X0;
		y = g_GT911.Y0;
		
		if (x > 799)
		{
			x = 799;
		}
		
		if (y > 479)
		{
			y = 479;
		}
		
		if(s_tp_down == 0)
		{
			s_tp_down = 1;
			g_GT911_Point.x = x;
			g_GT911_Point.y = y;
			g_GT911_Point.Pressed = 1;
		}
		else
		{
			g_GT911_Point.x = x;
			g_GT911_Point.y = y;
			g_GT911_Point.Pressed = 1;
		}
  }
	/* 实时清零 */
	count = 0;	
}

//MODULE_DESCRIPTION("GTP Series Driver");
//MODULE_LICENSE("GPL");
