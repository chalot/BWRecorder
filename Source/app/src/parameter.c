/*
 * parameter.c
 *
 * 因系统参数较多，所需存储空间较大，硬件上需外扩一片EEPROM和一块SRAM，
 * 所有参数存放在外部EEPROM中，而运行时从EEPROM载入到外扩SRAM，内部RAM只做堆栈用
 * 数据存储格式：小端
 *
 *  Created on: 2013-12-27
 *      Author: gaozhengdong
 */

#include <stm32f2xx_conf.h>
#include "parameter.h"
#include "utility.h"

static tPARAM_SYSTEM_STORAGE tParam_System_St;
static tPARAM_DEVICE_STORAGE  tParam_Device_St;
static tPARAM_RUNTIME_STORAGE  tParam_Runtime_St;
//static tPARAM_RECTAREA_STORAGE  tParam_RectArea_St;
//static tPARAM_ROUNDAREA_STORAGE  tParam_RoundArea_St;
//static tPARAM_POLYGONAREA_STORAGE  tParam_PolygonArea_St;
static tUPGRADEINFO_STORAGE		tUpgInfo_St;

tPARAM_SYSTEM *ptParam_System = &tParam_System_St.tSystemParam;
tPARAM_DEVICE *ptParam_Device = &tParam_Device_St.tDeviceParam;
tPARAM_RUNTIME *ptParam_Runtime = &tParam_Runtime_St.tRuntimeParam;
//tPARAM_RECTAREA *ptParam_Rect = &tParam_RectArea_St.tRectArea;
//tPARAM_ROUNDAREA *ptParam_RoundArea = &tParam_RoundArea_St.tRoundArea;
//tPARAM_POLYGONAREA *ptParam_PolygonArea = &tParam_PolygonArea_St.tPolygonArea;
tUPGRADEINFO *ptUpgInfo = &tUpgInfo_St.tUpgradeInfo;


#if 1
/**设置系统参数宏，参数类型U8*/
#define set_sysparam_u8(id, len, pVal)	\
										do {	\
											(id = *((u8*)pVal));	\
										}while(0)

/**设置系统参数宏，参数类型U16*/
#define set_sysparam_u16(id, len, pVal)	\
										do {	\
											(id = *((u8*)pVal));	\
											ENDIAN_U16(id);		\
										}while(0)

/**设置系统参数宏，参数类型U32*/
#define set_sysparam_u32(id, len, pVal)	\
										do {	\
											(id = *((u8*)pVal));	\
											ENDIAN_U32(id);		\
										}while(0)

/**设置系统参数宏，字符串型参数*/
#define set_sysparam_str(id, len, pVal)  \
							do {	\
								memcpy_(id, pVal, len);	\
								id[len] = '\0';	\
							}while(0)

/**读取系统参数项，U32型参数*/
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

/**读取系统参数项，U32型参数*/
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

/**读取系统参数项，U32型参数*/
#define get_sysparam_u8(pAdd, id, val)	\
		do	{	\
			tPARAMITEM_U8* pItem;		\
			pItem = (tPARAMITEM_U8*)pAdd;	\
			pItem->u32Id = id;	\
			pItem->u8Len = 1;		\
			pItem->u8Value = val;	\
			ENDIAN_U32(pItem->u32Id);		\
		}while(0);

/**读取系统参数项，STRING型参数*/
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

/**读取系统参数项，数组型参数*/
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
/**设置系统参数宏，参数类型U8*/
static __inline__ void set_sysparam_u8(id, len, pVal)
{
	id = *((u8*)pVal);
}

/**设置系统参数宏，参数类型U16*/
static __inline__ void set_sysparam_u16(id, len, pVal)
{
	id = *((u16*)pVal);
	ENDIAN_U16(id);
}

/**设置系统参数宏，参数类型U32*/
static __inline__ void set_sysparam_u32(id, len, pVal)
{
	id = *((u32*)pVal);
	ENDIAN_U32(id);
}

/**设置系统参数宏，字符串型参数*/
static __inline__ void set_sysparam_str(id, len, pVal)
{
	memcpy_(id, pVal, len);
	id[len] = '\0';
}

/**读取系统参数项，U32型参数*/
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

/**读取系统参数项，U32型参数*/
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

