/**   
 * @Title: sd_storage.c
 * @Description: TODO(CAN及GPS历史数据存储管理)
 * @author Zhengd.Gao zhengdgao@163.com
 * @date 2013-8-28 下午4:26:04
 * @version V1.0
 *
 * 功能：SD卡读写管理，存储两类数据：GPS数据和CAN数据
 *
 * 【GPS数据】
 *  按年-月-日建立三级目录，字符串格式，按行存储。
 *
 *
 *
 * 【文件表示】
 *  1、不建立索引文件，最大的原因是索引文件本身维护的开销较大，且难于管理，与记录数据的一致性难以保证。
 *
 *  2、目录结构：SD卡中文件分为CAN和GPS两个顶层目录。
 *
 *  /CAN/year_month_day_hour.can
 *  		...
 *  		year_month_day_hour.can
 *
 *  /GPS/year_month_day.gps
 *  		...
 *      	year_month_day.gps
 *
 *  在{时}目录下，为根据当前小时时刻最大可能创建的24个文件。必须注意的是，同一个文件严格遵守只保存该小时
 *  计时范围内的数据帧。
 *
 *  3、文件格式：
 *  记录格式：时间偏移 + 数据内容
 *  时间偏移为当前时刻相对于0时0分的ms数；
 *  数据内容为定长结构；
 *
 *
 * 【写过程】
 *  存储过程包括缓存及写数据两大部分。
 *  1、CAN驱动在接收到一个CAN帧后，立即调用一次存储过程，通知新帧保存，回调过程执行复杂度应尽可能小，以保证系统响应能力；
 *  2、写时机：由QF框架控制，在检测到系统处理空闲时通知写，方法是在QF_OnIdle()中调用SD卡写回调函数。
 *  SD卡存储单元在得到新帧到达消息后，可选择将CAN帧缓冲；在得到写通知后，将缓存内容写SD卡。
 *
 *
 * 【查找过程】
 *
 *  参数：start_tick(year,month,day,hour,minute,second),
 *  		 end_tick(year,month,day,hour,minute,second)
 *
 *  定位起始文件：
 *
 *  //查找第一个文件，起始小时对应的文件可能不存在，递增小时数依序遍历文件，直至结束小时
 *  for(cur_hour < end_tick.hour)
 *   if(f_open(TIME2FN(year,month,day,cur_hour)))
 *   {
 *   	//定位成功
 *   	break;
 *   }
 *
 *  定位记录：
 *  在文件定位成功后，根据参数的时分秒，换算成秒数，以秒数为关键字，在文件中用二分查找法定位首个记录位置。
 *
 *  多个文件读：
 *  当查询时间段超过1个小时时，会出现记录跨文件的情况。有两种方式：一，先将所有的文件查找出来，在读时不再
 *  查找文件；二，仅以起始时间定位首个文件，在读时判断若当前文件已读取完毕，但未到达指定时间段结束时刻，则
 *  再次搜索写个记录文件并定位记录位置。本实现采用方法二，原因是第一种方法需用额外的内存去管理多个文件列表，
 *  而第二种方法更加实时、简单、高效，这对嵌入式软件来说，应以最简单的方式实现功能要求为要。
 *
 *  存储文件粒度以小时为最小存储划分，若当前写数据进入新的小时记录周期，则关闭当前文件，新建一个新文件写。
 *  粒度的大小对索引效率至关重要，以小时为单位划分，索引次数为查询时间差，时间复杂度O（n）。
 *
 *  文件不再以一个SESSION周期作为文件完整性保存依据，而是根据时间来控制，原因是以SESSION为单位直观上看
 *  好像比较合理，但存在着文件覆盖的周期不确定，带来索引的难度，且实际上也没有必要这么做，本身查询
 *  就以起始时间和结束时间为条件，而没有以某一个SESSION为条件。
 *
 *
 *
 * 【修改，满足国标GPS项目存储需求，2014-07-01】
 *
 *
 *    /------ Config 配置信息
 * 		|
 * 		|--- Picture 图片
 |
 * 		|--- Audio 音频
 * 		|
 * 		|--- CAN CAN透传数据
 * 		|
 * 		|--- Log  行驶记录
 *				|
 *				|--- DriveSpeed 行驶速度             文件名称：年 - 月 — 日 — 时
 *				|
 *				|--- Accident 事故疑点
 *				|
 *				|--- OvertimeDrive 超时驾驶
 *				|
 *				|--- PositionInfo 位置信息
 *				|
 *				|--- DriverInfo 驾驶人身份
 *				|
 *				|--- Distance 里程
 *				|
 *				|--- InstallParam 安装参数记录
 *				|
 *				|--- Power 外部供电记录
 *				|
 *				|--- ParamChange 参数修改记录
 *				|
 *				|--- SpeedState 速度状态日志
 *
 *
 * 存储数据类型：
 * 1、【行驶速度记录】，1s间隔持续记录并存储行驶状态，包括时间，平均速度，状态，不少于最近48小时
 * 				存储容量：48*3600=172800条 *2B= 345600B = 345K,
 *				存储策略：
 *
 * 2、【事故疑点记录】，0.2秒间隔，持续记录20s，不少于100条
 * 				情况1:行驶时，外部供电断开；
 * 				情况2：行驶时，位置10s内无变化
 * 				存储容量： 234B*100 = 23400B=23K
 *
 * 3、【超时驾驶记录】，不少于100条
 * 				存储容量：50B*100 =5000B=5K

 * 4、【位置信息记录】，1min间隔，360小时
 *				存储容量：360 * 60 = 21600=22K
 *
 * 5、【驾驶人身份记录】，不小于200条
 * 				存储容量：25B*200=5000B=5K
 *
 * 6、【里程记录】
 *
 *
 * 7、【安装参数记录】
 *
 *
 *--------- 日志记录 --------
 * 8、【外部供电记录】，不少于100条
 * 				存储容量：7B*100=700B=1K
 *

 * 9、【参数修改记录】，不少于100条
 * 				存储容量：7B*100=700B=1K


 * 10、【速度状态日志】，从开始时间连续60s，不少于10条
 * 				存储容量：133B*10=1330B=2K
 *
 *
 */

