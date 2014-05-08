/*
 * utility.c
 *
 *  Created on: 2013-5-15
 *      Author: gaozhengdong
 *
 *  [2014-01-20] �޸�ע�ͷ�񣬷���doxygen��ʽ��
 */
#include <stm32f2xx_conf.h>
#include "utility.h"
#include "trace.h"

//HexתASC�ַ�
#define	Hex2Char(hex)		((hex >= 0 && hex <= 9) ? (hex + 0x30) : ( hex - 10 + 'A' ))

/**
 * �������������
 * @param pu8Mem������ͷ
 * @param u32Size����������
 */
void ZeroMem(u8* pu8Mem, u32 u32Size) {
	//while(u32Size)
	for (; u32Size > 0; u32Size--) {
		*pu8Mem++ = 0;
		//u32Size--;
	}
}

/**
 * �洢������ָ������
 *
 * @param pBuf����������ַָ��
 * @param u8Data���������
 * @param u16Size������ֽ���
 */
void memset_(u8 *pBuf, u8 u8Data, u16 u16Size) {
	u16 i;
	u8* pu8Buf = (u8*) pBuf;

	for (i = 0; i < u16Size; i++) {
		pu8Buf[i] = u8Data;
	}
}

/**
 * �ڴ濽��
 * @param pDstBuf��Ŀ�Ļ�������ַָ��
 * @param pSrcBuf��Դ��������ַָ��
 * @param u16Size�������ֽ���
 */
void memcpy_(u8 *pDstBuf, u8 *pSrcBuf, u16 u16Size) {
	u16 i;
	u8* pu8DstBuf = (u8*) pDstBuf;
	u8* pu8SrcBuf = (u8*) pSrcBuf;

	for (i = 0; i < u16Size; i++) {
		pu8DstBuf[i] = pu8SrcBuf[i];
	}
}

/**
 * �������Ƚ�
 * @param buf1	Ŀ�Ļ�������ַָ��
 * @param buf2	Դ��������ַָ��
 * @param u16Size	�Ƚϵ��ֽ���
 * @return	TRUE=��ͬ��FALSE=��ͬ
 */
BOOL memcmp_(u8 *buf1, u8* buf2, u16 u16Size) {
	u16 i;
	u8 *pu8Buf1 = (u8*) buf1;
	u8 *pu8Buf2 = (u8*) buf2;

	for (i = 0; i < u16Size; i++) {
		if (pu8Buf1[i] != pu8Buf2[i]) {
			return FALSE;
		}
	}

	return TRUE;
}

/**
 * ʮ�����ֽ�ת����BCD��
 *
 * @param u8Data	��ת�ֽڣ���56ת����0x56
 * @return
 */
u8 MISC_U82BCD(u8 u8Data) {
	return ((u8Data / 10) << 4) | (u8Data % 10);
}

/**
 * BCD��ת����ʮ������
 *
 * @param u8Data	��ת����BCD�ֽڣ���0x56ת����56
 * @return	ʮ������
 */
u8 MISC_BCD2U8(u8 u8Data) {
	return ((u8Data >> 4) * 10) + (u8Data & 0x0F);
}

/**
 * �ӳ�ָ��΢����
 *
 * @param u32us		΢����
 */
void Delay_1us(u32 u32us) {
	u16 i;
	while (u32us) {
		for (i = 40; i; i--) {
			;
		}

		u32us--;
	}
}

/**
 * �ӳ�ָ��������
 *
 * @param nTime		����
 */
void Delay_1ms(u32 nTime) {
	u32 volatile u32Tmp;
	while (nTime--) {
		for (u32Tmp = 0; u32Tmp < 10000; u32Tmp++) {
			;
		}
	}
}

/**
 * ����ָ�����������ڼ�
 * @param u16Year	��
 * @param u8Month	��
 * @param u8Day		��
 * @return	��������0=�����գ�1~6=����һ~������
 */
u8 MISC_CalcuDayofWeek(u16 u16Year, u8 u8Month, u8 u8Day) {

	return 0;
}

/**
 * �ж��Ƿ�����
 *
 * @param u16Year
 * @return	TRUE=���꣬FALSE=ƽ��
 */
BOOL MISC_IsLeapYear(u16 u16Year) {
	//�������(�ܱ�4���������ܱ�100�����������ܱ�400����)?
	return ((!(u16Year % 4) && (u16Year % 100)) || !(u16Year % 400)) ?
			TRUE : FALSE;
}

/**
 * ���㵱���ж�����
 *
 * @param u16Year	��
 * @param u8Month	��
 * @return	����������
 */
u8 MISC_DaysofMonth(u16 u16Year, u8 u8Month) {
	if (2 == u8Month) {
		if (MISC_IsLeapYear(u16Year)) {
			return 29;
		} else {
			return 28;
		}
	} else if ((1 == u8Month) || (3 == u8Month) || (5 == u8Month)
			|| (7 == u8Month) || (8 == u8Month) || (10 == u8Month)
			|| (12 == u8Month)) {
		return 31;
	} else {
		return 30;
	}
}

