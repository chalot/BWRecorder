#ifndef __MYIIC_H
#define __MYIIC_H

#include <stm32f2xx_conf.h>
#include <type.h>
#include <board.h>

#if 0
//#define USE_WP_PIN     /*选择是否使用写保护引脚，如果不适用写保护，请注释掉此语句*/
#define I2C_SDA_PORT           GPIO_PORT4
#define I2C_SCL_PORT           GPIO_PORT4
#define I2C_SDA_PIN            GPIO_PIN_8
#define I2C_SCL_PIN            GPIO_PIN_A

#ifdef USE_WP_PIN
   #define I2C_WP_PORT         GPIO_PORT4
   #define I2C_WP_PIN          GPIO_PIN_9
#endif

#define I2C_SDA_DIR_IN()       IO_GPIOSetInput(I2C_SDA_PORT,I2C_SDA_PIN)
#define I2C_SDA_DIR_OUT()      IO_GPIOSetOutput(I2C_SDA_PORT,I2C_SDA_PIN)
//#define I2C_SCL_DIR_IN()       IO_GPIOSetInput(I2C_SCL_PORT,I2C_SCL_PIN)
#define I2C_SCL_DIR_OUT()      IO_GPIOSetOutput(I2C_SCL_PORT,I2C_SCL_PIN)

#define I2C_SDA_DAT(X)        do{IO_GPIOWriteData(I2C_SDA_PORT,I2C_SDA_PIN,X<<8);}while(0)
#define I2C_SCL_DAT(X)        do{IO_GPIOWriteData(I2C_SCL_PORT,I2C_SCL_PIN,X<<10);}while(0)

#define I2C_SDA_READ()         IO_GPIOReadData(I2C_SDA_PORT, I2C_SDA_PIN)

/*Define  the wp pin*/
#ifdef USE_WP_PIN
 //  #define I2C_WP_DIR_IN()     IO_GPIOSetInput(I2C_WP_PORT,I2C_WP_PIN)
   #define I2C_WP_DIR_OUT()    IO_GPIOSetOutput(I2C_WP_PORT,I2C_WP_PIN)
   #define I2C_WP_DAT(X)       do{IO_GPIOWriteData(I2C_WP_PORT,I2C_WP_PIN,X<<9);}while(0)
#endif

#endif

//============================End of Contrl pins define===================================================================================================

/* The define of MB85RC64V */
#define E_WRITE		0xA0	//定义I2C器件地址SDA和方向位W
#define E_READ		0xA1	//定义I2C器件地址SDA和方向位R

#define DEVICE_ADDR0       0X00 //**** 000 *
#define DEVICE_ADDR1       0X01 //**** 001 *
#define DEVICE_ADDR2       0X02 //**** 010 *




#define I2C_SDA_DIR_IN()    do { 	\
								GPIO_InitTypeDef GPIO_InitStructure;		\
								GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;	\
								GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		\
								GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	\
								GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	\
								GPIO_Init(I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 	\
							} while(0);

#define I2C_SDA_DIR_OUT()    do { 	\
								GPIO_InitTypeDef GPIO_InitStructure;		\
								GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;	\
								GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		\
								GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		\
								GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	\
								GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		\
								GPIO_Init(I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 	\
							} while(0);

#define I2C_SCL_DIR_OUT()    do { 	\
								GPIO_InitTypeDef GPIO_InitStructure;		\
								GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;	\
								GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		\
								GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		\
								GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	\
								GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		\
								GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure); 	\
							} while(0);

#define I2C_SDA_DAT(X)       do{ 	\
								if(X == 1) 		\
									GPIO_SetBits(I2C_SDA_GPIO_PORT, I2C_SDA_PIN);	\
								else												\
									GPIO_ResetBits(I2C_SDA_GPIO_PORT, I2C_SDA_PIN);	\
							}while(0);

#define I2C_SCL_DAT(X)       do{			\
								if(X == 1) 		\
									GPIO_SetBits(I2C_SCL_GPIO_PORT, I2C_SCL_PIN);	\
								else												\
									GPIO_ResetBits(I2C_SCL_GPIO_PORT, I2C_SCL_PIN);	\
							}while(0);


#define I2C_SDA_READ()        GPIO_ReadInputDataBit(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)


////IO方向设置
//#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
//#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}
//
////IO操作函数
//#define IIC_SCL    PBout(10) //SCL//#define IIC_SDA    PBout(11) //SDA//#define READ_SDA   PBin(11)  //输入SDA
#if 1//IIC所有操作函数
void IIC_Init(void);     //初始化IIC的IO口
void IIC_Start(void);	//发送IIC开始信号
void IIC_Stop(void);	  //发送IIC停止信号
void IIC_Send_Byte(u8 txd);	//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
u8 IIC_Wait_Ack(void); 	//IIC等待ACK信号
void IIC_Ack(void);		//IIC发送ACK信号
void IIC_NAck(void);	//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_Read_One_Byte(u8 daddr, u8 addr);
#endif


void I2C_CtrlPin_Init(void);
void I2CStart(void);
void I2CStop(void);
unsigned char I2CSendByte(unsigned char I2CData);
unsigned char I2CReceiveByte(void);
unsigned char ACK_From_I2C(void);
void Ack_To_I2C(void);
void NotAck_To_I2C(void);
void InitI2C(void);
unsigned char  Write_Data_To_I2C(unsigned char Device_Addr, unsigned int I2C_Adr, unsigned char *DataPtr,unsigned int Data_Size);
unsigned char  Read_Data_From_I2C(unsigned char Device_Addr, unsigned int I2C_Adr, unsigned char *DataPtr,unsigned int Data_Size);

#endif

