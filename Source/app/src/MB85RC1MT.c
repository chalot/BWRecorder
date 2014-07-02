/*
 *********************************************************************
 ** FileName： MB85RCxx.c
 ** Function： 利用GPIO口模拟I2C通信，驱动富士通MB85RC系列铁电存储器
 **            支持的型号：MB85RC64/64V,MB85RC128,MB85RC256V
 ** Version：  V1.0
 ** Date：     2012-2-21i
 ** Author:    Qizhongying,
 ** E-mail:    qizhongying@hobos.com.cn
 ** Copyright: 上海本宏电子科技.HOBOS.
 ** CPU:        MB9BF506R
 ** 注意：此程序仅供参考之用，不能保证其中不存在BUG
 *********************************************************************
 ** 注意：如果硬件中使用写保护引脚，请在头文件中宏定义“USE_WP_PIN”；
 **       否则，请注释掉该语句！！
 */

#include "MB85RC1MT.h"
#include <utility.h>
#include <myiic.h>


#define sEE_I2C                          I2C1
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C1
#define sEE_I2C_SCL_PIN                  GPIO_Pin_6                  /* PB.06 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource6
#define sEE_I2C_SCL_AF                   GPIO_AF_I2C1
#define sEE_I2C_SDA_PIN                  GPIO_Pin_9                  /* PB.09 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource9
#define sEE_I2C_SDA_AF                   GPIO_AF_I2C1


/* The define of MB85RC64V */
#define E_WRITE		0xA0	//定义I2C器件地址SDA和方向位W
#define E_READ		0xA1	//定义I2C器件地址SDA和方向位R

#define DEVICE_ADDR0       0X00 //**** 000 *
#define DEVICE_ADDR1       0X01 //**** 001 *
#define DEVICE_ADDR2       0X02 //**** 010 *

#define I2C_SDA_DIR_IN()    do { 	\
								GPIO_InitTypeDef GPIO_InitStructure;		\
								GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;	\
								GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		\
								GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	\
								GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	\
								GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 	\
							} while(0);

#define I2C_SDA_DIR_OUT()    do { 	\
								GPIO_InitTypeDef GPIO_InitStructure;		\
								GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;	\
								GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		\
								GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		\
								GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	\
								GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		\
								GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 	\
							} while(0);

#define I2C_SCL_DIR_OUT()    do { 	\
								GPIO_InitTypeDef GPIO_InitStructure;		\
								GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;	\
								GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		\
								GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		\
								GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	\
								GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		\
								GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure); 	\
							} while(0);

#define I2C_SDA_DAT_1       GPIO_SetBits(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_PIN)
#define I2C_SDA_DAT_0       GPIO_ReSetBits(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_PIN)

#define I2C_SCL_DAT_1       GPIO_SetBits(sEE_I2C_SCL_GPIO_PORT, sEE_I2C_SCL_PIN)
#define I2C_SCL_DAT_0       GPIO_ReSetBits(sEE_I2C_SCL_GPIO_PORT, sEE_I2C_SCL_PIN)

#define I2C_SDA_READ()      GPIO_ReadInputDataBit(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_PIN)


static void I2C_CtrlPin_Init(void);
static void I2CStart(void);
static void I2CStop(void);
static unsigned char I2CSendByte(unsigned char I2CData);
static unsigned char I2CReceiveByte(void);
static unsigned char ACK_From_I2C(void);
static void Ack_To_I2C(void);
static void NotAck_To_I2C(void);
static void InitI2C(void);
static unsigned char  Write_Data_To_I2C(unsigned char Device_Addr, unsigned int I2C_Adr,
		unsigned char *DataPtr,unsigned int Data_Size);
static unsigned char  Read_Data_From_I2C(unsigned char Device_Addr, unsigned int I2C_Adr,
		unsigned char *DataPtr,unsigned int Data_Size);


#define _nop_()      Delay_1us(2)        /*I2C总线延时可以根据自己的MCU运行时钟频率进行适当的调整*/


/*
*-------------------------------------------------------------------------------
* 函数功能： IO初始化
* 输入数据： 无
* 返 回 值： 无
*-------------------------------------------------------------------------------
*/
void I2C_CtrlPin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    I2C_SDA_DIR_OUT();
    I2C_SCL_DIR_OUT();

