/*
 * utility.c
 *
 *  Created on: 2013-5-15
 *      Author: gaozhengdong
 *
 *  [2014-01-20] 修改注释风格，符合doxygen格式。
 */
#include <stm32f2xx_conf.h>
#include "utility.h"
#include "trace.h"

//Hex转ASC字符
#define	Hex2Char(hex)		((hex >= 0 && hex <= 9) ? (hex + 0x30) : ( hex - 10 + 'A' ))

/**
 * 清除缓冲区内容
 * @param pu8Mem，缓区头
 * @param u32Size，缓区长度
 */
void ZeroMem(u8* pu8Mem, u32 u32Size) {
	//while(u32Size)
	for (; u32Size > 0; u32Size--) {
		*pu8Mem++ = 0;
		//u32Size--;
	}
}

/**
 * 存储区设置指定内容
 *
 * @param pBuf，缓冲区地址指针
 * @param u8Data，填充数据
 * @param u16Size，填充字节数
 */
void memset_(u8 *pBuf, u8 u8Data, u16 u16Size) {
	u16 i;
	u8* pu8Buf = (u8*) pBuf;

	for (i = 0; i < u16Size; i++) {
		pu8Buf[i] = u8Data;
	}
}

/**
 * 内存拷贝
 * @param pDstBuf，目的缓冲区地址指针
 * @param pSrcBuf，源缓冲区地址指针
 * @param u16Size，拷贝字节数
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
 * 缓冲区比较
 * @param buf1	目的缓冲区地址指针
 * @param buf2	源缓冲区地址指针
 * @param u16Size	比较的字节数
 * @return	TRUE=相同，FALSE=不同
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
 * 十进制字节转换成BCD码
 *
 * @param u8Data	待转字节，如56转换成0x56
 * @return
 */
u8 MISC_U82BCD(u8 u8Data) {
	return ((u8Data / 10) << 4) | (u8Data % 10);
}

/**
 * BCD码转换成十进制数
 *
 * @param u8Data	需转换的BCD字节，如0x56转换成56
 * @return	十进制数
 */
u8 MISC_BCD2U8(u8 u8Data) {
	return ((u8Data >> 4) * 10) + (u8Data & 0x0F);
}

/**
 * 延迟指定微秒数
 *
 * @param u32us		微妙数
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
 * 延迟指定毫秒数
 *
 * @param nTime		毫秒
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
 * 计算指定日期是星期几
 * @param u16Year	年
 * @param u8Month	月
 * @param u8Day		日
 * @return	星期数，0=星期日，1~6=星期一~星期六
 */
u8 MISC_CalcuDayofWeek(u16 u16Year, u8 u8Month, u8 u8Day) {

	return 0;
}

/**
 * 判断是否闰年
 *
 * @param u16Year
 * @return	TRUE=闰年，FALSE=平年
 */
BOOL MISC_IsLeapYear(u16 u16Year) {
	//是闰年否(能被4整除但不能被100整除，或者能被400整除)?
	return ((!(u16Year % 4) && (u16Year % 100)) || !(u16Year % 400)) ?
			TRUE : FALSE;
}

/**
 * 计算当月有多少天
 *
 * @param u16Year	年
 * @param u8Month	月
 * @return	该年月天数
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
 * 计算字符长度，以'\0'结束
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
 * HEX串转字符串
 * @param pData			Hex数据
 * @param u16DataLen	Hex数据长度
 * @param pStr			字符串首地址
 * @param u16CLen		字符串长度
 * @return	转换后的字符串长度
 */
u16 Hex2String(u8* pData, u16 u16DataLen, char* pStr, u16 u16CLen) {
	u16 i = 0;
	char* p = NULL;

	//参数检查
	if ((NULL == pData) || (0 == u16DataLen) || (NULL == pStr))
		return 0;

	p = pStr;

	//校验缓区长度，应至少不小于Hex数据长度的2倍+1('\0')
	if (u16CLen < ((u16DataLen << 1) + 1))
		return 0;

	//按字节转换字符
	for (; i < u16DataLen; i++) {
		*p++ = Hex2Char((*pData >> 4));
		*p++ = Hex2Char((*pData & 0x0F));

		pData++;
	}

	//填充字符串结束符
	*p = '\0';

	return ((u16) (p - pStr));
}

/**
 * 计算CRC冗余码
 *
 * @param pu8Buffer		[IN] 	要计算的序列
 * @param u16Size		[IN] 	序列长度
 * @return	u8,CRC冗余码
 */
u8 CalculateCRC8(u8* pu8Buffer, u16 u16Size) {
	u16 i, j;
	u8 u8CRC, u8Code;

	//CRC-8初值 = 0
	u8CRC = 0;

	//计算一帧数据CRC-8
	for (i = 0; i < u16Size; i++) {
		//当前流代码
		u8Code = pu8Buffer[i];

		//字节8位二进制位
		for (j = 0; j < 8; j++) {
			//输入二进制流(bit0)与CRC-bit0异或
			if ((u8Code ^ u8CRC) & 0x01) {
				u8CRC = ((u8CRC ^ 0x18) >> 1) | 0x80;
			} else {
				u8CRC >>= 1;
			}

			//输入二进制流移位
			u8Code >>= 1;
		}
	}

	//返回CRC
	return u8CRC;
}

/**
 * 获取系统当前时刻距离某一基准时间点的毫秒数
 *
 * @return u32,	毫秒数
 */
u32 GetCurTicksInMSeconds() {

	return 0;
}

/**
 * 循环生成包序号
 *
 * @return	u16，	包序号
 */
u16 GeneratePacketSequenceNumber() {
	static u16 seq = 0;
	seq++;

	return seq;
}

/**
 * 计算校验和，累加
 *
 * @param pData		[IN]	数据
 * @param datalen	[IN]	数据长度
 * @return	校验和
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
 * 字符串比较
 *
 * @param s1	[IN]	串1
 * @param s2	[IN]	串2
 * @return	TRUE=相同，FALSE=不同
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
 * 字符串拷贝
 *
 * @param s1	[IN]	目的串串
 * @param s2	[IN]	源串
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
 * 数组给定CRC值校验
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
