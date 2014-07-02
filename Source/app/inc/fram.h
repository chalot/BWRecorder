/** @file FRAM.h
 * @brief brief description
 * @author <gao zhengdong, zhengdgao@163.com>
 * @date 2014年3月25日下午2:56:07
 * @version V1.0
 * @note
 * detailed description
 */
#ifndef FRAM_H_
#define FRAM_H_

#include <stm32f2xx_conf.h>



void FRAM_Lowlevel_Init();
int FRAM_Init();
int FRAM_SelfDiagnose();
int FRAM_WriteByte(u32 address, u8 data);
int FRAM_WriteBuffer(u32 address, u8 *pBuf, u16 u16BufLen);
int FRAM_ReadByte(u32 address, u8 *pData);
int FRAM_ReadBuffer(u32 address, u8 *pBuf, u16 u16DataLen);
int FRAM_EraseBytes(u32 address, u16 size);
int FRAM_ReadBufferCRC(u32 address, u16 u16DataLen, u8 *pu8CRC);

#endif /* FRAM_H_ */
