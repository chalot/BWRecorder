#ifndef	__GPS_VDR_COMM_H__
#define	__GPS_VDR_COMM_H__

#include <stm32f2xx_conf.h>
#include <type.h>
#include <board.h>
#include <systimer.h>
#include "ff.h"
#include "sdcard_interface.h"
#include <qevents.h>

#pragma 	pack(1)


#define UART_VDR	USART6 ///记录仪串口

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

typedef tVERSION	tACK_VERSION;///应答帧

#define		VDR_CMD_GET_DRIVERINFO	0x01	///驾驶人信息
typedef struct {
	u8	aDriverLicense[18];
} tDRIVERLICENSEINFO;	///表A.7

//#define tACK_DRIVERLICENSEINFO tDRIVERLICENSEINFO		///应答帧
typedef tDRIVERLICENSEINFO	tACK_DRIVERLICENSEINFO;

#if 0
/*时间日期格式*/
typedef struct {///BCD码
	u8	u8Year;
	u8	u8Month;
	u8	u8Day;
	u8	u8Hour;
	u8	u8Minute;
	u8	u8Second;
}BCDTIME;		///表A.8
#endif

#define		VDR_CMD_GET_RTC						0x02	///时间

typedef BCDTIME	tRTC;
typedef BCDTIME	tACK_RTC;	///应答帧

#define		VDR_CMD_GET_DISTANCE				0x03	///里程
typedef struct {
	BCDTIME	time_RTC;
	BCDTIME	time_FirstInstall;
	u8		aOriginDist_BCD[4];
	u8		aTotalDist_BCD[4];
}tINITDISTANCE;		///表A.9
//#define tACK_INITDISTANCE	tINITDISTANCE	///应答帧
typedef tINITDISTANCE	tACK_INITDISTANCE;

#define		VDR_CMD_GET_PULSE					0x04	///脉冲系数
typedef struct {
	BCDTIME	time_RTC;
	u16		u16Pulse;
}tPULSE;	///表A.10
//#define tACK_PULSE	tPULSE	///应答帧
typedef tPULSE	tACK_PULSE;


#define		VDR_CMD_GET_CARINFO					0x05	///车辆信息
typedef struct {
	u8	aIdentification[17];	///车辆识别代号
	u8	aLicense[12];		///机动车号牌号码，后3个字节备用
	u8	aClass[12];
}tCARINFO;	///表A.11
//#define tACK_CARINFO	tCARINFO	///应答帧
typedef tCARINFO	tACK_CARINFO;

#define		VDR_CMD_GET_STATECONFIG_INFO		0x06	///状态信号配置信息
#define 	STATE_CONFIG_AMOUNT		5	///状态配置信号字节个数
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

/**第1字节状态信号,第1字节的D7表示制动，D6表示左转向，D5表示右转向，D4表示远光，D3表示近光，D2～D0由用户自定义*/
typedef struct {
	u8	bit_Halt		:	1;	///D7
	u8	bit_Left		:	1;	///D6
	u8	bit_Right		:	1;	///D5
	u8	bit_LightFar	:	1;	///D4
	u8	bit_LightNear	:	1;	///D3
	u8	bit_reserved	:	3;	///D2～D0
} tSTATE_BYTE1;

/**记录仪信号类型*/
typedef enum {
	VDR_SIG_HALT = 0,
	VDR_SIG_LEFT,
	VDR_SIG_RIGHT,
	VDR_SIG_LIGHTFAR,
	VDR_SIG_LIGHTNEAR,

} eVDRSIGNAL;

/**1表示有操作，0表示无操作*/
typedef enum { OPERATE = 1, NONOPERATE = !OPERATE } eVDRSIGOPT;

typedef struct {
	BCDTIME	time_RTC;
	u8		u8Bytes;
	tSTATECONFIG	tConf[STATE_CONFIG_AMOUNT];
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
//#define tACK_UNIQID	tVDRID
typedef tVDRID	tACK_UNIQID;

#define		VDR_CMD_GET_SPEED					0x08	///行驶速度
/*命令*/
typedef struct {
	BCDTIME	time_Begin;
	BCDTIME	time_End;
	u16		u16PackAmt;
}tCMD_SPEED;

typedef struct {
	u8	u8AvrSpeed;
	u8	u8Status;
} tITEM_SPEED;
/*应答*/
typedef struct {
	BCDTIME	time_Start;
	tITEM_SPEED atItem[60];
}tACK_SPEED;	///表A.17


#define		VDR_CMD_GET_POSITION				0x09 	///位置
/*命令帧格式同采集速度*/
//#define 	tCMD_POSITION	tCMD_SPEED
typedef tCMD_SPEED	tCMD_POSITION;

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
	BCDTIME	time_Start;
	tITEM_POS	atItem[60];
}tACK_POS;	///表A.19

