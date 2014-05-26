/*
 * parameter.c
 *
 * ��ϵͳ�����϶࣬����洢�ռ�ϴ�Ӳ����������һƬEEPROM��һ��SRAM��
 * ���в���������ⲿEEPROM�У�������ʱ��EEPROM���뵽����SRAM���ڲ�RAMֻ����ջ��
 * ���ݴ洢��ʽ��С��
 *
 *  Created on: 2013-12-27
 *      Author: gaozhengdong
 */

#include <stm32f2xx_conf.h>
#include "parameter.h"
#include "utility.h"

/**
 * ȫ�ֲ�������
 */
static tPARAM_FACTORY_STORAGE tParam_Factory_St;	///��������
tPARAM_FACTORY *ptParam_Factory = &tParam_Factory_St.tFactoryParam;

static tPARAM_SYSTEM_STORAGE tParam_System_St;	///ϵͳ����
tPARAM_SYSTEM *ptParam_System = &tParam_System_St.tSystemParam;

static tPARAM_DEVICE_STORAGE tParam_Device_St;	///�豸����
tPARAM_DEVICE *ptParam_Device = &tParam_Device_St.tDeviceParam;

static tPARAM_RUNTIME_STORAGE tParam_Runtime_St;	///����ʱ����
tPARAM_RUNTIME *ptParam_Runtime = &tParam_Runtime_St.tRuntimeParam;

static tPARAM_DRIVER_STORAGE tParam_Driver_St;	///��ʻ����Ϣ����
tPARAM_DRIVER *ptParam_Driver = &tParam_Driver_St.tDrivers;

static tUPGRADEINFO_STORAGE tParam_UpgInfo_St;		///��������
tUPGRADEINFO *ptUpgInfo = &tParam_UpgInfo_St.tUpgradeInfo;

static tPARAM_RECTAREA_STORAGE tParam_RectArea_St;		///�����������
tPARAM_RECTAREA *ptParam_Rect = &tParam_RectArea_St.tRectArea;

static tPARAM_ROUNDAREA_STORAGE tParam_RoundArea_St;	///Բ���������
tPARAM_ROUNDAREA *ptParam_RoundArea = &tParam_RoundArea_St.tRoundArea;

static tPARAM_POLYGONAREA_STORAGE tParam_PolygonArea_St;	///����β���
tPARAM_POLYGONAREA *ptParam_Polygon = &tParam_PolygonArea_St.tPolygonArea;

static tPARAM_ROUTE_STORAGE tParam_Route_St;	///·�߲���
tPARAM_ROUTE *ptParam_Route = &tParam_Route_St.tRoute;

static tPARAM_EVT_STORAGE tParam_Evt_St;	///�¼�����
tEVT_INFO *ptParam_Evt = &tParam_Evt_St.tEvt;

static tPARAM_MSGOD_MENU_STORAGE tParam_MsgODMenu_St;	///�㲥�˵�����
tMSGODMENU_INFO *ptParam_MsgODMenu = &tParam_MsgODMenu_St.tMsgODMenu;

static tPARAM_MSGOD_MSG_STORAGE tParam_MsgODMsg_St;	///�㲥��Ϣ����
tMSGODMSG_INFO *ptParam_MsgODMsg = &tParam_MsgODMsg_St.tMsgODMsg;

static tPARAM_PHONEBOOK_STORAGE	tParam_PhoneBook_St;	///�绰��
tPHONEBOOK_INFO *ptParam_PhoneBook = &tParam_PhoneBook_St.tPhoneBook;

typedef struct {
	void * ptParam_St;	///�����ڴ�ṹ��
	u16 u16ParamSize;	///��������
	u32 u32Offset;		///�ڴ洢�����е���ʼƫ��λ��
} tPARAM_ST;

///�������ö��壬��ENUMֵ˳���ϸ�һ��
static tPARAM_ST tParams_System[] = {
		{ .ptParam_St = (void*)&tParam_Factory_St,
				.u16ParamSize = sizeof(tPARAM_FACTORY_STORAGE),
				.u32Offset = EEPROM_ADDR_FACTORYPARAM },
		{ .ptParam_St = (void*)&tParam_Device_St,
				.u16ParamSize = sizeof(tPARAM_DEVICE_STORAGE),
				.u32Offset = EEPROM_ADDR_DEVICE },
		{ .ptParam_St = (void*)&tParam_System_St,
				.u16ParamSize = sizeof(tPARAM_SYSTEM_STORAGE),
				.u32Offset = EEPROM_ADDR_SYSTEM},
		{ .ptParam_St = (void*)&tParam_Runtime_St,
				.u16ParamSize = sizeof(tParam_Runtime_St),
				.u32Offset = EEPROM_ADDR_RUNTIME},
		{ .ptParam_St = (void*)&tParam_UpgInfo_St,
				.u16ParamSize = sizeof(tParam_UpgInfo_St),
				.u32Offset = EEPROM_ADDR_UPGRADE},
};

typedef struct {
	u32 u32EEPROMAddr;
	u8 *pStorageAddr;
	u16 u16StorageSize;
	u16 u16ItemSize;
	u8 u8ItemAmount;
} PARAM_CONFIG;

static PARAM_CONFIG tConf[] = {
		///�㲥�˵�
		{ EEPROM_ADDR_MSGOD_MENU, &tParam_MsgODMenu_St, sizeof(tParam_MsgODMenu_St),
				MSGOD_MENU_ITEM_SIZE, MSGOD_MENU_MSG_SIZE},
		///�㲥��Ϣ
		{ EEPROM_ADDR_MSGOD_MSG, &tParam_MsgODMsg_St, sizeof(tParam_MsgODMsg_St),
				MSGOD_MSG_ITEM_SIZE, MSGOD_MSG_MSG_SIZE},
		///�¼�
		{ EEPROM_ADDR_EVENT, &tParam_Evt_St, sizeof(tParam_Evt_St),
				EVT_ITEM_SIZE, EVT_MSG_SIZE},
		///��������
		{ EEPROM_ADDR_RECTANGLE, &tParam_RectArea_St, sizeof(tParam_RectArea_St),
				sizeof(tItem_RectArea_Ext), RECT_AREA_ITEM_SIZE },
		///Բ������
		{ EEPROM_ADDR_ROUND, &tParam_RoundArea_St, sizeof(tPARAM_RECTAREA_STORAGE),
				sizeof(tItem_RoundArea_Ext), ROUND_AREA_ITEM_SIZE },
		///���������
		{ EEPROM_ADDR_POLYGON, &tParam_PolygonArea_St, sizeof(tPARAM_RECTAREA_STORAGE),
				sizeof(tItem_Polygon_Ex), POLYGON_AREA_ITEM_SIZE },
		///·��
		{ EEPROM_ADDR_ROUTE, &tParam_Route_St, sizeof(tPARAM_ROUTE_STORAGE),
				sizeof(tITEM_CORNERPOINT_Ex), PARAM_ROUTE_ITEM_MAX },
		///��ʻ��
		{ EEPROM_ADDR_DRIVER, &tParam_Driver_St, sizeof(tParam_Driver_St),
				sizeof(tPARAM_DRIVER), DRIVERS_PERCAR_MAXNUM },
		///�绰��
		{ EEPROM_ADDR_PHONEBOOK, &tParam_PhoneBook_St, sizeof(tParam_PhoneBook_St),
				sizeof(tPHONEBOOK_ITEM), PHONE_ITEM_SIZE },

};