#include "ff.h"
#include "sdcard_interface.h"
#include "utility.h"
#include "diskio.h"
//#include "error.h"
//#include "gps.h"
#include "trace.h"
#include <qp_port.h>
#include <VdrComm.h>

Q_DEFINE_THIS_MODULE("sd_storage.c")

/**记录所在路径*/
static const char* Dir[] = {
		"Speed",			///行驶速度
		"Accident",			/// 事故疑点
		"OvertimeDrive",	/// 超时驾驶
		"PositionInfo", 	/// 位置信息
		"DriverInfo", 		///驾驶人身份
		"Distance",			///里程
		"InstallParam",		///安装参数记录
		"Power",			///外部供电记录
		"ParamChange",		///参数修改记录
		"SpeedState",  		///速度状态日志
};

static const u16 RECORD_SIZE[] = {
		sizeof(tRECORD_SPEED),
		sizeof(tRECORD_ACCIDENT),
		sizeof(tRECORD_OVERTIMEDRIVING),
		sizeof(tRECORD_POS),
		sizeof(tRECORD_DRIVERLOG),
		sizeof(tRECORD_DIST),
		sizeof(tRECORD_INSTALLPARAM ),
		sizeof(tRECORD_POWER),
		sizeof(tRECORD_PARAMCHANGE),
		sizeof(tRECORD_SPEEDSTATUS),
};

#define DUMP_MAXFRAMES_PERCALL		5	//空闲时执行一次存储调用，最大保存帧数
#define GET_TICKS(h,m,s)	((((h)* 3600) + ((m) * 60) + (s))* 1000) //分秒数转换成毫秒

static FATFS fs;	//文件系统

static T_QSESSION qSession;	//查询SESION，同一时刻系统只能有一个SESSION有效


/**BCD码转换成整数*/
#define BCD2INTEGER(byteBCD, byte) 	(byte = (byteBCD >> 4) * 10 +(byteBCD & 0x0F))
/**整数转成BCD码*/
#define INTEGER2BCD(byteBCD, byte)  (byteBCD = ((byte / 10) << 4) + (byte % 10))

/*---------- 本地函数声明 ---------------------------------------------------------------------*/
static s8 time_compare(TIME *t1, TIME *t2);
static BOOL VDR_SeekRecord(u32 tick, u16 u16Recordsize, u32 *pu32Offset);
//static int SD_DumpCANFrame(CANRAWFRAME* pFrame);
static void VDR_SeekNextFileBeginRecord(void);
static void VDR_SeekPreviousFileLastRecord(u16 u16RecordSize);
static int VDR_Dump2File(TIME *pTime, u8 *pBlock, u16 u16BlockLen);
static void VDR_GetGPSInfo(tPOSITION *ptPos);
/*----------—————————---------------------------------------------------------------------*/

/**
 * @Func	SD卡初始化
 * @Param	无
 * @Ret		0=成功，否则为错误代码(RC)
 */
int SD_Init() {
	if (!SD_Detect()) {
		return -ERR_SDCARD_NOTEXISTED;
	}

	//初始化成功
	if (disk_initialize(0) != RES_OK) {
		return -ERR_SDCARD_INIT;
	}

	//注册工作区
	f_mount(0, &fs);

	return 0;
}

/**
 * @Func	清除查询会话状态
 * @Param	class			[in]	查询类型，只能是CAN，GPS
 * @Ret		无
 */
void SD_ClearQuerySession() {
	f_close(&qSession.file);
	ZeroMem((u8*) &qSession, sizeof(T_QSESSION));
}

#if 0
int VDR_NewQuerySession(eLOGCLASS eClass, TIME *pTime_Start, TIME *pTime_End) {
	FRESULT res;
	BOOL bFind = 0;
	FILINFO fno;
	BOOL ret;
	u32 u32Offset;
	int iRet;

	/**清除旧的查询状态*/
	SD_ClearQuerySession(&qSession);

	/**确保结束时刻应大于等于开始时刻*/
	if(-1 != time_compare(pTime_Start, pTime_End))
	return -ERR_PARAM_INVALID;

	/**查找起始文件及第一条记录位置*/
	iRet = VDR_FindStartFileAndRecord(eClass, pTime_Start, pTime_End);
	if(iRet < 0)
	return iRet;

	/**查找结束文件及最后一条记录位置*/
	iRet = VDR_FindStartFileAndRecord(eClass, pTime_Start, pTime_End);
	if(iRet < 0)
	return iRet;

	/*填充会话信息*/
	time_copy(&qSession.t_Start, pTime_Start);
	time_copy(&qSession.t_End, pTime_End);

	/*文件操作完后立即关闭*/
	f_close(&qSession.file);

	return 0;
}
#endif

