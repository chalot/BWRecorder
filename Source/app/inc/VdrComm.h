#ifndef	__GPS_VDR_COMM_H__
#define	__GPS_VDR_COMM_H__

#include <stm32f2xx_conf.h>
#include <type.h>
#include <board.h>

#pragma 	pack(1)

#define UART_VDR	USART6


#define    	VDR_Send_Enable()			{USART_ITConfig(UART_VDR, USART_IT_TXE,ENABLE);}
#define		VDR_Send_Disable()			{USART_ITConfig(UART_VDR, USART_IT_TXE,DISABLE);}
#define    	VDR_Recv_Enable()			{USART_ITConfig(UART_VDR, USART_IT_RXNE,ENABLE);}
#define		VDR_Recv_Disable()			{USART_ITConfig(UART_VDR, USART_IT_RXNE,DISABLE);}


#define		VDR_VINCODE_LEN				17					//VIN码长度
#define		VDR_CARDNUM_LEN				12					//驾驶证号号码长度
#define		VDR_CARDSPECIFY_LEN			12					//车牌分类长度
#define		VDR_DRVCODE_LEN				3					//驾驶员代码长度
#define		VDR_DRVNUM_LEN				18					//驾驶证号码长度
#define		VDR_MAX_DRVIE_FATIGUE_NUM	15					//驾驶超时记录次数最大值
#define		VDR_MAX_DRIVERCNT			5					//记录仪记录的司机数目


//超时定义
#define		VDR_SERIAL_CHAR_TIMEOUT			5				//50ms字节接收间隔超时
#define		VDR_PACKAGE_HANDLE_TIMEOUT		100				// 1s数据包处理超时

//数据字头定义
#define		_PC2VDR_COMM_FRAME_HEAD1		0xAA				//通讯机发送到记录仪起始字头1
#define		_PC2VDR_COMM_FRAME_HEAD2		0x75				//通讯机发送到记录仪起始字头2

#define		_VDR2PC_COMM_FRAME_HEAD1		0x55				//记录仪发送到通讯机起始字头1
#define		_VDR2PC_COMM_FRAME_HEAD2		0x7A				//记录仪发送到通讯机起始字头2

//采集数据命令接收出错时应答出错标志字
#define		_VDR2PC_COMM_GET_ERR_TAG			0xFA

//设置参数命令数据接收出错时应答出错标志字
#define		_PC2VDR_COMM_SET_ERR_TAG			0xFB

typedef struct
{
	u8		u8Head1;
	u8		u8Head2;
	u8		u8Cmd;
	u16		u16DataLength;
	u8		u8Reserved;
}tFRAMEHEAD;

//通讯机与记录仪通讯的数据帧格式
typedef struct
{
	u8		u8Head1;
	u8		u8Head2;
	u8		u8Cmd;
	u16		u16DataLength;
	u8		u8Reserved;
	u8		*pu8Data;
	u8		u8Checksum;
}tFRAME;

//错误应答帧格式定义
typedef struct
{
	u8		u8Head1;
	u8		u8Head2;
	u8		u8ErrTag;
	u8		u8Reserved;
	u8		u8Checksum;
}tACK_ERRFRAME;


//采集数据命令字定义
//#define		VDR_CMD_GET_VERSION					0x00	//采集记录仪版本
//#define		VDR_CMD_GET_DRIVERINFO				0x01	///驾驶人信息
//#define		VDR_CMD_GET_RTC						0x02	///时间
//#define		VDR_CMD_GET_DISTANCE				0x03	///里程
//#define		VDR_CMD_GET_PULSE					0x04	///脉冲系数
//#define		VDR_CMD_GET_CARINFO					0x05	///车辆信息
//#define		VDR_CMD_GET_STATECONFIG_INFO		0x06	///状态信号配置信息
//#define		VDR_CMD_GET_ID						0x07	///唯一性ID
//#define		VDR_CMD_GET_SPEED					0x08	///行驶速度
//#define		VDR_CMD_GET_POSITION				0x09 	///位置
//#define		VDR_CMD_GET_ACCIDENT				0x10	///事故疑点
//#define		VDR_CMD_GET_OVERTIME_DRIVING		0x11	///超时驾驶
//#define		VDR_CMD_GET_DRIVER_LOG_INFO			0x12	///登入签出
//#define		VDR_CMD_GET_POWER					0x13	///供电
//#define		VDR_CMD_GET_PARAM_CHANGE			0x14	///参数修改
//#define		VDR_CMD_GET_SPEED_STATUS_LOG		0x15	///速度状态日志