///ȡ���е�һ���ֽڣ���Ч��־
#define IS_VALID(type, i)	(*(tConf[type].pStorageAddr + PREFIX_SIZE + 1 + i * tConf[type].u16ItemSize))
///ȡ������
#define ITEM_AMOUNT(type) (*(tConf[type].pStorageAddr + PREFIX_SIZE))
///ȡID
#define ITEM_ID(type, i) (*(u32*)(tConf[type].pStorageAddr + PREFIX_SIZE + 1 + i * tConf[type].u16ItemSize + 1))
///��洢λ��
#define ITEM_OFFSET(type, i)	(tConf[type].pStorageAddr + PREFIX_SIZE + 1 + i * tConf[type].u16ItemSize)


#if 1
/**����ϵͳ�����꣬��������U8*/
#define set_sysparam_u8(id, len, pVal)	\
										do {	\
											(id = *((u8*)pVal));	\
										}while(0)

/**����ϵͳ�����꣬��������U16*/
#define set_sysparam_u16(id, len, pVal)	\
										do {	\
											(id = *((u8*)pVal));	\
											ENDIAN_U16(id);		\
										}while(0)

/**����ϵͳ�����꣬��������U32*/
#define set_sysparam_u32(id, len, pVal)	\
										do {	\
											(id = *((u8*)pVal));	\
											ENDIAN_U32(id);		\
										}while(0)

/**����ϵͳ�����꣬�ַ����Ͳ���*/
#define set_sysparam_str(id, len, pVal)  \
										do {	\
											memcpy_(id, pVal, len);	\
											id[len] = '\0';	\
										}while(0)

/**��ȡϵͳ�����U32�Ͳ���*/
#define get_sysparam_u32(pAdd, id, val)	\
		do	{	\
			tPARAMITEM_U32* pItem;		\
			pItem = (tPARAMITEM_U32*)pAdd;	\
			pItem->u32Id = id;	\
			pItem->u8Len = 4;		\
			pItem->u32Value = val;	\
			ENDIAN_U32(pItem->u32Id);		\
			ENDIAN_U32(pItem->u32Value);		\
		}while(0);

/**��ȡϵͳ�����U32�Ͳ���*/
#define get_sysparam_u16(pAdd, id, val)	\
		do	{	\
			tPARAMITEM_U16* pItem;		\
			pItem = (tPARAMITEM_U16*)pAdd;	\
			pItem->u32Id = id;	\
			pItem->u8Len = 2;		\
			pItem->u16Value = val;	\
			ENDIAN_U32(pItem->u32Id);		\
			ENDIAN_U16(pItem->u16Value);		\
		}while(0);

/**��ȡϵͳ�����U32�Ͳ���*/
#define get_sysparam_u8(pAdd, id, val)	\
		do	{	\
			tPARAMITEM_U8* pItem;		\
			pItem = (tPARAMITEM_U8*)pAdd;	\
			pItem->u32Id = id;	\
			pItem->u8Len = 1;		\
			pItem->u8Value = val;	\
			ENDIAN_U32(pItem->u32Id);		\
		}while(0);

/**��ȡϵͳ�����STRING�Ͳ���*/
#define get_sysparam_str(pAdd, id, str)	\
		do	{	\
			tPARAMITEM_STR* pItem;		\
			pItem = (tPARAMITEM_STR*)pAdd;	\
			pItem->u32Id = id;	\
			pItem->u8Len = strlen_(str);		\
			pAdd += sizeof(tPARAMITEM_STR);	\
			strcpy_(pAdd + sizeof(tPARAMITEM_STR), str);	\
			ENDIAN_U32(pItem->u32Id);		\
		}while(0);

/**��ȡϵͳ����������Ͳ���*/
#define get_sysparam_mem(pAdd, id, len, str)	\
		do	{	\
			tPARAMITEMHEAD* pItem;		\
			pItem = (tPARAMITEMHEAD*)pAdd;	\
			pItem->u32Id = id;	\
			pItem->u8Len = len;		\
			pAdd += sizeof(tPARAMITEMHEAD);	\
			strcpy_(pAdd + sizeof(tPARAMITEMHEAD), str);	\
			ENDIAN_U32(pItem->u32Id);		\
		}while(0);
#endif

#if 0
/**����ϵͳ�����꣬��������U8*/
static __inline__ void set_sysparam_u8(id, len, pVal)
{
	id = *((u8*)pVal);
}

/**����ϵͳ�����꣬��������U16*/
static __inline__ void set_sysparam_u16(id, len, pVal)
{
	id = *((u16*)pVal);
	ENDIAN_U16(id);
}

/**����ϵͳ�����꣬��������U32*/
static __inline__ void set_sysparam_u32(id, len, pVal)
{
	id = *((u32*)pVal);
	ENDIAN_U32(id);
}

/**����ϵͳ�����꣬�ַ����Ͳ���*/
static __inline__ void set_sysparam_str(id, len, pVal)
{
	memcpy_(id, pVal, len);
	id[len] = '\0';
}

/**��ȡϵͳ�����U32�Ͳ���*/
static __inline__ void get_sysparam_u32(pAdd, id, val)
{
	tPARAMITEM_U32* pItem;
	pItem = (tPARAMITEM_U32*)pAdd;
	pItem->u32Id = id;
	pItem->u8Len = 4;
	pItem.u32Value = val;
	ENDIAN_U32(pItem->u32Id);
	ENDIAN_U32(pItem.u32Value);
}

/**��ȡϵͳ�����U32�Ͳ���*/
static __inline__ void get_sysparam_u16(pAdd, id, val)
{
	tPARAMITEM_U16* pItem;
	pItem = (tPARAMITEM_U16*)pAdd;
	pItem->u32Id = id;
	pItem->u8Len = 2;
	pItem.u16Value = val;
	ENDIAN_U32(pItem->u32Id);
	ENDIAN_U16(pItem.u16Value);
}

/**��ȡϵͳ�����U32�Ͳ���*/
static __inline__ void get_sysparam_u8(pAdd, id, val)
{
	tPARAMITEM_U8* pItem;
	pItem = (tPARAMITEM_U8*)pAdd;
	pItem->u32Id = id;
	pItem->u8Len = 1;
	pItem.u8Value = val;
	ENDIAN_U32(pItem->u32Id);
}

/**��ȡϵͳ�����STRING�Ͳ���*/
static __inline__ void get_sysparam_str(pAdd, id, str)
{
	tPARAMITEM_STR* pItem;
	pItem = (tPARAMITEM_STR*)pAdd;
	pItem->u32Id = id;
	pItem->u8Len = strlen_(str);
	pAdd += sizeof(tPARAMITEM_STR);
	strcpy_(pAdd + sizeof(tPARAMITEM_STR), str);
	ENDIAN_U32(pItem->u32Id);
}

/**��ȡϵͳ����������Ͳ���*/
static __inline__ void get_sysparam_mem(pAdd, id, len, str)
{
	tPARAMITEMHEAD* pItem;
	pItem = (tPARAMITEMHEAD*)pAdd;
	pItem->u32Id = id;
	pItem->u8Len = len;
	pAdd += sizeof(tPARAMITEMHEAD);
	strcpy_(pAdd + sizeof(tPARAMITEMHEAD), str);
	ENDIAN_U32(pItem->u32Id);
}
#endif

/**
 * ϵͳ�ָ�����������
 *
 * @return 	int��0=�ɹ���-1=ʧ��
 */
int RecoverSystemDefaultParam(void) {

	return 0;
}

/**
 * ��������ϵͳ����
 *
 * @return	int��0=�ɹ���-1=ʧ��
 */
int Load_CommonParams(void) {
	u8 i = 0;
	while (i < (u8) PARAM_TYPEEND) {
		int ret = Load_Params((ePARAMTYPE) i);
		if (ret < 0) ///��һ�����Ͳ�������ʧ�ܣ�����
			return ret;
		i++;
	}
	return 0;
}

/**
 * ��������ϵͳ����
 *
 * @return	int��0=�ɹ���-1=ʧ��
 */
int Save_CommonParams(void) {

	return 0;
}

