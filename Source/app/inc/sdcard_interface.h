/**   
* @Title: sdcard.h  
* @Description: TODO(用一句话描述该文件做什么) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-5-27 上午11:28:38 
* @version V1.0   
*/ 


#ifndef SDCARD_H_
#define SDCARD_H_

#include "type.h"
#include "mempool.h"
#include "ff.h"
#include "systimer.h"
#include "gps.h"

#pragma pack(1)


typedef enum{
	eLOG_Speed = 0,
	eLOG_Accident, //事故疑点
	eLOG_OvertimeDrive, // 超时驾驶
	eLOG_PositionInfo, //位置信息
	eLOG_DriverInfo, //驾驶人身份
	eLOG_Distance, //里程
	eLOG_InstallParam, //安装参数记录
	eLOG_Power, //外部供电记录
	eLOG_ParamChange, //参数修改记录
	eLOG_SpeedState, //速度状态日志

} eLOGCLASS;

#if 0

/*CAN帧存储格式，在原始帧信息基础上添加了必要的字段（dir方向，tick时刻），便于存储*/
typedef struct _CANRAWFRAME
{
	u32 id;				/*ID*/
	u8	dir		:	1;	/*收发方向，0：Tx，1：Rx*/
	u8	len		:	7;	/*帧长度*/
	u8  data[8];		/*帧数据*/
	u32	tick;			/*收发时刻，单位0.1ms。发送时，填充实际发送时刻值；接收时，填充实际接收时刻值*/
} CANRAWFRAME;


#endif

//GPS存储信息
typedef	struct
{
	u8		bInfoValid			:	4;		//定位有效否，1=有效，0=无效
	u8      bAntenaState		:	4;		//天线状态，1=拔出，0=正常
	u8		Latitude[4];			//纬度，单位=10^-3分，压缩BCD码
	u8		Longtitude[4];			//经度，单位=10^-3分，压缩BCD码
	u8		Altitude[2];			//海拔高度，单位=米，压缩BCD码
	u8		Speed[2];				//速度，单位=10米/时，压缩BCD码
	u8		Direction[2];				//航向，单位=分，压缩BCD码
	u8		u8ValidSatellites;		//正在使用卫星数
	u8		u8VisibleSatellites;	//可见卫星数
//	u16		u16Year;				//当前取得的GPS信息-年
//	u8		u8Month;				//当前取得的GPS信息-月
//	u8		u8Day;					//当前取得的GPS信息-日
//	u8		u8Hour;					//当前取得的GPS信息-时
//	u8		u8Minute;				//当前取得的GPS信息-分
//	u8		u8Second;				//当前取得的GPS信息-秒

}	_tGPSSaveInfo;/*T_GPSINFO;*/


/*CAN帧记录格式*/
//typedef struct _CAN_DUMPRECORD
//{
//	u32 	tick;	//时刻偏移
//	CANRAWFRAME info;	//帧信息
//} T_CAN_DUMPRECORD;

#define T_CAN_DUMPRECORD	CANRAWFRAME

/*GPS记录格式*/
typedef struct _GPS_DUMPRECORD
{
	u32 	tick;	//时刻偏移
//	T_GPSINFO info;	//帧信息
} T_GPS_DUMPRECORD;

typedef union _DUMPRECORD
{
	uint8_t eCAN[sizeof(T_CAN_DUMPRECORD)];
	uint8_t eGPS[sizeof(T_GPS_DUMPRECORD)];
}T_DUMPRECORD;

#define SESSION_FILE_LEN	7		///会话中的文件长度，格式：年_月_日
#define SESSION_FILELIST_LEN	100	///会话中的文件列表总数

/*查询会话*/
typedef struct _QSESSION
{
	u8		IsActive;		///是否活动状态，1=被占用，0=空闲
	u8 		u8SessionType;	///查询类型
	u8		u8Cmd;			//查询命令
	FIL		file;  			//打开文件句柄
//	char 	fn_Start[SESSION_FILE_LEN];	//第一条记录所在文件名
	u32 	u32Offset_Start;///第一条记录在文件中的偏移量
//	char 	fn_End[SESSION_FILE_LEN];		//最后一条记录所在文件名
	u32 	u32Offset_End;	///最后一条记录在文件中的偏移量
//	char 	fn_Cur[SESSION_FILE_LEN];		//当前打开文件名
	u32		u32Offset_Cur; 	//当前读取偏离位置
	char 	aFileList[100][7];	///符合搜索条件的文件列表
	u8		u8FileAmt;		///文件总数
	u8		u8FileStart;		///第一条记录文件在文件列表中的索引号
	u8		u8FileEnd;			///最后一条记录文件在文件列表中的索引号
	u8		u8FileCur;		///当前打开文件在文件列表中的索引号
	TIME 	t_Start;		//查询参数开始时间
	TIME 	t_End;			//查询参数结束时间
	TIME 	t_Cur;			//当前查找时间
	u8 		bAllBlocksFinishedReadout;///所有数据读取完毕标志
	u16 	u16BlocksToTrans;	///待传输数据总块数
	u16		u16BlocksTransed;	///已待传输数据总块数

} T_QSESSION;

