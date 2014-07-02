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
*				|--- DriveSpeed 行驶速度             文件名称：年 - 月 — 日 — 时 — 分
*				|
*				|--- Accident ，事故疑点
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
*/ 

#include "ff.h"
//#include "encryption.h"
#include "sdcard_interface.h"
#include "utility.h"
#include "diskio.h"
//#include "error.h"
//#include "gps.h"
#include "trace.h"

#define 	DUMP_MAXFRAMES_PERCALL		5	//空闲时执行一次存储调用，最大保存帧数


static FATFS fs;	//文件系统

static T_QSESSION qsession;	//查询SESION，同一时刻每个查询类型只能有一个SESSION有效
static T_WSESSION can_wsession; //CAN存储会话
static T_WSESSION gps_wsession; //GPS存储会话

#define GET_TICKS(h,m)	(((h) * 60 + (m))* 1000) //分秒数转换成毫秒


static s8 time_compare(TIME *t1, TIME *t2);
static BOOL seek_record(u32 tick);
static int SD_DumpFrame(CANRAWFRAME* pFrame);

/**
 * @Func	SD卡初始化
 * @Param	无
 * @Ret		0=成功，否则为错误代码(RC)
 */
int SD_Init()
{
	if(!SD_Detect())
	{
		return -ERR_SDCARD_NOTEXISTED;
	}

	//初始化成功
	if(disk_initialize(0) != RES_OK)
	{
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
void SD_ClearQuerySession()
{
	f_close(&qsession.file);
	ZeroMem((u8*)&qsession, sizeof(T_QSESSION));
}

/**
 * @Func	启动一个新的查询 ，按时间条件定位首条记录位置
 * @Param	class			[in]	查询类型，只能是CAN，GPS
 * 			start_time		[in]	起始时间
 * 			end_time		[in]	结束时间
 * @Ret		TRUE=成功，FALSE=失败
 */
BOOL SD_NewQuerySession(TIME *start_time, TIME *end_time)
{
	FRESULT res;
	TIME time;
	BOOL bFind = 0;
	FILINFO fno;
	BOOL ret;

	if(-1 != time_compare(start_time, end_time))	//结束时刻应大于等于开始时刻
		return FALSE;

	/*循环查询记录文件*/
	time_copy(&qsession.t_cur, &time);
	do
	{
		sprintf_(qsession.fn_cur, "0:/CAN/%d_%d_%d_%d.can",
				qsession.t_cur.year, qsession.t_cur.month, qsession.t_cur.day, qsession.t_cur.hour);//将时间转换成文件名
		TRACE_(QS_USER, NULL, "[SD] trying to Seek file: %s", qsession.fn_cur);

		res = f_open(&qsession.file, qsession.fn_cur, FA_OPEN_EXISTING | FA_READ);//打开已有文件
		if(FR_OK == res) //文件存在，查找成功
		{
			res = f_stat(qsession.fn_cur, &fno); //取文件属性
			if(FR_OK != res)
			{
				TRACE_(QS_USER, NULL, "[SD] can not get file info, operation failed!");
				f_close(&qsession.file);
				return FALSE;
			}

			if(0 == fno.fsize) //确保不是空文件
			{
				TRACE_(QS_USER, NULL, "[SD] error! file is empty.");
				f_close(&qsession.file);
				return FALSE;
			}

			bFind = 1;
			break;
		}

		time_nexthour(&qsession.t_cur);//失败，递增小时继续查找
	}
	while(1 != time_compare(&qsession.t_cur, &qsession.t_end)); //直至超过结束时间

	if(0 == bFind)	//指定时间段无记录文件，失败
	{
		TRACE_(QS_USER, NULL, "[SD] none file found.");
		return FALSE;
	}

	/*文件定位成功，下一步开始查找记录起始位置，定位记录是根据分秒数*/
	TRACE_(QS_USER, NULL, "[SD] succeed to find first file: %s", qsession.fn_cur);

	ret = seek_record(GET_TICKS(start_time->minute, start_time->second));	//定位文件首条记录位置
	if(!ret)
	{
		TRACE_(QS_USER, NULL, "[SD] can not seek to start record, failed!");
		return FALSE;
	}

	/*填充会话信息*/
	time_copy(&qsession.t_start, start_time);
	time_copy(&qsession.t_end, end_time);

	sprintf_(qsession.fn_end, "0:/CAN/%d_%d_%d_%d",
				time.year, time.month, time.day, time.hour);//保存结束文件名

	return TRUE;
}

/**
 * @Func	提取数据块
 * @Param	class			[in]	查询类型，只能是CAN，GPS
 * @Ret		无
 */
u16 SD_RetrieveRecordBlock(u8 blocks, u8 *pBuf, u16 bufSize)
{
	FIL	*f;
	FILINFO *fno;
	FRESULT res;
	u32 record_size;
	u32 already_read;
	T_CAN_DUMPRECORD *pRecord;

	if(NULL == pBuf)
		return 0;

	f = &qsession.file; //文件句柄

	record_size = sizeof(T_CAN_DUMPRECORD);

	res = f_read(f, pBuf, blocks * record_size, &already_read);  //读取指定条数记录
	if(FR_OK != res)
	{
		TRACE_(QS_USER, NULL, "[SD] can not read record block, failed!");
		f_close(f);
		return 0;
	}

	/*读取后，计算下一次读取文件及位置*/
	if(strcmp_(qsession.fn_cur, qsession.fn_end)) //最后文件？
	{
		/*检查读出的第一条记录时间是否超过本次查询指定结束时间*/
		pRecord = (T_CAN_DUMPRECORD*)pBuf;
		if(pRecord->tick > GET_TICKS(qsession.t_end.minute, qsession.t_end.second))
		{
			TRACE_(QS_USER, NULL, "[SD] Congratulation! All records has taken sucsessfully.");
			f_close(f);
			SD_ClearQuerySession(&qsession);
			already_read = 0; //记录越界，读取完毕
		}

		/*最后一个文件读取完毕，自动结束查询，但返回实际读取字节数*/
		if(0 == f_eof(f))
		{
			TRACE_(QS_USER, NULL, "[SD] Last file to end, query finished.");
			f_close(f);
			SD_ClearQuerySession(&qsession);
		}
	}
	else
	{
		/*检查是否已到文件尾，若是，则需根据查询结束时间查找后续文件，读指针移到下一文件读取位置*/
		if(0 == f_eof(f))	//到文件尾,查找下一个待读取文件
		{
			u8 bFind = 0;
			FILINFO *fno;

			time_nexthour(&qsession.t_cur);//递增当前查询小时继续查找

			do
			{
				/*将时间转换成记录文件名*/
				sprintf_(qsession.fn_cur, "0:/CAN/%d_%d_%d_%d.can",
						qsession.t_cur.year, qsession.t_cur.month, qsession.t_cur.day, qsession.t_cur.hour);

				TRACE_(QS_USER, NULL, "[SD] trying to Seek file: %s", qsession.fn_cur);

				res = f_open(&qsession.file, qsession.fn_cur, FA_OPEN_EXISTING | FA_READ);//打开已有文件
				if(FR_OK == res) //文件存在，查找成功
				{
					res = f_stat(qsession.fn_cur, &fno); //取文件属性
					if(FR_OK != res)
					{
						TRACE_(QS_USER, NULL, "[SD] can not get file info, operation failed!");
						f_close(&qsession.file);
						return FALSE;
					}

					if(0 == (u32)fno->fsize) //确保不是空文件
					{
						TRACE_(QS_USER, NULL, "[SD] error! file is empty.");
						f_close(&qsession.file);
						return FALSE;
					}

					bFind = 1;
					break;
				}
				time_nexthour(&qsession.t_cur);//失败，递增小时继续查找
			}
			while(1 != time_compare(&qsession.t_cur, &qsession.t_end)); //直至超过结束时间，退出循环

			if(0 == bFind)	//指定时间段无记录文件，失败
			{
				TRACE_(QS_USER, NULL, "[SD] none file found.");
				return 0;
			}

			f_lseek(&qsession.file, 0);//读指针移到文件头，准备下一次读
		}

	}

	return already_read;

}

/**
 * @Func	时间拷贝
 * @Param	t1			[in]	时间1
 * 			t2			[in]	时间2
 * @Ret		无
 */
void time_copy(TIME *t1, TIME *t2)
{
	t1->year = t2->year;
	t1->month = t2->month;
	t1->day = t2->day;
	t1->hour = t2->hour;
	t1->minute = t2->minute;
	t1->second = t2->second;
}

/**
 * @Func	计算下一个小时表示
 * @Param	t1			[in]	原时间
 * @Ret		t1			[out]	递增1小时后的时间
 */
void time_nexthour(TIME *t)
{
	t->hour++;
	if(	t->hour >= 24)
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
 * @Func	时间比较
 * @Param	t1			[in]	时间1
 * 			t2			[in]	时间2
 * @Ret		-1:t1<t2; 0:t1=t2; 1:t1>t2
 */
s8 time_compare(TIME *t1, TIME *t2)
{
	if(t1->year > t2->year)
		return 1;
	if(t1->year < t2->year)
		return -1;
	if(t1->month > t2->month)
		return 1;
	if(t1->month < t2->month)
		return -1;
	if(t1->day > t1->day)
		return 1;
	if(t1->day < t1->day)
		return -1;
	if(t1->hour > t2->hour)
		return 1;
	if(t1->hour < t2->hour)
		return -1;
	if(t1->minute > t2->minute)
		return 1;
	if(t1->minute < t2->minute)
		return -1;
	if(t1->second > t2->second)
		return 1;
	if(t1->second < t2->second)
		return -1;

	return 0;
}

//
/**
 * @Func	开始使用折半查找算法查找文件记录
 * @Param	class			[in]	查询类型，只能是CAN，GPS
 * 			tick			[in]	分秒换算成的ms数，在单个文件内查找，记录偏离单位是分秒
 * @Ret		-1:t1<t2; 0:t1=t2; 1:t1>t2
 */
BOOL seek_record(u32 tick)
{
	FIL *f;
	FRESULT res;
	T_CAN_DUMPRECORD record;
	UINT read;

	u32 low = 0, high = 0, mid = 0;
	u32 mid_ticks = 0;
	u8 record_size = 0;

	f = &qsession.file;

	low = 0;
	high = qsession.file.fsize;
	record_size = sizeof(T_CAN_DUMPRECORD);

	/*定位首条记录位置*/
	f_lseek(f, 0); //指向文件头

	if(0 == tick) //以0分0秒开始，则不需查找
	{
		return TRUE;
	}

	/*看首条记录是否就已经大于参数时间，大于则直接以第一条记录，不需查找*/
	res = f_read(f, (u8*)&record, record_size, &read); //读文件折半处的记录
	if (res != FR_OK )
	{
		f_close(f);
		return FALSE;
	}

	mid_ticks = record.tick;

	if(tick < mid_ticks) //首条记录的时间大于请求查询时间，不需再查找了
	{
		f_lseek(f, 0);
		return TRUE;
	}

	/*其他情况下，折半查找*/
	while(low + record_size < high)
	{
		mid = (high - low) / record_size;
		mid = low + (mid >> 1) * record_size;
		res = f_lseek(f, mid); //读指针调整到折半记录处
		if (res != FR_OK )
		{
			f_close(f);
			return FALSE;
		}

		res = f_read(f, (u8*)&record, record_size, &read); //读文件折半处的记录
   		if (res != FR_OK )
		{
			f_close(f);
			return FALSE;
		}

   		mid_ticks = record.tick;

		if (tick > mid_ticks)
			low = mid;	//时间较小
		else if (tick < mid_ticks)
			high = mid;	//时间较大
		else if(tick == mid_ticks)
			low = mid;
	}

	f_lseek(f, low); //文件读指针跳到首条记录位置
	return TRUE;
}

/**
 * @Func	请求存储原始帧，注意在接收到一帧后，应立即调用该过程
 * @Param	pNode			[in]	节点
 * @Ret		无
 */
void SD_RequestDumpFrame(DATANODE* pNode)
{
	if(pNode)
		pNode->reference ++;
}

/**
 * @Func	存储原始帧
 * @Param	pFrame			[in]	帧数据
 * @Ret		0=成功，否则为错误代码(RC)
 */
void SD_DoDumpFrame()
{
	DATANODE* pNode;
	u8 dump_times = 0;

	pNode = MP_GetAllocNodeHead(); //取分配链表首节点

	while(pNode && dump_times < DUMP_MAXFRAMES_PERCALL)
	{
		SD_DumpFrame((CANRAWFRAME*)pNode->data); //存储，无论是否成功都释放节点
		MP_DecountReference(pNode);
		MP_FreeNode(pNode); //释放节点

		pNode = pNode->next; //分配链表的后续节点
		dump_times ++;
	}
}

/**
 * @Func	存储原始帧
 * @Param	pFrame			[in]	帧数据
 * @Ret		0=成功，否则为错误代码(RC)
 */
int SD_DumpFrame(CANRAWFRAME* pFrame)
{
	char fn[50];
	TIME timer;
	FRESULT res;
	u32 bw = 0;

	SysTimer_GetRaw(&timer); //取当前时间

	encode((u8*)pFrame, sizeof(CANRAWFRAME));	//数据加密

	sprintf_(fn, "0:/CAN/%d_%d_%d_%d.can",
			timer.year, timer.month, timer.day, timer.hour);	//将时间转换成文件名
	TRACE_(QS_USER, NULL, "[SD] trying to Seek file: %s", fn);

	if(strcmp_(fn, can_wsession.fn_cur)) //文件已存在
	{
		if(NULL == can_wsession.file) //首次打开
		{
			//打开文件，不存在创建新文件
			res = f_open(can_wsession.file, can_wsession.fn_cur, FA_OPEN_ALWAYS | FA_WRITE);
			if(FR_OK == res) //文件存在，查找成功
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
		strcpy_(can_wsession.fn_cur, fn); //保留当前文件名

		if(can_wsession.file) //关闭旧文件
		{
			f_close(can_wsession.file);
		}

		//打开文件，不存在创建新文件
		res = f_open(can_wsession.file, can_wsession.fn_cur, FA_OPEN_ALWAYS | FA_WRITE);
		if(FR_OK == res) //文件存在，查找成功
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

/**
 * @Func	存储GPS数据
 * @Param	pGpsInfo			[in]	定位数据
 * @Ret		0=成功，否则为错误代码(RC)
 */
int SD_DumpGpsInfo(_tGPSInfo* pGpsInfo)
{
	/*命名：年月日*/
	char fn[50];
	TIME timer;
	FRESULT res;
	u32 *bw = NULL;
	static char gpsInfo[100];

	SysTimer_GetRaw(&timer); //取当前时间

	sprintf_(fn, "0:/GPS/%d_%d_%d.gps",
					timer.year, timer.month, timer.day);	//将时间转换成文件名

	sprintf_(gpsInfo, "[GPS] %d-%d-%d:%d-%d-%d, valid = %d, antena = %d, Lon=%x.%x%x%x, Lat=%x.%x%x%x, Speed=%x%x, view_S=%x, visib_S=%x\n",
			pGpsInfo->u16Year, pGpsInfo->u8Month, pGpsInfo->u8Day, pGpsInfo->u8Hour,pGpsInfo->u8Minute, pGpsInfo->u8Second,
			(pGpsInfo->u16GpsInfoValid & _GPSINFO_GPS_VALID) ? 1 : 0, pGpsInfo->bAntenaState,
			pGpsInfo->Longtitude[0], pGpsInfo->Longtitude[1], pGpsInfo->Longtitude[2], pGpsInfo->Longtitude[3],
			pGpsInfo->Latitude[0], pGpsInfo->Latitude[1], pGpsInfo->Latitude[2], pGpsInfo->Latitude[3],
			pGpsInfo->Speed[0], pGpsInfo->Speed[1], pGpsInfo->u8ValidSatellites, pGpsInfo->u8VisibleSatellites
			);

	if(strcmp_(fn, gps_wsession.fn_cur)) //文件已存在
	{
		if(NULL == gps_wsession.file) //首次打开
		{
			//打开文件，不存在创建新文件
			res = f_open(gps_wsession.file, gps_wsession.fn_cur, FA_OPEN_ALWAYS | FA_WRITE);
			if(FR_OK == res) //文件存在，查找成功
			{
				f_lseek(gps_wsession.file, 0);
			}
			else
			{
				f_close(gps_wsession.file);
				return -res;
			}
		}

		res = f_write(&gps_wsession.file, gpsInfo, strlen_(gpsInfo), &bw); ///写记录
		if(res)
		{
			return -res; ///写出错
		}
	}
	else
	{
		strcpy_(gps_wsession.fn_cur, fn); //保留当前文件名

		if(gps_wsession.file) //关闭旧文件
		{
			f_close(gps_wsession.file);
		}

		//打开文件，不存在创建新文件
		res = f_open(gps_wsession.file, gps_wsession.fn_cur, FA_OPEN_ALWAYS | FA_WRITE);
		if(FR_OK == res) //文件存在，查找成功
		{
			res = f_open(gps_wsession.file, gps_wsession.fn_cur, FA_OPEN_ALWAYS | FA_WRITE);
			if(res)
			{
				return -res;
			}
		}
		else
		{
			f_close(gps_wsession.file);
			return -res;
		}
	}

	return 0;
}

/**
 * @Func	返回SD卡空闲容量
 * @Param	free		[in]	空闲容量大小，单位KB
 * @Ret		0=成功，否则错误代码
 */
int SD_GetFree(u32 *free)
{
	FATFS *fs;
	FRESULT res;
	DWORD fre_clust, fre_sect, tot_sect;

	/* Get volume information and free clusters of drive 1 */
	res = f_getfree("0:", &fre_clust, &fs);
	if (res)
	{
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
int SD_Getvolume(u32 *volume)
{
	FATFS *fs;
	FRESULT res;
	DWORD tot_sect;

	/* Get total sectors and free sectors */
	tot_sect = (fs->max_clust - 2) * fs->csize;

	return (tot_sect >> 1);
}