/**
 * ��ȡָ�����Ͳ���
 *
 * @param 	eType	��������
 * @return	int��0=�ɹ���-1=ʧ��
 */
int Load_Params(eSYSPARAM eType) {


	return 0;
}

/**
 * ����ָ�����Ͳ���
 *
 * @param 	eType ��������
 * @return	int��0=�ɹ���-1=ʧ��
 */
int Save_Params(eSYSPARAM eType) {

	return 0;
}

/**
 * ȡϵͳ����
 *
 * @param id		[IN]	����ID
 * @param val		[OUT]	����ֵ
 * @return
 */
int Get_SystemParam(u32 id, void* val) {
//	val = (void*)(ADDRESS(id) + 3);
	return 0;
}

/**
 * ����ϵͳ����
 *
 * @param id	��������
 * @param len	��������
 * @param pVal	����ֵ
 * @return	0=�ɹ���-1=ʧ��
 */
int Set_SystemParam(u32 id, u8 len, u8 *pVal) {
	switch (id) {
	case PARAM_ALIVE_INTERVAL: ///=	0x0001,	�ն��������ͼ������λΪs
		set_sysparam_u8(ptParam_System->u32LinkAliveTimeOut, len, pVal);
		break;
	case PARAM_TCPACK_TIMEOUT: ///=	0x0002,TCPӦ��ʱ����λΪs
		set_sysparam_u8(ptParam_System->u32TCPAckTimeout, len, pVal);
		break;
	case PARAM_TCP_RESENDTIMES: ///=	0x0003,	TCP��Ϣ�ش�����
		set_sysparam_u8(ptParam_System->u32TCPReSendTimes, len, pVal);
		break;
	case PARAM_UDPACK_TIMEOUT: ///=	0x0004,	UDPӦ��ʱ����λΪs
		set_sysparam_u8(ptParam_System->u32UDPAckTimeout, len, pVal);
		break;
	case PARAM_UDP_RESENDTIMES: ///=	0x0005,	UDP��Ϣ�ش�����
		set_sysparam_u8(ptParam_System->u32UDPReSendTimes, len, pVal);
		break;
	case PARAM_SMSACK_TIMEOUT: ///=	0x0006,	SMSӦ��ʱ����λΪs
		set_sysparam_u8(ptParam_System->u32SMSAckTimeout, len, pVal);
		break;
	case PARAM_SMS_RESENDTIMES: ///=	0x0007,	SMS��Ϣ�ش�����
		set_sysparam_u8(ptParam_System->u32SMSReSendTimes, len, pVal);
		break;
		/**0X0008 ----0X000F ����*/
	case PARAM_MAINSRV_APN: ///	=	0x0010,	��������APN������ͨ�Ų��ŷ��ʵ�
		set_sysparam_str(ptParam_System->au8MainServerApn, len, pVal);
		break;
	case PARAM_MAINSRV_USR: ///	=	0x0011,	������������ͨ�Ų����û���
		set_sysparam_str(ptParam_System->au8MainServerUsr, len, pVal);
		break;
	case PARAM_MAINSRV_PSWD: ///=	0x012,	������������ͨ�Ų�������
		set_sysparam_str(ptParam_System->au8MainServerPswd, len, pVal);
		break;
	case PARAM_MAINSRV_IP: ///=	0x0013,	����������ַ��IP������
		set_sysparam_str(ptParam_System->au8MainServerIP, len, pVal);
		break;
	case PARAM_VICESRV_APN: ///=	0x0014,	���ݷ�����APN������ͨ�Ų��ŷ��ʵ�
		set_sysparam_str(ptParam_System->au8ViceServerApn, len, pVal);
		break;
	case PARAM_VICESRV_USR: ///=	0x0015,	���ݷ���������ͨ�Ų����û���
		set_sysparam_str(ptParam_System->au8ViceServerUsr, len, pVal);
		break;
	case PARAM_VICESRV_PSWD: ///=	0x0016,	���ݷ���������ͨ�Ų�������
		set_sysparam_str(ptParam_System->au8ViceServerPswd, len, pVal);
		break;
	case PARAM_VICESRV_IP: ///=	0x0017,	���ݷ�������ַ��IP������
		set_sysparam_str(ptParam_System->au8ViceServerIP, len, pVal);
		break;
	case PARAM_TCP_PORT: ///	=	0x0018,	TCP�˿�
		set_sysparam_u32(ptParam_System->u32TCPPort, len, pVal);
		break;
	case PARAM_UDP_PORT: ///=	0x0019,	UDP�˿�
		set_sysparam_u32(ptParam_System->u32UDPPort, len, pVal);
		break;

		/**0X001A ----0X001F ����*/
#ifdef JTT808_Ver_2013	///�°�����Э�鲿��	case PARAM_ICVERIFY_MS_IPOrDNS: ///=	0x001A,	��·����֤IC����֤��������IP��ַ������		set_sysparam_str(ptParam_System->IC_Verify_MS_IPOrDNS, len, pVal);		break;
	case PARAM_ICVERIFY_TCPPORT: ///=	0x001B,	��·����֤IC����֤��������TCP�˿�
		set_sysparam_u32(ptParam_System->IC_Verify_TCP_Port, len, pVal);
		break;
	case PARAM_ICVERIFY_UDPPORT: ///=	0x001C,	��·����֤IC����֤��������UDP�˿�
		set_sysparam_u32(ptParam_System->IC_Verify_UDP_Port, len, pVal);
		break;
	case PARAM_ICVERIFY_BS_UDPPORT: ///=	0x001D,	��·����֤IC�����ݷ�����IP��ַ���������˿�ͬ��������
		set_sysparam_str(ptParam_System->IC_Verify_BS_IPOrDNS, len, pVal);
		break;
#endif///JTT808_Ver_2013	case PARAM_POSRPT_STRATEGE: ///=	0x0020,�ն�λ�û㱨����		set_sysparam_u32(ptParam_System->u32PosRptStrategy, len, pVal);
		break;
	case PARAM_POSRPT_METHOD: ///=	0x0021,�ն˻㱨����
		set_sysparam_u32(ptParam_System->u32PosRptMethod, len, pVal);
		break;
	case PARAM_DRV_UNREG_RPTPERIOD: ///=	0x0022,	��ʻԱδ��¼�㱨ʱ��������λΪ��
		set_sysparam_u32(ptParam_System->u32RptIntervalDrvUnReg, len, pVal);
		break;
		/**0X0023 ----0X0026 ����*/
	case PARAM_RPTPERIOD_ONSLEEP: ///=0x0027,����ʱ�㱨ʱ��������λΪ��
		set_sysparam_u32(ptParam_System->u32RptIntervalSleep, len, pVal);
		break;
	case PARAM_RPTPERIOD_URGENT: ///=0x0028,��������ʱ�㱨ʱ��������λΪ��
		set_sysparam_u32(ptParam_System->u32RptIntervalEmergency, len, pVal);
		break;
	case PARAM_RPTPERIOD_DEFAULT: ///=0x0029,ȱʡʱ�㱨ʱ��������λΪ��
		set_sysparam_u32(ptParam_System->u32RptIntervalDefault, len, pVal);
		break;

		/**0X002A ----0X002B ����*/

	case PARAM_RPTDIST_DEFAULT: ///=	0x002C,	ȱʡʱ����㱨�������λΪ��
		set_sysparam_u32(ptParam_System->u32RptDistanceDefault, len, pVal);
		break;
	case PARAM_RPTDIST_DRV_UNREG: ///=	0x002D,	��ʻԱδ��¼����㱨�������λΪ��
		set_sysparam_u32(ptParam_System->u32RptDistanceDrvUnReg, len, pVal);
		break;
	case PARAM_RPTDIST_SLEEP: ///=	0x002E,	����ʱ����㱨�������λΪ��
		set_sysparam_u32(ptParam_System->u32RptDistanceSleep, len, pVal);
		break;
	case PARAM_RPTDIST_URGENT: ///=	0x002F,	��������ʱ����㱨�������λΪ��
		set_sysparam_u32(ptParam_System->u32RptDistanceEmergency, len, pVal);
		break;
	case PARAM_INFLECTION_POINT_ANGLE: ///=	0x0030,	�յ㲹���Ƕȣ���λΪ��(< 180)
		set_sysparam_u32(ptParam_System->u32InflectionSuppleAngle, len, pVal);
		break;
	case PARAM_BARRIER_RADIUS: ///=	0x031,	����Χ���뾶(�Ƿ���λ��ֵ)����λΪ��
		set_sysparam_u32(ptParam_System->u16BarrierRadius, len, pVal);
		break;

		/**0X0032----0X003F ����*/

	case PARAM_MONITORCENTER_PHONENUM: ///	=	0x0040,	���ƽ̨�绰����
		set_sysparam_str(ptParam_System->aMonitorPlatformPhone, len, pVal);
		break;
	case PARAM_RESET_PHONENUM: ///=	0x0041,	��λ�绰����
		set_sysparam_str(ptParam_System->aResetPhone, len, pVal);
		break;
	case PARAM_RECOVERFACTORY_PHONENUM: ///	=	0x0042, �ظ��������õ绰����
		set_sysparam_str(ptParam_System->aRecoverFactorySettingPhone, len, pVal);
		break;
	case PARAM_MONITORCENTER_SMS_NUM: ///=	0x0043,	���ƽ̨SMS�绰����
		set_sysparam_str(ptParam_System->aCenterSMSPhone, len, pVal);
		break;
	case PARAM_RECVTXTALARM_SMS_NUM: ///=	0x0044,	�����ն�SMS�ı������绰����
		set_sysparam_str(ptParam_System->aRcvTerminalSMSAlarmPhone, len, pVal);
		break;

	case PARAM_ANSWERCALLING_STRATEGE: ///=	0x0045,///�ն˵绰��������
		set_sysparam_u32(ptParam_System->u32PhoneListenStratagy, len, pVal);
		break;
	case PARAM_TOUT_PERCALL: ///	=	0x0046,///�ն�ÿ��ͨ��ʱ�ͨ��ʱ��
		set_sysparam_u32(ptParam_System->u32PhoneConnTimeoutEachTime, len, pVal);
		break;
	case PARAM_TOUT_PERMONTHCALL: ///=	0x0047,///�ն�ÿ���ͨ��ʱ��
		set_sysparam_u32(ptParam_System->u32PhoneConnTimeoutCurMonth, len, pVal);
		break;

	case PARAM_MONITORLISTEN_PHONENUM: ///=	0x0048,	�����绰����
		set_sysparam_str(ptParam_System->aVoiceMonitorPhone, len, pVal);
		break;
	case PARAM_SUPERVISE_SMS_NUM: ///=	0x0049,	���ƽ̨��Ȩ���ź���
		set_sysparam_str(ptParam_System->aMonitorPlatformPriviligPhone, len, pVal);
		break;

		/**0X004A----0X004F ����*/

	case PARAM_ALARM_MASK: ///=	0x0050,����������
		set_sysparam_u32(ptParam_System->u32AlarmMask, len, pVal);
		break;
	case PARAM_SMSCTRL_ONALARM: ///=	0x0051,���������ı�SMS����
		set_sysparam_u32(ptParam_System->u32SendSMSOnAlarmCtrl, len, pVal);
		break;
	case PARAM_PICCTRL_ONALARM: ///=	0x0052,�������㿪��
		set_sysparam_u32(ptParam_System->u32CaptureOnAlarmCtrl, len, pVal);
		break;
	case PARAM_PICCTRL_STORE_ONALARM: ///=	0x0053,	��������洢��־
		set_sysparam_u32(ptParam_System->u32AlarmPictureStorageCtrl, len, pVal);
		break;
	case PARAM_KEYALARM_FLAG: ///=	0x0054,�ؼ���־
		set_sysparam_u32(ptParam_System->u32KeyAlarmMask, len, pVal);
		break;
	case PARAM_MAXSPEED: ///=	0x0055,	   ����ٶȣ���λΪ����ÿСʱ(km/h)
		set_sysparam_u32(ptParam_System->u32SpeedMax, len, pVal);
		break;
	case PARAM_OVERSPEED_LASTING_TIMEOUT: ///=	0x0056,	���ٳ���ʱ�䣬��λΪ��
		set_sysparam_u32(ptParam_System->u32OverSpeedContinousTime, len, pVal);
		break;
	case PARAM_CONTINUOUS_DRV_TIMEOUT: ///=	0x0057,	������ʻʱ�����ޣ���λΪ��
		set_sysparam_u32(ptParam_System->u32ContinousDriveTimeThreshold, len, pVal);
		break;
	case PARAM_DRV_TOTAL_EACHDAY_TIMEOUT: ///=	0x0058,	�����ۼƼ�ʻʱ�䣬��λΪ��
		set_sysparam_u32(ptParam_System->u32DriveTimesTodayThreshold, len, pVal);
		break;
	case PARAM_LEAST_REST_TIMEOUT: ///=	0x0059,	��С��Ϣʱ�䣬��λΪ��
		set_sysparam_u32(ptParam_System->u32MinRestTime, len, pVal);
		break;
	case PARAM_MAX_STOPPING_TIMEOUT: ///	=	0x005A,	�ͣ��ʱ�䣬��λΪ��
		set_sysparam_u32(ptParam_System->u32MaxStopTime, len, pVal);
		break;

		/**0X005B----0X006F */
#ifdef JTT808_Ver_2013
	case PARAM_OVERSPEED_PREALARM_DIF: ///=	0x005B,	���ٱ���Ԥ����ֵ����λ1/10Km/h
		set_sysparam_u16(ptParam_System->u16OverSpeed_PreAlarm_Dif, len, pVal);
		break;
	case PARAM_FATIGE_PREALARM_DIF: ///=	0x005C,	ƣ�ͼ�ʻԤ����ֵ����λΪ�룬>0
		set_sysparam_u16(ptParam_System->u16Fatige_PreAlarm_Dif, len, pVal);
		break;
	case PARAM_COLLISION_SETTING: ///=0x005D,
		set_sysparam_u16(ptParam_System->u16CollisionSetting, len, pVal);
		break;
	case PARAM_SIDECOLLAPSE_SETTING: ///=0x005E,�෭�����������ã��෭�Ƕȣ���λ1�ȣ�Ĭ��30��
		set_sysparam_u16(ptParam_System->u16SideCollapseSetting, len, pVal);
		break;

		/**0X005F----0X0063 */
	case PARAM_PERIODIC_PHOTO_CTRL: ///=	0x0064,	��ʱ���տ���
		memcpy_((u8*) &ptParam_System->tPhotoCtrl_Periodic, len, pVal);
		ENDIAN_PU32(&ptParam_System->tPhotoCtrl_Periodic); ///ת��С��
		break;
	case PARAM_PERDIST_PHOTO_CTRL: ///=	0x0065,	�������տ���
		memcpy_((u8*) &ptParam_System->tPhotoCtrl_Dist, len, pVal);
		ENDIAN_PU32(&ptParam_System->tPhotoCtrl_Dist); ///ת��С��
		break;
		/**0X0066----0X006F */
#endif///JTT808_Ver_2013	case PARAM_VIDEO_QUALITY:	///=	0x0070,	ͼƬ/��Ƶ������1-10,1���		set_sysparam_u32(ptParam_System->u32PictureQuality, len, pVal);
		break;
	case PARAM_BRIGHTNESS:	///=	0x0071,	���ȣ�0-255
		set_sysparam_u32(ptParam_System->u32PictureBrightness, len, pVal);
		break;
	case PARAM_CONTRASTNESS:	///=	0x0072,	�Աȶȣ�0-127
		set_sysparam_u32(ptParam_System->u32PictureContrastness, len, pVal);
		break;
	case PARAM_SATURATION:	///=	0x0073,	���Ͷȣ�0-127
		set_sysparam_u32(ptParam_System->u32PictureSaturation, len, pVal);
		break;
	case PARAM_CHROMA:	///=	0x0074,	ɫ�ȣ�0-255
		set_sysparam_u32(ptParam_System->u32PictureChroma, len, pVal);
		break;

		/**0X0075----0X007F*/

	case PARAM_VEHICLE_KILOMETRES:	///=	0x0080,	������̱������1/10km
		set_sysparam_u32(ptParam_System->u32Kilometers, len, pVal);
		break;
	case PARAM_VEHICLE_PROVINCE_ID:	///=	0x0081,	�������ڵ�ʡ��ID
		set_sysparam_u16(ptParam_System->u16ProviceId, len, pVal);
		break;
	case PARAM_VEHICLE_CITY_ID:	///=	0x0082,	�������ڵ�����ID
		set_sysparam_u16(ptParam_System->u16CountyId, len, pVal);
		break;
	case PARAM_VEHICLE_ID:	///=	0x0083,	���ƺ���
		set_sysparam_str(ptParam_System->au8Plate, len, pVal);
		break;
	case PARAM_VEHICLE_ID_COLOR:	///=	0x0084,	������ɫ
		set_sysparam_u8(ptParam_System->u8Color, len, pVal);
		break;

#ifdef JTT808_Ver_2013
	case PARAM_GNSS_MODE:	///0x0090 GNSS ��λģʽ
		set_sysparam_u8(ptParam_System->u8GNSS_Mode, len, pVal);
		break;
	case PARAM_GNSS_BOUNDRATE:	///	0x0091 GNSS ������
		set_sysparam_u8(ptParam_System->u8GNSS_Baund, len, pVal);
		break;
	case PARAM_GNSS_DMSG_OUTFREQ:	///	0x0092 GNSS ģ����ϸ��λ�������Ƶ��
		set_sysparam_u8(ptParam_System->u8GNSS_DetailMsg_OutFreq, len, pVal);
		break;
	case PARAM_GNSS_DMSG_RETRIEVEFREQ:	///	0x0093 GNSS ģ����ϸ��λ���ݲɼ�Ƶ��
		set_sysparam_u32(ptParam_System->u32GNSS_DetailMsg_RetrieveFreq, len, pVal);
		break;
	case PARAM_GNSS_DMSG_RPTTYPE:	///	0x0094 GNSS ģ����ϸ��λ�����ϴ���ʽ��
		set_sysparam_u8(ptParam_System->u8GNSS_DetailMsg_RptType, len, pVal);
		break;
	case PARAM_GNSS_DMSG_RPTSETTING:	///	0x0095 GNSS ģ����ϸ��λ�����ϴ�����
		set_sysparam_u32(ptParam_System->u32GNSS_DetailMsg_RptSetting, len, pVal);
		break;
	case PARAM_CAN_CH1_RETRIEVEINTERVAL:	///	0x0100 ����ͨ��1 �ɼ�ʱ����(ms)
		set_sysparam_u32(ptParam_System->u32CAN_Ch1_RetrieveInterval, len, pVal);
		break;
	case PARAM_CAN_CH1_UPLOADINTERVAL:	///	0x0101 ����ͨ��1 �ϴ�ʱ����
		set_sysparam_u16(ptParam_System->u16CAN_Ch1_UploadInterval, len, pVal);
		break;
	case PARAM_CAN_CH2_RETRIEVEINTERVAL:	///	0x0102 CAN ����ͨ��2 �ɼ�ʱ����
		set_sysparam_u32(ptParam_System->u32CAN_Ch2_RetrieveInterval, len, pVal);
		break;
	case PARAM_CAN_CH2_UPLOADINTERVAL:	///	0x0103 CAN ����ͨ��2 �ϴ�ʱ����
		set_sysparam_u16(ptParam_System->u16CAN_Ch2_UploadInterval, len, pVal);
		break;
	case PARAM_CAN_SOLORETRIEVE:	///	0x0110 CAN ����ID �����ɼ�����
		set_sysparam_u8(ptParam_System->u8GNSS_Mode, len, pVal);
		break;
#endif
	default:
		return -1;
	}

	return 0;
}