/**
 * 启动一个新的查询 ，按时间条件定位首条记录位置
 * 所有查询共用一个SESSION，因为同一时刻只能响应一个查询
 *
 * @param start_time	起始时间
 * @param end_time		结束时间
 * @return	0=成功，否则错误代码
 */
int VDR_NewQuerySession(eLOGCLASS eClass, TIME *start_time, TIME *end_time) {
	FRESULT res;
	BOOL bFind = 0;
	FILINFO fno;
	BOOL ret;
	u32 u32Offset;

	/*清除旧的查询状态*/
	SD_ClearQuerySession(&qSession);

	/*填充会话参数*/
	time_copy(&qSession.t_Start, start_time);
	time_copy(&qSession.t_End, end_time);
	qSession.u8SessionType = (u8) eClass;

	/*确保结束时刻应大于等于开始时刻*/
	if (-1 != time_compare(start_time, end_time))
		return -ERR_PARAM_INVALID;

	TRACE_(QS_USER, NULL, "[VDR] ******** NEW Query Session **********");
	TRACE_(QS_USER, NULL, "[VDR] type = %s, from %d-%d-%d:%d-%d-%d to %d-%d-%d:%d-%d-%d ", Dir[eClass],
			start_time->year, start_time->month, start_time->day, start_time->hour, start_time->minute,
			start_time->second, end_time->year, end_time->month, end_time->day, end_time->hour, end_time->minute,
			end_time->second);

	/*循环查询记录文件，直至找到包含开始时间的第一个文件*/
	time_copy(&qSession.t_Cur, &start_time);
	do {
		//将时间转换成文件名
		sprintf_(qSession.fn_Cur, "0:/%s/%d_%d_%d.can",
				Dir[eClass], qSession.t_Cur.year, qSession.t_Cur.month,	qSession.t_Cur.day);
		TRACE_(QS_USER, NULL, "[VDR] Seeking file: %s", qSession.fn_Cur);

		res = f_open(&qSession.file, qSession.fn_Cur, FA_OPEN_EXISTING | FA_READ);	//打开已有文件
		if ((FR_OK == res) && (qSession.file.fsize > 0)) {	//文件存在且非空，查找成功
			bFind = 1;
			break;
		}
		else {
			f_close(&qSession.file);
		}

		time_GetNextDay(&qSession.t_Cur);	//失败，递增小时继续查找
	} while (1 != time_compare(&qSession.t_Cur, &qSession.t_End)); //直至超过结束时间

	if (0 == bFind)	//指定时间段无记录文件，失败
			{
		return -ERR_VDR_FILE_NOTEXIST;
	}

//	TRACE_(QS_USER, NULL, "[VDR] Start file name : %s", qSession.fn_Cur);

	/*保存开始文件名*/
	strcpy_(qSession.fn_Start, qSession.fn_Cur);

	/*下一步开始查找记录起始位置，次数文件句柄处于打开状态，定位记录是根据时分秒数*/
	ret = VDR_SeekRecord(	qSession.file,
							GET_TICKS(start_time->hour, start_time->minute, start_time->second),
							RECORD_SIZE(eClass),
							&qSession.u32Offset_Start);
	if (!ret) {
		return -ERR_VDR_FILE_SEEK;
	}

	/*起始文件使用完后立即关闭*/
	f_close(&qSession.file);

	TRACE_(QS_USER, NULL, "[VDR] Seeking END file >>>>>>>>>> ");

	/*定位最后一条记录所在文件名*/
	time_copy(&qSession.t_End, &end_time);
	do {
		//将时间转换成文件名
		sprintf_(qSession.fn_End, "0:/%s/%d_%d_%d",
				Dir[eClass], qSession.t_End.year, qSession.t_End.month,	qSession.t_End.day);
		TRACE_(QS_USER, NULL, "[VDR] Seeking file: %s", qSession.fn_End);

		res = f_open(&qSession.file, qSession.fn_End, FA_OPEN_EXISTING | FA_READ);	//打开已有文件
		if ((FR_OK == res) && (qSession.file.fsize > 0))	//文件存在且非空，查找成功
				{
			bFind = 1;
			break;
		}
		else {
			f_close(&qSession.file);
		}

		time_GetPrevDay(&qSession.t_End);	//失败，逆序查找前一天
	} while (1 == time_compare(&qSession.t_End, &qSession.t_Start)); //直至到达开始时间

	if (!bFind)
		return -ERR_VDR_FILE_NOTEXIST;

	/*定位最后一条记录位置*/
	ret = VDR_SeekRecord(	qSession.file,
							GET_TICKS(end_time->hour, end_time->minute, end_time->second),
							RECORD_SIZE(eClass),
							&qSession.u32Offset_End);

	/*文件操作完后立即关闭*/
	f_close(&qSession.file);

	if (!ret) {
		return -ERR_VDR_FILE_SEEK;
	}

	TRACE_(QS_USER, NULL, "[VDR] ========= Query Session Result ===========");
	TRACE_(QS_USER, NULL, "[VDR] start file = %s, offset = %d", qSession.fn_Start, qSession.u32Offset_Start);
	TRACE_(QS_USER, NULL, "[VDR] end file = %s, offset = %d", qSession.fn_End, qSession.u32Offset_End);

	return 0;
}

