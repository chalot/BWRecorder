#ifndef __MYIIC_H
#define __MYIIC_H

#include <stm32f2xx_conf.h>
#include <type.h>
#include <board.h>


//#define USE_WP_PIN     /*ѡ���Ƿ�ʹ��д�������ţ����������д��������ע�͵������*/


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


////IO��������
//#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
//#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}
//
////IO��������
//#define IIC_SCL    PBout(10) //SCL//#define IIC_SDA    PBout(11) //SDA//#define READ_SDA   PBin(11)  //����SDA//IIC���в�������
void IIC_Init(void);     //��ʼ��IIC��IO��
void IIC_Start(void);	//����IIC��ʼ�ź�
void IIC_Stop(void);	  //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);	//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 	//IIC�ȴ�ACK�ź�
void IIC_Ack(void);		//IIC����ACK�ź�
void IIC_NAck(void);	//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_Read_One_Byte(u8 daddr, u8 addr);

#endif

