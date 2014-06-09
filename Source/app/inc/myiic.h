#ifndef __MYIIC_H
#define __MYIIC_H

#include <stm32f2xx_conf.h>
#include <type.h>
#include <board.h>


//#define USE_WP_PIN     /*选择是否使用写保护引脚，如果不适用写保护，请注释掉此语句*/


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
//#define IIC_SCL    PBout(10) //SCL//#define IIC_SDA    PBout(11) //SDA//#define READ_SDA   PBin(11)  //输入SDA//IIC所有操作函数
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

