/**   
 * @Title: gps.c  
 * @Description: TODO(用一句话描述该文件做什么) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-7-29 上午10:32:44 
 * @version V1.0   
 */
/*
 G595 GPS 模块，内部已集成了天线侦测电路，当客户采用我司的G595 GPS 模块时，可
 以不贴外部侦测电路的物料，只需要软件稍稍做下修改，就可以为客户节约0.3$-0.37$的成本呀！当外部
 天线出现短路或断路时， 通过串口输出“ $GPTXT,01,01,02,ANTSTATUS=SHORT*6D ” 或
 “$GPTXT,01,01,02,ANTSTATUS=OPEN*2B”语句来告诉客户外接天线的状态
 当系统检测到“$GPTXT,01,01,02,ANTSTATUS=SHORT*6D”输出时，表示天线连接已经短路。
 当系统检测到“$GPTXT,01,01,02,ANTSTATUS=OPEN*2B”输出时，表示天线连接已经断路
 当天线正常工作时，TXT 语句是没有输出的
 */

#include <stm32f2xx_conf.h>
#include <gps.h>
#include <comm.h>
#include <bsp.h>
#include <parameter.h>
#include <utility.h>
#include <trace.h>
#include <protocol.h>
#include <board.h>
#include <systimer.h>
#include <qevt.h>

Q_DEFINE_THIS_MODULE("gps.c")

//#define TRACE_GPS	//打印GPS接收信息

#define _GPS_RESPONSE_NUMBER_ 7	//GPS NMEA响应帧类型数
//GPS响应字符串
char* GPS_ResponseString[_GPS_RESPONSE_NUMBER_] = { "$GPRMC",	//GPS输出00：推荐定位信息
		"$GPGGA",		//GPS输出01：GPS定位信息
		"$GPGLL",		//GPS输出02：GPS定位地理信息
		"$GPGSA",		//GPS输出03：当前卫星信息
		"$GPGSV",		//GPS输出04：可见卫星信息
		"$GPVTG",		//GPS输出05：地面速度信息
		"$GPTXT",		//天线状态报告
		};

//GPS响应代码
typedef enum {
	_GPS_RES_GPRMC_ = 0,	//GPS输出00：推荐定位信息
	_GPS_RES_GPGGA_,			//GPS输出01：GPS定位信息
	_GPS_RES_GPGLL_,			//GPS输出01：GPS定位信息
	_GPS_RES_GPGSA_,			//GPS输出03：当前卫星信息
	_GPS_RES_GPGSV_,			//GPS输出04：可见卫星信息
	_GPS_RES_GPVTG_,			//GPS输出05：地面速度信息
	_GPS_RES_GPTXT_,
	_GPS_RES_UNKNOWN_ = 254,	//FE: 未知
	_GPS_RES_EMPTY_ = 255	//FF: 空，无响应

} _eGPS_RESPONSE_;

#define		_GPS_NMEA_MSG_LEN_MAX_		500		//NMEA协议帧最大长度#define		_GPS_MSG_QUENE_LEN_			1024	//消息队列长度static u8 aMsg_Buffer[_GPS_NMEA_MSG_LEN_MAX_];	//NMEA协议数据帧临时缓存
static u8 *rpcGPSMessage = NULL;

//串口收发缓区
#define RX_BUF_SIZE		1024	//接收缓区长度static u8 buf_Rx[RX_BUF_SIZE];	//接收缓区
static tCOMM Comm_Rx;  			//接收缓区

static _tGPSInfo tGpsInfo;	//GPS实时数据

//速度平滑窗口
typedef struct {
	u8 aSpeedArray[5];
	u16 u16SpeedArray_Counter;
	u16 u16AverageSpeed;

} tSPEEDSMOOTER;

//高度平滑窗口
typedef struct {
	u8 aAltitudeArray[5];
	u32 u32AltitudeArray_Counter;
	u16 u16AverageAltitude;

} tALTITUDESMOOTER;

static tSPEEDSMOOTER tSpeedSmooth;	//速度平滑器
static tALTITUDESMOOTER tAltitudeSmooth;	//高度平滑器

static u8 gps_ErrType = 0;	//错误类型

//===================== 本地函数声明 =================================================================
static u16 GPS_GetResponseCode(void);	//查找GPS响应代码
static u16 GPS_GetSegmentResponseCode(u8 u8StartIndex, u8 u8EndIndex);//查找GPS字段响应代码
static BOOL GPS_JumpToNextSegment(void);	//跳到本信息下一字段
static BOOL GPS_GetUTCTime(void);	//取UTC时间
static void GPS_GetUTCDate(u8 u8Format);	//取UTC日期
static BOOL GPS_GetLatitude(void);	//取纬度
static BOOL GPS_GetLongitude(void);	//取经度
static void GPS_GetSpeed(void);	//取速度
static void GPS_GetHeading(void);	//取航向
static void GPS_GetHeight(void);	//取速度
static u16 GPS_StrToInteger(void);	//ASCII数字字符串转换成整数
static float GPS_StrToFloat(void);	//ASCII数字字符串转换成浮点数
static u8 GPS_StrToByte(u8 u8Count);	//取字节数据
static u16 GPS_StrToWord(u8 u8Count);	//取字数据
static u16 GPS_GetIntegerLen(void);	//计算整数部分数字长度
static u16 GPS_SpeedAverage(u16 u16Speed);	//计算平均速度
static void GPS_AltitudeAverage(s16 s16Altitude);	//高度平滑
static void GPS_SyncSystemTime(void);	//同步系统时间
static u16 GPS_RetrieveMessage(u8* pu8Buffer, u16 read, u16 write);
//===================================================================================================

/**
 * @Func	GPS初始化
 * @Param	无
 * @Ret		无
 */
void GPS_Init() {
	Comm_Init(&Comm_Rx, buf_Rx, RX_BUF_SIZE); //接收缓区初始化

	tGpsInfo.u16GpsInfoValid |= _GPSINFO_FIRSTTIME; //等待第一次定位
}

/**
 * @Func	解析处理GPS数据帧，循环提取，直至空
 * @Param	无
 * @Ret		无
 */
void GPS_HandleMsg() {
//#define TRACE_GPS
	u16 msgLen = 0;
	/*暂存缓区读、写位置*/
	u16 write = Comm_Rx.write;

	do {

		msgLen = GPS_RetrieveMessage(aMsg_Buffer, Comm_Rx.read, write);
		if (msgLen > 0) {
			aMsg_Buffer[msgLen] = '\0';
#ifdef TRACE_GPS
			TRACE_(QS_USER, NULL, "[GPS] %s", aMsg_Buffer);
#endif
			GPS_ParseMsg(aMsg_Buffer, msgLen);
			Comm_Rx.read += msgLen;
			if (Comm_Rx.read >= RX_BUF_SIZE)
				Comm_Rx.read -= RX_BUF_SIZE;
		}
	} while (msgLen > 0);

}

/**
 * @Func	提取GPS数据帧
 * @Param	无
 * @Ret		无
 */