#define		VDR_CMD_GET_ACCIDENT				0x10	///事故疑点
/*命令帧格式同采集速度*/
//#define 	tCMD_ACCIDENT	tCMD_SPEED
typedef 	tCMD_SPEED	tCMD_ACCIDENT;

typedef struct {
	u8	u8Speed;
	u8	u8Status;
}tITEM_ACCIDENT;

typedef struct {
	BCDTIME	time_End;
	u8		aLicense[18];
	tITEM_ACCIDENT	atItem[100];
	tPOSITION	tLastValidPos;
}tACK_ACCIDENT;		///表A.22

#define		VDR_CMD_GET_OVERTIME_DRIVING		0x11	///超时驾驶
/*命令帧格式同采集速度*/
//#define 	tCMD_OVERTIME_DRIVING	tCMD_SPEED
typedef tCMD_SPEED		tCMD_OVERTIME_DRIVING;

typedef struct {
	u8		aLicense[18];
	BCDTIME	time_Begin;
	BCDTIME	time_End;
	tPOSITION	tPos_Begin;
	tPOSITION	tPos_End;
}tACK_OVERTIMEDRIVING;	///表A.24


#define		VDR_CMD_GET_DRIVER_LOG_INFO			0x12	///登入签出
/*命令帧格式同采集速度*/
//#define 	tCMD_DRIVERLOG	tCMD_SPEED
typedef tCMD_SPEED	tCMD_DRIVERLOG;

typedef struct {
	BCDTIME	time;
	u8		aLicense[18];
	u8		u8EvtType;	///01:登陆；02：退出
}tACK_DRIVERLOG;	///表A.26


#define		VDR_CMD_GET_POWER					0x13	///供电
/*命令帧格式同采集速度*/
//#define 	tCMD_POWER	tCMD_SPEED
typedef	tCMD_SPEED	tCMD_POWER;

typedef struct {
	BCDTIME	time;
	u8		u8EvtType;	///01:通电；02：断电
}tACK_POWER;	///表A.28

#define		VDR_CMD_GET_PARAM_CHANGE			0x14	///参数修改
/*命令帧格式同采集速度*/
//#define 	tCMD_PARAM_CHANGE	tCMD_SPEED
typedef	tCMD_SPEED	tCMD_PARAM_CHANGE;

typedef struct {
	BCDTIME	time;
	u8		u8EvtType;	///为设置参数命令字
}tACK_PARAMCHANGE;		///表A.30

#define		VDR_CMD_GET_SPEED_STATUS_LOG		0x15	///速度状态日志
/*命令帧格式同采集速度*/
//#define 	tCMD_SPEED_STATUS_LOG	tCMD_SPEED
typedef	tCMD_SPEED	tCMD_SPEED_STATUS_LOG;

typedef struct {
	u8		u8Speed_Log;	///记录速度，来自速度传感器
	u8		u8Speed_Ref;	///参考速度，来自GPS
}tITEM_SPEEDSTATE;

typedef struct {
	u8		u8SpeedState;	///01：正常，02：异常
	BCDTIME	time_Begin;
	BCDTIME	time_End;
	tITEM_SPEEDSTATE	tItem[60];	///从开始时间后1分钟内，每秒记录一次
}tACK_SPEEDSTATUS;		///表A.32


/**
 * 设置参数类
 */
#define VDR_CMD_SET_VECHICLE_INFO		0x82	///车辆信息，A.11
/*命令帧，无应答帧*/
//#define tCMD_SET_VECHICLE_INFO 	tACK_CARINFO
typedef tACK_CARINFO	tCMD_SET_VECHICLE_INFO;

#define VDR_CMD_SET_FIRST_INSTLL_DATA	0x83	///初次安装日期	A.8
/*命令帧，无应答帧*/
//#define tCMD_SET_INSTALL_DATA	BCDTIME
typedef BCDTIME	tCMD_SET_INSTALL_DATA;

