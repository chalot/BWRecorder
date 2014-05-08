/** @file param_formatmsg.h
 * @brief brief description
 * @author <gao zhengdong, zhengdgao@163.com>
 * @date 2014��3��26������2:07:46
 * @version V1.0
 * @note
 * ���� - �¼��洢��ʽ�����ʽӿ�
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

///������Ϣ��EEPROM�еĴ洢�׵�ַ
#define EEPROM_ADDR_EVENT		0x333	///�¼���Ϣ��EEPROM���׵�ַ#define EEPROM_ADDR_MSGOD_MENU	0x333	///�㲥�˵���Ϣ��EEPROM���׵�ַ#define EEPROM_ADDR_MSGOD_MSG	0x333	///�㲥��Ϣ��EEPROM���׵�ַ#define EEPROM_ADDR_PHONEBOOK	0x333	///�绰����EEPROM���׵�ַ
#define EEPROM_ADDR_ROUND		0x333	///�绰����EEPROM���׵�ַ#define EEPROM_ADDR_RECTANGLE	0x333	///�绰����EEPROM���׵�ַ#define EEPROM_ADDR_POLYGON		0x333	///�绰����EEPROM���׵�ַ
#define EVT_MSG_SIZE			50 	///�¼�������󳤶�#define EVT_ITEM_SIZE			50	///�¼�������#define MSGOD_MENU_MSG_SIZE		50 	///�㲥�˵����ݳ���#define MSGOD_MENU_ITEM_SIZE	50 	///�㲥�˵�����#define MSGOD_MSG_MSG_SIZE		50	///�㲥��Ϣ���ݳ���#define MSGOD_MSG_ITEM_SIZE		50	///�㲥��Ϣ����#define PHONEBOOK_MSG_SIZE		50	///�绰��������¼����#define PHONEBOOK_ITEM_SIZE		50	///�绰����¼������
///�¼�������
typedef struct
{
	u8 u8Id;	///�¼�ID,���ն�����ͬID���¼����򱻸���
	u8 u8Len;///�¼���Ϣʵ�ʳ���
	u16 u16Offset;///�¼���Ϣƫ�Ƶ�ַ�����������ͷ
	u8 u8CRC;///�¼�����CRC
}tITEMINDEX;

///�¼���Ϣ�洢ͷ
typedef struct
{
	u8 u8Amount;	///�¼�����
	tITEMINDEX index[EVT_ITEM_SIZE];///�¼���������
}tEVT_HEADINFO;

///�¼���Ϣ�洢��ʽ
typedef struct
{
	u32 prefix;	///ǰ׺
	tEVT_HEADINFO headInfo;
	u8 u8CRC;///У����
}tPARAM_EVT_HEADINFO_STORAGE;

///�㲥�˵��洢ͷ
typedef struct
{
	u8 u8Amount;	///�¼�����
	tITEMINDEX index[MSGOD_MENU_ITEM_SIZE];///�¼���������
}tMSGODMENU_HEADINFO;

///�㲥�˵��洢��ʽ
typedef struct
{
	u32 prefix;	///ǰ׺
	tMSGODMENU_HEADINFO headInfo;
	u8 u8CRC;///У����
}tPARAM_MSGOD_MENU_HEADINFO_STORAGE;

///�㲥��Ϣ�洢ͷ
typedef struct
{
	u8 u8Amount;	///�¼�����
	tITEMINDEX index[MSGOD_MSG_ITEM_SIZE];///�¼���������
}tMSGODMSG_HEADINFO;

///�㲥��Ϣ�洢��ʽ
typedef struct
{
	u32 prefix;	///ǰ׺
	tMSGODMSG_HEADINFO headInfo;
	u8 u8CRC;///У����
}tPARAM_MSGOD_MSG_HEADINFO_STORAGE;

///�绰���洢ͷ
typedef struct
{
	u8 u8Amount;	///�绰������
	tITEMINDEX index[MSGOD_MSG_ITEM_SIZE];///��������
}tPHONEBOOK_HEADINFO;

///�绰���洢��ʽ
typedef struct
{
	u32 prefix;	///ǰ׺
	tMSGODMSG_HEADINFO headInfo;
	u8 u8CRC;///У����
}tPARAM_PHONEBOOK_HEADINFO_STORAGE;

/*************** �ⲿ�ӿ� *****************************************************************/
int PARAM_FormatMsg_GetHead(u8 type);
int PARAM_FormatMsg_SaveHead(u8 type);
int PARAM_FormatMsg_GetAmount(u8 type);
int PARAM_FormatMsg_GetItem(u8 type, u8 u8ItemIndex, u8* pu8ItemId, u8 *pu8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_ApendItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_ReplaceItem(u8 type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg);
int PARAM_FormatMsg_EraseAll(u8 type);

#endif

#endif /* EVT_H_ */