u16 GPS_RetrieveMessage(u8* pu8Buffer, u16 read, u16 write) {
	u8 u8Data = 0;
	u16 _tail = 0;
	u16 _write = 0;
	u16 _read = 0;
	u8 *pMsg = pu8Buffer;
	u16 len = 0;
	u8 i = 0;
	u8 bMsgReceive = 0;

//	/*暂存缓区读、写位置*/
//	_write = Comm_Rx.write;
//	_read = Comm_Rx.read;
	_read = read;
	_write = write;

	//搜索协议头0x24字符位置
	while ((_read != _write) && (*(Comm_Rx.pBuf + _read) != '$')) {
		//指向下一字符
		_read++;

		WRAP_AROUND(_read, RX_BUF_SIZE);
	}

	if (_read == _write) //缓区空
			{
		return 0;
	}

	//转储帧
	while (_read != _write) {
		//字符转储
		pMsg[i] = *(Comm_Rx.pBuf + _read);

		//到达帧尾？
		if ((i > 3) && (pMsg[i - 1] == 0x0D) && (pMsg[i] == 0x0A)) {
			//正确收到一个完整帧
			bMsgReceive = TRUE;

			//停止读数据
			break;
		}

		_read++;
		WRAP_AROUND(_read, RX_BUF_SIZE);
		i++;
	}

	//缓区搜索完，没有完整帧？
	if (!bMsgReceive) {
		return 0;
	}

	//正确提取一个完整NMEA帧，读指针跳过该帧
//	Comm_Rx.read = _read;

	//返回有效帧长度
	return (i + 1);
}

/**
 * @Func	GPS串口中断处理，注意须清除其他标志位，否则会出现一段时间后无接收数据
 * @Param	无
 * @Ret		无
 */
void ISR_GPS_COM(void) {
	u8 u8Data;

	//接收中断
	if (USART_GetITStatus(GPS_COM, USART_IT_RXNE)) {
		USART_ClearITPendingBit(GPS_COM, USART_IT_RXNE);

		//读取数据
		u8Data = (u8) USART_ReceiveData(GPS_COM);

		//存储数据
		*(Comm_Rx.pBuf + Comm_Rx.write++) = u8Data;
		WRAP_AROUND(Comm_Rx.write, RX_BUF_SIZE);
	}
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_ORE) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_ORE);        //读SR

		//读取数据
		u8Data = (u8) USART_ReceiveData(GPS_COM);		  //读DR

	}
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_PE) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_PE);        //读SR

		//读取数据
		u8Data = (u8) USART_ReceiveData(GPS_COM);		  //读DR

	}
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_FE) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_FE);        //读SR

		//读取数据
		u8Data = (u8) USART_ReceiveData(GPS_COM);		  //读DR

	}
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_NE) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_NE);        //读SR

		//读取数据
		u8Data = (u8) USART_ReceiveData(GPS_COM);		  //读DR

	}
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_IDLE) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_IDLE);        //读SR

		//读取数据
		u8Data = (u8) USART_ReceiveData(GPS_COM);		  //读DR

	}
	//LBD LIN断开检测标志
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_LBD) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_LBD);        //读SR
	}
	//传输完成检测标志
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_TC) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_TC);        //读SR
	}
	//CTS标志
	if (USART_GetFlagStatus(GPS_COM, USART_FLAG_CTS) != RESET) {
		USART_ClearFlag(GPS_COM, USART_FLAG_CTS);        //读SR
	}

}

/**
 * @Func	GPS信息交换处理，GPS串口收到一帧完整的NMEA数据后，调用此过程处理GPS信息
 * @Param	pcFrame,		[IN] 数据帧指针
 * 			u32FrameSize,	[IN] 数据帧长度
 * @Ret		无
 */