/**
 * 从起始文件第一条记录开始，正序读取单条记录
 *
 * 文件名称为qSession.fn_Cur，当前读位置为qSession.u32Offset_Cur
 * 注意，此函数前提条件是文件必须已打开
 *
 * @param blocks
 * @param u16BlockSize
 * @param pBuf
 * @param bufSize
 * @param pu16RecvBytes
 * @return
 */
int VDR_RetrieveSingleRecordForward(u8 *pBuf, u16 bufSize) {

	FIL *f;
	FILINFO *fno;
	FRESULT res;
	u32 record_size;
	u32 already_read;
	u16 u16RecordSize;

	Q_ASSERT(qSession.file.fs != NULL);	///文件必须已经打开

	if (NULL == pBuf)
		return -ERR_PARAM_INVALID;

	if(NULL == qSession.file.fs)
		return -ERR_VDR_FILE_NOTEXIST;

	f = &qSession.file; ///文件句柄

	u16RecordSize = RECORD_SIZE[qSession.u8SessionType];	///得到各类型单条记录的长度

	f_lseek(f, qSession.u32Offset_Cur);	///定位读指针到待读偏移位置
	res = f_read(f, pBuf, u16RecordSize, &already_read);  ///读取指定条数记录
	if (FR_OK != res) {
		f_close(f);
		return -ERR_SD_WRITE;
	}

	if (already_read != u16RecordSize) {	///实际读取字节数不够，错误
		f_close(f);
		return -ERR_SD_READ;
	}

	qSession.u32Offset_Cur += u16RecordSize;	///更新读指针位置

	/**定位到下一文件的第一条记录*/
	if (qSession.u32Offset_Cur >= f->fsize) {
		f_close(&qSession.file);
		/*已经是结束文件，则读取结束*/
		if (strcmp_(qSession.fn_Cur, qSession.fn_End)) {
			qSession.bAllBlocksFinishedReadout = 1;	///设置读取完毕标志
		}
		else {
			VDR_SeekNextFileBeginRecord();	///读取中间文件
		}
	}

	return 0;
}

/**
 * 定位到下一文件的第一条记录
 */
void VDR_SeekNextFileBeginRecord() {
	FRESULT res;
	u8 bFind = 0;

	/*循环查询记录文件，直至找到包含开始时间的第一个文件*/
	time_GetNextDay(&qSession.t_Cur);	//递增小时继续查找
	while (!strcmp_(qSession.fn_Cur, qSession.fn_End)) {

		//将时间转换成文件名
		sprintf_(qSession.fn_Cur, "0:/%s/%d_%d_%d", Dir[qSession.u8SessionType], qSession.t_Cur.year,
				qSession.t_Cur.month, qSession.t_Cur.day);

		res = f_open(&qSession.file, qSession.fn_Cur, FA_OPEN_EXISTING | FA_READ);	//打开已有文件
		if ((FR_OK == res) && (qSession.file.fsize > 0))	//文件存在且非空，查找成功
				{
			bFind = 1;
			break;
		}
		else {
			f_close(&qSession.file);
		}

		time_GetNextDay(&qSession.t_Cur);
	}

	if ((bFind) || strcmp_(qSession.fn_Cur, qSession.fn_End)) {
		qSession.u32Offset_Cur = 0;
	}
}

/**
 * 从结束文件最后一条记录开始，逆序提取单条记录
 *
 * 文件名称为qSession.fn_Cur，当前读位置为qSession.u32Offset_Cur
 *
 * @param u16BlockSize			记录块大小
 * @param pBuf					接收缓区
 * @param bufSize				接收缓区长度
 * @param pu16RecvBytes			实际读取字节数
 * @return	0=成功，否则错误代码
 */
int VDR_RetrieveSingleRecordBackward(u16 u16RecordSize, u8 *pBuf, u16 bufSize, u16 *pu16ReadBytes) {

	FIL *f;
	FILINFO *fno;
	FRESULT res;
	u32 record_size;
	u32 already_read;

	if (NULL == pBuf)
		return -ERR_PARAM_INVALID;

	/*打开文件*/
	res = f_open(&qSession.file, qSession.fn_Cur, FA_OPEN_EXISTING | FA_READ);	//打开已有文件
	if (FR_OK != res) //文件存在，查找成功
			{
		f_close(&qSession.file);
		return -ERR_SD_FILENOTEXISTED;
	}

	f = &qSession.file; //文件句柄

	f_lseek(f, qSession.u32Offset_Cur);	///定位读指针到待读偏移位置

	res = f_read(f, pBuf, u16RecordSize, &already_read);  ///读取指定条数记录
	if (FR_OK != res) {
		f_close(f);
		return -ERR_SD_WRITE;
	}

	if (already_read != u16RecordSize) {	///实际读取字节数不够，错误
		f_close(f);
		return -ERR_SD_READ;
	}

	*pu16ReadBytes = u16RecordSize; 	///返回实际读取字节数

	if (qSession.u32Offset_Cur > 0) {
		Q_ASSERT(qSession.u32Offset_Cur >= u16RecordSize);
		qSession.u32Offset_Cur -= u16RecordSize;
	}
	else {
		/**定位到前一文件的最后一条记录*/
		f_close(&qSession.file);
		if (strcmp_(qSession.fn_Cur, qSession.fn_Start))
			qSession.bAllBlocksFinishedReadout = 1;	///设置读取完毕标志
		else
			VDR_SeekPreviousFileLastRecord(u16RecordSize);
	}

	return 0;
}

