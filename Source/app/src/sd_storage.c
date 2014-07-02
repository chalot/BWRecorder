/**   
* @Title: sd_storage.c  
* @Description: TODO(CAN��GPS��ʷ���ݴ洢����)
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-8-28 ����4:26:04 
* @version V1.0   
*
* ���ܣ�SD����д�����洢�������ݣ�GPS���ݺ�CAN����
*
* ��GPS���ݡ�
*  ����-��-�ս�������Ŀ¼���ַ�����ʽ�����д洢��
*
*
*
* ���ļ���ʾ��
*  1�������������ļ�������ԭ���������ļ�����ά���Ŀ����ϴ������ڹ������¼���ݵ�һ�������Ա�֤��
*
*  2��Ŀ¼�ṹ��SD�����ļ���ΪCAN��GPS��������Ŀ¼��
*
*  /CAN/year_month_day_hour.can
*  		...
*  		year_month_day_hour.can
*
*  /GPS/year_month_day.gps
*  		...
*      	year_month_day.gps
*
*  ��{ʱ}Ŀ¼�£�Ϊ���ݵ�ǰСʱʱ�������ܴ�����24���ļ�������ע����ǣ�ͬһ���ļ��ϸ�����ֻ�����Сʱ
*  ��ʱ��Χ�ڵ�����֡��
*
*  3���ļ���ʽ��
*  ��¼��ʽ��ʱ��ƫ�� + ��������
*  ʱ��ƫ��Ϊ��ǰʱ�������0ʱ0�ֵ�ms����
*  ��������Ϊ�����ṹ��
*
*
* ��д���̡�
*  �洢���̰������漰д�������󲿷֡�
*  1��CAN�����ڽ��յ�һ��CAN֡����������һ�δ洢���̣�֪ͨ��֡���棬�ص�����ִ�и��Ӷ�Ӧ������С���Ա�֤ϵͳ��Ӧ������
*  2��дʱ������QF��ܿ��ƣ��ڼ�⵽ϵͳ�������ʱ֪ͨд����������QF_OnIdle()�е���SD��д�ص�������
*  SD���洢��Ԫ�ڵõ���֡������Ϣ�󣬿�ѡ��CAN֡���壻�ڵõ�д֪ͨ�󣬽���������дSD����
*
*
* �����ҹ��̡�
*
*  ������start_tick(year,month,day,hour,minute,second),
*  		 end_tick(year,month,day,hour,minute,second)
*
*  ��λ��ʼ�ļ���
*
*  //���ҵ�һ���ļ�����ʼСʱ��Ӧ���ļ����ܲ����ڣ�����Сʱ����������ļ���ֱ������Сʱ
*  for(cur_hour < end_tick.hour)
*   if(f_open(TIME2FN(year,month,day,cur_hour)))
*   {
*   	//��λ�ɹ�
*   	break;
*   }
*
*  ��λ��¼��
*  ���ļ���λ�ɹ��󣬸��ݲ�����ʱ���룬�����������������Ϊ�ؼ��֣����ļ����ö��ֲ��ҷ���λ�׸���¼λ�á�
*
*  ����ļ�����
*  ����ѯʱ��γ���1��Сʱʱ������ּ�¼���ļ�������������ַ�ʽ��һ���Ƚ����е��ļ����ҳ������ڶ�ʱ����
*  �����ļ�������������ʼʱ�䶨λ�׸��ļ����ڶ�ʱ�ж�����ǰ�ļ��Ѷ�ȡ��ϣ���δ����ָ��ʱ��ν���ʱ�̣���
*  �ٴ�����д����¼�ļ�����λ��¼λ�á���ʵ�ֲ��÷�������ԭ���ǵ�һ�ַ������ö�����ڴ�ȥ�������ļ��б�
*  ���ڶ��ַ�������ʵʱ���򵥡���Ч�����Ƕ��ʽ�����˵��Ӧ����򵥵ķ�ʽʵ�ֹ���Ҫ��ΪҪ��
*
*  �洢�ļ�������СʱΪ��С�洢���֣�����ǰд���ݽ����µ�Сʱ��¼���ڣ���رյ�ǰ�ļ����½�һ�����ļ�д��
*  ���ȵĴ�С������Ч��������Ҫ����СʱΪ��λ���֣���������Ϊ��ѯʱ��ʱ�临�Ӷ�O��n����
*
*  �ļ�������һ��SESSION������Ϊ�ļ������Ա������ݣ����Ǹ���ʱ�������ƣ�ԭ������SESSIONΪ��λֱ���Ͽ�
*  ����ȽϺ������������ļ����ǵ����ڲ�ȷ���������������Ѷȣ���ʵ����Ҳû�б�Ҫ��ô���������ѯ
*  ������ʼʱ��ͽ���ʱ��Ϊ��������û����ĳһ��SESSIONΪ������
*
*
*
* ���޸ģ��������GPS��Ŀ�洢����2014-07-01��
*
*
*    /------ Config ������Ϣ
* 		|
* 		|--- Picture ͼƬ
		|
* 		|--- Audio ��Ƶ
* 		|
* 		|--- CAN CAN͸������
* 		|
* 		|--- Log  ��ʻ��¼
*				|
*				|--- DriveSpeed ��ʻ�ٶ�             �ļ����ƣ��� - �� �� �� �� ʱ �� ��
*				|
*				|--- Accident ���¹��ɵ�
*				|
*				|--- OvertimeDrive ��ʱ��ʻ
*				|
*				|--- PositionInfo λ����Ϣ
*				|
*				|--- DriverInfo ��ʻ�����
*				|
*				|--- Distance ���
*				|
*				|--- InstallParam ��װ������¼
*				|
*				|--- Power �ⲿ�����¼
*				|
*				|--- ParamChange �����޸ļ�¼
*				|
*				|--- SpeedState �ٶ�״̬��־
*
*
* �洢�������ͣ�
* 1������ʻ�ٶȼ�¼����1s���������¼���洢��ʻ״̬������ʱ�䣬ƽ���ٶȣ�״̬�����������48Сʱ
* 				�洢������48*3600=172800�� *2B= 345600B = 345K,
*				�洢���ԣ�
*
* 2�����¹��ɵ��¼����0.2������������¼20s��������100��
* 				���1:��ʻʱ���ⲿ����Ͽ���
* 				���2����ʻʱ��λ��10s���ޱ仯
* 				�洢������ 234B*100 = 23400B=23K
*
* 3������ʱ��ʻ��¼����������100��
* 				�洢������50B*100 =5000B=5K

* 4����λ����Ϣ��¼����1min�����360Сʱ
*				�洢������360 * 60 = 21600=22K
*
* 5������ʻ����ݼ�¼������С��200��
* 				�洢������25B*200=5000B=5K
*
* 6������̼�¼��
*
*
* 7������װ������¼��
*
*
*--------- ��־��¼ --------
* 8�����ⲿ�����¼����������100��
* 				�洢������7B*100=700B=1K
*

* 9���������޸ļ�¼����������100��
* 				�洢������7B*100=700B=1K


* 10�����ٶ�״̬��־�����ӿ�ʼʱ������60s��������10��
* 				�洢������133B*10=1330B=2K
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

#define 	DUMP_MAXFRAMES_PERCALL		5	//����ʱִ��һ�δ洢���ã���󱣴�֡��


static FATFS fs;	//�ļ�ϵͳ

static T_QSESSION qsession;	//��ѯSESION��ͬһʱ��ÿ����ѯ����ֻ����һ��SESSION��Ч
static T_WSESSION can_wsession; //CAN�洢�Ự
static T_WSESSION gps_wsession; //GPS�洢�Ự

#define GET_TICKS(h,m)	(((h) * 60 + (m))* 1000) //������ת���ɺ���


static s8 time_compare(TIME *t1, TIME *t2);
static BOOL seek_record(u32 tick);
static int SD_DumpFrame(CANRAWFRAME* pFrame);

/**
 * @Func	SD����ʼ��
 * @Param	��
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
int SD_Init()
{
	if(!SD_Detect())
	{
		return -ERR_SDCARD_NOTEXISTED;
	}

	//��ʼ���ɹ�
	if(disk_initialize(0) != RES_OK)
	{
		return -ERR_SDCARD_INIT;
	}

	//ע�Ṥ����
	f_mount(0, &fs);

	return 0;
}

/**
 * @Func	�����ѯ�Ự״̬
 * @Param	class			[in]	��ѯ���ͣ�ֻ����CAN��GPS
 * @Ret		��
 */
