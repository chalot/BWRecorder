/**   
 * @Title: error.c  
 * @Description: TODO(ÓÃÒ»¾ä»°ÃèÊö¸ÃÎÄ¼þ×öÊ²Ã´) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-5-28 ÉÏÎç10:01:22 
 * @version V1.0   
 */

#include "error.h"
#include "type.h"

struct ERRDESP {
	u8 u8ErrCode;
	char* description;
};

/**
 * ´íÎóÃèÊö
 */
static const struct ERRDESP ErrString[] = { { ERR_COMMAND_NOT_SUPPORT, "command not supported" }, { ERR_PARAM_INVALID,
		"param invalid" }, { ERR_MSG_INVALID, "Host cmd not invalid" },

{ ERR_PARAM_DEVICEID_INVALID, "device ID is invalid" },

{ ERR_PARAM_PROGRAM_FAILED, "EE2PROM program failed" },

{ ERR_UPG_BLOCK_CHECKSUM, "upgrade file block crc checksum error" }, { ERR_UPD_FLASH_WRITE,
		"write upg block to flash error" },

/*
 {0, "xxx"},
 {0, "xxx"},
 {0, "xxx"},
 {0, "xxx"},
 {0, "xxx"},
 */

/*SD¿¨´æÈ¡*/
{ ERR_SDCARD_NOTEXISTED, "SDcard does not existed." }, { ERR_SDCARD_INIT, "SDcard init failed." }, { ERR_SD_READ,
		"SD card can not read" }, { ERR_SD_WRITE, "SD card can not write to" }, { ERR_SD_FILENOTEXISTED,
		"the file you read does not existed" }, { ERR_SD_FILECREATEFAILED, "the file can not be created" },

{ ERR_UNSUPPORT, "unsurpported error code" },

};

/**
 * ·µ»Ø´íÎóÃèÊö
 *
 * @param u8ErrCode		´íÎó´úÂë
 * @return char*		´íÎóÃèÊö×Ö·û´®
 */
char*
GetErrorDescription(u8 u8ErrCode) {
	if (u8ErrCode > ERR_UNSUPPORT)
		return (void*) 0;

	return ErrString[u8ErrCode].description;
}

