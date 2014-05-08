/*
 * utility.h
 *
 *  Created on: 2013-5-15
 *      Author: gaozhengdong
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <type.h>


void	ZeroMem(u8* pu8Mem, u32 u32Size);
void	memset_(u8 *pu8Buf, u8 u8Data, u16 u16Size);
void	memcpy_(u8 *pu8DestBuf, u8 *pu8SrcBuf, u16 u16Size);
BOOL	memcmp_(u8 *buf1, u8 *buf2, u16 u16Size);

u16 	strlen_(char *pData);
BOOL 	strcmp_(const char *s1, const char *s2);
void 	strcpy_(char *s1, char *s2);

u8		MISC_U82BCD(u8 u8Data);
u8		MISC_BCD2U8(u8 u8Data);
void	Delay_1us(u32 u32us);
void	Delay_1ms(u32 u32ms);
u8		MISC_DaysofMonth(u16 u16Year, u8 u8Month);
BOOL	MISC_IsLeapYear(u16 u16Year);
u8		MISC_CalcuDayofWeek(u16 u16Year, u8 u8Month, u8 u8Day);
u32 	GetCurTicksInMSeconds();
u8		CalculateCRC8(u8* pu8Buffer, u16 u16Size);
u16		Hex2String(u8* pData, u16 u16DataLen, char* pStr, u16 u16CLen);

#define	abs(x,y)		((x > y)? (x - y):(y-x))
#define	difference(x,y) (((x) > (y)) ? (x):(y))
#define	max(a,b)	((a > b )? (a):(b))
#define	min(a,b)	((a < b )? (a):(b))


#define ENDIAN_U16(data)  \
				do {	\
					u8 temp;	\
					temp = (u8)(((data) & 0xFF00) >> 8); 	\
					data = ((data) << 8) | temp; 	\
				} while(0)


#define ENDIAN_U32(data)  \
				do {	\
					u8 h_1; 	\
					u8 h_2;		\
					u8 h_3;		\
					u8 h_4;		\
					h_1 = (data & 0xFF000000) >> 24;	\
					h_2 = (data & 0x00FF0000) >> 16;	\
					h_3 = (data & 0x0000FF00) >> 8;	\
					h_4 = data & 0x000000FF;		\
					data = (u32)h_4 << 24 | (u32)h_3 << 16 | (u32)h_2 << 8 | h_1;	\
				} while(0)


#define WRAP_AROUND(p, size)	do {			\
								if(p >= size)	\
									p = 0;		\
								}while(0)

#define WRAP_REVERSE(p, size)	do {					\
								if(p == 0)				\
									p = size - 1;		\
								else					\
									p --;				\
								}while(0)

u16 GeneratePacketSequenceNumber();

u8 CalculateChecksum(u8 *pData, u16 datalen);

BOOL CheckCRC(u8 u8CRC, u8 *pData, u16 u16DataLen);


#endif /* UTILITY_H_ */