void GPS_ParseMsg(u8* pcFrame, u32 u32FrameSize) {
	//长度有效否? 内容合法否(首字符必须是"$"，尾字符必须是"cr")?
	if (!u32FrameSize || !pcFrame || *pcFrame != '$'
			|| *(pcFrame + u32FrameSize - 2) != 0x0D
			|| *(pcFrame + u32FrameSize - 1) != 0x0A) {
		return;
	}

	//GPS响应消息指针
	rpcGPSMessage = pcFrame;

	//获取GPS响应代码
	switch (GPS_GetResponseCode()) {
	case _GPS_RES_GPTXT_:

		break;

		//GPS输出00：推荐定位信息
		//$GPRMC,UTC时间,定位状态,纬度,南/北纬,经度,东/西经,地面速度,真北航向,UTC日期,磁偏角,磁偏角方向,模式指示*检查和<CR><LF>
		//1)  UTC时间:  格式为"hhmmss.sss"，"hh"表示时，取值范围00~23，"mm"表示分，取值范围为00~59，"ss"表示秒，取值范围为00~59，"sss"为秒的小数部分。
		//2)  定位状态: 1个字符。'A'表示有效定位，'V'表示无效定位。
		//3)  纬度:		格式为"ddmm.mmmm"，"dd"表示度，取值范围00~89，"mm"表示分，取值范围为00~59，"mmmm"为分的小数部分。
		//4)  南/北纬:  N=北纬(正)，S=南纬(负)
		//5)  经度:		格式为"dddmm.mmmm"，"ddd"表示度，取值范围00~179，"mm"表示分，取值范围为00~59，"mmmm"为分的小数部分。
		//6)  东/西经:	E=东经(正)，W=西经(负)
		//7)  地面速度: 单位：节，1节=1海里/时=1.852Km/h。
		//8)  真北航向:	单位：度。
		//9)  UTC日期:	格式为"ddmmyy"。其中"dd"为日期，取值范围为1~31；"mm"为月份，取值范围为1~12；"yy"为年，取值范围为00~99。
		//10) 磁偏角方向:"E"表示偏东，"W"表示偏西。
		//11) 模式指示:	1个字符。A=Autonomous, D=DGPS, E=DR
		//例，$GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598,,*10
	case _GPS_RES_GPRMC_:

		//跳过“$GPRMC,”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}

		//取UTC时间："hhmmss.sss"，忽略小数部分
		if (!GPS_GetUTCTime())
			break;

		//跳过“UTC时间,”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//判断“定位状态”是否有效定位?
		if (*(rpcGPSMessage) != 'A') {
			//置无效定位标志
			tGpsInfo.u16GpsInfoValid &= ~_GPSINFO_GPS_VALID;
			tGpsInfo.u16GpsInfoValid |= _GPSINFO_FIRSTTIME;

			//无效
			break;
		}

		//跳过“定位状态”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//取纬度：“dmm.mmmm”
		if (!GPS_GetLatitude())
			break;

		//跳过“南/北纬”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//取经度：“dddmm.mmmm”
		if (!GPS_GetLongitude())
			break;

		//跳过“东/西经”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}

		//取地面速度
		GPS_GetSpeed();

		//跳过“地面速度”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}

		//取航向
		GPS_GetHeading();

		//跳过“真北航向”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}

		//UTC日期
		GPS_GetUTCDate(6);

		//跳过“UTC日期”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}

//SIM548 GPS响应消息体中无磁偏角
#ifndef	_GPS_PRODUCT_WAVECOM_SIM548_
		//跳过“磁偏角”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
#endif

		//跳过“磁偏角方向”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}

		//判断“GPS模式”是否自主定位(A)或差分定位(D)?
		if ((*(rpcGPSMessage) != 'A') && (*(rpcGPSMessage) != 'D')) {
			//无效
//				break;
		}

		if (tGpsInfo.u16GpsInfoValid & _GPSINFO_FIRSTTIME) {
			//计算平均速度
			GPS_SpeedAverage(tGpsInfo.u16Speed);

			tGpsInfo.u16GpsInfoValid &= ~_GPSINFO_FIRSTTIME;
			tGpsInfo.u16GpsInfoValid |= _GPSINFO_GPS_VALID;

			//同步系统时间
			GPS_SyncSystemTime();
		}

		TRACE_(QS_USER, NULL,
				"[GPRMC] %d-%d-%d:%d-%d-%d, Lon=%x%x%x%x, Lat=%x%x%x%x, Speed=%x%x, total_Sat = %d, viewd_Sat = %d",
				tGpsInfo.u16Year, tGpsInfo.u8Month, tGpsInfo.u8Day,
				tGpsInfo.u8Hour, tGpsInfo.u8Minute, tGpsInfo.u8Second,
				tGpsInfo.Longtitude[0], tGpsInfo.Longtitude[1],
				tGpsInfo.Longtitude[2], tGpsInfo.Longtitude[3],
				tGpsInfo.Latitude[0], tGpsInfo.Latitude[1],
				tGpsInfo.Latitude[2], tGpsInfo.Latitude[3], tGpsInfo.Speed[0],
				tGpsInfo.Speed[1], tGpsInfo.u8VisibleSatellites,
				tGpsInfo.u8ValidSatellites);
		break;

		//GPS输出01：GPS定位信息
		//$GPGGA,UTC时间,纬度,南/北纬,经度,东/西经,GPS状态,卫星数量,HDOP,海拔高度,M,地球椭球面相对高度,M,差分时间,差分站ID号,*检查和<CR><LF>
		//1)  UTC时间：	hhmmss.sss,	格式=122.1 - 222.3。其中"hh"表示时，取值范围00~23，"mm"表示分，取值范围为00~59，"ss"表示秒，取值范围为00~59，"DD"为秒的小数部分。
		//2)  纬度:		XXMM.mmmm,	格式=12.1 - 32.4
		//3)  南/北纬:	N=北纬(正)，S=南纬(负)
		//4)  经度:		XXXMM.mmmm,	格式=12.1 - 32.4
		//5)  东/西经:	E=东经(正)，W=西经(负)
		//6)  GPS状态:	"0"表示未定位；"1"表示非差分定位；"2"为差分定位，"3"表示秒脉冲模式；"6"表示正在估算
		//7)  卫星数量:	00 - 12
		//8)  HDOP:		HDOP水平精度因子。最短字符数=1.1，最长字符数=3.1。取值范围为0.5~99.9。
		//9)  海拔高度:	格式=1.1 - 5.1。海拔以下时首字符为"-"，取值范围为-9999.9~99999.9。单位为1米。
		//10) 地球椭球面相对高度: 地球椭球面(WGS-84标准)相对大地水平面的高度。字符数为1~4，可以为空
		//11) 差分时间:	字符数为1~5。即从最近一次接收到差分信号开始的秒数。如果不是差分定位，该域为空。
		//12) 差分站ID号: 4个字符。取值范围为0000~1023。如果不是差分定位，该域为空。
	case _GPS_RES_GPGGA_:

		//跳过“$GPGGA,”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//取UTC时间
		if (!GPS_GetUTCTime())
			break;

		//跳过“UTC时间,”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//取纬度
		GPS_GetLatitude();

		//跳过“南/北纬”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//取经度
		GPS_GetLongitude();

		//跳过“东/西经”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//判断“GPS状态”是否非差分定位(1)或差分定位(2)?
		if ((*(rpcGPSMessage) != '1') && (*(rpcGPSMessage) != '2'))
			//无效定位
			break;

		//跳过“GPS状态”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//卫星数量
		tGpsInfo.u8ValidSatellites = (u8) GPS_StrToInteger();
		if (tGpsInfo.u8ValidSatellites < 3)
			break;

		//跳过“卫星数量”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//跳过“HDOP”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}

		//海拔高度
		GPS_GetHeight();

		//高度平滑
		GPS_AltitudeAverage(tGpsInfo.s16Altitude);

		/*
		 if(tGpsInfo.u16GpsInfoValid & _GPSINFO_FIRSTTIME)
		 {
		 //计算平均速度
		 GPS_SpeedAverage(tGpsInfo.u16Speed);

		 tGpsInfo.u16GpsInfoValid &= ~_GPSINFO_FIRSTTIME;
		 tGpsInfo.u16GpsInfoValid |= _GPSINFO_GPS_VALID;

		 //同步系统时间
		 GPS_SyncSystemTime();
		 }
		 */
		break;

		/*
		 //GPS输出02：GPS定位地理信息
		 //$GPGLL,纬度,南/北纬,经度,东/西经,UTC时间,定位状态,模式指示*检查和<CR><LF>
		 //1)  纬度:		XXMM.mmmm,	格式=12.1 - 32.4
		 //2)  南/北纬:	N=北纬(正)，S=南纬(负)
		 //3)  经度:		XXXMM.mmmm,	格式=12.1 - 32.4
		 //4)  东/西经:	E=东经(正)，W=西经(负)
		 //5)  UTC时间：	hhmmss.sss,	格式=122.1 - 222.3。其中"hh"表示时，取值范围00~23，"mm"表示分，取值范围为00~59，"ss"表示秒，取值范围为00~59，"DD"为秒的小数部分。
		 //6)  定位状态: 1个字符。"A"表示有效定位，"V"表示无效定位。
		 //7)  模式指示: 1个字符。"A"表示自主定位，"D"表示差分定位，"E"表示估算，"N"表示数据无效。
		 case	_GPS_RES_GPGLL_:

		 break;
		 */

		//GPS输出04：可见卫星信息
		//$GPGSV,GSV语句的总数量,本句GSV语句的编号,可见卫星的总数,PRN码,卫星仰角,卫星方位角,信噪比,…PRN码,卫星仰角,卫星方位角,信噪比*检查和<CR><LF>
		//1)  GSV语句的总数量: 本语句一行最多可以包含4颗卫星的信息，当卫星数多于4时，可见卫星信息将分成多行输出。1个字符。取值范围为1~9。
		//2)  本句GSV语句的编号: 1个字符。取值范围为1~9。
		//3)  可见卫星的总数: 1~2个字符。取值范围为0~12。
		//4)  PRN码: 伪随机噪声码。2个字符。正在用于解算位置的卫星号，取值范围为01~32。
		//5)  卫星仰角: 1~3个字符。取值范围为0~90 。
		//6)  卫星方位角: 1~3个字符。取值范围为0~359 。
		//7)  信噪比: 0~2个字符。取值范围为0~99dB，没有跟踪到卫星时为空。
	case _GPS_RES_GPGSV_: {
		u16 nGSVTotalSentence;
		u16 nGSVSequence;

		//跳过“$GPGSV,”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//获取“语句总数”
		if ((nGSVTotalSentence = GPS_StrToInteger()) == 0) {
			break;
		}

		//跳过“语句总数”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//获取“本句GSV语句的编号”
		nGSVSequence = GPS_StrToInteger();

		//跳过“本句GSV语句的编号”
		if (!GPS_JumpToNextSegment()) {
			//数据错误
			break;
		}
		//获取“可见卫星的总数” 正在使用的卫星数量（00 - 12）
		if ((tGpsInfo.u8VisibleSatellites = GPS_StrToInteger()) == 0) {
			break;
		}

		/*
		 //还未接收到任何GSV语句?或者接收到一个新的GSV语句?
		 if( ( m_cGPS_GSV_TotalSentence == 0 ) ||
		 ( m_cGPS_GSV_TotalSentence != nGSVTotalSentence ) ||
		 ( m_cGPS_VisibleSatellites != nGSVTotalSatellites ) )
		 {
		 //是否第1句?
		 if(nGSVSequence == 1)
		 {
		 //总语句数
		 m_cGPS_GSV_TotalSentence = nGSVTotalSentence;
		 //当前语句序列号
		 m_cGPS_GSV_SentenceSequence = nGSVSequence;

		 //卫星总数
		 m_cGPS_VisibleSatellites = nGSVTotalSatellites;
		 //已经取得的数量
		 m_cGPS_GottenVSNumber = 0;

		 //清除数据缓冲区
		 memset(m_taGPS_VisibleSatellitesInfo, 0, sizeof(_tBWP_SATINFO) * 12);
		 }
		 else
		 {
		 //语句出错
		 m_cGPS_GSV_TotalSentence = 0;
		 break;
		 }
		 }

		 //以前已经接收到GSV数据，检查语句顺序是否连续?
		 else if(nGSVSequence == (m_cGPS_GSV_SentenceSequence + 1))
		 {
		 //当前语句序列号
		 m_cGPS_GSV_SentenceSequence = nGSVSequence;
		 }

		 else
		 {
		 //语句出错
		 m_cGPS_GSV_TotalSentence = 0;
		 m_cGPS_VisibleSatellites = 0;
		 m_cGPS_GottenVSNumber = 0;
		 break;
		 }

		 bErr = FALSE;
		 for(i = 0; i < 4; i++)
		 {
		 //跳过上一字段
		 if(!GPS_JumpToNextSegment(rpcGPSMessage))
		 {
		 //数据错误
		 bErr = TRUE;
		 break;
		 }

		 //行结束?
		 if((*rpcGPSMessage == 0x0D) || (*rpcGPSMessage == '*'))
		 break;

		 if(m_cGPS_GottenVSNumber < 12)
		 {
		 //获取“PRN”=卫星号
		 m_taGPS_VisibleSatellitesInfo[m_cGPS_GottenVSNumber].cPRN = (u8)GPS_StrToInteger(rpcGPSMessage);

		 //跳过“PRN”
		 if(!GPS_JumpToNextSegment(rpcGPSMessage))
		 {
		 //数据错误
		 bErr = TRUE;
		 break;
		 }

		 //获取“卫星仰角”，单位=1度
		 m_taGPS_VisibleSatellitesInfo[m_cGPS_GottenVSNumber].cElevation = (u8)GPS_StrToInteger(rpcGPSMessage);

		 //跳过“卫星仰角”
		 if(!GPS_JumpToNextSegment(rpcGPSMessage))
		 {
		 //数据错误
		 bErr = TRUE;
		 break;
		 }

		 //获取“卫星方位角”，单位=1度
		 m_taGPS_VisibleSatellitesInfo[m_cGPS_GottenVSNumber].wAzimuth = (u16)GPS_StrToInteger(rpcGPSMessage);

		 //跳过“卫星方位角”
		 if(!GPS_JumpToNextSegment(rpcGPSMessage))
		 {
		 //数据错误
		 bErr = TRUE;
		 break;
		 }

		 //获取“信噪比”，单位=1dB，0~99dB
		 m_taGPS_VisibleSatellitesInfo[m_cGPS_GottenVSNumber].cSN = (u8)GPS_StrToInteger(rpcGPSMessage);

		 //已获取卫星数+1
		 m_cGPS_GottenVSNumber++;
		 }

		 else
		 {
		 BWNVTRACE( (_BWNV_printf_LEVEL_FORCE_, L"GPS卫星信息：可见卫星数大于12!\n") );
		 }
		 }

		 if(bErr)
		 {
		 //语句序列出错
		 m_cGPS_GSV_TotalSentence = 0;

		 BWNVTRACE( (_BWNV_printf_LEVEL_GPS_, L"GPS信息：卫星信息GSV，语句序列错误\n") );
		 break;
		 }

		 //如果是最后一句，且卫星数量取完
		 if((m_cGPS_GSV_SentenceSequence == m_cGPS_GSV_TotalSentence) && (m_cGPS_GottenVSNumber == m_cGPS_VisibleSatellites))
		 {
		 //进入保护
		 EnterCriticalSection(&m_CS_GPS_Infor);

		 //是否请求了卫星信息?
		 if(m_cGPS_NeedSatelliteInfo)
		 {
		 //卫星信息
		 memcpy((u8*)&m_taSYS_VisibleSatellitesInfo, (u8*)m_taGPS_VisibleSatellitesInfo, sizeof(_tBWP_SATINFO) * 12);

		 //设置有效GPS信息
		 tGPSFSM.u16GpsInfoValid |= _GPSINFO_SATINFO_VALID;
		 }

		 //清除GSV总语句数
		 m_cGPS_GSV_TotalSentence = 0;

		 //退出保护
		 LeaveCriticalSection(&m_CS_GPS_Infor);
		 }
		 */
//			BWNVTRACE( (_BWNV_printf_LEVEL_GPS_, L"GPS信息：卫星信息GSV，卫星数=%d\n", m_cGPS_GottenVSNumber) );
	}
//		TRACE_(QS_USER, NULL, "[GPGSV] total_Sat = %d, viewd_Sat = %d",
//				tGpsInfo.u8VisibleSatellites, tGpsInfo.u8ValidSatellites);

		break;
		//GPS输出03：当前卫星信息
		//$GPGSA,模式,定位类型,PRN码,PRN码,PRN码,PRN码,PRN码,PRN码,PRN码,PRN码,PRN码,PRN码,PRN码,PRN码,PDOP位置精度因子,HDOP水平精度因子,VDOP垂直精度因子*检查和<CR><LF>
		//1)  模式:		1个字符，"A"表示自动，允许自动在2D和3D方式之间切换；"M"表示手动，GPS被强制以2D或3D方式工作。
		//2)  定位类型: 1个字符。"1"表示没有定位；"2"表示2D定位；"3"表示3D定位。
		//3)  PRN码:    伪随机噪声码。均为2个字符。正在用于解算位置的卫星号，取值范围为01~32。
		//4)  PDOP位置精度因子: 最短字符数=1.1，最长字符数=3.1。取值范围为0.5~99.9。
		//5)  HDOP水平精度因子: 最短字符数=1.1，最长字符数=3.1。取值范围为0.5~99.9。
		//6)  VDOP垂直精度因子: 最短字符数=1.1，最长字符数=3.1。取值范围为0.5~99.9。
	case _GPS_RES_GPGSA_:

		break;
		//GPS输出05：地面速度信息
		//$GPVTG,真北航向,T,磁北航向,M,地面速度1,N,地面速度2,K,模式指示*检查和<CR><LF>
		//1)  相对真北的航向: 格式=1.1 - 3.2。取值范围为000.0~359.9 。
		//2)  相对磁北的航向: 格式=1.1 - 3.2。取值范围为000.0~359.9 。
		//3)  地面速度1: 格式=1.1 - 4.2。取值范围为000.0~999.9节。1节=1海里/时=1.852Km/h。
		//4)  地面速度2: 格式=1.1 - 4.2。取值范围为0000.0~1851.8Km/h。
		//5)  模式指示:	1个字符。"A"表示自主定位，"D"表示差分定位，"E"表示估算，"N"表示数据无效。
	case _GPS_RES_GPVTG_:

		break;

		//FF: 空，无响应
	case _GPS_RES_EMPTY_:
		break;
	default:
		break;
	}
}

