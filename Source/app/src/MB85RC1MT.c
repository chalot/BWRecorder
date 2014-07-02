/*
 *********************************************************************
 ** FileName�� MB85RCxx.c
 ** Function�� ����GPIO��ģ��I2Cͨ�ţ�������ʿͨMB85RCϵ������洢��
 **            ֧�ֵ��ͺţ�MB85RC64/64V,MB85RC128,MB85RC256V
 ** Version��  V1.0
 ** Date��     2012-2-21i
 ** Author:    Qizhongying,
 ** E-mail:    qizhongying@hobos.com.cn
 ** Copyright: �Ϻ�������ӿƼ�.HOBOS.
 ** CPU:        MB9BF506R
 ** ע�⣺�˳�������ο�֮�ã����ܱ�֤���в�����BUG
 *********************************************************************
 ** ע�⣺���Ӳ����ʹ��д�������ţ�����ͷ�ļ��к궨�塰USE_WP_PIN����
 **       ������ע�͵�����䣡��
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
#define E_WRITE		0xA0	//����I2C������ַSDA�ͷ���λW
#define E_READ		0xA1	//����I2C������ַSDA�ͷ���λR

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


#define _nop_()      Delay_1us(2)        /*I2C������ʱ���Ը����Լ���MCU����ʱ��Ƶ�ʽ����ʵ��ĵ���*/


/*
*-------------------------------------------------------------------------------
* �������ܣ� IO��ʼ��
* �������ݣ� ��
* �� �� ֵ�� ��
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
* �������ܣ� I2C���߳�ʼ��
* �������ݣ� ��
* �� �� ֵ�� ��
*-------------------------------------------------------------------------------
*/
void InitI2C(void)
{
   I2C_SCL_DAT(0);
	I2CStop();
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� ����������ʼ�ź�
* �������ݣ� ��
* �� �� ֵ�� ��
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
* �������ܣ� �������߽����ź�
* �������ݣ� ��
* �� �� ֵ�� ��
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
* �������ܣ� Ӧ�𣬷�������ʱ���ź�,������ʱ�ӵ�ƽΪ���ڼ�SDA�ϵ�
             ״̬,��������I2C���߷�������.
* �������ݣ� ��
* �� �� ֵ�� ACKCondition������SCLΪ�ߵ�ƽ�ڼ�SDA��״̬
*-------------------------------------------------------------------------------
*/
unsigned char ACK_From_I2C(void)
{
   unsigned char ACKCondition;

   _nop_();
   I2C_SCL_DAT_1;
   I2C_SDA_DIR_IN();                   /*������������Ϊ���룬׼���������϶�ȡӦ���ź�*/
	_nop_();
	I2C_SCL_DAT_1;
	_nop_();
	if(I2C_SDA_READ()){
		ACKCondition = 1;                /*δ����Ӧ���ź�*/
	}
	else{
		ACKCondition = 0;                /*���յ�Ӧ���ź�*/
	}
	I2C_SCL_DAT_0;
   I2C_SDA_DIR_OUT();                  /*����SDAΪ���*/

   return(ACKCondition);
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� ��I2C���߷���Ӧ���źţ��������ؽ���
* �������ݣ� ��
* �� �� ֵ�� ��
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

/*���ͷ�Ӧ���ź�*/
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
 * ��I2C���߷���һ���ֽڵ����ݣ�����ȡ�����豸��Ӧ���ź�
 *
 * @param I2CData	��Ҫ���͵�I2C���ߵ�����
 * @return	�����ϵ��豸��Ӧ���źţ�������ͳɹ�����0����֮����1
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
		I2C_SCL_DAT(1); /*����ʱ���ź��ߣ�������������,SCL��0��Ϊ1ʱ����������*/
		_nop_();
		I2C_SCL_DAT(0);
		_nop_();
	}
	return (IIC_Wait_Ack());
}