/*写会话*/
//typedef struct _WSESSION
//{
//	FIL	*file;  	//文件句柄
//	char fn_cur[20];	//当前查询文件名
//
//} T_WSESSION;

/**
 * @Func	SD卡初始化
 * @Param	无
 * @Ret		0=成功，否则为错误代码(RC)
 */
//int SDCard_Init();

/**
 * @Func	存储原始帧
 * @Param	pFrame			[in]	帧数据
 * @Ret		0=成功，否则为错误代码(RC)
 */
//int SD_DumpFrame(CANRAWFRAME* pFrame);

/**
 * @Func 	在SD卡中根据起始时间和接收时间查找历史记录，精确到秒
 *			提交一次查询请求对应创建一个新的查询索引号，根据此索引号区别不同的查询请求，和定位数据段；
 *			一般同一时间只能存在一个有效查询索引
 * @Param	tick_Begin		[in]	起始时间
 * 			tick_End		[in]	结束时间
 * 			rId				[in]	有效查询索引号
 * @Ret		0=成功；否则失败，返回错误代码（RC）
  */
//int SD_StartNewQuery(eLOGCLASS class, TIME *tick_Begin, TIME *tick_End/*, u8 *rId*/);

/**
 * @Func 	读取帧块数据，根据查询索引ID来唯一确定查询请求，可能对同一索引ID执行多次查询请求，这发生
 * 			在数据量大需要分块读取的情况下。
 * @Param	rId				[in]	查询索引号
 * 			pMsgBuf			[in]	接收缓区
 * 			u16MsgBufSize	[in]	接收缓区长度
 * @Ret		u16，实际读取字节数，=0说明读取结束
 */
//u16 SD_RetrieveFrameBlock(eLOGCLASS class, u8 *pMsgBuf, u16 u16MsgBufSize);

/**
 * @Func	通知车辆启动或开始充电状态，依据协议要求，视为开始一个新的驾驶循环或充电循环，
 * 			需创建一个新的存储文件保存CAN帧数据
 * @Param	time			[in]	当前时刻，可以作为文件名命名依据
 * @Ret		无
 */
//void SD_NotifyDriveOrChargeSessionStart(TIME *time);

/**
 * @Func	通知车辆停车或结束充电状态，依据协议要求，视为一个驾驶循环或充电循环结束，
 * 			终止文件存储
 * @Param	time			[in]	当前时刻
 * @Ret		无
 */
//void SD_NotifyDriveOrChargeSessionTerminal(TIME *time);

/**
 * @Func	存储GPS数据
 * @Param	pGpsInfo			[in]	定位数据
 * @Ret		0=成功，否则为错误代码(RC)
 */
//int SD_DumpGpsInfo(_tGPSSaveInfo* pGpsInfo);
//int SD_DumpGpsInfo(_tGPSInfo* pGpsInfo);

/**
 * @Func	请求存储原始帧，注意在接收到一帧后，应立即调用该过程
 * @Param	pNode			[in]	节点
 * @Ret		无
 */
//void SD_RequestDumpFrame(DATANODE* pNode);

/**
 * @Func	存储原始帧
 * @Param	pFrame			[in]	帧数据
 * @Ret		0=成功，否则为错误代码(RC)
 */
//void SD_DoDumpFrame();

/**
 * @Func	返回SD卡空闲容量
 * @Param	free		[in]	空闲容量大小，单位KB
 * @Ret		0=成功，否则错误代码
 */
int SD_GetFree(u32 *free);

/**
 * @Func	返回SD卡总容量
 * @Param	free		[in]	空闲容量大小，单位KB
 * @Ret		0=成功，否则错误代码
 */
int SDCard_Getvolume(u32 *volume);

BOOL SDCard_Clear();
u8 SDCard_GetExistedState();//SD卡拔插状态
u8 SDCard_GetStorageState();//SD卡存储状态
u8 SDCard_GetRWState();//SD卡读写状态


#pragma pack()

#endif /* SDCARD_H_ */