/***********************************************************************************************************
 * 功能描述：	查找GPS响应代码
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	u16,			响应代码
 * 设    计：	高正东，2009-02-04
 ***********************************************************************************************************/
u16 GPS_GetResponseCode() {
	u16 nResCode = 0;

	//查找GPS响应代码
	nResCode = GPS_GetSegmentResponseCode(_GPS_RES_GPRMC_, _GPS_RES_UNKNOWN_);

	//返回第一部分响应
	return nResCode;
}

/***********************************************************************************************************
 * 功能描述：	查找GPS字段响应代码
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 *				u8StartIndex,	[IN] 起始搜索响应号
 *				u8EndIndex,		[IN] 结束搜索响应号
 * 出    口：	u16,			响应代码
 * 设    计：	高正东，2009-02-04
 ***********************************************************************************************************/
u16 GPS_GetSegmentResponseCode(u8 u8StartIndex, u8 u8EndIndex) {
	u16 nResCode;
	u16 nLen = 0;

	//结束?
	if (*rpcGPSMessage == 0x0D) {
		return _GPS_RES_EMPTY_;
	}

	//遍历所有可能的响应
	for (nResCode = u8StartIndex; nResCode <= u8EndIndex; nResCode++) {
		//想定的响应串长度
		nLen = strlen_(GPS_ResponseString[nResCode]);

		//比较响应字符串是否已知的字符串
		if (memcmp_((char*) rpcGPSMessage, GPS_ResponseString[nResCode],
				nLen)) {
			//找到响应字符串，代码=ResCode
			break;
		}
	}

	//未搜索到?
	if (nResCode > u8EndIndex) {
		//未知响应
		return _GPS_RES_UNKNOWN_;
	}

	//匹配到响应后，将指针移动到响应字符串后
	*rpcGPSMessage += nLen;

	return nResCode;

}