void SD_ClearQuerySession()
{
	f_close(&qsession.file);
	ZeroMem((u8*)&qsession, sizeof(T_QSESSION));
}

/**
 * @Func	����һ���µĲ�ѯ ����ʱ��������λ������¼λ��
 * @Param	class			[in]	��ѯ���ͣ�ֻ����CAN��GPS
 * 			start_time		[in]	��ʼʱ��
 * 			end_time		[in]	����ʱ��
 * @Ret		TRUE=�ɹ���FALSE=ʧ��
 */
BOOL SD_NewQuerySession(TIME *start_time, TIME *end_time)
{
	FRESULT res;
	TIME time;
	BOOL bFind = 0;
	FILINFO fno;
	BOOL ret;

	if(-1 != time_compare(start_time, end_time))	//����ʱ��Ӧ���ڵ��ڿ�ʼʱ��
		return FALSE;

	/*ѭ����ѯ��¼�ļ�*/
	time_copy(&qsession.t_cur, &time);
	do
	{
		sprintf_(qsession.fn_cur, "0:/CAN/%d_%d_%d_%d.can",
				qsession.t_cur.year, qsession.t_cur.month, qsession.t_cur.day, qsession.t_cur.hour);//��ʱ��ת�����ļ���
		TRACE_(QS_USER, NULL, "[SD] trying to Seek file: %s", qsession.fn_cur);

		res = f_open(&qsession.file, qsession.fn_cur, FA_OPEN_EXISTING | FA_READ);//�������ļ�
		if(FR_OK == res) //�ļ����ڣ����ҳɹ�
		{
			res = f_stat(qsession.fn_cur, &fno); //ȡ�ļ�����
			if(FR_OK != res)
			{
				TRACE_(QS_USER, NULL, "[SD] can not get file info, operation failed!");
				f_close(&qsession.file);
				return FALSE;
			}

			if(0 == fno.fsize) //ȷ�����ǿ��ļ�
			{
				TRACE_(QS_USER, NULL, "[SD] error! file is empty.");
				f_close(&qsession.file);
				return FALSE;
			}

			bFind = 1;
			break;
		}

		time_nexthour(&qsession.t_cur);//ʧ�ܣ�����Сʱ��������
	}
	while(1 != time_compare(&qsession.t_cur, &qsession.t_end)); //ֱ����������ʱ��

	if(0 == bFind)	//ָ��ʱ����޼�¼�ļ���ʧ��
	{
		TRACE_(QS_USER, NULL, "[SD] none file found.");
		return FALSE;
	}

	/*�ļ���λ�ɹ�����һ����ʼ���Ҽ�¼��ʼλ�ã���λ��¼�Ǹ��ݷ�����*/
	TRACE_(QS_USER, NULL, "[SD] succeed to find first file: %s", qsession.fn_cur);

	ret = seek_record(GET_TICKS(start_time->minute, start_time->second));	//��λ�ļ�������¼λ��
	if(!ret)
	{
		TRACE_(QS_USER, NULL, "[SD] can not seek to start record, failed!");
		return FALSE;
	}

	/*���Ự��Ϣ*/
	time_copy(&qsession.t_start, start_time);
	time_copy(&qsession.t_end, end_time);

	sprintf_(qsession.fn_end, "0:/CAN/%d_%d_%d_%d",
				time.year, time.month, time.day, time.hour);//��������ļ���

	return TRUE;
}

