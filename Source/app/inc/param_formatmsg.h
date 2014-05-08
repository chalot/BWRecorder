/** @file param_formatmsg.h
 * @brief brief description
 * @author <gao zhengdong, zhengdgao@163.com>
 * @date 2014年3月26日下午2:07:46
 * @version V1.0
 * @note
 * 参数 - 事件存储格式及访问接口
 */
#ifndef EVT_H_
#define EVT_H_

#if 0
enum {
	PARAM_FORMATMSG_EVT = 0,
	PARAM_FORMATMSG_MSGOD_MENU,
	PARAM_FORMATMSG_MSGOD_MSG,
	PARAM_FORMATMSG_PHONEBOOK,

	PARAM_FORMATMSG_END,

};

///各类信息在EEPROM中的存储首地址
#define EEPROM_ADDR_EVENT		0x333	///事件信息在EEPROM中首地址#define EEPROM_ADDR_MSGOD_MENU	0x333	///点播菜单信息在EEPROM中首地址#define EEPROM_ADDR_MSGOD_MSG	0x333	///点播信息在EEPROM中首地址#define EEPROM_ADDR_PHONEBOOK	0x333	///电话本在EEPROM中首地址
#define EEPROM_ADDR_ROUND		0x333	///电话本在EEPROM中首地址#define EEPROM_ADDR_RECTANGLE	0x333	///电话本在EEPROM中首地址#define EEPROM_ADDR_POLYGON		0x333	///电话本在EEPROM中首地址
#define EVT_MSG_SIZE			50 	///事件内容最大长度#define EVT_ITEM_SIZE			50	///事件最大个数#define MSGOD_MENU_MSG_SIZE		50 	///点播菜单内容长度#define MSGOD_MENU_ITEM_SIZE	50 	///点播菜单条数#define MSGOD_MSG_MSG_SIZE		50	///点播消息内容长数#define MSGOD_MSG_ITEM_SIZE		50	///点播消息条数#define PHONEBOOK_MSG_SIZE		50	///电话本单条记录长数#define PHONEBOOK_ITEM_SIZE		50	///电话本记录总条数
///事件项索引
typedef struct
{
	u8 u8Id;	///事件ID,若终端已有同ID的事件，则被覆盖
	u8 u8Len;///事件信息实际长度
	u16 u16Offset;///事件信息偏移地址，相对于索引头
	u8 u8CRC;///事件内容CRC
}tITEMINDEX;

///事件信息存储头
typedef struct
{
	u8 u8Amount;	///事件总数
	tITEMINDEX index[EVT_ITEM_SIZE];///事件索引数组
}tEVT_HEADINFO;

///事件信息存储格式
typedef struct
{
	u32 prefix;	///前缀
	tEVT_HEADINFO headInfo;
	u8 u8CRC;///校验码
}tPARAM_EVT_HEADINFO_STORAGE;

///点播菜单存储头
typedef struct
{
	u8 u8Amount;	///事件总数
	tITEMINDEX index[MSGOD_MENU_ITEM_SIZE];///事件索引数组
}tMSGODMENU_HEADINFO;

///点播菜单存储格式
typedef struct
{
	u32 prefix;	///前缀
	tMSGODMENU_HEADINFO headInfo;
	u8 u8CRC;///校验码
}tPARAM_MSGOD_MENU_HEADINFO_STORAGE;

///点播信息存储头
typedef struct
{
	u8 u8Amount;	///事件总数
	tITEMINDEX index[MSGOD_MSG_ITEM_SIZE];///事件索引数组
}tMSGODMSG_HEADINFO;

///点播信息存储格式
typedef struct
{
	u32 prefix;	///前缀
	tMSGODMSG_HEADINFO headInfo;
	u8 u8CRC;///校验码
}tPARAM_MSGOD_MSG_HEADINFO_STORAGE;

///电话本存储头
typedef struct
{
	u8 u8Amount;	///电话本总数
	tITEMINDEX index[MSGOD_MSG_ITEM_SIZE];///索引数组
}tPHONEBOOK_HEADINFO;

///电话本存储格式
typedef struct
{
	u32 prefix;	///前缀
	tMSGODMSG_HEADINFO headInfo;
	u8 u8CRC;///校验码
}tPARAM_PHONEBOOK_HEADINFO_STORAGE;

/*************** 外部接口 *****************************************************************/
int PARAM_FormatMsg_GetHead(u8 type);
int PARAM_FormatMsg_SaveHead(u8 type);
int PARAM_FormatMsg_GetAmount(u8 type);
int PARAM_FormatMsg_GetItem(u8 type, u8 u8ItemIndex, u8* pu8ItemId, u8 *pu8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_ApendItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_ReplaceItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_EraseAll(u8 type);

#endif

#endif /* EVT_H_ */