/***********************************************************************************************************
 * 功能描述：	跳到本信息下一字段
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	BOOL			真=成功，假=接收数据错误
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
BOOL GPS_JumpToNextSegment() {
	while (1) {
		//指针最多停留在结束处
		if (*rpcGPSMessage == 0x0D)
			break;

		//字符串中间不能出现“$”
		if (*rpcGPSMessage == '$') {
			*rpcGPSMessage = 0x0D;

			return FALSE;
		}

		//是分隔符?
		if (*rpcGPSMessage == ',') {
			//找到分隔符时，停止
			rpcGPSMessage++;

			//继续跳过空白,到下个非空白处
			while (1) {
				//未结束时，跳过一空格
				if (*rpcGPSMessage != ' ') {
					break;
				}

				//下一个
				rpcGPSMessage++;
			}

			break;
		}

		//下一字符
		rpcGPSMessage++;
	}

	return TRUE;
}

/***********************************************************************************************************
 * 功能描述：	取UTC时间
 * 格    式：	"hhmmss.sss"，"hh"表示时，取值范围00~23，"mm"表示分，取值范围为00~59，"ss"表示秒，
 *				取值范围为00~59，"sss"为秒的小数部分。
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	BOOL			真=成功，假=接收数据错误
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
BOOL GPS_GetUTCTime() {
	u16 nPou16Pos;

	//计算整数部分数字长度
	nPou16Pos = GPS_GetIntegerLen();

	//整数部分数字长度<5则出错?
	if (nPou16Pos < 5) {
		//出错
		return FALSE;
	}

	//“时(hh)”的字符个数
	nPou16Pos -= 4;

	//获取“时(hh)”，本地时间的“时” = UTC时间的“时” + 8
	tGpsInfo.u8Hour = GPS_StrToByte(nPou16Pos) + 8;
	//获取“分(mm)”
	tGpsInfo.u8Minute = GPS_StrToByte(2);
	//获取“秒(ss)”
	tGpsInfo.u8Second = GPS_StrToByte(2);
	//超时回绕
	if (tGpsInfo.u8Hour >= 24) {
		tGpsInfo.u8Hour -= 24;
	}

	return TRUE;
}

/***********************************************************************************************************
 * 功能描述：	取UTC日期
 * 格    式：	UTC日期: 6个字符，格式为"DDMMYY"。其中"DD"为日期，取值范围为1~31；"mm"为月份，
 *				取值范围为1~12；"YY"为年，取值范围为00~99。
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 *				u8Format,		[IN] 6="DDMMYY"格式，8="DDMMYYYY"格式
 * 出    口：	BOOL			真=成功，假=接收数据错误
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
void GPS_GetUTCDate(u8 u8Format) {
	//获取“日(DD)”
	tGpsInfo.u8Day = GPS_StrToByte(2);
	//获取“月(mm)”
	tGpsInfo.u8Month = GPS_StrToByte(2);

	if (u8Format == 6) {
		//获取“年(YY)”
		tGpsInfo.u16Year = GPS_StrToWord(2) + 2000;
	}

	else {
		//获取“年(YY)”
		tGpsInfo.u16Year = GPS_StrToWord(4);
	}

	return;
}

/***********************************************************************************************************
 * 功能描述：	取纬度
 * 格	式：	纬度:格式为"ddmm.mmmm"，"dd"表示度，取值范围00~89，"mm"表示分，
 取值范围为00~59，"mmmm"为分的小数部分。
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	BOOL			真=成功，假=接收数据错误
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
BOOL GPS_GetLatitude() {
	u16 u8Count;
	u16 nDeg, nMin, ndMin;

	//计算整数部分数字长度
	u8Count = GPS_GetIntegerLen();

	//整数部分长度<3则出错?
	if (u8Count < 3) {
		//出错
		return FALSE;
	}

	/*
	 纬度表示范围：00度00.000分——89度59.999分；
	 采用压缩BCD编码，但最高位是符号位。“正”表示“北纬”，“负”表示“南纬”；
	 “分”的单位是：千分之一分。
	 [例如]： 南纬30度37.901分表示为：
	 80H+3H，03H，79H，01H
	 */

	//度
	tGpsInfo.Latitude[0] = (*rpcGPSMessage - 0x30) & 0x0F;
	tGpsInfo.Latitude[1] = (*(rpcGPSMessage + 1) - 0x30) << 4;

	//分
	tGpsInfo.Latitude[1] |= *(rpcGPSMessage + 2) - 0x30;
	tGpsInfo.Latitude[2] = (*(rpcGPSMessage + 3) - 0x30) << 4;

	//秒
	if (*(rpcGPSMessage + 4) == '.') {
		tGpsInfo.Latitude[2] |= *(rpcGPSMessage + 5) - 0x30;
		tGpsInfo.Latitude[3] = (*(rpcGPSMessage + 6) - 0x30) << 4;
		tGpsInfo.Latitude[3] |= *(rpcGPSMessage + 7) - 0x30;
	} else {
		tGpsInfo.Latitude[2] &= 0xF0;
		tGpsInfo.Latitude[3] = 0;
	}

	//“度(dd)”的字符个数
	u8Count -= 2;

	//获取“度(dd)”
	nDeg = (u16) GPS_StrToByte(u8Count);

	//获取“分(mm)”
	nMin = (u16) GPS_StrToByte(2);

	//跳过小数点
	if (*rpcGPSMessage == '.') {
		rpcGPSMessage++;

		//获取“分小数部分(mmmm)”
		ndMin = (u16) GPS_StrToWord(4);
	} else {
		ndMin = 0;
	}

	//计算纬度，单位为10^-3分
	tGpsInfo.s32Latitude = (nDeg * 60 + nMin) * 1000 + ndMin * 0.1;

	//跳过“纬度,”
	if (!GPS_JumpToNextSegment()) {
		//数据错误
		return FALSE;
	}

	//判断南/北纬
	if (*rpcGPSMessage == 'S') {
		//南纬，最高位置1
		tGpsInfo.Latitude[0] |= 0x80;
	}

	return TRUE;
}