/**
 * @Func	��ȡ���ݿ�
 * @Param	class			[in]	��ѯ���ͣ�ֻ����CAN��GPS
 * @Ret		��
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

	f = &qsession.file; //�ļ����

	record_size = sizeof(T_CAN_DUMPRECORD);

	res = f_read(f, pBuf, blocks * record_size, &already_read);  //��ȡָ��������¼
	if(FR_OK != res)
	{
		TRACE_(QS_USER, NULL, "[SD] can not read record block, failed!");
		f_close(f);
		return 0;
	}

	/*��ȡ�󣬼�����һ�ζ�ȡ�ļ���λ��*/
	if(strcmp_(qsession.fn_cur, qsession.fn_end)) //����ļ���
	{
		/*�������ĵ�һ����¼ʱ���Ƿ񳬹����β�ѯָ������ʱ��*/
		pRecord = (T_CAN_DUMPRECORD*)pBuf;
		if(pRecord->tick > GET_TICKS(qsession.t_end.minute, qsession.t_end.second))
		{
			TRACE_(QS_USER, NULL, "[SD] Congratulation! All records has taken sucsessfully.");
			f_close(f);
			SD_ClearQuerySession(&qsession);
			already_read = 0; //��¼Խ�磬��ȡ���
		}

		/*���һ���ļ���ȡ��ϣ��Զ�������ѯ��������ʵ�ʶ�ȡ�ֽ���*/
		if(0 == f_eof(f))
		{
			TRACE_(QS_USER, NULL, "[SD] Last file to end, query finished.");
			f_close(f);
			SD_ClearQuerySession(&qsession);
		}
	}
	else
	{
		/*����Ƿ��ѵ��ļ�β�����ǣ�������ݲ�ѯ����ʱ����Һ����ļ�����ָ���Ƶ���һ�ļ���ȡλ��*/
		if(0 == f_eof(f))	//���ļ�β,������һ������ȡ�ļ�
		{
			u8 bFind = 0;
			FILINFO *fno;

			time_nexthour(&qsession.t_cur);//������ǰ��ѯСʱ��������

			do
			{
				/*��ʱ��ת���ɼ�¼�ļ���*/
				sprintf_(qsession.fn_cur, "0:/CAN/%d_%d_%d_%d.can",
						qsession.t_cur.year, qsession.t_cur.month, qsession.t_cur.day, qsession.t_cur.hour);

				TRACE_(QS_USER, NULL, "[SD] trying to Seek file: %s", qsession.fn_cur);

				res = f_open(&qsession.file, qsession.fn_cur, FA_OPEN_EXISTING | FA_READ);//�������ļ�
				if(FR_OK == res) //�ļ����ڣ����ҳɹ�
				{
					res = f_stat(qsession.fn_cur, &fno); //ȡ�ļ�����
					if(FR_OK != res)
					{
						TRACE_(QS_USER, NULL, "[SD] can not get file info, operation failed!");
						f_close(&qsession.file);
						return FALSE;
					}

					if(0 == (u32)fno->fsize) //ȷ�����ǿ��ļ�
					{
						TRACE_(QS_USER, NULL, "[SD] error! file is empty.");
						f_close(&qsession.file);
						return FALSE;
					}

					bFind = 1;
					break;
				}
				time_nexthour(&qsession.t_cur);//ʧ�ܣ�����Сʱ��������
			}
			while(1 != time_compare(&qsession.t_cur, &qsession.t_end)); //ֱ����������ʱ�䣬�˳�ѭ��

			if(0 == bFind)	//ָ��ʱ����޼�¼�ļ���ʧ��
			{
				TRACE_(QS_USER, NULL, "[SD] none file found.");
				return 0;
			}

			f_lseek(&qsession.file, 0);//��ָ���Ƶ��ļ�ͷ��׼����һ�ζ�
		}

	}

	return already_read;

}