/**读取系统参数项，U32型参数*/
static __inline__ void get_sysparam_u8(pAdd, id, val)
{
	tPARAMITEM_U8* pItem;
	pItem = (tPARAMITEM_U8*)pAdd;
	pItem->u32Id = id;
	pItem->u8Len = 1;
	pItem.u8Value = val;
	ENDIAN_U32(pItem->u32Id);
}

/**读取系统参数项，STRING型参数*/
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

/**读取系统参数项，数组型参数*/
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
 * 系统恢复到出厂设置
 *
 * @return 	int，0=成功，-1=失败
 */
int RecoverSystemDefaultParam(void)
{


	return 0;
}

/**
 * 载入所有系统参数
 *
 * @return	int，0=成功，-1=失败
 */
int Load_CommonParams(void)
{
	u8 i = 0;
	while(i < (u8)PARAM_TYPEEND)
	{
		int ret = Load_Params((ePARAMTYPE)i);
		if(ret < 0) ///任一种类型参数载入失败，返回
			return ret;
		i++;
	}
	return 0;
}

/**
 * 保存所有系统参数
 *
 * @return	int，0=成功，-1=失败
 */
int Save_CommonParams(void)
{

	return 0;
}

/**
 * 读取指定类型参数
 *
 * @param 	eType	参数类型
 * @return	int，0=成功，-1=失败
 */
int Load_Params(ePARAMTYPE eType)
{
	switch(eType)
	{
	case PARAM_TERMINALINFO:///设备信息

		break;
	case PARAM_SYSTEM:///系统信息
		break;
	case PARAM_RUNTIME:///运行信息
		break;
	case PARAM_RECTANGLEAREA:///矩形区域
		break;
	case PARAM_ROUNDAREA:///圆形区域
		break;
	case PARAM_POLYGONAREA:///多边形区域
		break;
	case PARAM_ROUTE:///路线
		break;

	case PARAM_ONDEMANDMSGINFO:///点播消息
		break;

	case PARAM_EVENT:

		break;

	default:
		return -1;
		break;
	}

	return 0;
}

/**
 * 保存指定类型参数
 *
 * @param 	eType 参数类型
 * @return	int，0=成功，-1=失败
 */
int Save_Params(ePARAMTYPE eType)
{


	return 0;
}

/**
 * 取系统参数
 *
 * @param id		[IN]	参数ID
 * @param val		[OUT]	参数值
 * @return
 */
int Get_SystemParam(u32 id, void* val)
{
//	val = (void*)(ADDRESS(id) + 3);
	return 0;
}

/**
 * 设置系统参数
 *
 * @param id	参数类型
 * @param len	参数长度
 * @param pVal	参数值
 * @return	0=成功，-1=失败
 */