/**
 * 定位到前一文件的最后一条记录
 *
 * @param u16RecordSize		单条记录长度
 */
void VDR_SeekPreviousFileLastRecord(u16 u16RecordSize) {
	FRESULT res;
	u8 bFind = 0;

	/*循环查询记录文件，直至找到包含开始时间的第一个文件*/

	time_GetPrevDay(&qSession.t_Cur);	//递增小时继续查找

	while (!strcmp_(qSession.fn_Cur, qSession.fn_Start)) {

		//将时间转换成文件名
		sprintf_(qSession.fn_Cur, "0:/%s/%d_%d_%d",
				Dir[qSession.u8SessionType], qSession.t_Cur.year, qSession.t_Cur.month, qSession.t_Cur.day);

		res = f_open(&qSession.file, qSession.fn_Cur, FA_OPEN_EXISTING | FA_READ);	//打开已有文件
		if ((FR_OK == res) && (qSession.file.fsize > 0)) {	//文件存在且非空，查找成功
			bFind = 1;
			break;
		}
		else {
			f_close(&qSession.file);
		}

		time_GetPrevDay(&qSession.t_Cur);
	}

	if ((bFind) || strcmp_(qSession.fn_Cur, qSession.fn_Start)) {
		qSession.u32Offset_Cur = qSession.file.fsize - u16RecordSize;
	}
}

/**
 * 判断记录是否全部读完
 *
 * @return	TRUE=完成，FALSE=未完成
 */
BOOL VDR_IsReadFinished() {

	if(qSession.bAllBlocksFinishedReadout == 1)
		return TRUE;

	return FALSE;
}


/**
 * @Func	时间拷贝
 * @Param	t1			[in]	时间1
 * 			t2			[in]	时间2
 * @Ret		无
 */
void time_copy(TIME *t1, TIME *t2) {
	t1->year = t2->year;
	t1->month = t2->month;
	t1->day = t2->day;
	t1->hour = t2->hour;
	t1->minute = t2->minute;
	t1->second = t2->second;
}

/**
 * 计算下一日历天
 *
 * @param t		当前日历天
 */
void time_GetNextDay(TIME *t) {
	u8 u8DaysofMonth = 0;

	t->day++;

	u8DaysofMonth = MISC_DaysofMonth(t->year/* + 2000*/, t->month); //取得当前月有多少天
	if (t->day >= u8DaysofMonth) {
		t->month += 1;
		t->day = 1;

		if (t->month > 12) {
			t->month = 1;
			t->year += 1;
		}
	}
}

/**
 * 计算前一日历天
 *
 * @param t		当前日历天
 */
void time_GetPrevDay(TIME *t) {
	u8 u8DaysofMonth;

	if (t->day > 1)
		t->day--;
	else {
		if (t->month > 1) {
			t->month--;
		}
		else {
			t->year--;
			t->month = 12;
			u8DaysofMonth = MISC_DaysofMonth(t->year, t->month);
			t->day = u8DaysofMonth;
		}
	}
}

#if 0
/**
 * @Func	计算下一个小时表示
 * @Param	t1			[in]	原时间
 * @Ret		t1			[out]	递增1小时后的时间
 */
void time_nexthour(TIME *t)
{
	t->hour++;
	if( t->hour >= 24)
	{
		u8 u8DaysofMonth;
		t->hour = 0;

		u8DaysofMonth = MISC_DaysofMonth(t->year/* + 2000*/, t->month); //取得当前月有多少天
		if(t->day >= u8DaysofMonth)
		{
			t->month += 1;
			t->day = 1;

			if(t->month > 12)
			{
				t->month = 1;
				t->year += 1;
			}
		}
	}
}

/**
 * 计算前一个小时表示
 *
 * @param t
 */
void time_prevhour(TIME *t) {
	u8 u8DaysofMonth;

	if(t->hour > 0)
	t->hour --;
	else {
		t->hour = 23;
		if(t->day > 1)
		t->day --;
		else {
			if(t->month > 1) {
				t->month --;
			}
			else {
				t->year --;
				t->month = 12;
				u8DaysofMonth = MISC_DaysofMonth(t->year, t->month); //取得当前月有多少天
				t->day = u8DaysofMonth;
			}
		}
	}
}
#endif

/**
 * @Func	时间比较
 * @Param	t1			[in]	时间1
 * 			t2			[in]	时间2
 * @Ret		-1:t1<t2; 0:t1=t2; 1:t1>t2
 */
s8 time_compare(TIME *t1, TIME *t2) {
	if (t1->year > t2->year)
		return 1;
	if (t1->year < t2->year)
		return -1;
	if (t1->month > t2->month)
		return 1;
	if (t1->month < t2->month)
		return -1;
	if (t1->day > t1->day)
		return 1;
	if (t1->day < t1->day)
		return -1;
	if (t1->hour > t2->hour)
		return 1;
	if (t1->hour < t2->hour)
		return -1;
	if (t1->minute > t2->minute)
		return 1;
	if (t1->minute < t2->minute)
		return -1;
	if (t1->second > t2->second)
		return 1;
	if (t1->second < t2->second)
		return -1;

	return 0;
}