/**
 * ȡϵͳ�����һ����������в����ֶ�
 *
 * @param _pt		������ַ
 * @return	�������
 */
u16 GetItem_AllSystemParam(u8 *pData) {
	u8 *_pt = pData;

	/**ͨ�Ų���*/
	get_sysparam_u32(_pt, PARAM_ALIVE_INTERVAL, ptParam_System->u32LinkAliveTimeOut);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_TCPACK_TIMEOUT, ptParam_System->u32TCPAckTimeout);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_TCP_RESENDTIMES, ptParam_System->u32UDPReSendTimes);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_UDPACK_TIMEOUT, ptParam_System->u32UDPAckTimeout);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_UDP_RESENDTIMES, ptParam_System->u32LinkAliveTimeOut);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_SMSACK_TIMEOUT, ptParam_System->u32SMSAckTimeout);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_SMS_RESENDTIMES, ptParam_System->u32SMSReSendTimes);
	_pt += sizeof(tPARAMITEM_U32);

	/**������������*/
	get_sysparam_str(_pt, PARAM_MAINSRV_APN, ptParam_System->au8MainServerApn);
	_pt += (5 + strlen_(ptParam_System->au8MainServerApn));

	get_sysparam_str(_pt, PARAM_MAINSRV_USR, ptParam_System->au8MainServerUsr);
	_pt += (5 + strlen_(ptParam_System->au8MainServerUsr));

	get_sysparam_str(_pt, PARAM_MAINSRV_PSWD, ptParam_System->au8MainServerPswd);
	_pt += (5 + strlen_(ptParam_System->au8MainServerPswd));

	get_sysparam_str(_pt, PARAM_MAINSRV_IP, ptParam_System->au8MainServerIP);
	_pt += (5 + strlen_(ptParam_System->au8MainServerIP));

	/**���ݷ���������*/
	get_sysparam_str(_pt, PARAM_VICESRV_APN, ptParam_System->au8ViceServerApn);
	_pt += (5 + strlen_(ptParam_System->au8ViceServerApn));

	get_sysparam_str(_pt, PARAM_VICESRV_USR, ptParam_System->au8ViceServerUsr);
	_pt += (5 + strlen_(ptParam_System->au8ViceServerUsr));

	get_sysparam_str(_pt, PARAM_VICESRV_PSWD, ptParam_System->au8ViceServerPswd);
	_pt += (5 + strlen_(ptParam_System->au8ViceServerPswd));

	get_sysparam_str(_pt, PARAM_VICESRV_IP, ptParam_System->au8ViceServerIP);
	_pt += (5 + strlen_(ptParam_System->au8ViceServerIP));

	get_sysparam_str(_pt, PARAM_TCP_PORT, ptParam_System->u32TCPPort);
	_pt += (5 + strlen_(ptParam_System->u32TCPPort));

	get_sysparam_str(_pt, PARAM_UDP_PORT, ptParam_System->u32UDPPort);
	_pt += (5 + strlen_(ptParam_System->u32UDPPort));

