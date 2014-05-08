/** @file eeprom.c
  * @brief brief description
  * @author <gao zhengdong, zhengdgao@163.com>
  * @date 2014��3��25������2:56:23
  * @version V1.0
  * @note
  * detailed description
  */
#include <fram.h>
#include <error.h>

/**
 * ����Ӳ����ʼ��
 */
void FRAM_Lowlevel_Init()
{

}

/**
 * EEPROM ��ʼ��
 *
 * @return
 */
int EEPROM_Init()
{

	return 0;
}

/**
 * EEPROM дһ���ֽ�
 *
 * @param address
 * @param data
 * @return 0=�ɹ������򷵻ش������
 */
int EEPROM_WriteByte(u32 address, u8 data)
{

	return 0;
}

/**
 * EEPROM д����
 *
 * @param address
 * @param pBuf
 * @param u16BufLen
 * @return	0=�ɹ������򷵻ش������
 */
int EEPROM_WriteBuffer(u32 address, u8 *pBuf, u16 u16BufLen)
{

	return 0;
}

/**
 * EEPROM ����ָ����ַ���ݣ�����
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
 * EEPROM ��һ���ֽ�
 *
 * @param address
 * @param pData
 * @return	0=�ɹ������򷵻ش������
 */
int EEPROM_ReadByte(u32 address, u8 *pData)
{

	return 0;
}

/**
 * EEPROM ����������ֽ�
 *
 * @param address		EEPROM��ַ
 * @param pBuf			���ջ���
 * @param u16DataLen	Ҫ��ȡ���ֽ���
 * @return 	0=�ɹ������򷵻ش������
 */
int EEPROM_ReadBuffer(u32 address, u8 *pBuf, u16 u16DataLen)
{

	return 0;
}

/**
 * ��EEPROM������CRC
 *
 * @param address		IN	EEPROM��ַ
 * @param pBuf			IN	���ջ���
 * @param u16DataLen	IN	Ҫ��ȡ���ֽ���
 * @param pu8CRC		OUT	CRC
 * @return	0=�ɹ������򷵻ش������
 */
int EEPROM_ReadBufferCRC(u32 address, u16 u16DataLen, u8 *pu8CRC)
{
	return 0;
}