#define VDR_CMD_SET_STATUS_CONFIG_INFO	0x84	///状态量配置信息	A.13
/*命令帧，无应答帧*/
//#define tCMD_SET_STAUS_CONFIG tITEM_STATECONFIG
typedef tACK_STATECONFIG	tCMD_SET_STAUS_CONFIG;

#define VDR_CMD_SET_TIME				0xC2	///时间	A.8
/*命令帧，无应答帧*/
//#define tCMD_SET_TIME	BCDTIME
typedef BCDTIME	tCMD_SET_TIME;

#define VDR_CMD_SET_PULSE				0xC3	///脉冲系数		A.10
/*命令帧，无应答帧*/
//#define tCMD_SET_PULSE	tACK_PULSE
typedef tACK_PULSE	tCMD_SET_PULSE;

#define VDR_CMD_SET_INITIALDIST			0xC4	///初始里程		A.9
/*命令帧，无应答帧*/
//#define tCMD_SET_INITIALDIST	tACK_INITDISTANCE
typedef tACK_INITDISTANCE	tCMD_SET_INITIALDIST;

/*检定命令字及数据块格式*/
#define tCMD_DIAG_ENTER_KEEP	0xE0	///进入检定状态
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

///*参数类型*/
//typedef enum {
//	VDR_PARAM_CARINFO = 0,
//	VDR_PARAM_INSTALLTIME,
//	VDR_PARAM_STATECONFIG,
//	VDR_PARAM_RTC,
//	VDR_PARAM_PULSE,
//	VDR_PARAM_INITDIST,
//	VDR_PARAM_LICENSE,
//	VDR_PARAM_ID,
//	VDR_PARAM_END,
//} eVDRPARAM;

///车辆信息参数
typedef struct {
	tCARINFO 	tCarInfo;
	u8 			u8CRC;
}tCARINFO_ST;

///记录仪时间参数
typedef struct {
	BCDTIME 	time;
	u8 		u8CRC;
}BCDTIME_ST;

///状态配置参数
typedef struct {
	tACK_STATECONFIG 	tStateConf;
	u8 				u8CRC;
}tSTATECONFIG_ST;

///脉冲系数参数
typedef struct {
	tPULSE		tPulse;
	u8 			u8CRC;
}tPULSE_ST;

///初始里程参数
typedef struct {
	tINITDISTANCE 	tInitDist;
	u8 				u8CRC;
}tINITDISTANCE_ST;

///驾照号码参数
typedef struct {
	u8 aLicense[18];
	u8 u8CRC;
}tLICENSE_ST;

///记录仪唯一ID参数
typedef struct {
	tVDRID 	tID;
	u8 		u8CRC;
}tUNIQID_ST;


/**
 * 记录仪全局参数，注意，参数保存在铁电中
 */
typedef struct {
	tCARINFO_ST			tCarInfo_St;		///车辆信息	A.11
	BCDTIME_ST 			tInstData_St;	///安装日期 表A.8
	tSTATECONFIG_ST 	tStateConf_St;		///状态量配置信息  表A.13
	BCDTIME_ST 			tRTC_St;		///记录仪时间 表A.8
	tPULSE_ST			tPulse_St;			///脉冲系数	 表A.10
	tINITDISTANCE_ST	tInitDist_St;		///里程  表A.9
	tLICENSE_ST			tLicense_St;	///驾驶证号码
	tUNIQID_ST			tID_St;			///记录仪唯一ID

} tVDRPARAM_ST;


/*记录仪各参数在铁电中的存储偏移地址*/
#define FRAM_ADDR_VDR_PARAM_START		0x3333	///记录仪参数起始偏移地址
#define FRAM_ADDR_VDR_PARAM_CARINFO		(FRAM_ADDR_VDR_PARAM_START)
#define FRAM_ADDR_VDR_PARAM_INSTALLTIME	(FRAM_ADDR_VDR_PARAM_CARINFO + sizeof(tCARINFO_ST))
#define FRAM_ADDR_VDR_PARAM_STATECONFIG	(FRAM_ADDR_VDR_PARAM_INSTALLTIME + sizeof(BCDTIME_ST))
#define FRAM_ADDR_VDR_PARAM_RTC			(FRAM_ADDR_VDR_PARAM_STATECONFIG + sizeof(tSTATECONFIG_ST))
#define FRAM_ADDR_VDR_PARAM_PULSE		(FRAM_ADDR_VDR_PARAM_RTC + sizeof(BCDTIME_ST))
#define FRAM_ADDR_VDR_PARAM_INITDIST	(FRAM_ADDR_VDR_PARAM_PULSE + sizeof(tPULSE_ST))
#define FRAM_ADDR_VDR_PARAM_LICENSE		(FRAM_ADDR_VDR_PARAM_INITDIST + sizeof(tINITDISTANCE_ST))
#define FRAM_ADDR_VDR_PARAM_ID			(FRAM_ADDR_VDR_PARAM_LICENSE + sizeof(tLICENSE_ST))



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