#ifdef JTT808_Ver_2013
	get_sysparam_str(_pt, PARAM_ICVERIFY_MS_IPOrDNS, ptParam_System->IC_Verify_MS_IPOrDNS);
	_pt += (5 + strlen_(ptParam_System->IC_Verify_MS_IPOrDNS));

	get_sysparam_u32(_pt, PARAM_ICVERIFY_TCPPORT, ptParam_System->IC_Verify_TCP_Port);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_ICVERIFY_UDPPORT, ptParam_System->IC_Verify_UDP_Port);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_str(_pt, PARAM_ICVERIFY_BS_UDPPORT, ptParam_System->IC_Verify_BS_IPOrDNS);
	_pt += (5 + strlen_(ptParam_System->IC_Verify_BS_IPOrDNS));
#endif

	/**λ�û㱨����*/
	get_sysparam_u32(_pt, PARAM_POSRPT_STRATEGE, ptParam_System->u32PosRptStrategy);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_POSRPT_METHOD, ptParam_System->u32PosRptMethod);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_DRV_UNREG_RPTPERIOD, ptParam_System->u32RptIntervalDrvUnReg);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_RPTPERIOD_ONSLEEP, ptParam_System->u32RptIntervalSleep);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_RPTPERIOD_URGENT, ptParam_System->u32RptIntervalEmergency);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_RPTPERIOD_DEFAULT, ptParam_System->u32RptIntervalDefault);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_RPTDIST_DEFAULT, ptParam_System->u32RptDistanceDefault);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_RPTDIST_DRV_UNREG, ptParam_System->u32RptDistanceDrvUnReg);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_RPTDIST_SLEEP, ptParam_System->u32RptDistanceSleep);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_RPTDIST_URGENT, ptParam_System->u32RptDistanceEmergency);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_INFLECTION_POINT_ANGLE, ptParam_System->u32InflectionSuppleAngle);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u16(_pt, PARAM_BARRIER_RADIUS, ptParam_System->u16BarrierRadius);
	_pt += sizeof(tPARAMITEM_U16);

	/**�绰����*/
	get_sysparam_str(_pt, PARAM_MONITORCENTER_PHONENUM, ptParam_System->aMonitorPlatformPhone);
	_pt += (5 + strlen_(ptParam_System->aMonitorPlatformPhone));

	get_sysparam_str(_pt, PARAM_RESET_PHONENUM, ptParam_System->aResetPhone);
	_pt += (5 + strlen_(ptParam_System->aResetPhone));

	get_sysparam_str(_pt, PARAM_RECOVERFACTORY_PHONENUM, ptParam_System->aRecoverFactorySettingPhone);
	_pt += (5 + strlen_(ptParam_System->aRecoverFactorySettingPhone));

	get_sysparam_str(_pt, PARAM_MONITORCENTER_SMS_NUM, ptParam_System->aCenterSMSPhone);
	_pt += (5 + strlen_(ptParam_System->aCenterSMSPhone));

	get_sysparam_str(_pt, PARAM_RECVTXTALARM_SMS_NUM, ptParam_System->aRcvTerminalSMSAlarmPhone);
	_pt += (5 + strlen_(ptParam_System->aRcvTerminalSMSAlarmPhone));

	get_sysparam_u32(_pt, PARAM_ANSWERCALLING_STRATEGE, ptParam_System->u32InflectionSuppleAngle);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_TOUT_PERCALL, ptParam_System->u32InflectionSuppleAngle);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_TOUT_PERMONTHCALL, ptParam_System->u32InflectionSuppleAngle);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_str(_pt, PARAM_MONITORLISTEN_PHONENUM, ptParam_System->aVoiceMonitorPhone);
	_pt += (5 + strlen_(ptParam_System->aVoiceMonitorPhone));

	get_sysparam_str(_pt, PARAM_SUPERVISE_SMS_NUM, ptParam_System->aMonitorPlatformPriviligPhone);
	_pt += (5 + strlen_(ptParam_System->aMonitorPlatformPriviligPhone));

	/**��������*/
	get_sysparam_u32(_pt, PARAM_ALARM_MASK, ptParam_System->u32AlarmMask);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_SMSCTRL_ONALARM, ptParam_System->u32SendSMSOnAlarmCtrl);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_PICCTRL_ONALARM, ptParam_System->u32CaptureOnAlarmCtrl);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_PICCTRL_STORE_ONALARM, ptParam_System->u32AlarmPictureStorageCtrl);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_KEYALARM_FLAG, ptParam_System->u32KeyAlarmMask);
	_pt += sizeof(tPARAMITEM_U32);

	/**���ټ�����*/
	get_sysparam_u32(_pt, PARAM_MAXSPEED, ptParam_System->u32SpeedMax);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_OVERSPEED_LASTING_TIMEOUT, ptParam_System->u32OverSpeedContinousTime);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_CONTINUOUS_DRV_TIMEOUT, ptParam_System->u32ContinousDriveTimeThreshold);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_DRV_TOTAL_EACHDAY_TIMEOUT, ptParam_System->u32DriveTimesTodayThreshold);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_LEAST_REST_TIMEOUT, ptParam_System->u32MinRestTime);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_MAX_STOPPING_TIMEOUT, ptParam_System->u32MaxStopTime);
	_pt += sizeof(tPARAMITEM_U32);

