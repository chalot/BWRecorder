/** @file parameter_area.c
 * @brief brief description
 * @author <gao zhengdong, zhengdgao@163.com>
 * @date 2014年4月1日下午1:54:54
 * @version V1.0
 * @note
 * 圆形区域、矩形区域、多边形区域，统一操作接口
 * 数据存储在FRAM内
 */

#include <parameter.h>
#include <param_area.h>
#include <utility.h>
#include <error.h>

static tPARAM_RECTAREA_STORAGE tParam_RectArea_St;
tPARAM_RECTAREA *ptParam_Rect = &tParam_RectArea_St.tRectArea;

static tPARAM_ROUNDAREA_STORAGE tParam_RoundArea_St;
tPARAM_ROUNDAREA *ptParam_RoundArea = &tParam_RoundArea_St.tRoundArea;

static tPARAM_POLYGONAREA_STORAGE tParam_PolygonArea_St;
tPARAM_POLYGONAREA *ptParam_Polygon = &tParam_PolygonArea_St.tPolygonArea;

static tPARAM_ROUTE_STORAGE tParam_Route_St;
tPARAM_ROUTE *ptParam_Route = &tParam_Route_St.tRoute;

typedef struct {
	u32 u32EEPROMAddr;
	u8 *pStorageAddr;
	u16 u16StorageSize;
	u16 u16ItemSize;
	u8 u8ItemAmount;
} PARAM_AREA_CONFIG;

static PARAM_AREA_CONFIG tConf[] = {
		{ EEPROM_ADDR_RECTANGLE, &tParam_RectArea_St, sizeof(tPARAM_RECTAREA_STORAGE),
									sizeof(tItem_RectArea_Ext), RECT_AREA_ITEM_SIZE },
		{ EEPROM_ADDR_ROUND, &tParam_RoundArea_St, sizeof(tPARAM_RECTAREA_STORAGE),
									sizeof(tItem_RoundArea_Ext), ROUND_AREA_ITEM_SIZE },
		{ EEPROM_ADDR_POLYGON, &tParam_PolygonArea_St, sizeof(tPARAM_RECTAREA_STORAGE),
									sizeof(tItem_Polygon_Ex), POLYGON_AREA_ITEM_SIZE },
		{ EEPROM_ADDR_POLYGON, &tParam_Route_St, sizeof(tPARAM_ROUTE_STORAGE),
				sizeof(tITEM_CORNERPOINT_Ex), PARAM_ROUTE_ITEM_MAX },
};

///取项中第一个字节，有效标志
#define IS_VALID(type, i)	(*(tConf[type].pStorageAddr + 1 + 1 + i * tConf[type].u16ItemSize))
///取项总数
#define ITEM_AMOUNT(type) (*(tConf[type].pStorageAddr + 1))
///取ID
#define ITEM_ID(type) (*(u32*)(tConf[type].pStorageAddr + 1 + 1 + i * tConf[type].u16ItemSize + 1))
///项存储位置
#define ITEM_OFFSET(type, i)	(tConf[type].pStorageAddr + 1 + 1 + i * tConf[type].u16ItemSize)

/**
 * 读存储头
 *
 * @param type
 * @return
 */
int PARAM_Area_Load(u8 type) {
	u8 iRet, u8CRC;

	if (type >= PARAM_AREA_END)
		return -ERR_PARAM_INVALID;

	ZeroMem((u8*) tConf[type].pStorageAddr, tConf[type].u16StorageSize);

	iRet = EEPROM_ReadBuffer(tConf[type].u32EEPROMAddr, (u8*) tConf[type].pStorageAddr, tConf[type].u16StorageSize);

	if (iRet == -1)
		return -ERR_EEPROM_RW;

	u8CRC = *(tConf[type].pStorageAddr + tConf[type].u16StorageSize - 1);

	///内容校验
	iRet = CheckCRC(u8CRC, (u8*) tConf[type].pStorageAddr, tConf[type].u16StorageSize - 1);

	if (0 == iRet)
		return ERR_EEPROM_CRC;

	return 0;
}