#define VDR_CMD_GET_ERR_ACK		0xAA	///出错命令字，自定义
#define VDR_CMD_SET_ERR_ACK		0xBB	///出错命令字，自定义

/**
 * 命令及应答格式
 */
#define		VDR_CMD_GET_VERSION					0x00	//采集记录仪版本
typedef struct {
	u8	u8Version;	///BCD
	u8	u8FixID;
} tVERSION;		///表A.6

#define tACK_VERSION tVERSION	///应答帧

#define		VDR_CMD_GET_DRIVERINFO				0x01	///驾驶人信息
typedef struct {
	u8	aDriverLicense[18];
} tDRIVERLICENSEINFO;	///表A.7

#define tACK_DRIVERLICENSEINFO tDRIVERLICENSEINFO		///应答帧

/*时间日期格式*/
typedef struct {///BCD码
	u8	u8Year;
	u8	u8Month;
	u8	u8Day;
	u8	u8Hour;
	u8	u8Minute;
	u8	u8Second;
}tTIME;		///表A.8

#define		VDR_CMD_GET_RTC						0x02	///时间
typedef struct {
	tTIME	time;
}tRTC;

#define tACK_RTC	tRTC	///应答帧

#define		VDR_CMD_GET_DISTANCE				0x03	///里程
typedef struct {
	tTIME	time_RTC;
	tTIME	time_First;
	u8		aOriginDist[4];
	u8		aTotalDist[4];
}tINITDISTANCE;		///表A.9
#define tACK_INITDISTANCE	tINITDISTANCE	///应答帧

#define		VDR_CMD_GET_PULSE					0x04	///脉冲系数
typedef struct {
	tTIME	time_RTC;
	u16		u16Pulse;
}tPULSE;	///表A.10
#define tACK_PULSE	tPULSE	///应答帧


#define		VDR_CMD_GET_CARINFO					0x05	///车辆信息
typedef struct {
	u8	aIdentification[17];
	u8	aLicense[12];
	u8	aClass[12];
}tCARINFO;	///表A.11
#define tACK_CARINFO	tCARINFO	///应答帧

#define		VDR_CMD_GET_STATECONFIG_INFO		0x06	///状态信号配置信息
typedef struct {
	u8		aD0[10];
	u8		aD1[10];
	u8		aD2[10];
	u8		aD3[10];
	u8		aD4[10];
	u8		aD5[10];
	u8		aD6[10];
	u8		aD7[10];
} tSTATECONFIG;	///表A.13

typedef struct {
	tTIME	time_RTC;
	u8		u8Bytes;
	tSTATECONFIG	tStateConf;
} tACK_STATECONFIG;	///表A.12


#define		VDR_CMD_GET_ID						0x07	///唯一性ID
typedef struct {
	u8		aCCC_Code[7];
	u8		aProductType[16];
	u8		u8Year;
	u8		u8Month;
	u8		u8Day;
	u32		u32SequenceNum;
	u8		u8Reserved[5];
}tVDRID;	///表A.14
#define tACK_UNIQID	tVDRID

#define		VDR_CMD_GET_SPEED					0x08	///行驶速度
/*命令*/
typedef struct {
	tTIME	time_Begin;
	tTIME	time_End;
	u16		u16PackAmnt;
}tCMD_SPEED;

typedef struct {
	u8	u8AvrSpeed;
	u8	u8Status;
} tITEM_SPEED;
/*应答*/
typedef struct {
	tTIME	time_Start;
	tITEM_SPEED atItem[60];
}tACK_SPEED;	///表A.17


#define		VDR_CMD_GET_POSITION				0x09 	///位置
/*命令帧格式同采集速度*/
#define 	tCMD_POSITION	tCMD_SPEED

typedef struct {
	u32		u32Long;
	u32		u32Lat;
	u16		u16Height;
}tPOSITION;	///表A.20

typedef struct {
	tPOSITION	tPos;
	u8		u8AvrSpeed;
}tITEM_POS;

typedef struct {
	tTIME	time_Start;
	tITEM_POS	atItem[60];
}tACK_POS;	///表A.19

#define		VDR_CMD_GET_ACCIDENT				0x10	///事故疑点
/*命令帧格式同采集速度*/
#define 	tCMD_ACCIDENT	tCMD_SPEED

typedef struct {
	u8	u8Speed;
	u8	u8Status;
}tITEM_ACCIDENT;

typedef struct {
	tTIME	time_End;
	u8		aLicense[18];
	tITEM_ACCIDENT	atItem[100];
	tPOSITION	tLastValidPos;
}tACK_ACCIDENT;		///表A.22

