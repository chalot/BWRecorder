/** @file evt.c
 * @brief brief description
 * @author <gao zhengdong, zhengdgao@163.com>
 * @date 2014��3��26������2:07:55
 * @version V1.0
 * @note
 * ����/����������ͳһ�����ӿڡ��������¼����㲥�˵����㲥��Ϣ���绰��
 * ����˼�룺�ڴ�ֻ�洢������Ϣ���������ݴ洢��EEPROM�������С�
 * Ŀ�ģ������ڴ����ģ���ߴ��븴����
 * ���ݴ洢��ʽ������������������¼
 *
 * ע�⣬���ڵ绰�����������Ƕ�����������ϵ������CRCֵΪ�����ֶ�ID���������ǹ̶����Ȼ������洢�ı䳤
 * ���ݣ���Э���ʽһ�£�
 */

#include <parameter.h>
#include <error.h>
#include <utility.h>

///�¼�
static tPARAM_EVT_HEADINFO_STORAGE tParam_Evt_St;
tEVT_HEADINFO *ptParam_EvtInfo = &tParam_Evt_St.headInfo;

///�㲥�˵�
static tPARAM_MSGOD_MENU_HEADINFO_STORAGE tParam_MsgOD_Menu_St;
tMSGODMENU_HEADINFO *ptParam_MsgOD_Menu = &tParam_MsgOD_Menu_St.headInfo;

///�㲥��Ϣ
static tPARAM_MSGOD_MSG_HEADINFO_STORAGE tParam_MsgOD_Msg_St;
tMSGODMSG_HEADINFO *ptParam_MsgOD_Msg = &tParam_MsgOD_Msg_St.headInfo;

///�绰��
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

///����EEPROM�е�ƫ��λ�ã������ͷ
#define ITEM_EEPROM_OFFSET(i, type) (i * tIndexProp[type].u8ItemSize)

///��ʵ����Ŀ��
#define	ITEM_AMOUNT(type) (*(tIndexProp[type].pStorageAddr + 1))

///����ͷCRC�ֶ������ͷƫ��λ��
#define CRC_STORAGE_OFFSET(type) (1 + tIndexProp[type].u8ItemAmount * sizeof(tITEMINDEX))

/**
 * ���洢����ͷ��Ϣ
 *
 * @param type		IN		��������
 * @return	0=�ɹ�������������
 */
int PARAM_FormatMsg_GetHead(u8 type) {
	int ret;

	if (type >= PARAM_FORMATMSG_END)
		return -ERR_PARAM_INVALID;

	ZeroMem(tIndexProp[type].pStorageAddr, tIndexProp[type].u16StorageSize);

	///��ȡ����ͷ
	ret = EEPROM_ReadBuffer(tIndexProp[type].u32EEPROMAddress,
			tIndexProp[type].pStorageAddr, tIndexProp[type].u16StorageSize);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	///����ͷУ��
	if (CheckCRC(*(tIndexProp[type].pStorageAddr + CRC_STORAGE_OFFSET(type)),///����ͷCRCλ��
			tIndexProp[type].pStorageAddr, tIndexProp[type].u16StorageSize - 1))
		return 0;

	return -ERR_EEPROM_CRC;
}

/**
 * ��������ͷ
 *
 * @return	0=�ɹ���-1=ʧ��
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
 * ������
 *
 * @param type
 * @return
 */
int PARAM_FormatMsg_GetAmount(u8 type) {
	u8 u8Amount = ITEM_AMOUNT(type);
	return u8Amount;
}

/**
 * ��ָ������¼���Ϣ
 *
 * @param u8Index		IN		����
 * @param pu8EvtLen		OUT		�¼�ID
 * @param pu8Len		OUT		�¼���Ϣ����
 * @param pEvtMsg		OUT		�¼���Ϣ
 * @return	0=�ɹ�������������
 */
int PARAM_FormatMsg_GetItem(u8 type, u8 u8ItemIndex, u8* pu8ItemId,
		u8 *pu8ItemLen, u8 *pItemMsg) {
	tITEMINDEX *pItemIndex;
	int ret;

	if ((type >= PARAM_FORMATMSG_END) || (u8ItemIndex >= ITEM_AMOUNT(type))
			|| (pItemMsg == NULL ))
		return -ERR_PARAM_INVALID;

	pItemIndex = (tITEMINDEX *) (tIndexProp[type].pStorageAddr + 2); ///��������ʼλ��

	*pu8ItemId = pItemIndex->u8Id;
	*pu8ItemLen = pItemIndex->u8Len;

	ret = EEPROM_ReadBuffer(
			tIndexProp[type].u32EEPROMAddress
					+ ITEM_EEPROM_OFFSET(u8ItemIndex, type), pItemMsg,
			pItemIndex->u8Len);

	if (ret != 0)
		return -1;	///EEPROM����

	if (!CheckCRC(pItemIndex->u8CRC, pItemMsg, pItemIndex->u8Len))
		return -1;	///У���

	return 0;
}

