/** @file evt.c
 * @brief brief description
 * @author <gao zhengdong, zhengdgao@163.com>
 * @date 2014年3月26日下午2:07:55
 * @version V1.0
 * @note
 * 定长/不定长参数统一操作接口。包括：事件、点播菜单、点播消息、电话本
 * 基本思想：内存只存储索引信息，数据内容存储在EEPROM或铁电中。
 * 目的：降低内存消耗，提高代码复用性
 * 数据存储方式：定长索引，定长记录
 *
 * 注意，对于电话本，索引仍是定长，但以联系人名称CRC值为索引字段ID；项分配的是固定长度缓区，存储的变长
 * 内容，与协议格式一致；
 */

#include <parameter.h>
#include <error.h>
#include <utility.h>

///事件
static tPARAM_EVT_HEADINFO_STORAGE tParam_Evt_St;
tEVT_HEADINFO *ptParam_EvtInfo = &tParam_Evt_St.headInfo;

///点播菜单
static tPARAM_MSGOD_MENU_HEADINFO_STORAGE tParam_MsgOD_Menu_St;
tMSGODMENU_HEADINFO *ptParam_MsgOD_Menu = &tParam_MsgOD_Menu_St.headInfo;

///点播消息
static tPARAM_MSGOD_MSG_HEADINFO_STORAGE tParam_MsgOD_Msg_St;
tMSGODMSG_HEADINFO *ptParam_MsgOD_Msg = &tParam_MsgOD_Msg_St.headInfo;

///电话本
static tPARAM_PHONEBOOK_HEADINFO_STORAGE tParam_PhoneBook_St;
tPHONEBOOK_HEADINFO *ptParam_PhoneBook_St = &tParam_PhoneBook_St.headInfo;

typedef struct {
	u8 u8Type;
	u16 u16StorageSize;
	u32 u32EEPROMAddress;
	u8 u8ItemSize;
	u8 u8ItemAmount;
	u8 *pStorageAddr;
} tINDEXPROPERTY;

static const tINDEXPROPERTY tIndexProp[] = { { PARAM_FORMATMSG_EVT,
		sizeof(tPARAM_EVT_HEADINFO_STORAGE), EEPROM_ADDR_EVENT,
		EVT_MSG_SIZE, EVT_ITEM_SIZE, &tParam_Evt_St }, {
		PARAM_FORMATMSG_MSGOD_MENU, sizeof(tPARAM_MSGOD_MENU_HEADINFO_STORAGE),
		EEPROM_ADDR_MSGOD_MENU, MSGOD_MENU_MSG_SIZE, MSGOD_MENU_ITEM_SIZE,
		&tParam_MsgOD_Menu_St }, { PARAM_FORMATMSG_MSGOD_MSG,
		sizeof(tPARAM_MSGOD_MSG_HEADINFO_STORAGE), EEPROM_ADDR_MSGOD_MSG,
		MSGOD_MSG_MSG_SIZE, MSGOD_MSG_ITEM_SIZE, &tParam_MsgOD_Msg_St }, {
		PARAM_FORMATMSG_PHONEBOOK, sizeof(tPARAM_PHONEBOOK_HEADINFO_STORAGE),
		EEPROM_ADDR_PHONEBOOK, PHONEBOOK_MSG_SIZE, PHONEBOOK_ITEM_SIZE,
		&tParam_PhoneBook_St }, };

///项在EEPROM中的偏移位置，相对于头
#define ITEM_EEPROM_OFFSET(i, type) (i * tIndexProp[type].u8ItemSize)

///项实际条目数
#define	ITEM_AMOUNT(type) (*(tIndexProp[type].pStorageAddr + 1))

///索引头CRC字段相对于头偏移位置
#define CRC_STORAGE_OFFSET(type) (1 + tIndexProp[type].u8ItemAmount * sizeof(tITEMINDEX))

/**
 * 读存储索引头信息
 *
 * @param type		IN		参数类型
 * @return	0=成功，否则错误代码
 */