#define		VDR_CMD_GET_OVERTIME_DRIVING		0x11	///超时驾驶
/*命令帧格式同采集速度*/
#define 	tCMD_OVERTIME_DRIVING	tCMD_SPEED

typedef struct {
	u8		aLicense[18];
	tTIME	time_Begin;
	tTIME	time_End;
	tPOSITION	tPos_Begin;
	tPOSITION	tPos_End;
}tACK_OVERTIMEDRIVING;	///表A.24


#define		VDR_CMD_GET_DRIVER_LOG_INFO			0x12	///登入签出

typedef struct {
	tTIME	time;
	u8		aLicense[18];
	u8		u8EvtType;	///01:登陆；02：退出
}tACK_DRIVERLOG;	///表A.26


#define		VDR_CMD_GET_POWER					0x13	///供电
/*命令帧格式同采集速度*/
#define 	tCMD_POWER	tCMD_SPEED

typedef struct {
	tTIME	time;
	u8		u8EvtType;	///01:通电；02：断电
}tACK_POWER;	///表A.28

#define		VDR_CMD_GET_PARAM_CHANGE			0x14	///参数修改
/*命令帧格式同采集速度*/
#define 	tCMD_PARAM_CHANGE	tCMD_SPEED

typedef struct {
	tTIME	time;
	u8		u8EvtType;	///为设置参数命令字
}tACK_PARAMCHANGE;		///表A.30

#define		VDR_CMD_GET_SPEED_STATUS_LOG		0x15	///速度状态日志
/*命令帧格式同采集速度*/
#define 	tCMD_SPEED_STATUS_LOG	tCMD_SPEED

typedef struct {
	u8		u8Speed_Log;
	u8		u8Speed_Ref;
}tITEM_SPEEDSTATE;

typedef struct {
	u8		u8SpeedState;	///01：正常，02：异常
	tTIME	time_Begin;
	tTIME	time_End;
	tITEM_SPEEDSTATE	tItem[60];	///从开始时间后1分钟内，每秒记录一次
}tACK_SPEEDSTATUS;		///表A.32


/**
 * 设置参数类
 */
#define VDR_CMD_SET_VECHICLE_INFO		0x82	///车辆信息，A.11
/*命令帧，无应答帧*/
#define tCMD_SET_VECHICLE_INFO 	tACK_CARINFO

#define VDR_CMD_SET_FIRST_INSTLL_DATA	0x83	///初次安装日期	A.8
/*命令帧，无应答帧*/
#define tCMD_SET_INSTALL_DATA	tTIME

#define VDR_CMD_SET_STATUS_CONFIG_INFO	0x84	///状态量配置信息	A.13
/*命令帧，无应答帧*/
#define tCMD_SET_STAUS_CONFIG tITEM_STATECONFIG

#define VDR_CMD_SET_TIME				0xC2	///时间	A.8
/*命令帧，无应答帧*/
#define tCMD_SET_TIME	tTIME

#define VDR_CMD_SET_PULSE				0xC3	///脉冲系数		A.10
/*命令帧，无应答帧*/
#define tCMD_SET_PULSE	tACK_PULSE

#define VDR_CMD_SET_INITIALDIST			0xC4	///初始里程		A.9
/*命令帧，无应答帧*/
#define tCMD_SET_INITIALDIST	tACK_DISTANCE


/**
 * 检定命令字及数据块格式
 */
#define tCMD_DIAG_ENTER_KEEP			0xE0	///进入检定状态
/*命令帧空，应答帧空*/

#define tCMD_DIAG_DIST_ERR_MEASURE		0xE1	///里程误差测量
/*命令帧空，应答帧*/
typedef struct {
	u8	aUniqID[35];	///唯一ID
	u16 u16Pulse;	///脉冲系数
	u16 u16Speed;	///当前速度
	u32	u32CheckedDist;	///检定里程
	u8 	u8State;	///状态信号
}tACK_DIAG_DIST_CHECK;

#define tCMD_DIAG_PULSE_ERR_MEASURE		0xE2	///脉冲系数误差测量
/*命令帧空，应答帧空*/

#define tCMD_DIAG_RTC_ERR_MEASURE		0xE3	///时间误差测量
/*命令帧空，应答帧空*/

#define tCMD_DIAG_RETURN_NORMAL			0xE4	///返回正常工作状态
/*命令帧空，应答帧空*/

/*记录仪全局参数*/

