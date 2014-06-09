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

static u8 I2CSendByte(unsigned char I2CData);
static u8 I2CReceiveByte(void);

#define _nop_()      Delay_1us(2)        /*I2C总线延时可以根据自己的MCU运行时钟频率进行适当的调整*/



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
static BOOL Write_Data_To_Fram(u8 Device_Addr, u16 I2C_Adr, u8 *DataPtr, u16 Data_Size) {
	I2CStart();      // 初始化IIC
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
	I2CStop();      // 停止IIC
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
static BOOL Read_Data_From_Fram(u8 Device_Addr, u16 I2C_Adr, u8 *DataPtr, u16 Data_Size) {
	I2CStart();      // 初始化IIC
	if (I2CSendByte(E_WRITE | ((Device_Addr & 0x07) << 1)))
		return FALSE;      // 发动字节 1010 A2 A1 A0 0

	if (I2CSendByte(I2C_Adr >> 8))
		return FALSE;       // 发送地址高8位

	if (I2CSendByte(I2C_Adr & 0x00FF))
		return FALSE;         // 发送地址低8位

	I2CStart();               // 再次发送开始信号

	if (I2CSendByte(E_READ | ((Device_Addr & 0x07) << 1)))
		return FALSE;        // 发动字节 1010 A2 A1 A0 1

	while (Data_Size--)      // 读取数据
	{
		*DataPtr = I2CReceiveByte();
		DataPtr++;
		if (Data_Size != 0) {
			Ack_To_I2C();
		}
	}

	NotAck_To_I2C();
	I2CStop();
	return TRUE;
}

/**
 * 测试函数
 */
void I2C_test_RC64(void) {

	u8 buf[35], buf1[35] = "shanghaibenhongdianzi";

	InitI2C();

	Write_Data_To_Fram(0x00, 0x0000, buf1, 35);
	Read_Data_From_Fram(0x00, 0x0000, buf, 50);
}