/**
 * ��I2C���߽���1���ֽ�,�����ظ��ֽ�,������Ӧ���ź�
 *
 * @return	���߶�ȡ������
 */
static u8 I2CReceiveByte(void) {
	u8 I2CData = 0;
	u8 idx;

	I2C_SDA_DIR_IN();
	/*������������Ϊ��������*/
	_nop_();
	for (idx = 0; idx < 8; idx++) {
		I2C_SCL_DAT(0);
		_nop_();
		I2C_SCL_DAT(1); /*����ʱ���źţ���������״̬*/
		if (I2C_SDA_READ()) {
			I2CData |= (0x80 >> idx);
		}
		_nop_();
		I2C_SCL_DAT(0);
		_nop_();
	}
	I2C_SCL_DAT(0);
	I2C_SDA_DIR_OUT();
	/*������������Ϊ�������*/
	return (I2CData);
}

/**
 * RC1M��I2C�����ϵ������������ȡ����
 *
 * @param Device_Addr		I2C�������豸�ĵ�ַ��RC64������8����
 * @param I2C_Adr			�豸�Ĵ洢�����ַ
 * @param DataPtr			��Ҫ���͸��豸������ ��Ҫ���豸�϶�ȡ������
 * @param Data_Size			��Ҫ���ͺͽ������ݵĳ���
 * @return TRUE=�ɹ���FALSE=ʧ��
 */
unsigned char Write_Data_To_I2C(unsigned char Device_Addr, unsigned int I2C_Adr,
		unsigned char *DataPtr,unsigned int Data_Size) {
	IIC_Start();      // ��ʼ��IIC
	if (I2CSendByte(E_WRITE | ((Device_Addr & 0x07) << 1)))
		return FALSE;     // �����ֽ� 1010 A2 A1 A0 0

	if (I2CSendByte(I2C_Adr >> 8))
		return FALSE;                     // ���͵�ַ��8λ

	if (I2CSendByte(I2C_Adr & 0x00FF))
		return FALSE;                     // ���͵�ַ��8λ

	while (Data_Size--)     // ѭ��д������
	{
		if (I2CSendByte(*DataPtr))
			return FALSE;
		DataPtr++;
	}
	IIC_Stop();      // ֹͣIIC
	return TRUE;
}

/**
 * RC1M��I2C�����ϵ������������ȡ����
 *
 * @param Device_Addr		I2C�������豸�ĵ�ַ��RC64������8����
 * @param I2C_Adr			�豸�Ĵ洢�����ַ
 * @param DataPtr			��Ҫ���豸�϶�ȡ������
 * @param Data_Size			�������ݵĳ���
 * @return	TRUE=�ɹ���FALSE=ʧ��
 */
unsigned char Read_Data_From_I2C(unsigned char Device_Addr,unsigned int I2C_Adr,
		unsigned char *DataPtr,unsigned int Data_Size) {
	IIC_Start();      // ��ʼ��IIC
	if (I2CSendByte(E_WRITE | ((Device_Addr & 0x07) << 1)))
		return FALSE;      // �����ֽ� 1010 A2 A1 A0 0

	if (I2CSendByte(I2C_Adr >> 8))
		return FALSE;       // ���͵�ַ��8λ

	if (I2CSendByte(I2C_Adr & 0x00FF))
		return FALSE;         // ���͵�ַ��8λ

	IIC_Start();               // �ٴη��Ϳ�ʼ�ź�

	if (I2CSendByte(E_READ | ((Device_Addr & 0x07) << 1)))
		return FALSE;        // �����ֽ� 1010 A2 A1 A0 1

	while (Data_Size--)      // ��ȡ����
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
 * ���Ժ���
 */
void I2C_test_RC64(void) {
	I2C_CtrlPin_Init();
	u8 buf[35], buf1[35] = "shanghaibenhongdianzi";


	Write_Data_To_Fram(0x00, 0x0000, buf1, 35);
	Read_Data_From_Fram(0x00, 0x0000, buf, 50);
}