int Set_SystemParam(u32 id, u8 len, u8 *pVal)
{
	switch(id)
	{
	case PARAM_ALIVE_INTERVAL:///=	0x0001,	终端心跳发送间隔，单位为s
		set_sysparam_u8(ptParam_System->u32LinkAliveTimeOut, len, pVal);	break;
	case PARAM_TCPACK_TIMEOUT:///=	0x0002,TCP应答超时，单位为s
		set_sysparam_u8(ptParam_System->u32TCPAckTimeout, len, pVal);		break;
	case PARAM_TCP_RESENDTIMES:///=	0x0003,	TCP消息重传次数
		set_sysparam_u8(ptParam_System->u32TCPReSendTimes, len, pVal);		break;
	case PARAM_UDPACK_TIMEOUT:///=	0x0004,	UDP应答超时，单位为s
		set_sysparam_u8(ptParam_System->u32UDPAckTimeout, len, pVal);		break;
	case PARAM_UDP_RESENDTIMES:///=	0x0005,	UDP消息重传次数
		set_sysparam_u8(ptParam_System->u32UDPReSendTimes, len, pVal);		break;
	case PARAM_SMSACK_TIMEOUT:///=	0x0006,	SMS应答超时，单位为s
		set_sysparam_u8(ptParam_System->u32SMSAckTimeout, len, pVal);		break;
	case PARAM_SMS_RESENDTIMES:///=	0x0007,	SMS消息重传次数
		set_sysparam_u8(ptParam_System->u32SMSReSendTimes, len, pVal);		break;
	/**0X0008 ----0X000F 保留*/
	case PARAM_MAINSRV_APN:///	=	0x0010,	主服务器APN，无线通信拨号访问点
		set_sysparam_str(ptParam_System->au8MainServerApn, len, pVal);		break;
	case PARAM_MAINSRV_USR:///	=	0x0011,	主服务器无线通信拨号用户名
		set_sysparam_str(ptParam_System->au8MainServerUsr, len, pVal);		break;
	case PARAM_MAINSRV_PSWD	:///=	0x012,	主服务器无线通信拨号密码
		set_sysparam_str(ptParam_System->au8MainServerPswd, len, pVal);		break;
	case PARAM_MAINSRV_IP	:///=	0x0013,	主服务器地址，IP或域名
		set_sysparam_str(ptParam_System->au8MainServerIP, len, pVal);		break;
	case PARAM_VICESRV_APN	:///=	0x0014,	备份服务器APN，无线通信拨号访问点
		set_sysparam_str(ptParam_System->au8ViceServerApn, len, pVal);		break;
	case PARAM_VICESRV_USR	:///=	0x0015,	备份服务器无线通信拨号用户名
		set_sysparam_str(ptParam_System->au8ViceServerUsr, len, pVal);		break;
	case PARAM_VICESRV_PSWD	:///=	0x0016,	备份服务器无线通信拨号密码
		set_sysparam_str(ptParam_System->au8ViceServerPswd, len, pVal);		break;
	case PARAM_VICESRV_IP:///=	0x0017,	备份服务器地址，IP或域名
		set_sysparam_str(ptParam_System->au8ViceServerIP, len, pVal);		break;
	case PARAM_TCP_PORT:///	=	0x0018,	TCP端口
		set_sysparam_u32(ptParam_System->u32TCPPort, len, pVal);			break;
	case PARAM_UDP_PORT	:///=	0x0019,	UDP端口
		set_sysparam_u32(ptParam_System->u32UDPPort, len, pVal);			break;

/**0X001A ----0X001F 保留*/
#ifdef JTT808_Ver_2013	///新版增加协议部分
	case PARAM_ICVERIFY_MS_IPOrDNS:///=	0x001A,	道路运输证IC卡认证主服务器IP地址或域名
		set_sysparam_str(ptParam_System->IC_Verify_MS_IPOrDNS, len, pVal);	break;
	case PARAM_ICVERIFY_TCPPORT	:///=	0x001B,	道路运输证IC卡认证主服务器TCP端口
		set_sysparam_u32(ptParam_System->IC_Verify_TCP_Port, len, pVal);	break;
	case PARAM_ICVERIFY_UDPPORT	:///=	0x001C,	道路运输证IC卡认证主服务器UDP端口
		set_sysparam_u32(ptParam_System->IC_Verify_UDP_Port, len, pVal);	break;
	case PARAM_ICVERIFY_BS_UDPPORT:///=	0x001D,	道路运输证IC卡备份服务器IP地址或域名，端口同主服务器
		set_sysparam_str(ptParam_System->IC_Verify_BS_IPOrDNS, len, pVal);	break;
#endif///JTT808_Ver_2013

	case PARAM_POSRPT_STRATEGE:///=	0x0020,终端位置汇报策略
		set_sysparam_u32(ptParam_System->u32PosRptStrategy, len, pVal);	break;
	case PARAM_POSRPT_METHOD:///=	0x0021,终端汇报方案
		set_sysparam_u32(ptParam_System->u32PosRptMethod, len, pVal);	break;
	case PARAM_DRV_UNREG_RPTPERIOD:///=	0x0022,	驾驶员未登录汇报时间间隔，单位为秒
		set_sysparam_u32(ptParam_System->u32RptIntervalDrvUnReg, len, pVal);	break;
/**0X0023 ----0X0026 保留*/
	case PARAM_RPTPERIOD_ONSLEEP:///=0x0027,休眠时汇报时间间隔，单位为秒
		set_sysparam_u32(ptParam_System->u32RptIntervalSleep, len, pVal);	break;
	case PARAM_RPTPERIOD_URGENT	:///=0x0028,紧急报警时汇报时间间隔，单位为秒
		set_sysparam_u32(ptParam_System->u32RptIntervalEmergency, len, pVal);	break;
	case PARAM_RPTPERIOD_DEFAULT:///=0x0029,缺省时汇报时间间隔，单位为秒
		set_sysparam_u32(ptParam_System->u32RptIntervalDefault, len, pVal);	break;

/**0X002A ----0X002B 保留*/

	case PARAM_RPTDIST_DEFAULT	:///=	0x002C,	缺省时距离汇报间隔，单位为米
		set_sysparam_u32(ptParam_System->u32RptDistanceDefault, len, pVal);	break;
	case PARAM_RPTDIST_DRV_UNREG:///=	0x002D,	驾驶员未登录距离汇报间隔，单位为米
		set_sysparam_u32(ptParam_System->u32RptDistanceDrvUnReg, len, pVal);	break;
	case PARAM_RPTDIST_SLEEP:///=	0x002E,	休眠时距离汇报间隔，单位为米
		set_sysparam_u32(ptParam_System->u32RptDistanceSleep, len, pVal);	break;
	case PARAM_RPTDIST_URGENT:///=	0x002F,	紧急报警时距离汇报间隔，单位为米
		set_sysparam_u32(ptParam_System->u32RptDistanceEmergency, len, pVal);	break;
	case PARAM_INFLECTION_POINT_ANGLE:///=	0x0030,	拐点补传角度，单位为度(< 180)
		set_sysparam_u32(ptParam_System->u32InflectionSuppleAngle, len, pVal);	break;
	case PARAM_BARRIER_RADIUS:///=	0x031,	电子围栏半径(非法移位阈值)，单位为米
		set_sysparam_u32(ptParam_System->u16BarrierRadius, len, pVal);	break;

/**0X0032----0X003F 保留*/

	case PARAM_MONITORCENTER_PHONENUM:///	=	0x0040,	监控平台电话号码
		set_sysparam_str(ptParam_System->aMonitorPlatformPhone, len, pVal);	break;
	case PARAM_RESET_PHONENUM:///=	0x0041,	复位电话号码
		set_sysparam_str(ptParam_System->aResetPhone, len, pVal);	break;
	case PARAM_RECOVERFACTORY_PHONENUM:///	=	0x0042, 回复出厂设置电话号码
		set_sysparam_str(ptParam_System->aRecoverFactorySettingPhone, len, pVal);	break;
	case PARAM_MONITORCENTER_SMS_NUM:///=	0x0043,	监控平台SMS电话号码
		set_sysparam_str(ptParam_System->aCenterSMSPhone, len, pVal);	break;
	case PARAM_RECVTXTALARM_SMS_NUM	:///=	0x0044,	接收终端SMS文本报警电话号码
		set_sysparam_str(ptParam_System->aRcvTerminalSMSAlarmPhone, len, pVal);	break;

	case PARAM_ANSWERCALLING_STRATEGE:///=	0x0045,///终端电话接听策略
		set_sysparam_u32(ptParam_System->u32PhoneListenStratagy, len, pVal);	break;
	case PARAM_TOUT_PERCALL:///	=	0x0046,///终端每次通话时最长通话时间
		set_sysparam_u32(ptParam_System->u32PhoneConnTimeoutEachTime, len, pVal);	break;
	case PARAM_TOUT_PERMONTHCALL:///=	0x0047,///终端每月最长通话时间
		set_sysparam_u32(ptParam_System->u32PhoneConnTimeoutCurMonth, len, pVal);	break;

	case PARAM_MONITORLISTEN_PHONENUM:///=	0x0048,	监听电话号码
		set_sysparam_str(ptParam_System->aVoiceMonitorPhone, len, pVal);	break;
	case PARAM_SUPERVISE_SMS_NUM:///=	0x0049,	监管平台特权短信号码
		set_sysparam_str(ptParam_System->aMonitorPlatformPriviligPhone, len, pVal);	break;

/**0X004A----0X004F 保留*/

	case PARAM_ALARM_MASK:///=	0x0050,报警屏蔽字
		set_sysparam_u32(ptParam_System->u32AlarmMask, len, pVal);	break;
	case PARAM_SMSCTRL_ONALARM	:///=	0x0051,报警发送文本SMS开关
		set_sysparam_u32(ptParam_System->u32SendSMSOnAlarmCtrl, len, pVal);	break;
	case PARAM_PICCTRL_ONALARM	:///=	0x0052,报警拍摄开关
		set_sysparam_u32(ptParam_System->u32CaptureOnAlarmCtrl, len, pVal);	break;
	case PARAM_PICCTRL_STORE_ONALARM:///=	0x0053,	报警拍摄存储标志
		set_sysparam_u32(ptParam_System->u32AlarmPictureStorageCtrl, len, pVal);	break;
	case PARAM_KEYALARM_FLAG:///=	0x0054,关键标志
		set_sysparam_u32(ptParam_System->u32KeyAlarmMask, len, pVal);	break;
	case PARAM_MAXSPEED	:///=	0x0055,	   最高速度，单位为公里每小时(km/h)
		set_sysparam_u32(ptParam_System->u32SpeedMax, len, pVal);	break;
	case PARAM_OVERSPEED_LASTING_TIMEOUT:///=	0x0056,	超速持续时间，单位为秒
		set_sysparam_u32(ptParam_System->u32OverSpeedContinousTime, len, pVal);	break;
	case PARAM_CONTINUOUS_DRV_TIMEOUT:///=	0x0057,	连续驾驶时间门限，单位为秒
		set_sysparam_u32(ptParam_System->u32ContinousDriveTimeThreshold, len, pVal);	break;
	case PARAM_DRV_TOTAL_EACHDAY_TIMEOUT:///=	0x0058,	当天累计驾驶时间，单位为秒
		set_sysparam_u32(ptParam_System->u32DriveTimesTodayThreshold, len, pVal);	break;
	case PARAM_LEAST_REST_TIMEOUT:///=	0x0059,	最小休息时间，单位为秒
		set_sysparam_u32(ptParam_System->u32MinRestTime, len, pVal);	break;
	case PARAM_MAX_STOPPING_TIMEOUT:///	=	0x005A,	最长停车时间，单位为秒
		set_sysparam_u32(ptParam_System->u32MaxStopTime, len, pVal);	break;

/**0X005B----0X006F */
#ifdef JTT808_Ver_2013
	case PARAM_OVERSPEED_PREALARM_DIF:///=	0x005B,	超速报警预警差值，单位1/10Km/h
		set_sysparam_u16(ptParam_System->u16OverSpeed_PreAlarm_Dif, len, pVal);	break;
	case PARAM_FATIGE_PREALARM_DIF:///=	0x005C,	疲劳驾驶预警差值，单位为秒，>0
		set_sysparam_u16(ptParam_System->u16Fatige_PreAlarm_Dif, len, pVal);	break;
	case PARAM_COLLISION_SETTING:///=0x005D,
		set_sysparam_u16(ptParam_System->u16CollisionSetting, len, pVal);	break;
	case PARAM_SIDECOLLAPSE_SETTING	:///=0x005E,侧翻报警参数设置，侧翻角度，单位1度，默认30度
		set_sysparam_u16(ptParam_System->u16SideCollapseSetting, len, pVal);	break;

/**0X005F----0X0063 */
	case PARAM_PERIODIC_PHOTO_CTRL:///=	0x0064,	定时拍照控制
		memcpy_((u8*)&ptParam_System->tPhotoCtrl_Periodic, len, pVal);
		ENDIAN_PU32(&ptParam_System->tPhotoCtrl_Periodic);	///转成小端
		break;
	case PARAM_PERDIST_PHOTO_CTRL:///=	0x0065,	定距拍照控制
		memcpy_((u8*)&ptParam_System->tPhotoCtrl_Dist, len, pVal);
		ENDIAN_PU32(&ptParam_System->tPhotoCtrl_Dist);	///转成小端
		break;
/**0X0066----0X006F */
#endif///JTT808_Ver_2013

	case PARAM_VIDEO_QUALITY:///=	0x0070,	图片/视频质量，1-10,1最好
		set_sysparam_u32(ptParam_System->u32PictureQuality, len, pVal);	break;
	case PARAM_BRIGHTNESS:///=	0x0071,	亮度，0-255
		set_sysparam_u32(ptParam_System->u32PictureBrightness, len, pVal);	break;
	case PARAM_CONTRASTNESS:///=	0x0072,	对比度，0-127
		set_sysparam_u32(ptParam_System->u32PictureContrastness, len, pVal);	break;
	case PARAM_SATURATION:///=	0x0073,	饱和度，0-127
		set_sysparam_u32(ptParam_System->u32PictureSaturation, len, pVal);	break;
	case PARAM_CHROMA:///=	0x0074,	色度，0-255
		set_sysparam_u32(ptParam_System->u32PictureChroma, len, pVal);	break;

/**0X0075----0X007F*/

	case PARAM_VEHICLE_KILOMETRES:///=	0x0080,	车辆里程表读数，1/10km
		set_sysparam_u32(ptParam_System->u32Kilometers, len, pVal);	break;
	case PARAM_VEHICLE_PROVINCE_ID:///=	0x0081,	车辆所在地省域ID
		set_sysparam_u16(ptParam_System->u16ProviceId, len, pVal);	break;
	case PARAM_VEHICLE_CITY_ID:///=	0x0082,	车辆所在地市域ID
		set_sysparam_u16(ptParam_System->u16CountyId, len, pVal);	break;
	case PARAM_VEHICLE_ID :///=	0x0083,	车牌号码
		set_sysparam_str(ptParam_System->au8Plate, len, pVal);	break;
	case PARAM_VEHICLE_ID_COLOR	:///=	0x0084,	车牌颜色
		set_sysparam_u8(ptParam_System->u8Color, len, pVal);	break;

#ifdef JTT808_Ver_2013
	case PARAM_GNSS_MODE:///0x0090 GNSS 定位模式
		set_sysparam_u8(ptParam_System->u8GNSS_Mode, len, pVal);	break;
	case PARAM_GNSS_BOUNDRATE:///	0x0091 GNSS 波特率
		set_sysparam_u8(ptParam_System->u8GNSS_Baund, len, pVal);	break;
	case PARAM_GNSS_DMSG_OUTFREQ:///	0x0092 GNSS 模块详细定位数据输出频率
		set_sysparam_u8(ptParam_System->u8GNSS_DetailMsg_OutFreq, len, pVal);	break;
	case PARAM_GNSS_DMSG_RETRIEVEFREQ:///	0x0093 GNSS 模块详细定位数据采集频率
		set_sysparam_u32(ptParam_System->u32GNSS_DetailMsg_RetrieveFreq, len, pVal);	break;
	case PARAM_GNSS_DMSG_RPTTYPE:///	0x0094 GNSS 模块详细定位数据上传方式：
		set_sysparam_u8(ptParam_System->u8GNSS_DetailMsg_RptType, len, pVal);	break;
	case PARAM_GNSS_DMSG_RPTSETTING:///	0x0095 GNSS 模块详细定位数据上传设置
		set_sysparam_u32(ptParam_System->u32GNSS_DetailMsg_RptSetting, len, pVal);	break;
	case PARAM_CAN_CH1_RETRIEVEINTERVAL:///	0x0100 总线通道1 采集时间间隔(ms)
		set_sysparam_u32(ptParam_System->u32CAN_Ch1_RetrieveInterval, len, pVal);	break;
	case PARAM_CAN_CH1_UPLOADINTERVAL:///	0x0101 总线通道1 上传时间间隔
		set_sysparam_u16(ptParam_System->u16CAN_Ch1_UploadInterval, len, pVal);	break;
	case PARAM_CAN_CH2_RETRIEVEINTERVAL:///	0x0102 CAN 总线通道2 采集时间间隔
		set_sysparam_u32(ptParam_System->u32CAN_Ch2_RetrieveInterval, len, pVal);	break;
	case PARAM_CAN_CH2_UPLOADINTERVAL:///	0x0103 CAN 总线通道2 上传时间间隔
		set_sysparam_u16(ptParam_System->u16CAN_Ch2_UploadInterval, len, pVal);	break;
	case PARAM_CAN_SOLORETRIEVE:///	0x0110 CAN 总线ID 单独采集设置
		set_sysparam_u8(ptParam_System->u8GNSS_Mode, len, pVal);	break;
#endif
	default:
		return -1;
	}

	return 0;
}