/***********************************************************************************************************
 * 功能描述：	取经度
 * 格	式：	经度:格式为"dddmm.mmmm"，"ddd"表示度，取值范围00~179，"mm"表示分，取值范围为00~59，
 "mmmm"为分的小数部分。东/西经: E=东经(正)，W=西经(负)
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	BOOL			真=成功，假=接收数据错误
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
BOOL GPS_GetLongitude() {
	u16 u8Count;
	u16 nDeg, nMin, ndMin;

	//计算整数部分数字长度
	u8Count = GPS_GetIntegerLen();

	//整数部分长度<3则出错?
	if (u8Count < 3) {
		//出错
		return FALSE;
	}

	//经度:度
	tGpsInfo.Longtitude[0] = (*rpcGPSMessage - 0x30) << 4;
	tGpsInfo.Longtitude[0] |= *(rpcGPSMessage + 1) - 0x30;
	tGpsInfo.Longtitude[1] = (*(rpcGPSMessage + 2) - 0x30) << 4;
	tGpsInfo.Longtitude[1] |= *(rpcGPSMessage + 3) - 0x30;

	//经度:分
	tGpsInfo.Longtitude[2] = (*(rpcGPSMessage + 4) - 0x30) << 4;

	//经度:秒
	if (*(rpcGPSMessage + 5) == '.') {
		tGpsInfo.Longtitude[2] |= *(rpcGPSMessage + 6) - 0x30;
		tGpsInfo.Longtitude[3] = (*(rpcGPSMessage + 7) - 0x30) << 4;
		tGpsInfo.Longtitude[3] |= *(rpcGPSMessage + 8) - 0x30;
	} else {
		tGpsInfo.Longtitude[2] &= 0xF0;
		tGpsInfo.Longtitude[3] = 0;
	}

	//“度(dd)”的字符个数
	u8Count -= 2;

	//获取“度(dd)”
	nDeg = (u16) GPS_StrToByte(u8Count);

	//获取“分(mm)”
	nMin = (u16) GPS_StrToByte(2);

	//跳过小数点
	if (*rpcGPSMessage == '.') {
		rpcGPSMessage++;

		//获取“分小数部分(mmmm)”
		ndMin = (u16) GPS_StrToWord(4);
	} else {
		ndMin = 0;
	}

	//计算经度，单位为10^-3分
	tGpsInfo.s32Longitude = (nDeg * 60 + nMin) * 1000 + ndMin * 0.1;

	//跳过“经度,”
	if (!GPS_JumpToNextSegment()) {
		//数据错误
		return FALSE;
	}

	//判断东/西经
	if (*rpcGPSMessage == 'W') {
		//西经
		tGpsInfo.s32Longitude = -tGpsInfo.s32Longitude;
		//西经，最高位置1
		tGpsInfo.Longtitude[0] |= 0x80;
	}

	return TRUE;
}

/***********************************************************************************************************
 * 功能描述：	取速度
 * 格	式：	单位为节，1节=1海里/时=1.852Km/h。
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	无
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
void GPS_GetSpeed() {
	u16 u8Count;
	u16 nNodeI = 0, nNodeD = 0;
	u8 u8Speed_Hundred;
	u8 u8Speed_Ten;
	u8 u8Speed_One;
	u8 u8Speed;
	u16 u16Speed;

	//计算整数部分数字长度
	u8Count = GPS_GetIntegerLen();

	//当前位置不是小数点?
	if (u8Count) {
		//获取“节整数部分”
		nNodeI = (u16) GPS_StrToWord(u8Count);
	}

	//跳过小数点
	if (*rpcGPSMessage == '.') {
		rpcGPSMessage++;

		//获取“节小数部分”
		nNodeD = (u16) GPS_StrToWord(3);
	}

	//计算速度，单位为0.1公里/时
	u16Speed = (nNodeI + nNodeD * 0.001) * 18.52;

	//进行速度平滑，实际速度取平滑后的值
	tGpsInfo.u16Speed = GPS_SpeedAverage(u16Speed);

	//调度屏，速度
	u8Speed = tGpsInfo.u16Speed * 0.1;		//公里/时
	u8Speed_Hundred = u8Speed * 0.01;	//百位
	u8Speed_Ten = (u8Speed - u8Speed_Hundred * 100) * 0.1;	//十位
	u8Speed_One = u8Speed - u8Speed_Hundred * 100 - u8Speed_Ten * 10;	//个位
			/*
			 速度表示方法：
			 表示范围：0——9999公里/小时
			 采用压缩BCD编码。
			 [例如]： 120公里/小时表示为：
			 01H，20H
			 */
	tGpsInfo.Speed[0] = u8Speed_Hundred;
	tGpsInfo.Speed[1] = u8Speed_Ten << 4;
	tGpsInfo.Speed[1] |= u8Speed_One;
}

/***********************************************************************************************************
 * 功能描述：	取航向
 * 格	式：	真北航向:	格式=1.1 - 3.2。取值范围为000.0~359.9 。
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	无
 * 设    计：	廖  颖，2007-12-20
 ***********************************************************************************************************/
void GPS_GetHeading() {
	/*
	 方位表示方法：
	 表示范围：000——359度
	 采用压缩BCD编码，正北为0度，顺时针方向计数。
	 单位为：度。
	 [例如]： 154度表示为：
	 01H，54H
	 */
	//方向角单位由分改为度。 fixed by GZD,2009-5-21
	u16 u16Head = GPS_StrToFloat();
	if (u16Head < 360) {
		tGpsInfo.u16Heading = u16Head;
	}

	//航向
	tGpsInfo.Direction[0] = tGpsInfo.u16Heading / 100;
	tGpsInfo.Direction[1] = ((tGpsInfo.u16Heading / 10) % 10) << 4;
	tGpsInfo.Direction[1] |= tGpsInfo.u16Heading % 10;
}

/***********************************************************************************************************
 * 功能描述：	取高度
 * 格	式：	海拔高度:	格式=1.1 - 5.1。海拔以下时首字符为"-"，取值范围为-9999.9~99999.9。单位为1米。
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	无
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
void GPS_GetHeight() {
	//获取高度，单位=1米
	tGpsInfo.s16Altitude = (short) (GPS_StrToFloat() + 0.5);

	/*	//航向
	 tGpsInfo.Altitude[0] = tGpsInfo.s16Altitude / 100;
	 tGpsInfo.Altitude[1] = ((tGpsInfo.s16Altitude / 10) % 10) << 4;
	 tGpsInfo.Altitude[1] |= tGpsInfo.s16Altitude % 10;

	 if(tGpsInfo.s16Altitude < 0)  //负数，最高位置1
	 {
	 tGpsInfo.Altitude[0] |= 0x80;
	 }*/
}

/***********************************************************************************************************
 * 功能描述：	ASCII数字字符串转换成整数
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	u16,			整型数
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
u16 GPS_StrToInteger() {
	u16 nResult = 0;			//结果=0
	s16 nSign = 1;				//符号位=正数

	//跳过空格
	while (*rpcGPSMessage == ' ') {
		rpcGPSMessage++;
	}

	//负数?
	if (*rpcGPSMessage == '-') {
		nSign = -1;

		rpcGPSMessage++;
	}

	//将当前字段转换成整型数
	while (1) {
		//计算
		u8 c = *rpcGPSMessage;

		//转换0-9数字
		if ((c >= '0') && (c <= '9')) {
			c -= 0x30;
		} else {
			break;
		}

		nResult = nResult * 10 + (u16) c;

		//下一个
		rpcGPSMessage++;
	}

	if (nResult) {
		nResult *= nSign;
	}

	//返回整型数结果
	return nResult;
}

/***********************************************************************************************************
 * 功能描述：	ASCII数字字符串转换成浮点数
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	u16,			整型数
 * 设    计：	廖  颖，2007-12-20
 ***********************************************************************************************************/