/**--------------------------------------------------------------------------------------------------
 * 记录仪外部存储器记录数据
 *-------------------------------------------------------------------------------------------------*/
/**
新国标对记录仪的性能要求包括：
1、通电前自检
在通电开始工作时，首先进行自检，自检正常后应以绿闪信号或显示屏显示式指示工作正常，如有故障则应以红闪信号或显示屏显示方式指示故障信息。
2、行驶记录
记录仪能以1秒的时间间隔持续记录并存储车辆行驶状态数据，行驶状态数据为：车辆在行驶过程中的实时时间、每秒钟间隔内对应的平均速度以及对应时间的状态信号。
有效数据记录应不少于最近48个单位小时。速度记录单位为km/h，测量范围至少为0-220km/h，分辨率等于或大于1km/h。
3、事故疑点记录
记录仪能以0.2秒的时间间隔持续记录并存储行驶结束前20秒实时时间对应的行驶状态数据，该行驶状态数据为：车辆行驶速度、制动等状态信号和行驶结束时的位置信息。
4、外部通电断开时
记录仪能以0.2秒的时间间隔持续记录并存储断电前20秒内的车辆行驶状态数据，该行驶状态数据为：车辆行驶速度、车辆制动等状态信号及断电时的位置信息。
5、车辆处于行驶状态且有效位置信息10秒内无变化时
记录仪应能以0.2秒的时间间隔持续记录并存储断电前20秒内的车辆行驶状态数据，此项行驶状态数据为：车辆行驶速度、车辆制动等状态信号及断电时的位置信息。事故疑点记录次数应不少于最近100条。
6、超时驾驶记录
记录仪能记录驾驶人超时驾驶的数据，数据内容包括：机动车驾驶证号码、连续驾驶开始时间及所在位置信息、连续驾驶结束时间及所在位置信息，记录次数应不少于最近100条。
*/

/*记录类型*/
typedef enum {
	RECORD_SPEED = 0,	///行驶速度记录
	RECORD_ACCIDENT,	///事故疑点记录
	RECORD_OVERTIMEDRIVING,	///超时驾驶记录
	RECORD_POS,		///位置信息记录
	RECORD_DRIVERLOG,	///驾驶人信息记录
	RECORD_POWER,		///外部供电记录
	RECORD_PARAMCHANGE,	///参数修改记录
	RECORD_SPEEDSTATUS,	///速度状态记录

	RECORD_END,
} eRECORD;



/**
 *  行驶速度记录，1s间隔持续记录并存储行驶状态，包括时间，平均速度，状态，不少于最近48小时
 *  存储路径： 				/——speed
 *  						 |——— 年_月_日_时1
 *  						 |——— 年_月_日_时2
 *  写周期：一分钟，126B
 *  存储容量：126*60*48=354K
 *
 *  存储数据结构：
 *  tSPEEDLOGITEM_1		x x x x x x | d d | d d |... | d d
 *  					年 月  日     时  分   秒   | 1s  | 2s  |    | 60s
 *  tSPEEDLOGITEM_2
 *
 * 	文件创建单位：以小时计，每小时创建一个新文件
 *	系统中保存最近48个文件。
 *	超过48小时的文件将自动删除
 */

/*速度信息上传结构体*/
typedef struct {
	u8 u8AvgSPeed;	///平均速度
	u8 u8State;		///状态
} tSPEEDPERSEC;

/*速度记录格式*/
typedef struct {
	BCDTIME time_Start; ///秒=0
	tSPEEDPERSEC tSpeed[60];
} tRECORD_SPEED;


/**
 * 事故疑点记录格式，0.2秒间隔，持续记录20s，不少于100条
 * 判断条件：在行驶状态下，发生如下两种情况时开始记录
 * 情况1:外部供电断开；
 * 情况2：位置10s内无变化
 * 存储容量： 234B
 */
