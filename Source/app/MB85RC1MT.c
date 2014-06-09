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

static u8 I2CSendByte(unsigned char I2CData);
static u8 I2CReceiveByte(void);

#define _nop_()      Delay_1us(2)        /*I2C������ʱ���Ը����Լ���MCU����ʱ��Ƶ�ʽ����ʵ��ĵ���*/



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
static BOOL Write_Data_To_Fram(u8 Device_Addr, u16 I2C_Adr, u8 *DataPtr, u16 Data_Size) {
	I2CStart();      // ��ʼ��IIC
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
	I2CStop();      // ֹͣIIC
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
static BOOL Read_Data_From_Fram(u8 Device_Addr, u16 I2C_Adr, u8 *DataPtr, u16 Data_Size) {
	I2CStart();      // ��ʼ��IIC
	if (I2CSendByte(E_WRITE | ((Device_Addr & 0x07) << 1)))
		return FALSE;      // �����ֽ� 1010 A2 A1 A0 0

	if (I2CSendByte(I2C_Adr >> 8))
		return FALSE;       // ���͵�ַ��8λ

	if (I2CSendByte(I2C_Adr & 0x00FF))
		return FALSE;         // ���͵�ַ��8λ

	I2CStart();               // �ٴη��Ϳ�ʼ�ź�

	if (I2CSendByte(E_READ | ((Device_Addr & 0x07) << 1)))
		return FALSE;        // �����ֽ� 1010 A2 A1 A0 1

	while (Data_Size--)      // ��ȡ����
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
 * ���Ժ���
 */
void I2C_test_RC64(void) {

	u8 buf[35], buf1[35] = "shanghaibenhongdianzi";

	InitI2C();

	Write_Data_To_Fram(0x00, 0x0000, buf1, 35);
	Read_Data_From_Fram(0x00, 0x0000, buf, 50);
}