/*参数类型*/
typedef enum {
	VDR_PARAM_CARINFO = 0,
	VDR_PARAM_INSTALLTIME,
	VDR_PARAM_STATECONFIG,
	VDR_PARAM_RTC,
	VDR_PARAM_PULSE,
	VDR_PARAM_INITDIST,
	VDR_PARAM_LICENSE,
	VDR_PARAM_ID,
	VDR_PARAM_END,
} eVDRPARAM;


typedef struct {
	tCARINFO 	tCarInfo;
	u8 			u8CRC;
}tCARINFO_ST;

typedef struct {
	tTIME 	time;
	u8 		u8CRC;
}tTIME_ST;

typedef struct {
	tSTATECONFIG 	tStateConf;
	u8 				u8CRC;
}tSTATECONFIG_ST;

typedef struct {
	tPULSE		tPulse;
	u8 			u8CRC;
}tPULSE_ST;

typedef struct {
	tINITDISTANCE 	tInitDist;
	u8 				u8CRC;
}tINITDISTANCE_ST;

typedef struct {
	u8 aLicense[18];	///驾驶证号码
	u8 u8CRC;
}tLICENSE_ST;

typedef struct {
	tVDRID 	tID;		///记录仪唯一ID
	u8 		u8CRC;
}tUNIQID_ST;


/**
 * 记录仪全局参数，注意，参数保存在铁电中
 */
typedef struct {
	tCARINFO_ST			tCarInfo_St;		///车辆信息	A.11
	tTIME_ST 			tInstData_St;	///安装日期 表A.8
	tSTATECONFIG_ST 	tStateConf_St;		///状态量配置信息  表A.13
	tTIME_ST 			tRTC_St;		///记录仪时间 表A.8
	tPULSE_ST			tPulse_St;			///脉冲系数	 表A.10
	tINITDISTANCE_ST	tInitDist_St;		///起始里程  表A.9
	tLICENSE_ST			tLicense_St;	///驾驶证号码
	tUNIQID_ST			tID_St;			///记录仪唯一ID

} tVDRPARAM_ST;


/*记录仪各参数在铁电中的存储偏移地址*/
#define FRAM_ADDR_VDR_PARAM_START		0x3333	///记录仪参数起始偏移地址
#define FRAM_ADDR_VDR_PARAM_CARINFO		(FRAM_ADDR_VDR_PARAM_START)
#define FRAM_ADDR_VDR_PARAM_INSTALLTIME	(FRAM_ADDR_VDR_PARAM_CARINFO + sizeof(tCARINFO_ST))
#define FRAM_ADDR_VDR_PARAM_STATECONFIG	(FRAM_ADDR_VDR_PARAM_INSTALLTIME + sizeof(tTIME_ST))
#define FRAM_ADDR_VDR_PARAM_RTC			(FRAM_ADDR_VDR_PARAM_STATECONFIG + sizeof(tSTATECONFIG_ST))
#define FRAM_ADDR_VDR_PARAM_PULSE		(FRAM_ADDR_VDR_PARAM_RTC + sizeof(tTIME_ST))
#define FRAM_ADDR_VDR_PARAM_INITDIST	(FRAM_ADDR_VDR_PARAM_PULSE + sizeof(tPULSE_ST))
#define FRAM_ADDR_VDR_PARAM_LICENSE		(FRAM_ADDR_VDR_PARAM_INITDIST + sizeof(tINITDISTANCE_ST))
#define FRAM_ADDR_VDR_PARAM_ID			(FRAM_ADDR_VDR_PARAM_LICENSE + sizeof(tLICENSE_ST))

/**
 * 参数读写
 */
int VDR_SaveParam(eVDRPARAM eParam, u8 *pBuf, u16 u16Len);
int VDR_LoadParam(void);
u8* VDR_GetParam(eVDRPARAM eParam);
void VDR_Err_Proc(u8 u8Cmd, int ret);


#if 0

/**芯片采用的是K9F1G08U0B，芯片大小为128MB，分为1024个BLOCK(块)，每块大小为128KB。每块分成64页，每页大小为2KB。*/
/**
 * 行驶速度记录，1s间隔持续记录并存储行驶状态，包括时间，平均速度，状态，不少于最近48小时
 * 48*3600=172800条 *2B= 345600B = 35K,
 * 存储策略：
 * 因NANDFLASH只能按页为单位读写，拟分配3个块，2个为工作块，保证读和擦除不发生在同一块上，另一块作为备份块，容错用，替换坏块。
 * 按页存储，页空闲区存时间信息，8B
 * 开辟2K内存空间，作为存储缓区，满时写入；
 * 在铁电中保存索引信息，包括当前读写位置；
 * 为使得检索方便，存储格式和上传格式保持一致，页内存组织：
 * tSPEEDLOGPERSEC1 。。。 tSPEEDLOGPERSEC 7
 * sizeof(tSPEEDLOGPERSEC) = 126B
 * 1页可以存放8个tSPEEDLOGPERSEC
 * 第8个到达后写入NANDFLASH
 *
 * 读取时，根据起始、结束时间，先根据块号的最后一页的空闲区时间查找块号，定位到块号后，再根据起始页的第一个页空闲区和最后一页的空闲区，对时间进行二分查找。
 */