#ifdef JTT808_Ver_2013
	get_sysparam_u16(_pt, PARAM_OVERSPEED_PREALARM_DIF, ptParam_System->u16OverSpeed_PreAlarm_Dif);
	_pt += sizeof(tPARAMITEM_U16);

	get_sysparam_u16(_pt, PARAM_FATIGE_PREALARM_DIF, ptParam_System->u16Fatige_PreAlarm_Dif);
	_pt += sizeof(tPARAMITEM_U16);

	get_sysparam_u16(_pt, PARAM_COLLISION_SETTING, ptParam_System->u16CollisionSetting);
	_pt += sizeof(tPARAMITEM_U16);

	get_sysparam_u16(_pt, PARAM_SIDECOLLAPSE_SETTING, ptParam_System->u16SideCollapseSetting);
	_pt += sizeof(tPARAMITEM_U16);

	/**0X005F----0X0063 */
	get_sysparam_u32(_pt, PARAM_PERIODIC_PHOTO_CTRL, *((u32* )&ptParam_System->tPhotoCtrl_Periodic));
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_PERDIST_PHOTO_CTRL, *((u32* )&ptParam_System->tPhotoCtrl_Dist));
	_pt += sizeof(tPARAMITEM_U32);
#endif

	/**����ͷ��ز�������*/
	get_sysparam_u32(_pt, PARAM_VIDEO_QUALITY, ptParam_System->u32PictureQuality);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_BRIGHTNESS, ptParam_System->u32PictureBrightness);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_CONTRASTNESS, ptParam_System->u32PictureContrastness);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_SATURATION, ptParam_System->u32PictureSaturation);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_CHROMA, ptParam_System->u32PictureChroma);
	_pt += sizeof(tPARAMITEM_U32);

	/**����������ز�������*/
	get_sysparam_u32(_pt, PARAM_VEHICLE_KILOMETRES, ptParam_System->u32Kilometers);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u16(_pt, PARAM_VEHICLE_PROVINCE_ID, ptParam_System->u16ProviceId);
	_pt += sizeof(tPARAMITEM_U16);

	get_sysparam_u16(_pt, PARAM_VEHICLE_CITY_ID, ptParam_System->u16CountyId);
	_pt += sizeof(tPARAMITEM_U16);

	get_sysparam_str(_pt, PARAM_VEHICLE_ID, ptParam_System->u16ProviceId);
	_pt += (5 + strlen_(ptParam_System->u16ProviceId));

	get_sysparam_u8(_pt, PARAM_VEHICLE_ID_COLOR, ptParam_System->u8Color);
	_pt += sizeof(tPARAMITEM_U8);