/**
 * 取系统参数项，一次性填充所有参数字段
 *
 * @param _pt		缓区地址
 * @return	参数项长度
 */
u16 GetItem_AllSystemParam(u8 *pData)
{
	u8 *_pt = pData;

/**通信参数*/
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

/**主服务器参数*/
	get_sysparam_str(_pt, PARAM_MAINSRV_APN, ptParam_System->au8MainServerApn);
	_pt += (5 + strlen_(ptParam_System->au8MainServerApn));

	get_sysparam_str(_pt, PARAM_MAINSRV_USR, ptParam_System->au8MainServerUsr);
	_pt += (5 + strlen_(ptParam_System->au8MainServerUsr));

	get_sysparam_str(_pt, PARAM_MAINSRV_PSWD, ptParam_System->au8MainServerPswd);
	_pt += (5 + strlen_(ptParam_System->au8MainServerPswd));

	get_sysparam_str(_pt, PARAM_MAINSRV_IP, ptParam_System->au8MainServerIP);
	_pt += (5 + strlen_(ptParam_System->au8MainServerIP));

/**备份服务器参数*/
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

/**位置汇报参数*/
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

/**电话参数*/
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

/**报警参数*/
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

/**超速监测参数*/
	get_sysparam_u32(_pt, PARAM_MAXSPEED, ptParam_System->u32SpeedMax);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_OVERSPEED_LASTING_TIMEOUT,
						ptParam_System->u32OverSpeedContinousTime);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt,PARAM_CONTINUOUS_DRV_TIMEOUT,
						ptParam_System->u32ContinousDriveTimeThreshold);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_DRV_TOTAL_EACHDAY_TIMEOUT,
							ptParam_System->u32DriveTimesTodayThreshold);
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
	get_sysparam_u32(_pt, PARAM_PERIODIC_PHOTO_CTRL, *((u32*)&ptParam_System->tPhotoCtrl_Periodic));
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt, PARAM_PERDIST_PHOTO_CTRL, *((u32*)&ptParam_System->tPhotoCtrl_Dist));
	_pt += sizeof(tPARAMITEM_U32);
