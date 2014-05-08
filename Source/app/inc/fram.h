/** @file eeprom.h
  * @brief brief description
  * @author <gao zhengdong, zhengdgao@163.com>
  * @date 2014年3月25日下午2:56:07
  * @version V1.0
  * @note
  * detailed description
  */
#ifndef EEPROM_H_
#define EEPROM_H_

#include <stm32f2xx_conf.h>

void FRAM_Lowlevel_Init();
int EEPROM_Init();
int EEPROM_WriteByte(u32 address, u8 data);
int EEPROM_WriteBuffer(u32 address, u8 *pBuf, u16 u16BufLen);
int EEPROM_ReadByte(u32 address, u8 *pData);
int EEPROM_ReadBuffer(u32 address, u8 *pBuf, u16 u16DataLen);
int EEPROM_EraseBytes(u32 address, u16 size);
int EEPROM_ReadBufferCRC(u32 address, u16 u16DataLen, u8 *pu8CRC);


#endif /* EEPROM_H_ */