/**
 * 清空
 *
 * @param type
 * @return
 */
int PARAM_Area_EraseAll(u8 type) {
	if (type >= PARAM_AREA_END)
		return -ERR_PARAM_INVALID;

	ZeroMem((u8*) tConf[type].pStorageAddr, tConf[type].u16StorageSize);

	return 0;
}

/**
 * 保存参数
 *
 * @param type
 * @return
 */
int PARAM_Area_Save(u8 type) {
	int ret;
	u8 crc_eeprom;
	u8 crc_mem;

	if (type >= PARAM_AREA_END)
		return -ERR_PARAM_INVALID;

	ret = EEPROM_WriteBuffer(tConf[type].u32EEPROMAddr, tConf[type].pStorageAddr, tConf[type].u16StorageSize);

	if (ret == -1)
		return -ERR_EEPROM_RW;

	ret = EEPROM_ReadBufferCRC(tConf[type].u32EEPROMAddr, tConf[type].u16StorageSize, &crc_eeprom);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	crc_mem = *(tConf[type].pStorageAddr + tConf[type].u16StorageSize - 1);

	if (crc_eeprom != crc_mem)
		return -ERR_EEPROM_CRC;

	return 0;
}

/**
 * 追加项
 *
 * @param type
 * @param u8Id
 * @param pItem
 * @param u16ItemLen
 * @return
 */
int PARAM_Area_AppendItem(u8 type, u32 u32Id, u8 *pItem, u16 u16ItemLen) {
	u8 u8Amount, i;
	u8 *pItemAddr;

	if (ITEM_AMOUNT(type) >= tConf[type].u8ItemAmount)	///事件缓区已满，不添加
		return -ERR_PARAM_INVALID;

	u8Amount = ITEM_AMOUNT(type);

	i = 0;
	while (i < u8Amount) {
		if (IS_VALID(type, i)) {
			break;
		}
		i++;
	}

	if (i >= u8Amount) ///没找到空闲项内容
		return 0;

	pItemAddr = ITEM_OFFSET(type, i);		///拷贝项
	memcpy_(pItemAddr, pItem, u16ItemLen);

	IS_VALID(type, i) = TRUE;	///设置有效标志

	*(tConf[type].pStorageAddr + 1) = u8Amount + 1;	///累加事件条目总数

	return 0;
}

/**
 * 修改项
 *
 * @param type
 * @param u8Id
 * @param pItem
 * @param u16ItemLen
 * @return
 */
int PARAM_Area_ReplaceItem(u8 type, u32 u32Id, u8 *pItem, u16 u16ItemLen) {
	u8 u8Amount, i;
	u8 pItemAddr;

	if (ITEM_AMOUNT(type) >= tConf[type].u8ItemAmount)	///事件缓区已满，不添加
		return -ERR_PARAM_INVALID;

	u8Amount = ITEM_AMOUNT(type);

	i = 0;
	while (i < u8Amount) {
		if (u32Id == ITEM_ID(type))
			break;
		i++;
	}

	if (i >= u8Amount) ///没找到空闲项内容
		return 0;

	pItemAddr = ITEM_OFFSET(type, i);		///拷贝项
	memcpy_(pItemAddr, pItem, u16ItemLen);

	return 0;

}

/**
 * 删除项
 *
 * @param type
 * @param u32Id
 * @return
 */
int PARAM_Area_DeleteItem(u8 type, u32 u32Id) {
	u8 u8Amount, i;

	if (ITEM_AMOUNT(type) >= tConf[type].u8ItemAmount)	///事件缓区已满，不添加
		return -ERR_PARAM_INVALID;

	u8Amount = ITEM_AMOUNT(type);

	i = 0;
	while (i < u8Amount) {
		if (u32Id == ITEM_ID(type))
			break;
		i++;
	}

	if (i >= u8Amount) ///没找到空闲项内容
		return 0;

	IS_VALID(type, i) = FALSE;	///设置有效标志
	*(tConf[type].pStorageAddr + 1) = u8Amount - 1;	///累加事件条目总数

	return 0;
}