//#define tRECORD_ACCIDENT	tACK_ACCIDENT
typedef tACK_ACCIDENT	tRECORD_ACCIDENT;

/**
 * 超时驾驶记录格式，不少于100条
 * 存储容量：50B*100 =5000B=5K
 */
typedef struct {
	BCDTIME	time;	///附加的时间，方便检索，实际上传数据要去除
	tACK_OVERTIMEDRIVING	tInfo;
} tRECORD_OVERTIMEDRIVING;


/**
 * 位置信息记录格式，1min间隔，360小时,15天
 * 存储容量： 666B*360=240K
 */
//#define tRECORD_POS		tACK_POS
typedef tACK_POS	tRECORD_POS;


/**
 * 驾驶人身份记录，不小于200条
 * 存储容量：25B*200=5000B=5K
 */
//#define tRECORD_DRIVERLOG	tACK_DRIVERLOG
typedef tACK_DRIVERLOG	tRECORD_DRIVERLOG;

/**
 * 里程记录
 */
//存储容量：4B
//#define tRECORD_DIST	u32
typedef u32	tRECORD_DIST;

/**
 * 4.4.1.2.7　安装参数记录，注意在规范中没有给出具体的安装参数记录结构体，自定义，可能不对？？？？
 *	记录仪应能记录安装时相关参数信息，具体参数包括：机动车号牌号码、机动车号牌分类、车辆识别代码、脉冲系数、记录仪初次安装时间和初始里程。
 */
typedef struct {
	tCARINFO	tCarInfo;	///机动车号牌号码、机动车号牌分类、车辆识别代码
	u16		u16Pulse;	///脉冲系数
	BCDTIME	time_FirstInstall;///记录仪初次安装时间和初始里程。
	u8		aOriginDist_BCD[4];
} tRECORD_INSTALLPARAM;


/*--------- 日志记录 -------------*/
/**
 * 外部供电记录，不少于100条
 * 存储容量：7B*100=700B=1K
 */
//#define tRECORD_POWER	tACK_POWER
typedef tACK_POWER	tRECORD_POWER;

/**
 * 参数修改记录，不少于100条
 * 存储容量：7B*100=700B=1K
 */
//#define tRECORD_PARAMCHANGE		tACK_PARAMCHANGE
typedef tACK_PARAMCHANGE	tRECORD_PARAMCHANGE;

/**
 * 速度状态日志，从开始时间连续60s，不少于10条
 * 每个日历天记录仪判断速度状态一次，同时存储速度状态日志
 * 存储容量：133B*10=1330B=1.3K
 */
//#define tRECORD_SPEEDSTATUS		tACK_SPEEDSTATUS
typedef tACK_SPEEDSTATUS	tRECORD_SPEEDSTATUS;

/**不同记录类型单条记录长度*/
//const u16 RECORD_SIZE[] = {
//		sizeof(tRECORD_SPEED),
//		sizeof(tRECORD_ACCIDENT),
//		sizeof(tRECORD_OVERTIMEDRIVING),
//		sizeof(tRECORD_POS),
//		sizeof(tRECORD_DRIVERLOG),
//		sizeof(tRECORD_DIST),
//		sizeof(tRECORD_INSTALLPARAM),
//		sizeof(tRECORD_POWER),
//		sizeof(tRECORD_PARAMCHANGE),
//		sizeof(tRECORD_SPEEDSTATUS),
//};

//extern u16 RECORD_SIZE[];

#define VDR_PARAM_CARINFO 			0
#define VDR_PARAM_INSTALLTIME		1
#define VDR_PARAM_STATECONFIG		2
#define VDR_PARAM_RTC				3
#define VDR_PARAM_PULSE				4
#define VDR_PARAM_INITDIST			5
#define VDR_PARAM_LICENSE			6
#define VDR_PARAM_ID				7
#define VDR_PARAM_END				8

/*参数类型*/
//typedef enum {
//	VDR_PARAM_CARINFO = 0,
//	VDR_PARAM_INSTALLTIME,
//	VDR_PARAM_STATECONFIG,
//	VDR_PARAM_RTC,
//	VDR_PARAM_PULSE,
//	VDR_PARAM_INITDIST,
//	VDR_PARAM_LICENSE,
//	VDR_PARAM_ID,
//	VDR_PARAM_END,
//} eVDRPARAM;