int PARAM_FormatMsg_GetHead(u8 type) {
	int ret;

	if (type >= PARAM_FORMATMSG_END)
		return -ERR_PARAM_INVALID;

	ZeroMem(tIndexProp[type].pStorageAddr, tIndexProp[type].u16StorageSize);

	///读取索引头
	ret = EEPROM_ReadBuffer(tIndexProp[type].u32EEPROMAddress,
			tIndexProp[type].pStorageAddr, tIndexProp[type].u16StorageSize);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	///索引头校验
	if (CheckCRC(*(tIndexProp[type].pStorageAddr + CRC_STORAGE_OFFSET(type)),///索引头CRC位置
			tIndexProp[type].pStorageAddr, tIndexProp[type].u16StorageSize - 1))
		return 0;

	return -ERR_EEPROM_CRC;
}

/**
 * 保存索引头
 *
 * @return	0=成功，-1=失败
 */
int PARAM_FormatMsg_SaveHead(u8 type) {
	int ret;
	u8 crc_eeprom;
	u8 crc_mem;

	if (type >= PARAM_FORMATMSG_END)
		return -ERR_PARAM_INVALID;

	ret = EEPROM_WriteBuffer(tIndexProp[type].u32EEPROMAddress,
			tIndexProp[type].pStorageAddr, tIndexProp[type].u16StorageSize);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	ret = EEPROM_ReadBufferCRC(tIndexProp[type].u32EEPROMAddress,
			tIndexProp[type].u16StorageSize, &crc_eeprom);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	crc_mem = *(tIndexProp[type].pStorageAddr + CRC_STORAGE_OFFSET(type));

	if (crc_eeprom != crc_mem)
		return -ERR_EEPROM_CRC;

	return 0;
}

/**
 * 读项数
 *
 * @param type
 * @return
 */
int PARAM_FormatMsg_GetAmount(u8 type) {
	u8 u8Amount = ITEM_AMOUNT(type);
	return u8Amount;
}

/**
 * 读指定项的事件信息
 *
 * @param u8Index		IN		索引
 * @param pu8EvtLen		OUT		事件ID
 * @param pu8Len		OUT		事件信息长度
 * @param pEvtMsg		OUT		事件信息
 * @return	0=成功，否则错误代码
 */
int PARAM_FormatMsg_GetItem(u8 type, u8 u8ItemIndex, u8* pu8ItemId,
		u8 *pu8ItemLen, u8 *pItemMsg) {
	tITEMINDEX *pItemIndex;
	int ret;

	if ((type >= PARAM_FORMATMSG_END) || (u8ItemIndex >= ITEM_AMOUNT(type))
			|| (pItemMsg == NULL ))
		return -ERR_PARAM_INVALID;

	pItemIndex = (tITEMINDEX *) (tIndexProp[type].pStorageAddr + 2); ///项数组起始位置

	*pu8ItemId = pItemIndex->u8Id;
	*pu8ItemLen = pItemIndex->u8Len;

	ret = EEPROM_ReadBuffer(
			tIndexProp[type].u32EEPROMAddress
					+ ITEM_EEPROM_OFFSET(u8ItemIndex, type), pItemMsg,
			pItemIndex->u8Len);

	if (ret != 0)
		return -1;	///EEPROM出错

	if (!CheckCRC(pItemIndex->u8CRC, pItemMsg, pItemIndex->u8Len))
		return -1;	///校验错

	return 0;
}

/**
 * 追加项，注意更新索引及写EEPROM数据，但索引头未保存，应在追加所有项完毕后，最后执行一次保存索引头
 *
 * @param u8EvtId		IN	事件ID
 * @param u8EvtLen		IN	事件长度
 * @param pEvtMsg		IN	事件信息
 * @return	0=成功，否则错误代码
 */
