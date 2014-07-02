/**   
 * @Title: error.h  
 * @Description: TODO(��һ�仰�������ļ���ʲô) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-5-28 ����10:01:06 
 * @version V1.0   
 */

#ifndef ERROR_H_
#define ERROR_H_

#include "type.h"

//#define DEVICE_DIAGNOSE_STATUS_NANDFLASH		0x00000001
//#define DEVICE_DIAGNOSE_STATUS_FRAM				0x00000001
//#define DEVICE_DIAGNOSE_STATUS_GPRS				0x00000001




/* ������� */
enum ErrCode {
	ERR_COMMAND_NOT_SUPPORT = 0,
	ERR_PARAM_INVALID,
	ERR_CHECKSUM,
	ERR_MSG_INVALID,
	ERR_PARAM_DEVICEID_INVALID,
	ERR_PARAM_PROGRAM_FAILED,
	ERR_UPG_BLOCK_CHECKSUM,
	ERR_UPD_FLASH_WRITE,

	/*SD����ȡ*/
	ERR_SDCARD_NOTEXISTED,
	ERR_SDCARD_INIT,

	ERR_SD_READ,
	ERR_SD_WRITE,
	ERR_SD_FILENOTEXISTED,
	ERR_SD_FILECREATEFAILED,

	/**
	 * EEPROM �������
	 */
	ERR_EEPROM_INITFAILED,	///��ʼ��ʧ��
	ERR_EEPROM_RW, ///��д����
	ERR_EEPROM_CRC,	///CRCУ���


	/**NANDFLASH �������*/
	ERR_NFLASH_RW,	///��д����

	/**3D���ٶȴ�����*/
	ERR_3DSENSOR_INITFAILED,	///3D���ٶȳ�ʼ��ʧ��


	//...

	ERR_UNSUPPORT

};

char* GetErrorDescription(u8 u8ErrCode);

#endif /* ERROR_H_ */