#endif

/**摄像头相关参数设置*/
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

/**车辆属性相关参数设置*/
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
/**以下为新协议增加的参数项*/
	get_sysparam_u8(_pt, PARAM_GNSS_MODE, ptParam_System->u8GNSS_Mode);
	_pt += sizeof(tPARAMITEM_U8);

	get_sysparam_u8(_pt, PARAM_GNSS_BOUNDRATE, ptParam_System->u8GNSS_Baund);
	_pt += sizeof(tPARAMITEM_U8);

	get_sysparam_u8(_pt,
			PARAM_GNSS_DMSG_OUTFREQ, ptParam_System->u8GNSS_DetailMsg_OutFreq);
	_pt += sizeof(tPARAMITEM_U8);

	get_sysparam_u32(_pt,
			PARAM_GNSS_DMSG_RETRIEVEFREQ, ptParam_System->u32GNSS_DetailMsg_RetrieveFreq);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u8(_pt,
			PARAM_GNSS_DMSG_RPTTYPE, ptParam_System->u8GNSS_DetailMsg_RptType);
	_pt += sizeof(tPARAMITEM_U8);

	get_sysparam_u32(_pt,
			PARAM_GNSS_DMSG_RPTSETTING, ptParam_System->u32GNSS_DetailMsg_RptSetting);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u32(_pt,
			PARAM_CAN_CH1_RETRIEVEINTERVAL, ptParam_System->u32CAN_Ch1_RetrieveInterval);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u16(_pt,
			PARAM_CAN_CH1_UPLOADINTERVAL, ptParam_System->u16CAN_Ch1_UploadInterval);
	_pt += sizeof(tPARAMITEM_U16);

	get_sysparam_u32(_pt,
			PARAM_CAN_CH2_RETRIEVEINTERVAL, ptParam_System->u32CAN_Ch2_RetrieveInterval);
	_pt += sizeof(tPARAMITEM_U32);

	get_sysparam_u16(_pt,
			PARAM_CAN_CH2_UPLOADINTERVAL, ptParam_System->u16CAN_Ch2_UploadInterval);
	_pt += sizeof(tPARAMITEM_U16);

	get_sysparam_mem(_pt,
			PARAM_CAN_SOLORETRIEVE, 8, ptParam_System->u8CAN_SingleRetrieveSetting);
	_pt += (5 + 8);

	/**0x0111-0x01FF BYTE[8] 用于其他CAN 总线ID 单独采集设置*/
	get_sysparam_mem(_pt,
			PARAM_CAN_SOLORETRIEVE_OTHER, 8, ptParam_System->u8CAN_SingleRetrieveSetting_Others);
	_pt += (5 + 8);

	/**0xF000-0xFFFF 用户自定义*/
#endif

	return ((u8)(_pt - pData));
}