#ifdef JTT808_Ver_2013
	/**����Ϊ��Э�����ӵĲ�����*/
	get_sysparam_u8(_pt, PARAM_GNSS_MODE, ptParam_System->u8GNSS_Mode);
	_pt += sizeof(tPARAMITEM_U8);

	get_sysparam_u8(_pt, PARAM_GNSS_BOUNDRATE, ptParam_System->u8GNSS_Baund);
	_pt += sizeof(tPARAMITEM_U8);

	get_sysparam_u8(_pt, PARAM_GNSS_DMSG_OUTFREQ, ptParam_System->u8GNSS_DetailMsg_OutFreq);
	_pt += sizeof(tPARAMITEM_U8);

	get_sysparam_u32(_pt, PARAM_GNSS_DMSG_RETRIEVEFREQ, ptParam_System->u32GNSS_DetailMsg_RetrieveFreq);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u8(_pt, PARAM_GNSS_DMSG_RPTTYPE, ptParam_System->u8GNSS_DetailMsg_RptType);
	_pt += sizeof(tPARAMITEM_U8);

	get_sysparam_u32(_pt, PARAM_GNSS_DMSG_RPTSETTING, ptParam_System->u32GNSS_DetailMsg_RptSetting);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_CAN_CH1_RETRIEVEINTERVAL, ptParam_System->u32CAN_Ch1_RetrieveInterval);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u16(_pt, PARAM_CAN_CH1_UPLOADINTERVAL, ptParam_System->u16CAN_Ch1_UploadInterval);
	_pt += sizeof(tPARAMITEM_U16);

	get_sysparam_u32(_pt, PARAM_CAN_CH2_RETRIEVEINTERVAL, ptParam_System->u32CAN_Ch2_RetrieveInterval);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u16(_pt, PARAM_CAN_CH2_UPLOADINTERVAL, ptParam_System->u16CAN_Ch2_UploadInterval);
	_pt += sizeof(tPARAMITEM_U16);

	get_sysparam_mem(_pt, PARAM_CAN_SOLORETRIEVE, 8, ptParam_System->u8CAN_SingleRetrieveSetting);
	_pt += (5 + 8);

	/**0x0111-0x01FF BYTE[8] ��������CAN ����ID �����ɼ�����*/
	get_sysparam_mem(_pt, PARAM_CAN_SOLORETRIEVE_OTHER, 8, ptParam_System->u8CAN_SingleRetrieveSetting_Others);
	_pt += (5 + 8);

	/**0xF000-0xFFFF �û��Զ���*/
#endif

	return ((u8) (_pt - pData));
}

/**
 * ���ⲿ������ز������ڴ棬
 * �ǽṹ������������ϵͳ����������ʱ�������豸����������������������Ϣ
 *
 * @param type		��������
 * @return	0=�ɹ������򷵻ش������
 */
int PARAM_LoadSysParam(u8 type) {
	int iRet;
	u8 u8CRC = 0;
	u8 *pParam = NULL;
	u32 u32FrameOffset = 0;
	u16 u16ParamSize = 0;

	Q_ASSERT(type < PARAM_TYPEEND);

	pParam = tParams_System[type].ptParam_St;
	u32FrameOffset = tParams_System[type].u32Offset;
	u16ParamSize = tParams_System[type].u16ParamSize;

	/**���ⲿ����������*/
	ZeroMem((u8*) pParam, u16ParamSize);
	iRet = FRAM_ReadBuffer(u32FrameOffset, pParam, u16ParamSize);
	if (iRet < 0)
		return iRet;

	/**����CRCУ��*/
	u8CRC = *(pParam + u16ParamSize - 1);
	iRet = CheckCRC(u8CRC, (u8*) pParam, u16ParamSize - 1);
	if (0 == iRet)
		return -ERR_EEPROM_CRC;	///У��ʧ��

	return 0;
}

/**
 * ����������ⲿ������
 * �ǽṹ������������ϵͳ����������ʱ�������豸����������������������Ϣ
 *
 * @param type		��������
 * @return		0=�ɹ������򷵻ش������
 */
int PARAM_SaveSysParam(u8 type) {
	int iRet;
	u8 u8CRC = 0;
	u8 _u8CRC = 0;
	u8 *pParam = NULL;
	u32 u32FrameOffset = 0;
	u16 u16ParamSize = 0;

	Q_ASSERT(type < PARAM_TYPEEND);

	pParam = tParams_System[type].ptParam_St;
	u32FrameOffset = tParams_System[type].u32Offset;
	u16ParamSize = tParams_System[type].u16ParamSize;

	/**���CRC�ֶ�*/
	_u8CRC = CalculateCRC8(pParam, u16ParamSize - 1);
	pParam[u16ParamSize - 1] = _u8CRC;

	/**д�������ⲿ�洢��*/
	iRet = FRAM_WriteBuffer(u32FrameOffset, pParam, u16ParamSize);
	if (iRet < 0)
		return iRet;

	/**д���ٶ��� CRCУ��*/
	iRet = FRAM_ReadBufferCRC(u32FrameOffset, u16ParamSize, &u8CRC);
	if (iRet < 0)
		return iRet;

	/**CRCУ���һ���Լ��*/
	if (u8CRC != _u8CRC)
		return -ERR_EEPROM_CRC;	///У��ʧ��

	return 0;
}

/**
 * ��ղ���
 *
 * @param type		��������
 * @return	0=�ɹ������򷵻ش������
 */
int PARAM_EraseSysParam(u8 type) {
	int iRet;

	Q_ASSERT(type < PARAM_TYPEEND);

	iRet = FRAM_EraseBytes(tParams_System[type].u32Offset, tParams_System[type].u16ParamSize);
	if (iRet < 0)
		return iRet;

	ZeroMem((u8*) tParams_System[type].ptParam_St, tParams_System[type].u16ParamSize);
	return 0;
}

/**
 * ���ⲿ������ز������ڴ棬
 * �ǽṹ������������ϵͳ����������ʱ�������豸����������������������Ϣ
 *
 * @param type		��������
 * @return	0=�ɹ������򷵻ش������
 */
int PARAM_LoadFMTParam(u8 type) {
	int iRet;
	u8 u8CRC = 0;
	u8 *pParam = NULL;
	u32 u32FrameOffset = 0;
	u16 u16ParamSize = 0;

	Q_ASSERT(type < PARAM_FMTEND);

	pParam = tConf[type].pStorageAddr;
	u32FrameOffset = tConf[type].u32EEPROMAddr;
	u16ParamSize = tConf[type].u16StorageSize;

	/**���ⲿ����������*/
	ZeroMem((u8*) pParam, u16ParamSize);
	iRet = FRAM_ReadBuffer(u32FrameOffset, pParam, u16ParamSize);
	if (iRet < 0)
		return iRet;

	/**����CRCУ��*/
	u8CRC = *(pParam + u16ParamSize - 1);
	iRet = CheckCRC(u8CRC, (u8*) pParam, u16ParamSize - 1);
	if (0 == iRet)
		return -ERR_EEPROM_CRC;	///У��ʧ��

	return 0;
}

/**
 * ����������ⲿ������
 * �ǽṹ������������ϵͳ����������ʱ�������豸����������������������Ϣ
 *
 * @param type		��������
 * @return		0=�ɹ������򷵻ش������
 */
