/** @file eeprom.c
  * @brief brief description
  * @author <gao zhengdong, zhengdgao@163.com>
  * @date 2014年3月25日下午2:56:23
  * @version V1.0
  * @note
  * detailed description
  */
#include <fram.h>
#include <error.h>

/**
 * 铁电硬件初始化
 */
void FRAM_Lowlevel_Init()
{

}

/**
 * EEPROM 初始化
 *
 * @return
 */
int EEPROM_Init()
{

	return 0;
}

/**
 * EEPROM 写一个字节
 *
 * @param address
 * @param data
 * @return 0=成功，否则返回错误代码
 */
int EEPROM_WriteByte(u32 address, u8 data)
{

	return 0;
}

/**
 * EEPROM 写缓区
 *
 * @param address
 * @param pBuf
 * @param u16BufLen
 * @return	0=成功，否则返回错误代码
 */
int EEPROM_WriteBuffer(u32 address, u8 *pBuf, u16 u16BufLen)
{

	return 0;
}

/**
 * EEPROM 擦除指定地址数据，清零
 *
 * @param address
 * @param size
 * @return
 */
int EEPROM_EraseBytes(u32 address, u16 size)
{

	return 0;
}

/**
 * EEPROM 读一个字节
 *
 * @param address
 * @param pData
 * @return	0=成功，否则返回错误代码
 */
int EEPROM_ReadByte(u32 address, u8 *pData)
{

	return 0;
}

/**
 * EEPROM 读连续多个字节
 *
 * @param address		EEPROM地址
 * @param pBuf			接收缓区
 * @param u16DataLen	要读取的字节数
 * @return 	0=成功，否则返回错误代码
 */
int EEPROM_ReadBuffer(u32 address, u8 *pBuf, u16 u16DataLen)
{

	return 0;
}

/**
 * 读EEPROM并计算CRC
 *
 * @param address		IN	EEPROM地址
 * @param pBuf			IN	接收缓区
 * @param u16DataLen	IN	要读取的字节数
 * @param pu8CRC		OUT	CRC
 * @return	0=成功，否则返回错误代码
 */
int EEPROM_ReadBufferCRC(u32 address, u16 u16DataLen, u8 *pu8CRC)
{
	return 0;
}