/**
 * 查找首条记录所在文件位置，折半查找
 *
 * @param file				已打开的文件句柄
 * @param tick				时间
 * @param u16Recordsize		单位记录长度
 * @param pu32Offset	OUT	在文件中的偏移量
 * @return
 */
BOOL VDR_SeekRecord(FIL *file, u32 tick, u16 u16Recordsize, u32 *pu32Offset) {
	FIL *f;
	FRESULT res;
	UINT read;
	TIME time;
	u32 u32Offset = 0;

	u32 low = 0, high = 0, mid = 0;
	u32 mid_ticks = 0;

	if (NULL == file)
		return FALSE;

	f = file;

	low = 0;
	high = qSession.file.fsize;

	/*定位首条记录位置*/
	f_lseek(f, 0); //指向文件头

	if (0 == tick) //以0分0秒开始，则不需查找
			{
		*pu32Offset = 0;
		return TRUE;
	}

	mid_ticks = GET_TICKS(time.hour, time.minute, time.second);

	if (tick < mid_ticks) //首条记录的时间大于请求查询时间，不需再查找了
			{
		f_lseek(f, 0);
		return TRUE;
	}

	/*其他情况下，折半查找*/
	while (low + u16Recordsize < high) {
		mid = (high - low) / u16Recordsize;
		mid = low + (mid >> 1) * u16Recordsize;
		res = f_lseek(f, mid); //读指针调整到折半记录处
		if (res != FR_OK) {
			f_close(f);
			return FALSE;
		}

		res = f_read(f, (u8*) &time, u16Recordsize, &read); //读文件折半处的记录
		if (res != FR_OK) {
			f_close(f);
			return FALSE;
		}

		mid_ticks = GET_TICKS(time.hour, time.minute, time.second);

		if (tick > mid_ticks)
			low = mid;	//时间较小
		else if (tick < mid_ticks)
			high = mid;	//时间较大
		else if (tick == mid_ticks)
			low = mid;
	}

	f_lseek(f, mid); //文件读指针跳到首条记录位置

	*pu32Offset = mid;

	return TRUE;
}

#if 0
/**
 * @Func	请求存储原始帧，注意在接收到一帧后，应立即调用该过程
 * @Param	pNode			[in]	节点
 * @Ret		无
 */
void SD_RequestDumpCANFrame(DATANODE* pNode)
{
	if(pNode)
	pNode->reference ++;
}

/**
 * @Func	存储原始帧
 * @Param	pFrame			[in]	帧数据
 * @Ret		0=成功，否则为错误代码(RC)
 */
void SD_DoDumpCANFrame()
{
	DATANODE* pNode;
	u8 dump_times = 0;

	pNode = MP_GetAllocNodeHead(); //取分配链表首节点

	while(pNode && dump_times < DUMP_MAXFRAMES_PERCALL)
	{
		SD_DumpCANFrame((CANRAWFRAME*)pNode->data); //存储，无论是否成功都释放节点
		MP_DecountReference(pNode);
		MP_FreeNode(pNode);//释放节点

		pNode = pNode->next;//分配链表的后续节点
		dump_times ++;
	}
}

/**
 * @Func	存储原始帧
 * @Param	pFrame			[in]	帧数据
 * @Ret		0=成功，否则为错误代码(RC)
 */
int SD_DumpCANFrame(CANRAWFRAME* pFrame)
{
	char fn[50];
	TIME timer;
	FRESULT res;
	u32 bw = 0;

	SysTimer_GetRaw(&timer); //取当前时间

	encode((u8*)pFrame, sizeof(CANRAWFRAME));//数据加密

	sprintf_(fn, "0:/CAN/%d_%d_%d_%d.can",
			timer.year, timer.month, timer.day, timer.hour);//将时间转换成文件名
	TRACE_(QS_USER, NULL, "[SD] trying to Seek file: %s", fn);

	if(strcmp_(fn, can_wsession.fn_Cur))//文件已存在
	{
		if(NULL == can_wsession.file) //首次打开
		{
			//打开文件，不存在创建新文件
			res = f_open(can_wsession.file, can_wsession.fn_Cur, FA_OPEN_ALWAYS | FA_WRITE);
			if(FR_OK == res)//文件存在，查找成功
			{
				f_lseek(can_wsession.file, 0);
			}
			else
			{
				f_close(can_wsession.file);
				return -res;
			}
		}

		res = f_write(can_wsession.file, (u8*)pFrame, sizeof(CANRAWFRAME), &bw); //写记录
		if(res || (bw != sizeof(CANRAWFRAME)))
		{
			return -res;
		}
	}
	else
	{
		strcpy_(can_wsession.fn_Cur, fn); //保留当前文件名

		if(can_wsession.file)//关闭旧文件
		{
			f_close(can_wsession.file);
		}

		//打开文件，不存在创建新文件
		res = f_open(can_wsession.file, can_wsession.fn_Cur, FA_OPEN_ALWAYS | FA_WRITE);
		if(FR_OK == res)//文件存在，查找成功
		{
			res = f_write(can_wsession.file, (u8*)pFrame, sizeof(CANRAWFRAME), &bw); //写记录
			if(res || (bw != sizeof(CANRAWFRAME)))
			{
				return -res;
			}
		}
		else
		{
			f_close(can_wsession.file);
			return -res;
		}
	}

	return 0;
}
#endif