#ifdef USE_WP_PIN
    IO_ConfigPort(I2C_WP_PORT,I2C_WP_PIN ,IO_GPIO_MODE);     /*config port */
    IO_GPIOWriteData(I2C_WP_PORT,I2C_WP_PIN,0x0001);        /* Write data   */
    IO_GPIOConfigPullup(I2C_WP_PORT,I2C_WP_PIN,IO_PULLUP_DISCONN); /*pull-up setting */

    I2C_WP_DIR_OUT();
#endif
}

/*
*-------------------------------------------------------------------------------
* 函数功能： I2C总线初始化
* 输入数据： 无
* 返 回 值： 无
*-------------------------------------------------------------------------------
*/
void InitI2C(void)
{
   I2C_SCL_DAT(0);
	I2CStop();
}

/*
*-------------------------------------------------------------------------------
* 函数功能： 发送总线起始信号
* 输入数据： 无
* 返 回 值： 无
*-------------------------------------------------------------------------------
*/
void I2CStart(void)
{
	I2C_SDA_DIR_OUT();

	I2C_SDA_DAT_1;
	I2C_SCL_DAT_1;
	_nop_();

	I2C_SDA_DAT_0;
	_nop_();

	I2C_SCL_DAT_0;
	_nop_();
}

/*
*-------------------------------------------------------------------------------
* 函数功能： 发送总线结束信号
* 输入数据： 无
* 返 回 值： 无
*-------------------------------------------------------------------------------
*/
void I2CStop(void)
{
	_nop_();
   I2C_SDA_DAT_0;
	_nop_();
	I2C_SCL_DAT_1;
	_nop_();
	I2C_SDA_DAT_1;
	_nop_();
}

/*
*-------------------------------------------------------------------------------
* 函数功能： 应答，发送总线时钟信号,并返回时钟电平为高期间SDA上的
             状态,可用于向I2C总线发送数据.
* 输入数据： 无
* 返 回 值： ACKCondition，返回SCL为高电平期间SDA的状态
*-------------------------------------------------------------------------------
*/
unsigned char ACK_From_I2C(void)
{
   unsigned char ACKCondition;

   _nop_();
   I2C_SCL_DAT_1;
   I2C_SDA_DIR_IN();                   /*数据引脚配置为输入，准备从总线上读取应答信号*/
	_nop_();
	I2C_SCL_DAT_1;
	_nop_();
	if(I2C_SDA_READ()){
		ACKCondition = 1;                /*未读到应答信号*/
	}
	else{
		ACKCondition = 0;                /*接收到应答信号*/
	}
	I2C_SCL_DAT_0;
   I2C_SDA_DIR_OUT();                  /*配置SDA为输出*/

   return(ACKCondition);
}

/*
*-------------------------------------------------------------------------------
* 函数功能： 向I2C总线发送应答信号，用于主控接收
* 输入数据： 无
* 返 回 值： 无
*-------------------------------------------------------------------------------
*/
void Ack_To_I2C(void)
{
	_nop_();
   I2C_SDA_DAT (0);
   _nop_();
	I2C_SCL_DAT_1;
	_nop_();
	I2C_SCL_DAT_0;
   _nop_();
	I2C_SDA_DAT_1;
   _nop_();

}

/*发送非应答信号*/
void NotAck_To_I2C(void)
{
	_nop_();
   I2C_SDA_DAT_1;
   _nop_();
	I2C_SCL_DAT_1;
	_nop_();
	I2C_SCL_DAT_0;
   _nop_();
}


/**
 * 向I2C总线发送一个字节的数据，并读取总线设备的应答信号
 *
 * @param I2CData	将要发送到I2C总线的数据
 * @return	总线上的设备的应答信号，如果发送成功返回0，反之返回1
 */
static u8 I2CSendByte(u8 I2CData) {
	u8 idx;
	_nop_();
	for (idx = 0; idx < 8; idx++) {
		if (I2CData & 0x80) {
			I2C_SDA_DAT(1);
		}
		else {
			I2C_SDA_DAT(0);
		}
		I2CData <<= 1;
		I2C_SCL_DAT(1); /*拉高时钟信号线，锁存总线数据,SCL由0变为1时，锁存数据*/
		_nop_();
		I2C_SCL_DAT(0);
		_nop_();
	}
	return (IIC_Wait_Ack());
}