/**
 * �����ַ����ȣ���'\0'����
 *
 * @param pData
 * @return
 */
u16 strlen_(char *pData) {
	u16 u16Len = 0;

	if (NULL == pData)
		return 0;

	while (*pData != '\0') {
		u16Len++;
		pData++;
	}

	return u16Len;
}

/**
 * HEX��ת�ַ���
 * @param pData			Hex����
 * @param u16DataLen	Hex���ݳ���
 * @param pStr			�ַ����׵�ַ
 * @param u16CLen		�ַ�������
 * @return	ת������ַ�������
 */
u16 Hex2String(u8* pData, u16 u16DataLen, char* pStr, u16 u16CLen) {
	u16 i = 0;
	char* p = NULL;

	//�������
	if ((NULL == pData) || (0 == u16DataLen) || (NULL == pStr))
		return 0;

	p = pStr;

	//У�黺�����ȣ�Ӧ���ٲ�С��Hex���ݳ��ȵ�2��+1('\0')
	if (u16CLen < ((u16DataLen << 1) + 1))
		return 0;

	//���ֽ�ת���ַ�
	for (; i < u16DataLen; i++) {
		*p++ = Hex2Char((*pData >> 4));
		*p++ = Hex2Char((*pData & 0x0F));

		pData++;
	}

	//����ַ���������
	*p = '\0';

	return ((u16) (p - pStr));
}

/**
 * ����CRC������
 *
 * @param pu8Buffer		[IN] 	Ҫ���������
 * @param u16Size		[IN] 	���г���
 * @return	u8,CRC������
 */
u8 CalculateCRC8(u8* pu8Buffer, u16 u16Size) {
	u16 i, j;
	u8 u8CRC, u8Code;

	//CRC-8��ֵ = 0
	u8CRC = 0;

	//����һ֡����CRC-8
	for (i = 0; i < u16Size; i++) {
		//��ǰ������
		u8Code = pu8Buffer[i];

		//�ֽ�8λ������λ
		for (j = 0; j < 8; j++) {
			//�����������(bit0)��CRC-bit0���
			if ((u8Code ^ u8CRC) & 0x01) {
				u8CRC = ((u8CRC ^ 0x18) >> 1) | 0x80;
			} else {
				u8CRC >>= 1;
			}

			//�������������λ
			u8Code >>= 1;
		}
	}

	//����CRC
	return u8CRC;
}

/**
 * ��ȡϵͳ��ǰʱ�̾���ĳһ��׼ʱ���ĺ�����
 *
 * @return u32,	������
 */
u32 GetCurTicksInMSeconds() {

	return 0;
}

/**
 * ѭ�����ɰ����
 *
 * @return	u16��	�����
 */
u16 GeneratePacketSequenceNumber() {
	static u16 seq = 0;
	seq++;

	return seq;
}

/**
 * ����У��ͣ��ۼ�
 *
 * @param pData		[IN]	����
 * @param datalen	[IN]	���ݳ���
 * @return	У���
 */
u8 CalculateChecksum(u8 *pData, u16 datalen) {
	u8 checksum = 0;
	u16 i = 0;

	if ((pData == NULL ) || (datalen == 0))
		return 0;

	while (i < datalen) {
		checksum += *(pData + i);
		i++;
	}

	return checksum;
}

/**
 * �ַ����Ƚ�
 *
 * @param s1	[IN]	��1
 * @param s2	[IN]	��2
 * @return	TRUE=��ͬ��FALSE=��ͬ
 */
BOOL strcmp_(const char *s1, const char *s2) {
	u16 i = 0;

	if (strlen_((char*) s1) != strlen_((char*) s2))
		return FALSE;

	while (s1[i] != '\0') {
		if (s1[i] != s2[i])
			return FALSE;
		i++;
	}

	return TRUE;
}

/**
 * �ַ�������
 *
 * @param s1	[IN]	Ŀ�Ĵ���
 * @param s2	[IN]	Դ��
 */
void strcpy_(char *s1, char *s2) {
	u16 len = strlen_(s2);
	u16 i = 0;

	while (s2[i] != '\0') {
		s1[i] = s2[i];
		i++;
	}

	s1[i] = '\0';
}

/**
 * �������CRCֵУ��
 *
 * @param u8CRC
 * @param pData
 * @param u16DataLen
 * @return
 */
BOOL CheckCRC(u8 u8CRC, u8 *pData, u16 u16DataLen) {
	u8 _u8CRC;

	_u8CRC = CalculateCRC8(pData, u16DataLen);

	if (_u8CRC == u8CRC)
		return TRUE;

	return FALSE;
}
