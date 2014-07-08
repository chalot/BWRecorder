/**   
 * @Title: error.c  
 * @Description: TODO(��һ�仰�������ļ���ʲô) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-5-28 ����10:01:22 
 * @version V1.0   
 */

#include "error.h"
#include "type.h"

struct ERRDESP {
	u8 u8ErrCode;
	char* description;
};

/**
 * ��������
 */
static const struct ERRDESP ErrString[] = {
		{ ERR_COMMAND_NOT_SUPPORT, "command not supported" },
		{ ERR_PARAM_INVALID,	"param invalid" },
		{ ERR_CHECKSUM,	"checksum err" },
		{ ERR_MSG_INVALID, "Host cmd not invalid" },
		{ ERR_PARAM_DEVICEID_INVALID, "device ID is invalid" },
		{ ERR_PARAM_PROGRAM_FAILED, "EE2PROM program failed" },
		{ ERR_UPG_BLOCK_CHECKSUM, "upgrade file block crc checksum error" },
		{ ERR_UPD_FLASH_WRITE,	"write upg block to flash error" },
/*SD����ȡ*/
		{ ERR_SDCARD_NOTEXISTED, "SDcard does not existed." },
		{ ERR_SDCARD_INIT, "SDcard init failed." },
		{ ERR_SD_READ,	"SD card can not read" },
		{ ERR_SD_WRITE, "SD card can not write to" },
		{ ERR_SD_FILENOTEXISTED, "the file you read does not existed" },
		{ ERR_SD_FILECREATEFAILED, "the file can not be created" },
/*��¼��*/
		{ ERR_VDR_FILE_NOTEXIST, "no log file matched seek condition" },
		{ ERR_VDR_FILE_SEEK, "file seek failed" },



/*EEPROM �������*/
		{ ERR_EEPROM_INITFAILED, "EEPROM Init failed" },	///��ʼ��ʧ��
		{ ERR_EEPROM_RW, "EEPROM R/W error" }, ///��д����
		{ ERR_EEPROM_CRC, "EEPROM CRC check error" }, ///CRCУ���


		/**NANDFLASH �������*/
		{ ERR_NFLASH_RW, "NandFlash R/W error"	}, ///��д����

		/*3D���ٶȴ�����*/
		{ ERR_NFLASH_RW, "3D Sensor init failed"	}, ///3D���ٶȳ�ʼ��ʧ��



		{ ERR_UNSUPPORT, "unsurpported error code" },

};

/**
 * ���ش�������
 *
 * @param u8ErrCode		�������
 * @return char*		���������ַ���
 */
char*
GetErrorDescription(u8 u8ErrCode) {
	if (u8ErrCode > ERR_UNSUPPORT)
		return (void*) 0;

	return ErrString[u8ErrCode].description;
}