#define VDR_LOG_SPEED_NF_BLOCK_BEGIN	48
#define VDR_LOG_SPEED_NF_BLOCK_END		(VDR_LOG_SPEED_NF_BLOCK_BEGIN + 3)
#define VDR_LOG_SPEED_NF_BLOCK_SIZE		3
typedef struct {
	u16 u16BlockId_Start;
	u16 u16BlockId_Last;

	/*数据存储信息*/
	u16 u16BlockId_Head;	///数据块下一待写入位置，块号
	u8 u8PageId_Head;		///数据块下一待写入位置，页号
	u16 u16BlockId_Tail;	///数据块尾位置，块号，仅在写回头覆盖时更新到下一块
	u8 u8PageId_Tail;		///数据块尾位置，页号，仅在写回头覆盖时更新到下一页

	/*上传数据索引信息*/
	u16 u16BlockId_UplBegin;	///数据块下一待读取位置，块号
	u8 u8PageId_UplBegin;		///数据块下一待读取位置，页号
	u16 u16PageOffset_UplBegin;	///页内偏移位置
	u16 u16BlockId_UplEnd;	///数据块下一待读取位置，块号
	u8 u8PageId_UplEnd;		///数据块下一待读取位置，页号
	u16 u16PageOffset_UplEnd;	///页内偏移位置
	u16 u16BlockId_UplCursor;	///数据块下一待读取位置，块号
	u8 u8PageId_UplCursor;		///数据块下一待读取位置，页号
	u16 u16PageOffset_UplCursor;	///页内偏移位置

	/*读写缓区*/
	u8 aPageBuf[2048];	///写缓区
	u16 u16WtPtr;		///写指针
	BOOL bPageEmpty;	///页缓区中第一个数据块保存时间
//	u8 aRdBuf[2048];
} tSPEEDLOGHEAD;
#endif

/**
 *  行驶速度记录，1s间隔持续记录并存储行驶状态，包括时间，平均速度，状态，不少于最近48小时
 */
/*速度信息上传结构体*/
typedef struct {
	u8 u8AvgSPeed;	///平均速度
	u8 u8State;		///状态
} tSPEEDLOGPERSEC;

typedef struct {
	tTIME time_Start; ///秒=0
	tSPEEDLOGPERSEC tSpeed[60];
} tSPEEDLOGITEM;

void VDR_Log_SaveSpeedInfo(tTIME *pTime, tSPEEDLOGPERSEC *pSpeedLog);
int VDR_Log_LocateSpeedInfo(tTIME *pTime_Begin, tTIME *pTime_End);
int VDR_Log_ReadSpeedInfo(u8 *pBuf);


/**
 * 事故疑点记录，0.2秒间隔，持续记录20s，不少于100条
 * 情况1:行驶时，外部供电断开；
 * 情况2：行驶时，位置10s内无变化
 * 存储容量： 234B*100 = 23400B=23K
 */


/**
 * 超时驾驶记录，不少于100条
 * 存储容量：50B*100 =5000B=5K
 */

/**
 * 位置信息记录，1min间隔，360小时
 */
typedef struct {
	tTIME	time_End;
	tPOSITION	tPos_Begin;
	u8 u8AvgSPeed;	///平均速度
	u8 u8State;		///状态
} tPOSLOGINFO;


/**
 * 驾驶人身份记录，不小于200条
 * 存储容量：25B*200=5000B=5K
 */

/**
 * 里程记录
 */

/**
 * 安装参数记录
 */

/*--------- 日志记录 -------------*/
/**
 * 外部供电记录，不少于100条
 * 存储容量：7B*100=700B=1K
 *
 */

/**
 * 参数修改记录，不少于100条
 * 存储容量：7B*100=700B=1K
 */

/**
 * 速度状态日志，从开始时间连续60s，不少于10条
 * 存储容量：133B*10=1330B=13K
 */


#pragma 	pack()

#endif //__GPS_VDR_COMM_H__