/**
 * 保存单条记录，包括入缓区及写文件
 *
 * @param pTime			时刻
 * @param pRecord		单条记录
 * @return	0=成功，否则错误代码
 */
int VDR_DumpRecord_Speed(TIME *pTime, tSPEEDPERSEC *pRecord) {
	static tRECORD_SPEED tSpeedLogBuf;	///本地缓区
	tTIME time_BCD;
	int ret = 0;

	TIME2BCDTIME(pTime, &time_BCD);	///时间格式转换成BCD时间

	/**保存起始时间*/
	if (GET_TICKS(pTime->hour, pTime->minute, pTime->second) == 0) {
		BCDTIME_CPY(&tSpeedLogBuf.time_Start, &time_BCD);
	}

	/**单条记录入缓区*/
	memcpy_((u8*)&tSpeedLogBuf.tSpeed[pTime->second], (u8*)pRecord, sizeof(tRECORD_SPEED));

	/**最后一秒种时写文件*/
	if(pTime->second == 59)	{
		ret = VDR_Dump2File(eLOG_Speed, tSpeedLogBuf.time_Start, (u8*)&tSpeedLogBuf, sizeof(tRECORD_SPEED));
		if(ret < 0) {
			ZeroMem((u8*)&tSpeedLogBuf, sizeof(tRECORD_SPEED));
			return ret;
		}
		ZeroMem((u8*)&tSpeedLogBuf, sizeof(tRECORD_SPEED));
	}

	return 0;
}

/**
 * 保存事故疑点记录
 *
 * @param pTime			时间
 * @param pRecord		单条记录
 * @return
 */
int VDR_DumpRecord_Accident(TIME *pTime, tITEM_ACCIDENT *pRecord) {
	static tRECORD_ACCIDENT tLogBuf;	///本地缓区
	static u8 u8Cur = 0;	///保存记录条数计数器
	tTIME time_BCD;
	int ret = 0;
	tLICENSE_ST *pLicense;

	TIME2BCDTIME(pTime, &time_BCD);	///时间格式转换成BCD时间

	memcpy_((u8*)&tLogBuf.atItem[u8Cur++], (u8*)pRecord, sizeof(tITEM_ACCIDENT));

	/*连续接受100个记录后，写入文件*/
	if(u8Cur == 100) {
		/**填充附加信息*/
		BCDTIME_CPY(&tLogBuf.time_End, &time_BCD);	///结束时间
		VDR_GetGPSInfo(&tLogBuf.tLastValidPos);		///位置信息
		pLicense = (tLICENSE_ST*)VDR_GetParam(VDR_PARAM_LICENSE);	///车牌号
		memcpy_(tLogBuf.aLicense, pLicense->aLicense, 18);

		/**记录块写入文件*/
		ret = VDR_Dump2File(eLOG_Speed, tLogBuf.time_End, (u8*)&tLogBuf, sizeof(tRECORD_ACCIDENT));
		if(ret < 0) {
			ZeroMem((u8*)&tLogBuf, sizeof(tRECORD_ACCIDENT));
			u8Cur = 0;
			return ret;
		}
		ZeroMem((u8*)&tLogBuf, sizeof(tRECORD_ACCIDENT));
		u8Cur = 0;
	}

	return 0;
}

/**
 * 保存无缓冲的单条记录
 *
 * 这些类型的记录周期不是很频繁，接受到后立即写文件，包括：超时驾驶记录，位置信息记录，驾驶人信息记录，外部供电记录，参数修改记录，速度状态记录
 *
 * @param pTime				时间
 * @param pRecord			记录
 * @param u16BlockLen		记录长度
 * @return	0=成功，否则错误代码
 */
int VDR_DumpRecord_Instant2File(TIME *pTime, u8 *pRecord, u16 u16BlockLen) {

	return VDR_Dump2File(pTime, pRecord, u16BlockLen);
}


/**
 * 保存记录，内容事先已准备好，是所有类型参数写文件的通用存储接口
 *
 * @param eClass		记录类型
 * @param pTime			时间日期
 * @param pBlock		记录数据块
 * @param u16BlockLen	记录数据块长度
 * @return
 */
int VDR_Dump2File(TIME *pTime, u8 *pBlock, u16 u16BlockLen) {
	char fname[20];
	FRESULT res;
	FIL file;
	u32 bw;

	if ((pBlock != NULL ) && (u16BlockLen > 0))
		return -ERR_PARAM_INVALID;

	/**将时间转换成文件名*/
	sprintf_(fname, "0:/%s/%d_%d_%d",
			Dir[eLOG_Accident], pTime->year, pTime->month, pTime->day);

	/**打开已有文件，写到文件尾*/
	res = f_open(&file, fname, FA_OPEN_EXISTING | FA_WRITE);
	if (FR_OK == res) {
		f_lseek(file, file.fsize);
		res = f_write(file, pBlock, u16BlockLen, &bw); //写记录
		if (res || (bw != u16BlockLen)) {
			f_close(&file);
			return -ERR_SD_WRITE;
		}
	}
	else {
		/**创建新文件，记录写入文件头*/
		res = f_open(&file, fname, FA_CREATE_NEW | FA_WRITE);
		if (FR_OK != res) {
			f_close(&file);
			return -ERR_SD_FILECREATEFAILED;
		}

		f_lseek(file, 0);
		res = f_write(file, pBlock, u16BlockLen, &bw);
		if (res || (bw != u16BlockLen)) {
			f_close(&file);
			return -ERR_SD_WRITE;
		}
	}

	f_close(&file);
	return 0;
}