/**
 * 从I2C总线接收1个字节,并返回该字节,不回送应答信号
 *
 * @return	总线读取的数据
 */
static u8 I2CReceiveByte(void) {
	u8 I2CData = 0;
	u8 idx;

	I2C_SDA_DIR_IN();
	/*配置数据引脚为输入引脚*/
	_nop_();
	for (idx = 0; idx < 8; idx++) {
		I2C_SCL_DAT(0);
		_nop_();
		I2C_SCL_DAT(1); /*拉高时钟信号，锁存总线状态*/
		if (I2C_SDA_READ()) {
			I2CData |= (0x80 >> idx);
		}
		_nop_();
		I2C_SCL_DAT(0);
		_nop_();
	}
	I2C_SCL_DAT(0);
	I2C_SDA_DIR_OUT();
	/*配置数据引脚为输出引脚*/
	return (I2CData);
}

/**
 * RC1M向I2C总线上的器件发送与读取数据
 *
 * @param Device_Addr		I2C总线上设备的地址（RC64最多挂载8个）
 * @param I2C_Adr			设备的存储区域地址
 * @param DataPtr			将要发送给设备的数据 将要从设备上读取的数据
 * @param Data_Size			将要发送和接收数据的长度
 * @return TRUE=成功，FALSE=失败
 */
unsigned char Write_Data_To_I2C(unsigned char Device_Addr, unsigned int I2C_Adr,
		unsigned char *DataPtr,unsigned int Data_Size) {
	IIC_Start();      // 初始化IIC
	if (I2CSendByte(E_WRITE | ((Device_Addr & 0x07) << 1)))
		return FALSE;     // 发动字节 1010 A2 A1 A0 0

	if (I2CSendByte(I2C_Adr >> 8))
		return FALSE;                     // 发送地址高8位

	if (I2CSendByte(I2C_Adr & 0x00FF))
		return FALSE;                     // 发送地址低8位

	while (Data_Size--)     // 循环写入数据
	{
		if (I2CSendByte(*DataPtr))
			return FALSE;
		DataPtr++;
	}
	IIC_Stop();      // 停止IIC
	return TRUE;
}

/**
 * RC1M向I2C总线上的器件发送与读取数据
 *
 * @param Device_Addr		I2C总线上设备的地址（RC64最多挂载8个）
 * @param I2C_Adr			设备的存储区域地址
 * @param DataPtr			将要从设备上读取的数据
 * @param Data_Size			接收数据的长度
 * @return	TRUE=成功，FALSE=失败
 */
unsigned char Read_Data_From_I2C(unsigned char Device_Addr,unsigned int I2C_Adr,
		unsigned char *DataPtr,unsigned int Data_Size) {
	IIC_Start();      // 初始化IIC
	if (I2CSendByte(E_WRITE | ((Device_Addr & 0x07) << 1)))
		return FALSE;      // 发动字节 1010 A2 A1 A0 0

	if (I2CSendByte(I2C_Adr >> 8))
		return FALSE;       // 发送地址高8位

	if (I2CSendByte(I2C_Adr & 0x00FF))
		return FALSE;         // 发送地址低8位

	IIC_Start();               // 再次发送开始信号

	if (I2CSendByte(E_READ | ((Device_Addr & 0x07) << 1)))
		return FALSE;        // 发动字节 1010 A2 A1 A0 1

	while (Data_Size--)      // 读取数据
	{
		*DataPtr = I2CReceiveByte();
		DataPtr++;
		if (Data_Size != 0) {
			IIC_Ack();
		}
	}

	IIC_NAck();
	IIC_Stop();
	return TRUE;
}

/**
 * 测试函数
 */
void I2C_test_RC64(void) {
	I2C_CtrlPin_Init();
	u8 buf[35], buf1[35] = "shanghaibenhongdianzi";


	Write_Data_To_Fram(0x00, 0x0000, buf1, 35);
	Read_Data_From_Fram(0x00, 0x0000, buf, 50);
}

