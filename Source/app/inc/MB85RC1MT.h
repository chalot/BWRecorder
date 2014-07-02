/**
 * 富士通铁电存储器MB85RC1MT驱动
 *
 * WP接地，始终写有效
 *
 */

#ifndef _MB85RC1MT_H_
#define _MB85RC1MT_H_

#include <type.h>
#include <stm32f2xx_conf.h>
#include <board.h>



#define E_WRITE	0xA0	  //定义I2C器件地址SDA和方向位W，写
#define E_READ	0xA1	  //定义I2C器件地址SDA和方向位R，读

#define DEVICE_ADDR0       0X00 //**** 000 *
#define DEVICE_ADDR1       0X01 //**** 001 *
#define DEVICE_ADDR2       0X02 //**** 010 *
#define DEVICE_ADDR3       0X03 //**** 011 *
#define DEVICE_ADDR4       0X04 //**** 100 *
#define DEVICE_ADDR5       0X05 //**** 101 *
#define DEVICE_ADDR6       0X06 //**** 110 *
#define DEVICE_ADDR7       0X07 //**** 111 *


//void I2C_CtrlPin_RCxx_Init(void);
//static void I2CStart(void);
//static void I2CStop(void);
//static unsigned char ACK_From_I2C(void);
//static void Ack_To_I2C(void);
//static void NotAck_To_I2C(void);
//void InitI2C(void);
BOOL Read_Data_From_Fram(u8 Device_Addr, u16 I2C_Adr, u8 *DataPtr, u16 Data_Size);
BOOL Write_Data_To_Fram(u8 Device_Addr, u16 I2C_Adr, u8 *DataPtr, u16 Data_Size);

void I2C_test_RC64(void);

#endif