float GPS_StrToFloat() {
	float fResult = 0.0, fE = 10.0;		//结果=0
	s16 nSign = 1;						//符号位=正数
	u16 nPou16er = 0;

	//跳过空格
	while (*rpcGPSMessage == ' ') {
		rpcGPSMessage++;
	}

	//负数?
	if (*rpcGPSMessage == '-') {
		nSign = -1;

		rpcGPSMessage++;
	}

	//将当前字段转换成整型数
	while (1) {
		//计算
		u8 c = *rpcGPSMessage;

		//转换0-9数字
		if ((c >= '0') && (c <= '9')) {
			c -= 0x30;

			if (!nPou16er) {
				fResult = (float) (fResult * 10.0) + (float) c;
			} else {
				fResult += (float) c / fE;
				fE *= 10.0;
			}
		} else if ((nPou16er == 0) && (c == '.')) {
			nPou16er++;
		} else {
			break;
		}

		//下一个
		rpcGPSMessage++;
	}

	if ((fResult != 0.0) && (nSign < 0)) {
		fResult *= nSign;
	}

	//返回结果
	return fResult;
}

/***********************************************************************************************************
 * 功能描述：	取字节数据
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 *				u8Count,			[IN] 为需要转换的字符数量
 * 出    口：	u8,			字节内容
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
u8 GPS_StrToByte(u8 u8Count) {
	u8 cResult = 0;

	while (1) {
		//计算
		u8 c = *rpcGPSMessage;

		//是0-9数字否?
		if ((c >= '0') && (c <= '9')) {
			c -= 0x30;
		} else {
			//其它字符，退出
			break;
		}

		cResult = cResult * 10 + c;

		//下一个
		rpcGPSMessage++;

		//字符个数-1
		if (u8Count > 0) {
			u8Count--;

			if (u8Count == 0) {
				break;
			}
		}
	}

	return cResult;
}

/***********************************************************************************************************
 * 功能描述：	取字数据
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 *				u8Count,			[IN] 为需要转换的字符数量
 * 出    口：	u16,			字内容
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
u16 GPS_StrToWord(u8 u8Count) {
	u16 wResult = 0;

	while (1) {
		u8 c;

		//计算
		c = *rpcGPSMessage;

		//转换数字
		if ((c >= '0') && (c <= '9')) {
			c -= 0x30;
		} else {
			//其它字符，退出
			break;
		}

		//累计
		wResult = wResult * 10 + (u16) c;

		//下一个
		rpcGPSMessage++;

		//字符个数-1
		if (u8Count > 0) {
			u8Count--;
			if (u8Count == 0) {
				break;
			}
		}
	}

	//不够字符数时，尾部补0(十进制数)
	while (u8Count) {
		wResult *= 10;

		//字符个数-1
		u8Count--;
	}

	return wResult;
}

/***********************************************************************************************************
 * 功能描述：	计算整数部分数字长度
 * 入    口：	rpcGPSMessage,	[IN] GPS响应字串指针引用
 * 出    口：	u16,			小数点位置
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
u16 GPS_GetIntegerLen() {
	u16 nPou16Pos = 0;
	u8* pcMsg = rpcGPSMessage;

	while (1) {
		u8 c = *pcMsg;

		//结束?
		if ((c < '0') || (c > '9')) {
			//不是0-9，则结束
			break;
		}

		pcMsg++;

		//整数部分数字长度+1
		nPou16Pos++;
	}

	//返回整数部分数字长度
	return nPou16Pos;
}

/***********************************************************************************************************
 * 功能描述：	计算平均速度
 * 入    口：	u16Speed,	[IN] 当前速度(10m/h)
 * 出    口：	无
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
u16 GPS_SpeedAverage(u16 u16Speed) {
	u16 u16AverageSpeed;

	tSpeedSmooth.aSpeedArray[4] = tSpeedSmooth.aSpeedArray[3];
	tSpeedSmooth.aSpeedArray[3] = tSpeedSmooth.aSpeedArray[2];
	tSpeedSmooth.aSpeedArray[2] = tSpeedSmooth.aSpeedArray[1];
	tSpeedSmooth.aSpeedArray[1] = tSpeedSmooth.aSpeedArray[0];
	tSpeedSmooth.aSpeedArray[0] = u16Speed;

	//速度计数器+1
	if (tSpeedSmooth.u16SpeedArray_Counter < 5) {
		tSpeedSmooth.u16SpeedArray_Counter++;
	}

	if (tSpeedSmooth.u16SpeedArray_Counter >= 5) {
		//速度突变(>8Km/h)?
		if (abs(u16Speed, tSpeedSmooth.u16AverageSpeed) > 800) {
			tSpeedSmooth.u16SpeedArray_Counter = 2;
		}
	}

	//速度平均
	switch (tSpeedSmooth.u16SpeedArray_Counter) {
	//2点：平均速度 = (速度i + 速度i-1) / 2
	case 2:
		u16AverageSpeed = (u16) ((tSpeedSmooth.aSpeedArray[0]
				+ tSpeedSmooth.aSpeedArray[1]) / 2);
		break;

		//3点：平均速度 = 0.3 * 速度i + 0.45 * 速度i-1 + 0.25 * 速度i-2
	case 3:
		u16AverageSpeed = (u16) ((tSpeedSmooth.aSpeedArray[0] * 6
				+ tSpeedSmooth.aSpeedArray[1] * 9
				+ tSpeedSmooth.aSpeedArray[2] * 5) / 20);
		break;

		//4点：平均速度 = 0.25 * 速度i + 0.4 * 速度i-1 + 0.2 * 速度i-2 + 0.15 * 速度i-3
	case 4:
		u16AverageSpeed = (u16) ((tSpeedSmooth.aSpeedArray[0] * 5
				+ tSpeedSmooth.aSpeedArray[1] * 8
				+ tSpeedSmooth.aSpeedArray[2] * 4
				+ tSpeedSmooth.aSpeedArray[3] * 3) / 20);
		break;

		//5点：平均速度 = 0.2 * 速度i + 0.375 * 速度i-1 + 0.175 * 速度i-2 + 0.15 * 速度i-3 + 0.1 * 速度i-4
	case 5:
		u16AverageSpeed = (u16) ((tSpeedSmooth.aSpeedArray[0] * 8
				+ tSpeedSmooth.aSpeedArray[1] * 15
				+ tSpeedSmooth.aSpeedArray[2] * 7
				+ tSpeedSmooth.aSpeedArray[3] * 6
				+ tSpeedSmooth.aSpeedArray[4] * 4) / 40);
		break;

		//1点：平均速度 = 速度i
	case 1:
	default:
		u16AverageSpeed = u16Speed;
		break;
	}

	tSpeedSmooth.u16AverageSpeed = u16AverageSpeed;

	return u16AverageSpeed;
}

/***********************************************************************************************************
 * 功能描述：	高度平滑
 * 入    口：	shAltitude,	[IN] 当前高度(m)
 * 出    口：	无
 * 设    计：	廖  颖，2005-11-16
 ***********************************************************************************************************/