int PARAM_SaveFMTParam(u8 type) {
	int iRet;
	u8 u8CRC = 0;
	u8 _u8CRC = 0;
	u8 *pParam = NULL;
	u32 u32FrameOffset = 0;
	u16 u16ParamSize = 0;

	Q_ASSERT(type < PARAM_TYPEEND);

	pParam = tConf[type].pStorageAddr;
	u32FrameOffset = tConf[type].u32EEPROMAddr;
	u16ParamSize = tConf[type].u16StorageSize;

	/**���CRC�ֶ�*/
	_u8CRC = CalculateCRC8(pParam, u16ParamSize - 1);
	pParam[u16ParamSize - 1] = _u8CRC;

	/**д�������ⲿ�洢��*/
	iRet = FRAM_WriteBuffer(u32FrameOffset, pParam, u16ParamSize);
	if (iRet < 0)
		return iRet;

	/**д���ٶ��� CRCУ��*/
	iRet = FRAM_ReadBufferCRC(u32FrameOffset, u16ParamSize, &u8CRC);
	if (iRet < 0)
		return iRet;

	/**CRCУ���һ���Լ��*/
	if (u8CRC != _u8CRC)
		return -ERR_EEPROM_CRC;	///У��ʧ��

	return 0;
}

/**
 * �����������
 *
 * @param type		��������
 * @return	0=�ɹ������򷵻ش������
 */
int PARAM_EraseFMTParam(eFMTMSG_PARAM type) {
	int iRet;

	Q_ASSERT(type < PARAM_FMTEND);

	iRet = FRAM_EraseBytes(tConf[type].u32EEPROMAddr, tConf[type].u16StorageSize);
	if (iRet < 0)
		return iRet;

	ZeroMem((u8*) tConf[type].pStorageAddr, tConf[type].u16StorageSize);

	return 0;
}

/**
 * ȡ������������Խṹ������
 *
 * @param type		��������
 * @return	��������
 */
u8 PARAM_GetAmount(eFMTMSG_PARAM type) {
	Q_ASSERT(type < PARAM_FMTEND);

	return ITEM_AMOUNT(type);
}

/**
 * ׷����
 * ע�⣬�������ڴ棬�������������Ϻ�Ӧ���ô洢���������в���д���ⲿ�洢��
 *
 * @param type			��������
 * @param u16ItemLen	���
 * @param pItemMsg		������
 * @return
 */
int PARAM_FormatMsg_ApendItem(eFMTMSG_PARAM type, u16 u16ItemLen, u8 *pItemMsg) {

	u32 u32Address;
	int ret;
	u8 u8CRC, crc_eeprom;
	u8 u8Amount;

	if (type >= PARAM_FMTEND)
		return -ERR_PARAM_INVALID;

	if (ITEM_AMOUNT(type) >= tConf[type].u8ItemAmount)	///�¼����������������
		return -ERR_PARAM_INVALID;

	u32Address = ITEM_OFFSET(type, u8Amount);	///��βλ��

	memcpy_(u32Address, pItemMsg, u16ItemLen);

	return 0;
}

#if 0
/**
 * ׷���ע�����������дEEPROM���ݣ�������ͷδ���棬Ӧ��׷����������Ϻ����ִ��һ�α�������ͷ
 *
 * @param u8EvtId		IN	�¼�ID
 * @param u8EvtLen		IN	�¼�����
 * @param pEvtMsg		IN	�¼���Ϣ
 * @return	0=�ɹ�������������
 */
int PARAM_FormatMsg_ApendItem(eFMTMSG_PARAM type, u8 u8ItemId, u8 u8ItemLen, u8 *pItemMsg) {
	u32 u32Address;
	int ret;
	u8 u8CRC, crc_eeprom;
	u8 u8Amount;

	if (type >= PARAM_FMTEND)
		return -ERR_PARAM_INVALID;

	if (ITEM_AMOUNT(type) >= tConf[type].u8ItemAmount)	///�¼����������������
		return -ERR_PARAM_INVALID;

	u8CRC = CalculateCRC8(pItemMsg, u8ItemLen);

	u8Amount = ITEM_AMOUNT(type);///������
	u32Address = ITEM_OFFSET(type, u8Amount);	///��βλ��

	ret = EEPROM_WriteBuffer(u32Address, pItemMsg, u8ItemLen);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	///�ٶ���У��CRCһ�Σ�ȷ������������ȷ
	ret = EEPROM_ReadBufferCRC(u32Address, u8ItemLen, &crc_eeprom);
	if (ret == -1)
		return -ERR_EEPROM_RW;

	if (u8CRC != crc_eeprom)	///����У�����
		return -ERR_EEPROM_CRC;

	///�ۼ��¼���Ŀ����
	*(tConf[type].pStorageAddr + PREFIX_SIZE) = u8Amount + 1;

	return 0;
}
#endif

/**
 * �滻�¼���ID��ͬ��������滻��Ҳ�ᵼ������ͷ����������Ϣ�����仯�����½�������Ҫ��������ͷһ��
 *
 * @param u8EvtId		IN	�¼�ID
 * @param u8EvtLen		IN	�¼���Ϣ����
 * @param pEvtMsg		IN	�¼���Ϣ����
 * @return	0=�ɹ���-1=ʧ��
 */
int PARAM_FormatMsg_ReplaceItem(u8 type, u8 u8ItemId, u16 u16ItemLen, u8 *pItemMsg) {
	int ret;
	u8 crc_eeprom;
	u8 u8ItemAmount;
	u8 i = 0;
	u32 u32ItemAddress;

	if (type >= PARAM_FMTEND)
		return -ERR_PARAM_INVALID;

	u8ItemAmount = ITEM_AMOUNT(type);	///������

	i = 0;
	while (i < u8ItemAmount) {
		if (ITEM_ID(type, i) == u8ItemId)
			break;
		i++;
	}

	if (i >= u8ItemAmount)	///����ڣ��˳�
		return 0;

	u32ItemAddress = ITEM_OFFSET(type, i);	///��βλ��

	memcpy_(u32ItemAddress, pItemMsg, u16ItemLen);

	return 0;
}

/**
 * ׷����
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

	if (ITEM_AMOUNT(type) >= tConf[type].u8ItemAmount)	///�¼����������������
		return -ERR_PARAM_INVALID;

	u8Amount = ITEM_AMOUNT(type);

	i = 0;
	while (i < u8Amount) {
		if (IS_VALID(type, i)) {
			break;
		}
		i++;
	}

	if (i >= u8Amount) ///û�ҵ�����������
		return 0;

	pItemAddr = ITEM_OFFSET(type, i);		///������
	memcpy_(pItemAddr, pItem, u16ItemLen);

	IS_VALID(type, i) = TRUE;	///������Ч��־

	*(tConf[type].pStorageAddr + 1) = u8Amount + 1;	///�ۼ��¼���Ŀ����

	return 0;
}

/**
 * �޸���
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

	if (ITEM_AMOUNT(type) >= tConf[type].u8ItemAmount)	///�¼����������������
		return -ERR_PARAM_INVALID;

	u8Amount = ITEM_AMOUNT(type);

	i = 0;
	while (i < u8Amount) {
		if (u32Id == ITEM_ID(type, i))
			break;
		i++;
	}

	if (i >= u8Amount) ///û�ҵ�����������
		return 0;

	pItemAddr = ITEM_OFFSET(type, i);		///������
	memcpy_(pItemAddr, pItem, u16ItemLen);

	return 0;

}

/**
 * ɾ����
 *
 * @param type
 * @param u32Id
 * @return
 */
int PARAM_Area_DeleteItem(u8 type, u32 u32Id) {
	u8 u8Amount, i;

	if (ITEM_AMOUNT(type) >= tConf[type].u8ItemAmount)	///�¼����������������
		return -ERR_PARAM_INVALID;

	u8Amount = ITEM_AMOUNT(type);

	i = 0;
	while (i < u8Amount) {
		if (u32Id == ITEM_ID(type, i))
			break;
		i++;
	}

	if (i >= u8Amount) ///û�ҵ�����������
		return 0;

	IS_VALID(type, i) = FALSE;	///������Ч��־

	*(tConf[type].pStorageAddr + 1) = u8Amount - 1;	///�ۼ��¼���Ŀ����

	return 0;
}