int PARAM_FormatMsg_ApendItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg) {
	tITEMINDEX *pItemIndex;
	u32 u32Address;
	int ret;
	u8 crc_eeprom;
	u8 u8Amount;

	if (type >= PARAM_FORMATMSG_END)
		return -ERR_PARAM_INVALID;

	if (ITEM_AMOUNT(type) >= tIndexProp[type].u8ItemAmount)	///事件缓区已满，不添加
		return -ERR_PARAM_INVALID;

	pItemIndex = (tITEMINDEX *) (tIndexProp[type].pStorageAddr + 2); ///项数组起始位置

	if (type == PARAM_FORMATMSG_PHONEBOOK) {
		///电话本的联系人CRC作为ID
		u8 u8NameLen = *(pItemMsg + *(pItemMsg + 1) + 2);	///号码长度
		u8 *pName = pItemMsg + *(pItemMsg + 1) + 2;
		pItemIndex->u8Id = CalculateCRC8(pName, u8NameLen);
	} else {
		pItemIndex->u8Id = u8ItemId;
	}

	pItemIndex->u8Len = u8ItemLen;
	pItemIndex->u8CRC = CalculateCRC8(pItemMsg, u8ItemLen);

	u8Amount = *(tIndexProp[type].pStorageAddr + 1); ///项总数

	u32Address = tIndexProp[type].u32EEPROMAddress
			+ ITEM_EEPROM_OFFSET(u8Amount, type);

	ret = EEPROM_WriteBuffer(u32Address, pItemMsg, u8ItemLen);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	///再读并校验CRC一次，确认数据内容正确
	ret = EEPROM_ReadBufferCRC(u32Address, u8ItemLen, &crc_eeprom);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	if (pItemIndex->u8CRC != crc_eeprom)	///内容校验错误
		return -ERR_EEPROM_CRC;

	///累加事件条目总数
	*(tIndexProp[type].pStorageAddr + 1) = u8Amount + 1;

	return 0;
}

/**
 * 替换事件，ID相同的项，内容替换，也会导致索引头中项索引信息发生变化，更新结束后需要保存索引头一次
 *
 * @param u8EvtId		IN	事件ID
 * @param u8EvtLen		IN	事件信息长度
 * @param pEvtMsg		IN	事件信息内容
 * @return	0=成功，-1=失败
 */
int PARAM_FormatMsg_ReplaceItem(u8 type, u8 u8ItemId, u8 u8ItemLen,
		u8 *pItemMsg) {
	tITEMINDEX *pItemIndex;
	int ret;
	u8 crc_eeprom;
	u8 u8ItemAmount;
	u8 i = 0;
	u32 u32ItemAddress;

	if (type >= PARAM_FORMATMSG_END)
		return -ERR_PARAM_INVALID;

	u8ItemAmount = ITEM_AMOUNT(type);	///项总数

	pItemIndex = (tITEMINDEX *) (tIndexProp[type].pStorageAddr + 2); ///项数组起始位置

	i = 0;
	while (i < u8ItemAmount) {
		if (pItemIndex->u8Id == u8ItemId)
			break;
		i++;
	}

	if (i >= u8ItemAmount)	///项不存在，退出
		return 0;

	u32ItemAddress = tIndexProp[type].u32EEPROMAddress
			+ ITEM_EEPROM_OFFSET(u8ItemAmount, type);

	ret = EEPROM_WriteBuffer(u32ItemAddress, pItemMsg, u8ItemLen);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	///更新项索引信息
	pItemIndex->u8Len = u8ItemLen;
	pItemIndex->u8CRC = CalculateCRC8(pItemMsg, u8ItemLen);

	///再读并校验CRC一次，确认数据内容正确
	ret = EEPROM_ReadBufferCRC(u32ItemAddress, u8ItemLen, &crc_eeprom);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	if (pItemIndex->u8CRC != crc_eeprom)	///内容校验错误
		return -ERR_EEPROM_CRC;

	return 0;
}

/**
 * 清空信息，包括索引和数据，同时清空内存和EEPROM中的信息
 *
 * @return	0=成功，否则错误代码
 */
int PARAM_FormatMsg_EraseAll(u8 type) {
	int ret;

	if (type >= PARAM_FORMATMSG_END)
		return -ERR_PARAM_INVALID;

	///擦除EEPROM
	ret = EEPROM_EraseBytes(tIndexProp[type].u32EEPROMAddress,
			tIndexProp[type].u16StorageSize);

	if (ret == -1)
		return -ERR_EEPROM_RW;

	///清空内存数据
	ZeroMem(tIndexProp[type].pStorageAddr, tIndexProp[type].u16StorageSize);

	return 0;
}