/**
 * @Func	ʱ�俽��
 * @Param	t1			[in]	ʱ��1
 * 			t2			[in]	ʱ��2
 * @Ret		��
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
 * @Func	������һ��Сʱ��ʾ
 * @Param	t1			[in]	ԭʱ��
 * @Ret		t1			[out]	����1Сʱ���ʱ��
 */
void time_nexthour(TIME *t)
{
	t->hour++;
	if(	t->hour >= 24)
	{
		u8 u8DaysofMonth;
		t->hour = 0;

		u8DaysofMonth = MISC_DaysofMonth(t->year/* + 2000*/, t->month); //ȡ�õ�ǰ���ж�����
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
 * @Func	ʱ��Ƚ�
 * @Param	t1			[in]	ʱ��1
 * 			t2			[in]	ʱ��2
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
 * @Func	��ʼʹ���۰�����㷨�����ļ���¼
 * @Param	class			[in]	��ѯ���ͣ�ֻ����CAN��GPS
 * 			tick			[in]	���뻻��ɵ�ms�����ڵ����ļ��ڲ��ң���¼ƫ�뵥λ�Ƿ���
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

	/*��λ������¼λ��*/
	f_lseek(f, 0); //ָ���ļ�ͷ

	if(0 == tick) //��0��0�뿪ʼ���������
	{
		return TRUE;
	}

	/*��������¼�Ƿ���Ѿ����ڲ���ʱ�䣬������ֱ���Ե�һ����¼���������*/
	res = f_read(f, (u8*)&record, record_size, &read); //���ļ��۰봦�ļ�¼
	if (res != FR_OK )
	{
		f_close(f);
		return FALSE;
	}

	mid_ticks = record.tick;

	if(tick < mid_ticks) //������¼��ʱ����������ѯʱ�䣬�����ٲ�����
	{
		f_lseek(f, 0);
		return TRUE;
	}

	/*��������£��۰����*/
	while(low + record_size < high)
	{
		mid = (high - low) / record_size;
		mid = low + (mid >> 1) * record_size;
		res = f_lseek(f, mid); //��ָ��������۰��¼��
		if (res != FR_OK )
		{
			f_close(f);
			return FALSE;
		}

		res = f_read(f, (u8*)&record, record_size, &read); //���ļ��۰봦�ļ�¼
   		if (res != FR_OK )
		{
			f_close(f);
			return FALSE;
		}

   		mid_ticks = record.tick;

		if (tick > mid_ticks)
			low = mid;	//ʱ���С
		else if (tick < mid_ticks)
			high = mid;	//ʱ��ϴ�
		else if(tick == mid_ticks)
			low = mid;
	}

	f_lseek(f, low); //�ļ���ָ������������¼λ��
	return TRUE;
}

/**
 * @Func	����洢ԭʼ֡��ע���ڽ��յ�һ֡��Ӧ�������øù���
 * @Param	pNode			[in]	�ڵ�
 * @Ret		��
 */
void SD_RequestDumpFrame(DATANODE* pNode)
{
	if(pNode)
		pNode->reference ++;
}

/**
 * @Func	�洢ԭʼ֡
 * @Param	pFrame			[in]	֡����
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
void SD_DoDumpFrame()
{
	DATANODE* pNode;
	u8 dump_times = 0;

	pNode = MP_GetAllocNodeHead(); //ȡ���������׽ڵ�

	while(pNode && dump_times < DUMP_MAXFRAMES_PERCALL)
	{
		SD_DumpFrame((CANRAWFRAME*)pNode->data); //�洢�������Ƿ�ɹ����ͷŽڵ�
		MP_DecountReference(pNode);
		MP_FreeNode(pNode); //�ͷŽڵ�

		pNode = pNode->next; //��������ĺ����ڵ�
		dump_times ++;
	}
}

/**
 * @Func	�洢ԭʼ֡
 * @Param	pFrame			[in]	֡����
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
int SD_DumpFrame(CANRAWFRAME* pFrame)
{
	char fn[50];
	TIME timer;
	FRESULT res;
	u32 bw = 0;

	SysTimer_GetRaw(&timer); //ȡ��ǰʱ��

	encode((u8*)pFrame, sizeof(CANRAWFRAME));	//���ݼ���

	sprintf_(fn, "0:/CAN/%d_%d_%d_%d.can",
			timer.year, timer.month, timer.day, timer.hour);	//��ʱ��ת�����ļ���
	TRACE_(QS_USER, NULL, "[SD] trying to Seek file: %s", fn);

	if(strcmp_(fn, can_wsession.fn_cur)) //�ļ��Ѵ���
	{
		if(NULL == can_wsession.file) //�״δ�
		{
			//���ļ��������ڴ������ļ�
			res = f_open(can_wsession.file, can_wsession.fn_cur, FA_OPEN_ALWAYS | FA_WRITE);
			if(FR_OK == res) //�ļ����ڣ����ҳɹ�
			{
				f_lseek(can_wsession.file, 0);
			}
			else
			{
				f_close(can_wsession.file);
				return -res;
			}
		}

		res = f_write(can_wsession.file, (u8*)pFrame, sizeof(CANRAWFRAME), &bw); //д��¼
		if(res || (bw != sizeof(CANRAWFRAME)))
		{
			return -res;
		}
	}
	else
	{
		strcpy_(can_wsession.fn_cur, fn); //������ǰ�ļ���

		if(can_wsession.file) //�رվ��ļ�
		{
			f_close(can_wsession.file);
		}

		//���ļ��������ڴ������ļ�
		res = f_open(can_wsession.file, can_wsession.fn_cur, FA_OPEN_ALWAYS | FA_WRITE);
		if(FR_OK == res) //�ļ����ڣ����ҳɹ�
		{
			res = f_write(can_wsession.file, (u8*)pFrame, sizeof(CANRAWFRAME), &bw); //д��¼
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
 * @Func	�洢GPS����
 * @Param	pGpsInfo			[in]	��λ����
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
int SD_DumpGpsInfo(_tGPSInfo* pGpsInfo)
{
	/*������������*/
	char fn[50];
	TIME timer;
	FRESULT res;
	u32 *bw = NULL;
	static char gpsInfo[100];

	SysTimer_GetRaw(&timer); //ȡ��ǰʱ��

	sprintf_(fn, "0:/GPS/%d_%d_%d.gps",
					timer.year, timer.month, timer.day);	//��ʱ��ת�����ļ���

	sprintf_(gpsInfo, "[GPS] %d-%d-%d:%d-%d-%d, valid = %d, antena = %d, Lon=%x.%x%x%x, Lat=%x.%x%x%x, Speed=%x%x, view_S=%x, visib_S=%x\n",
			pGpsInfo->u16Year, pGpsInfo->u8Month, pGpsInfo->u8Day, pGpsInfo->u8Hour,pGpsInfo->u8Minute, pGpsInfo->u8Second,
			(pGpsInfo->u16GpsInfoValid & _GPSINFO_GPS_VALID) ? 1 : 0, pGpsInfo->bAntenaState,
			pGpsInfo->Longtitude[0], pGpsInfo->Longtitude[1], pGpsInfo->Longtitude[2], pGpsInfo->Longtitude[3],
			pGpsInfo->Latitude[0], pGpsInfo->Latitude[1], pGpsInfo->Latitude[2], pGpsInfo->Latitude[3],
			pGpsInfo->Speed[0], pGpsInfo->Speed[1], pGpsInfo->u8ValidSatellites, pGpsInfo->u8VisibleSatellites
			);

	if(strcmp_(fn, gps_wsession.fn_cur)) //�ļ��Ѵ���
	{
		if(NULL == gps_wsession.file) //�״δ�
		{
			//���ļ��������ڴ������ļ�
			res = f_open(gps_wsession.file, gps_wsession.fn_cur, FA_OPEN_ALWAYS | FA_WRITE);
			if(FR_OK == res) //�ļ����ڣ����ҳɹ�
			{
				f_lseek(gps_wsession.file, 0);
			}
			else
			{
				f_close(gps_wsession.file);
				return -res;
			}
		}

		res = f_write(&gps_wsession.file, gpsInfo, strlen_(gpsInfo), &bw); ///д��¼
		if(res)
		{
			return -res; ///д����
		}
	}
	else
	{
		strcpy_(gps_wsession.fn_cur, fn); //������ǰ�ļ���

		if(gps_wsession.file) //�رվ��ļ�
		{
			f_close(gps_wsession.file);
		}

		//���ļ��������ڴ������ļ�
		res = f_open(gps_wsession.file, gps_wsession.fn_cur, FA_OPEN_ALWAYS | FA_WRITE);
		if(FR_OK == res) //�ļ����ڣ����ҳɹ�
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
 * @Func	����SD����������
 * @Param	free		[in]	����������С����λKB
 * @Ret		0=�ɹ�������������
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
 * @Func	����SD��������
 * @Param	volume		[in]	��������С����λKB
 * @Ret		0=�ɹ�������������
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