void GPS_AltitudeAverage(s16 shAltitude) {
	s16 shAverageAltitude;

	tAltitudeSmooth.aAltitudeArray[4] = tAltitudeSmooth.aAltitudeArray[3];
	tAltitudeSmooth.aAltitudeArray[3] = tAltitudeSmooth.aAltitudeArray[2];
	tAltitudeSmooth.aAltitudeArray[2] = tAltitudeSmooth.aAltitudeArray[1];
	tAltitudeSmooth.aAltitudeArray[1] = tAltitudeSmooth.aAltitudeArray[0];
	tAltitudeSmooth.aAltitudeArray[0] = shAltitude;

	//高度计数器+1
	if (tAltitudeSmooth.u32AltitudeArray_Counter < 5) {
		tAltitudeSmooth.u32AltitudeArray_Counter++;
	}

	if (tAltitudeSmooth.u32AltitudeArray_Counter >= 5) {
		//高度突变(>200m)?
		if (abs(shAltitude,tAltitudeSmooth.u16AverageAltitude) > 200) {
			tAltitudeSmooth.u32AltitudeArray_Counter = 2;
		}
	}

	//速度高度
	switch (tAltitudeSmooth.u32AltitudeArray_Counter) {
	//2点：平均高度 = (高度i + 高度i-1) / 2
	case 2:
		shAverageAltitude = (s16) ((tAltitudeSmooth.aAltitudeArray[0]
				+ tAltitudeSmooth.aAltitudeArray[1]) / 2);
		break;

		//3点：平均高度 = 0.3 * 高度i + 0.45 * 高度i-1 + 0.25 * 高度i-2
	case 3:
		shAverageAltitude = (s16) ((tAltitudeSmooth.aAltitudeArray[0] * 6
				+ tAltitudeSmooth.aAltitudeArray[1] * 9
				+ tSpeedSmooth.aSpeedArray[2] * 5) / 20);
		break;

		//4点：平均高度 = 0.25 * 高度i + 0.4 * 高度i-1 + 0.2 * 高度i-2 + 0.15 * 高度i-3
	case 4:
		shAverageAltitude = (s16) ((tAltitudeSmooth.aAltitudeArray[0] * 5
				+ tAltitudeSmooth.aAltitudeArray[1] * 8
				+ tAltitudeSmooth.aAltitudeArray[2] * 4
				+ tSpeedSmooth.aSpeedArray[3] * 3) / 20);
		break;

		//5点：平均高度 = 0.2 * 高度i + 0.375 * 高度i-1 + 0.175 * 高度i-2 + 0.15 * 高度i-3 + 0.1 * 速度i-4
	case 5:
		shAverageAltitude = (s16) ((tAltitudeSmooth.aAltitudeArray[0] * 8
				+ tAltitudeSmooth.aAltitudeArray[1] * 15
				+ tSpeedSmooth.aSpeedArray[2] * 7
				+ tAltitudeSmooth.aAltitudeArray[3] * 6
				+ tAltitudeSmooth.aAltitudeArray[4] * 4) / 40);
		break;

		//1点：平均高度 = 高度i
	case 1:
	default:
		shAverageAltitude = shAltitude;
		break;
	}

	tAltitudeSmooth.aAltitudeArray[0] = (s16) shAverageAltitude;
	tAltitudeSmooth.u16AverageAltitude = shAverageAltitude;
}

/*设置天线状态*/
void GPS_SetAntenaState(u8 state) {
	tGpsInfo.bAntenaState = state;
}

/*读取天线状态*/
u8 GPS_GetAntenaState() {

	return tGpsInfo.bAntenaState;
}

//获取当前GPS状态
_tGPSInfo* GPS_GetInfo() {
	return &tGpsInfo;
}

//是否定位成功
BOOL GPS_IsLocated() {
	if (tGpsInfo.u16GpsInfoValid & _GPSINFO_GPS_VALID)
		return TRUE;

	return FALSE;
}

//同步系统时间
void GPS_SyncSystemTime() {
	SYSTIMER timer;

	timer.year = tGpsInfo.u16Year - 2000;
	timer.month = tGpsInfo.u8Month;
	timer.day = tGpsInfo.u8Day;
	timer.hour = tGpsInfo.u8Hour;
	timer.minute = tGpsInfo.u8Minute;
	timer.second = tGpsInfo.u8Second;

	SysTimer_Set(&timer);

	TRACE_(QS_USER, NULL, "[GPS] Sync time: %d-%d-%d:%d-%d-%d", timer.year,
			timer.month, timer.day, timer.hour, timer.minute, timer.second);
}

//上报GPS点位信息
void GPS_ReportGpsInfo() {
	/**
	 TaskEvt	*pe;

	 pe = Q_NEW(TaskEvt, NEW_TASKSENDREQ_SIG);
	 pe->cmd = EVS15_CMD_GPS_AUTOREPORT;
	 pe->sequence = GeneratePacketSequenceNumber();
	 pe->address = 0;
	 pe->dataSize = 0;
	 pe->ret = 0;
	 pe->checksum = 0;
	 QACTIVE_POST(AO_Gprs, (QEvt*)pe, (void*)0);
	 */
}

//打印GPS调试信息
void TRACE_GPSINFO(/*_tGPSInfo *pGpsInfo*/) {
	TRACE_(QS_USER, NULL,
			"[GPS] %d-%d-%d:%d-%d-%d, Lon=%x.%x%x%x, Lat=%x.%x%x%x, Speed=%x%x, view_S=%x, visib_S=%x",
			tGpsInfo.u16Year, tGpsInfo.u8Month, tGpsInfo.u8Day, tGpsInfo.u8Hour,
			tGpsInfo.u8Minute, tGpsInfo.u8Second, tGpsInfo.Longtitude[0],
			tGpsInfo.Longtitude[1], tGpsInfo.Longtitude[2],
			tGpsInfo.Longtitude[3], tGpsInfo.Latitude[0], tGpsInfo.Latitude[1],
			tGpsInfo.Latitude[2], tGpsInfo.Latitude[3], tGpsInfo.Speed[0],
			tGpsInfo.Speed[1], tGpsInfo.u8ValidSatellites,
			tGpsInfo.u8VisibleSatellites);
}

//存储GPS数据，
void GPS_DumpInfo() {
//	_tGPSInfo* pGpsInfo;
//	_tGPSSaveInfo tInfo;
//
//	pGpsInfo = GPS_GetInfo();	//取当前位置
//	tInfo.bInfoValid = GPS_IsLocated();
//	tInfo.u8VisibleSatellites = pGpsInfo->u8VisibleSatellites;//可见卫星数
//	tInfo.u8ValidSatellites = pGpsInfo->u8ValidSatellites;//有效卫星数
//	memcpy_(tInfo.Longtitude, pGpsInfo->Longtitude, 4);//经度
//	memcpy_(tInfo.Latitude, pGpsInfo->Latitude, 4);
//	tInfo.Direction[0] = pGpsInfo->Altitude[0];//海拔高度
//	tInfo.Direction[1] = pGpsInfo->Altitude[1];
//	tInfo.Speed[0] = pGpsInfo->Speed[0];//速度
//	tInfo.Speed[1] = pGpsInfo->Speed[1];
//	tInfo.u16Year = pGpsInfo.u16Year;
//	tInfo.u8Month = pGpsInfo.u8Month;
//	tInfo.u8Day = pGpsInfo.u8Day;
//	tInfo.u8Hour = pGpsInfo.u8Hour;
//	tInfo.u8Minute = pGpsInfo.u8Minute;
//	tInfo.u8Second = pGpsInfo.u8Second;

//	SD_DumpGpsInfo(GPS_GetInfo());	//定位信息保存到SD卡
}

/**
 * GPS硬件初始化
 *
 */
void GPS_Lowlevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPS_RST_PIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPS_ANT_DET_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	///串口初始化
	USART_Configuration(COM_GPS, 9600);

}