/**--------------------------------------------------------------------------------------------------
 * 记录仪USB数据存储
 *-------------------------------------------------------------------------------------------------*/


#define VDR_USB_CODE_VERSION 	0x00	/// 执行标准版本年号 见表A.6
#define VDR_USB_CODE_DRIVERINFO	0x01	/// 当前驾驶人信息 见表A.7
#define VDR_USB_CODE_RTC 		0x02 	///实时时间 见表A.8
#define VDR_USB_CODE_DIST		0x03	/// 累计行驶里程 见表A.9
#define VDR_USB_CODE_PULSE		0x04 	///脉冲系数 见表A.10
#define VDR_USB_CODE_CARINFO	0x05	/// 车辆信息 见表A.11
#define VDR_USB_CODE_CONFIG		0x06	/// 状态信号配置信息 见表A.12
#define VDR_USB_CODE_UNIQID		0x07 	///记录仪唯一性编号 见表A.14
#define VDR_USB_CODE_SPEED		0x08	///08H 行驶速度记录 见表A.16
#define VDR_USB_CODE_POSITION	0x09	///09H 位置信息记录 见表A.18
#define VDR_USB_CODE_ACCIDENT	0x10	///10H 事故疑点记录 见表A.21 全部记录
#define VDR_USB_CODE_OTDRIVE	0x11	///11H 超时驾驶记录 见表A.23 全部记录
#define VDR_USB_CODE_DRIVERLOG	0x12	///12H 驾驶人身份记录 见表A.25 全部记录
#define VDR_USB_CODE_POWER		0x13	///13H 外部供电记录 见表A.26 全部记录
#define VDR_USB_CODE_PARAMCHG	0x14	///14H 参数修改记录 见表A.29 全部记录
#define VDR_USB_CODE_SPEEDSTATUS	0x15	///15H 速度状态日志 见表A.31 全部记录

typedef struct {
	u8 code;
	char* name;
} VDR_USBBLOCKINFO;


/**USB数据块结构*/
typedef struct {
	u8 u8Code;	//数据代码（1个字节）
	u8 aName[18];	//数据名称（18个字节）
	u32 u32Len;	//数据长度（4个字节）
	u8  pData[0];
} VDRUSBBLOCKHEAD;

/**
数据块个数（2个字节）
数据块1
数据块2
……
数据块n
校验值（1个字节）
*/
//typedef struct {
//	u16 u16BlockAmt;
//	VDRUSBBLOCKHEAD	blocks[0];
//	u8 u8Checksum;
//
//};

/**记录提取方向*/
#define FORWARD		1	///正序
#define BACKWARD	0	///逆序

/****************************************************************************************************************
 * 参数读写接口
 ***************************************************************************************************************/
int VDR_SaveParam(u8 eParam, u8 *pBuf, u16 u16Len);
int VDR_LoadParam(void);
u8* VDR_GetParam(u8 eParam);
void VDR_Err_Proc(u8 u8Cmd, int ret);


/****************************************************************************************************************
 * 串口及USB存储接口
 ***************************************************************************************************************/
int VDR_SDInit();
int VDR_InitUSBDEV();
void VDR_Comm_MemInit(void);

int VDR_DumpFiles2USBStorage(TIME *pTime);
void VDR_ClearQuerySession();
int VDR_NewQuerySession(T_QSESSION *qSession, VDRMultiAckEvt *pe);
int VDR_RetrieveSingleRecordForward(T_QSESSION *qSession, u8 *pBuf);
int VDR_RetrieveSingleRecordBackward(T_QSESSION *qSession, u8 *pBuf);

int VDR_DumpSpeedInfo(TIME *pTime);
int VDR_DumpPositionInfo(TIME *pTime);
int VDR_DumpAccidentInfo(TIME *pTime);
int VDR_DumpSpeedStateInfo(TIME *pTime);

void VDR_SetSignalState(eVDRSIGNAL eSig, eVDRSIGOPT opt);
void VDR_GetSignalState(tSTATE_BYTE1 *pState);

void VDR_Comm_SendSingleAckFrame(VDRAckEvt *pAckEvt);

u16 VDR_Comm_doRetrieveMessage(u8 *pMsg, u16 u16BeginPtr, u16 u16EndPtr);

void TIMECPY(TIME *t1, TIME *t2);
void BCDTIME_CPY(BCDTIME *pt1, BCDTIME *pt2);

#pragma 	pack()

#endif //__GPS_VDR_COMM_H__
