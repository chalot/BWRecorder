/**   
 * @Title: error.h  
 * @Description: TODO(用一句话描述该文件做什么) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-5-28 上午10:01:06 
 * @version V1.0   
 */

#ifndef ERROR_H_
#define ERROR_H_

#include "type.h"

//#define DEVICE_DIAGNOSE_STATUS_NANDFLASH		0x00000001
//#define DEVICE_DIAGNOSE_STATUS_FRAM				0x00000001
//#define DEVICE_DIAGNOSE_STATUS_GPRS				0x00000001




/* 错误代码 */
enum ErrCode {
	ERR_COMMAND_NOT_SUPPORT = 0,
	ERR_PARAM_INVALID,
	ERR_CHECKSUM,
	ERR_MSG_INVALID,
	ERR_PARAM_DEVICEID_INVALID,
	ERR_PARAM_PROGRAM_FAILED,
	ERR_UPG_BLOCK_CHECKSUM,
	ERR_UPD_FLASH_WRITE,

	/*SD卡存取*/
	ERR_SDCARD_NOTEXISTED,
	ERR_SDCARD_INIT,

	ERR_SD_READ,
	ERR_SD_WRITE,
	ERR_SD_FILENOTEXISTED,
	ERR_SD_FILECREATEFAILED,

	/**
	 * EEPROM 错误代码
	 */
	ERR_EEPROM_INITFAILED,	///初始化失败
	ERR_EEPROM_RW, ///读写错误
	ERR_EEPROM_CRC,	///CRC校验错


	/**NANDFLASH 错误代码*/
	ERR_NFLASH_RW,	///读写错误

	/**3D加速度传感器*/
	ERR_3DSENSOR_INITFAILED,	///3D加速度初始化失败


	//...

	ERR_UNSUPPORT

};

char* GetErrorDescription(u8 u8ErrCode);

#endif /* ERROR_H_ */