/**
 * @Func	返回SD卡空闲容量
 * @Param	free		[in]	空闲容量大小，单位KB
 * @Ret		0=成功，否则错误代码
 */
int SD_GetFree(u32 *free) {
	FATFS *fs;
	FRESULT res;
	DWORD fre_clust, fre_sect, tot_sect;

	/* Get volume information and free clusters of drive 1 */
	res = f_getfree("0:", &fre_clust, &fs);
	if (res) {
		return -res;
	}

	/* Get total sectors and free sectors */
//	tot_sect = (fs->max_clust - 2) * fs->csize;
	fre_sect = fre_clust * fs->csize;

	return (fre_sect >> 1);

	/* Print free space in unit of KiB (assuming 512 bytes/sector) */
//	printf("%lu KB total drive space.\n%lu KB available.\n",
//		   tot_sect / 2, fre_sect / 2);
}

/**
 * @Func	返回SD卡总容量
 * @Param	volume		[in]	总容量大小，单位KB
 * @Ret		0=成功，否则错误代码
 */
int SD_Getvolume(u32 *volume) {
	FATFS *fs;
	FRESULT res;
	DWORD tot_sect;

	/* Get total sectors and free sectors */
	tot_sect = (fs->max_clust - 2) * fs->csize;

	return (tot_sect >> 1);
}

/**
 * 整数时间转换成BCD时间
 * @param pBCDTime
 * @param pTime
 */
void BCDTIME2TIME(tTIME *pBCDTime, TIME *pTime) {
	if ((pBCDTime == NULL ) || (pTime == NULL )) {
		return;
	}

	BCD2INTEGER(pTime->year, pBCDTime->u8Year);
	pTime->year += 2000;
	BCD2INTEGER(pTime->month, pBCDTime->u8Month);
	BCD2INTEGER(pTime->day, pBCDTime->u8Day);
	BCD2INTEGER(pTime->hour, pBCDTime->u8Hour);
	BCD2INTEGER(pTime->minute, pBCDTime->u8Minute);
	BCD2INTEGER(pTime->second, pBCDTime->u8Second);
}

/**
 * BCD时间转换成整数时间
 *
 * @param pBCDTime
 * @param pTime
 */
void TIME2BCDTIME(tTIME *pBCDTime, TIME *pTime) {
	if ((pBCDTime == NULL ) || (pTime == NULL )) {
		return;
	}

	pTime->year -= 2000;
	INTEGER2BCD(pTime->year, pBCDTime->u8Year);
	INTEGER2BCD(pTime->month, pBCDTime->u8Month);
	INTEGER2BCD(pTime->day, pBCDTime->u8Day);
	INTEGER2BCD(pTime->hour, pBCDTime->u8Hour);
	INTEGER2BCD(pTime->minute, pBCDTime->u8Minute);
	INTEGER2BCD(pTime->second, pBCDTime->u8Second);
}


/**
 * BCD格式时间拷贝
 *
 * @param pt1
 * @param pt2
 */
void BCDTIME_CPY(tTIME *pt1, tTIME *pt2) {
	pt1->u8Year = pt2->u8Year;
	pt1->u8Month = pt2->u8Month;
	pt1->u8Day = pt2->u8Day;
	pt1->u8Hour = pt2->u8Hour;
	pt1->u8Minute = pt2->u8Minute;
	pt1->u8Second = pt2->u8Second;
}

/**
 * 获取当前定位信息
 *
 * @param ptPos
 */
void VDR_GetGPSInfo(tPOSITION *ptPos) {


}

#if 0
/*
 * 测试用例
 */
void main() {
	TIME start;
	TIME end;
	int ret;
	u8 record[RECORD_SIZE(eLOG_Speed)];
	u8 aFrameBuf[1024];
	u16 u16FrameLen = 0;
	u16 u16ReadBytes = 0;
	u8 u8Blocks = 0;

	start.year = 2014;
	start.month = 7;
	start.day = 10;
	start.hour = 2;
	start.minute = 1;
	start.second = 0;

	end.year = 2014;
	end.month = 7;
	end.day = 15;
	end.hour = 2;
	end.minute = 1;
	end.second = 0;

	/*连续读取5条记录，正序*/
	ret = VDR_NewQuerySession(eLOG_Speed, &start, &end);
	if(ret < 0) {
		TRACE_(QS_USER, NULL, "Query Session Create failed!");
		return;
	}

	do {
		ret = VDR_RetrieveSingleRecordForward(record, RECORD_SIZE(eLOG_Speed));
		if(ret < 0)
		break;
	}while(!VDR_IsReadFinished());

	/*查询完成，释放查询SESSION*/
	SD_ClearQuerySession(&qSession);

	/*连续读取记录，逆序*/
	ret = VDR_NewQuerySession(eLOG_Speed, &start, &end);
	if(ret < 0) {
		TRACE_(QS_USER, NULL, "Query Session Create failed!");
		return;
	}

	do {
		ret = VDR_RetrieveSingleRecordBackward(record, RECORD_SIZE(eLOG_Speed));
		if(ret < 0)
		break;
	}while(!VDR_IsReadFinished());

	/*查询完成，释放查询SESSION*/
	SD_ClearQuerySession(&qSession);
}
#endif