/**
 * ׷���ע�����������дEEPROM���ݣ�������ͷδ���棬Ӧ��׷����������Ϻ����ִ��һ�α�������ͷ
 *
 * @param u8EvtId		IN	�¼�ID
 * @param u8EvtLen		IN	�¼�����
 * @param pEvtMsg		IN	�¼���Ϣ
 * @return	0=�ɹ�������������
 */
int PARAM_FormatMsg_ApendItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg) {
	tITEMINDEX *pItemIndex;
	u32 u32Address;
	int ret;
	u8 crc_eeprom;
	u8 u8Amount;

	if (type >= PARAM_FORMATMSG_END)
		return -ERR_PARAM_INVALID;

	if (ITEM_AMOUNT(type) >= tIndexProp[type].u8ItemAmount)	///�¼����������������
		return -ERR_PARAM_INVALID;

	pItemIndex = (tITEMINDEX *) (tIndexProp[type].pStorageAddr + 2); ///��������ʼλ��

	if (type == PARAM_FORMATMSG_PHONEBOOK) {
		///�绰������ϵ��CRC��ΪID
		u8 u8NameLen = *(pItemMsg + *(pItemMsg + 1) + 2);	///���볤��
		u8 *pName = pItemMsg + *(pItemMsg + 1) + 2;
		pItemIndex->u8Id = CalculateCRC8(pName, u8NameLen);
	} else {
		pItemIndex->u8Id = u8ItemId;
	}

	pItemIndex->u8Len = u8ItemLen;
	pItemIndex->u8CRC = CalculateCRC8(pItemMsg, u8ItemLen);

	u8Amount = *(tIndexProp[type].pStorageAddr + 1); ///������

	u32Address = tIndexProp[type].u32EEPROMAddress
			+ ITEM_EEPROM_OFFSET(u8Amount, type);

	ret = EEPROM_WriteBuffer(u32Address, pItemMsg, u8ItemLen);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	///�ٶ���У��CRCһ�Σ�ȷ������������ȷ
	ret = EEPROM_ReadBufferCRC(u32Address, u8ItemLen, &crc_eeprom);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	if (pItemIndex->u8CRC != crc_eeprom)	///����У�����
		return -ERR_EEPROM_CRC;

	///�ۼ��¼���Ŀ����
	*(tIndexProp[type].pStorageAddr + 1) = u8Amount + 1;

	return 0;
}

/**
 * �滻�¼���ID��ͬ��������滻��Ҳ�ᵼ������ͷ����������Ϣ�����仯�����½�������Ҫ��������ͷһ��
 *
 * @param u8EvtId		IN	�¼�ID
 * @param u8EvtLen		IN	�¼���Ϣ����
 * @param pEvtMsg		IN	�¼���Ϣ����
 * @return	0=�ɹ���-1=ʧ��
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

	u8ItemAmount = ITEM_AMOUNT(type);	///������

	pItemIndex = (tITEMINDEX *) (tIndexProp[type].pStorageAddr + 2); ///��������ʼλ��

	i = 0;
	while (i < u8ItemAmount) {
		if (pItemIndex->u8Id == u8ItemId)
			break;
		i++;
	}

	if (i >= u8ItemAmount)	///����ڣ��˳�
		return 0;

	u32ItemAddress = tIndexProp[type].u32EEPROMAddress
			+ ITEM_EEPROM_OFFSET(u8ItemAmount, type);

	ret = EEPROM_WriteBuffer(u32ItemAddress, pItemMsg, u8ItemLen);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	///������������Ϣ
	pItemIndex->u8Len = u8ItemLen;
	pItemIndex->u8CRC = CalculateCRC8(pItemMsg, u8ItemLen);

	///�ٶ���У��CRCһ�Σ�ȷ������������ȷ
	ret = EEPROM_ReadBufferCRC(u32ItemAddress, u8ItemLen, &crc_eeprom);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	if (pItemIndex->u8CRC != crc_eeprom)	///����У�����
		return -ERR_EEPROM_CRC;

	return 0;
}

/**
 * �����Ϣ���������������ݣ�ͬʱ����ڴ��EEPROM�е���Ϣ
 *
 * @return	0=�ɹ�������������
 */
int PARAM_FormatMsg_EraseAll(u8 type) {
	int ret;

	if (type >= PARAM_FORMATMSG_END)
		return -ERR_PARAM_INVALID;

	///����EEPROM
	ret = EEPROM_EraseBytes(tIndexProp[type].u32EEPROMAddress,
			tIndexProp[type].u16StorageSize);

	if (ret == -1)
		return -ERR_EEPROM_RW;

	///����ڴ�����
	ZeroMem(tIndexProp[type].pStorageAddr, tIndexProp[type].u16StorageSize);

	return 0;
}
