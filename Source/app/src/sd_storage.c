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
 *				|--- DriveSpeed ��ʻ�ٶ�             �ļ����ƣ��� - �� �� �� �� ʱ
 *				|
 *				|--- Accident �¹��ɵ�
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

/**��¼����·��*/
static const char* Dir[] = {
		"Speed",			///��ʻ�ٶ�
		"Accident",			/// �¹��ɵ�
		"OvertimeDrive",	/// ��ʱ��ʻ
		"PositionInfo", 	/// λ����Ϣ
		"DriverInfo", 		///��ʻ�����
		"Distance",			///���
		"InstallParam",		///��װ������¼
		"Power",			///�ⲿ�����¼
		"ParamChange",		///�����޸ļ�¼
		"SpeedState",  		///�ٶ�״̬��־
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

#define DUMP_MAXFRAMES_PERCALL		5	//����ʱִ��һ�δ洢���ã���󱣴�֡��
#define GET_TICKS(h,m,s)	((((h)* 3600) + ((m) * 60) + (s))* 1000) //������ת���ɺ���

static FATFS fs;	//�ļ�ϵͳ

static T_QSESSION qSession;	//��ѯSESION��ͬһʱ��ϵͳֻ����һ��SESSION��Ч


/**BCD��ת��������*/
#define BCD2INTEGER(byteBCD, byte) 	(byte = (byteBCD >> 4) * 10 +(byteBCD & 0x0F))
/**����ת��BCD��*/
#define INTEGER2BCD(byteBCD, byte)  (byteBCD = ((byte / 10) << 4) + (byte % 10))

/*---------- ���غ������� ---------------------------------------------------------------------*/
static s8 time_compare(TIME *t1, TIME *t2);
static BOOL VDR_SeekRecord(u32 tick, u16 u16Recordsize, u32 *pu32Offset);
//static int SD_DumpCANFrame(CANRAWFRAME* pFrame);
static void VDR_SeekNextFileBeginRecord(void);
static void VDR_SeekPreviousFileLastRecord(u16 u16RecordSize);
static int VDR_Dump2File(TIME *pTime, u8 *pBlock, u16 u16BlockLen);
static void VDR_GetGPSInfo(tPOSITION *ptPos);
/*----------������������������---------------------------------------------------------------------*/

/**
 * @Func	SD����ʼ��
 * @Param	��
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
int SD_Init() {
	if (!SD_Detect()) {
		return -ERR_SDCARD_NOTEXISTED;
	}

	//��ʼ���ɹ�
	if (disk_initialize(0) != RES_OK) {
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

	/**����ɵĲ�ѯ״̬*/
	SD_ClearQuerySession(&qSession);

	/**ȷ������ʱ��Ӧ���ڵ��ڿ�ʼʱ��*/
	if(-1 != time_compare(pTime_Start, pTime_End))
	return -ERR_PARAM_INVALID;

	/**������ʼ�ļ�����һ����¼λ��*/
	iRet = VDR_FindStartFileAndRecord(eClass, pTime_Start, pTime_End);
	if(iRet < 0)
	return iRet;

	/**���ҽ����ļ������һ����¼λ��*/
	iRet = VDR_FindStartFileAndRecord(eClass, pTime_Start, pTime_End);
	if(iRet < 0)
	return iRet;

	/*���Ự��Ϣ*/
	time_copy(&qSession.t_Start, pTime_Start);
	time_copy(&qSession.t_End, pTime_End);

	/*�ļ�������������ر�*/
	f_close(&qSession.file);

	return 0;
}
#endif

/**
 * ����һ���µĲ�ѯ ����ʱ��������λ������¼λ��
 * ���в�ѯ����һ��SESSION����Ϊͬһʱ��ֻ����Ӧһ����ѯ
 *
 * @param start_time	��ʼʱ��
 * @param end_time		����ʱ��
 * @return	0=�ɹ�������������
 */
int VDR_NewQuerySession(eLOGCLASS eClass, TIME *start_time, TIME *end_time) {
	FRESULT res;
	BOOL bFind = 0;
	FILINFO fno;
	BOOL ret;
	u32 u32Offset;

	/*����ɵĲ�ѯ״̬*/
	SD_ClearQuerySession(&qSession);

	/*���Ự����*/
	time_copy(&qSession.t_Start, start_time);
	time_copy(&qSession.t_End, end_time);
	qSession.u8SessionType = (u8) eClass;

	/*ȷ������ʱ��Ӧ���ڵ��ڿ�ʼʱ��*/
	if (-1 != time_compare(start_time, end_time))
		return -ERR_PARAM_INVALID;

	TRACE_(QS_USER, NULL, "[VDR] ******** NEW Query Session **********");
	TRACE_(QS_USER, NULL, "[VDR] type = %s, from %d-%d-%d:%d-%d-%d to %d-%d-%d:%d-%d-%d ", Dir[eClass],
			start_time->year, start_time->month, start_time->day, start_time->hour, start_time->minute,
			start_time->second, end_time->year, end_time->month, end_time->day, end_time->hour, end_time->minute,
			end_time->second);

	/*ѭ����ѯ��¼�ļ���ֱ���ҵ�������ʼʱ��ĵ�һ���ļ�*/
	time_copy(&qSession.t_Cur, &start_time);
	do {
		//��ʱ��ת�����ļ���
		sprintf_(qSession.fn_Cur, "0:/%s/%d_%d_%d.can",
				Dir[eClass], qSession.t_Cur.year, qSession.t_Cur.month,	qSession.t_Cur.day);
		TRACE_(QS_USER, NULL, "[VDR] Seeking file: %s", qSession.fn_Cur);

		res = f_open(&qSession.file, qSession.fn_Cur, FA_OPEN_EXISTING | FA_READ);	//�������ļ�
		if ((FR_OK == res) && (qSession.file.fsize > 0)) {	//�ļ������ҷǿգ����ҳɹ�
			bFind = 1;
			break;
		}
		else {
			f_close(&qSession.file);
		}

		time_GetNextDay(&qSession.t_Cur);	//ʧ�ܣ�����Сʱ��������
	} while (1 != time_compare(&qSession.t_Cur, &qSession.t_End)); //ֱ����������ʱ��

	if (0 == bFind)	//ָ��ʱ����޼�¼�ļ���ʧ��
			{
		return -ERR_VDR_FILE_NOTEXIST;
	}

//	TRACE_(QS_USER, NULL, "[VDR] Start file name : %s", qSession.fn_Cur);

	/*���濪ʼ�ļ���*/
	strcpy_(qSession.fn_Start, qSession.fn_Cur);

	/*��һ����ʼ���Ҽ�¼��ʼλ�ã������ļ�������ڴ�״̬����λ��¼�Ǹ���ʱ������*/
	ret = VDR_SeekRecord(	qSession.file,
							GET_TICKS(start_time->hour, start_time->minute, start_time->second),
							RECORD_SIZE(eClass),
							&qSession.u32Offset_Start);
	if (!ret) {
		return -ERR_VDR_FILE_SEEK;
	}

	/*��ʼ�ļ�ʹ����������ر�*/
	f_close(&qSession.file);

	TRACE_(QS_USER, NULL, "[VDR] Seeking END file >>>>>>>>>> ");

	/*��λ���һ����¼�����ļ���*/
	time_copy(&qSession.t_End, &end_time);
	do {
		//��ʱ��ת�����ļ���
		sprintf_(qSession.fn_End, "0:/%s/%d_%d_%d",
				Dir[eClass], qSession.t_End.year, qSession.t_End.month,	qSession.t_End.day);
		TRACE_(QS_USER, NULL, "[VDR] Seeking file: %s", qSession.fn_End);

		res = f_open(&qSession.file, qSession.fn_End, FA_OPEN_EXISTING | FA_READ);	//�������ļ�
		if ((FR_OK == res) && (qSession.file.fsize > 0))	//�ļ������ҷǿգ����ҳɹ�
				{
			bFind = 1;
			break;
		}
		else {
			f_close(&qSession.file);
		}

		time_GetPrevDay(&qSession.t_End);	//ʧ�ܣ��������ǰһ��
	} while (1 == time_compare(&qSession.t_End, &qSession.t_Start)); //ֱ�����￪ʼʱ��

	if (!bFind)
		return -ERR_VDR_FILE_NOTEXIST;

	/*��λ���һ����¼λ��*/
	ret = VDR_SeekRecord(	qSession.file,
							GET_TICKS(end_time->hour, end_time->minute, end_time->second),
							RECORD_SIZE(eClass),
							&qSession.u32Offset_End);

	/*�ļ�������������ر�*/
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
 * ����ʼ�ļ���һ����¼��ʼ�������ȡ������¼
 *
 * �ļ�����ΪqSession.fn_Cur����ǰ��λ��ΪqSession.u32Offset_Cur
 * ע�⣬�˺���ǰ���������ļ������Ѵ�
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

	Q_ASSERT(qSession.file.fs != NULL);	///�ļ������Ѿ���

	if (NULL == pBuf)
		return -ERR_PARAM_INVALID;

	if(NULL == qSession.file.fs)
		return -ERR_VDR_FILE_NOTEXIST;

	f = &qSession.file; ///�ļ����

	u16RecordSize = RECORD_SIZE[qSession.u8SessionType];	///�õ������͵�����¼�ĳ���

	f_lseek(f, qSession.u32Offset_Cur);	///��λ��ָ�뵽����ƫ��λ��
	res = f_read(f, pBuf, u16RecordSize, &already_read);  ///��ȡָ��������¼
	if (FR_OK != res) {
		f_close(f);
		return -ERR_SD_WRITE;
	}

	if (already_read != u16RecordSize) {	///ʵ�ʶ�ȡ�ֽ�������������
		f_close(f);
		return -ERR_SD_READ;
	}

	qSession.u32Offset_Cur += u16RecordSize;	///���¶�ָ��λ��

	/**��λ����һ�ļ��ĵ�һ����¼*/
	if (qSession.u32Offset_Cur >= f->fsize) {
		f_close(&qSession.file);
		/*�Ѿ��ǽ����ļ������ȡ����*/
		if (strcmp_(qSession.fn_Cur, qSession.fn_End)) {
			qSession.bAllBlocksFinishedReadout = 1;	///���ö�ȡ��ϱ�־
		}
		else {
			VDR_SeekNextFileBeginRecord();	///��ȡ�м��ļ�
		}
	}

	return 0;
}

/**
 * ��λ����һ�ļ��ĵ�һ����¼
 */
void VDR_SeekNextFileBeginRecord() {
	FRESULT res;
	u8 bFind = 0;

	/*ѭ����ѯ��¼�ļ���ֱ���ҵ�������ʼʱ��ĵ�һ���ļ�*/
	time_GetNextDay(&qSession.t_Cur);	//����Сʱ��������
	while (!strcmp_(qSession.fn_Cur, qSession.fn_End)) {

		//��ʱ��ת�����ļ���
		sprintf_(qSession.fn_Cur, "0:/%s/%d_%d_%d", Dir[qSession.u8SessionType], qSession.t_Cur.year,
				qSession.t_Cur.month, qSession.t_Cur.day);

		res = f_open(&qSession.file, qSession.fn_Cur, FA_OPEN_EXISTING | FA_READ);	//�������ļ�
		if ((FR_OK == res) && (qSession.file.fsize > 0))	//�ļ������ҷǿգ����ҳɹ�
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
 * �ӽ����ļ����һ����¼��ʼ��������ȡ������¼
 *
 * �ļ�����ΪqSession.fn_Cur����ǰ��λ��ΪqSession.u32Offset_Cur
 *
 * @param u16BlockSize			��¼���С
 * @param pBuf					���ջ���
 * @param bufSize				���ջ�������
 * @param pu16RecvBytes			ʵ�ʶ�ȡ�ֽ���
 * @return	0=�ɹ�������������
 */
int VDR_RetrieveSingleRecordBackward(u16 u16RecordSize, u8 *pBuf, u16 bufSize, u16 *pu16ReadBytes) {

	FIL *f;
	FILINFO *fno;
	FRESULT res;
	u32 record_size;
	u32 already_read;

	if (NULL == pBuf)
		return -ERR_PARAM_INVALID;

	/*���ļ�*/
	res = f_open(&qSession.file, qSession.fn_Cur, FA_OPEN_EXISTING | FA_READ);	//�������ļ�
	if (FR_OK != res) //�ļ����ڣ����ҳɹ�
			{
		f_close(&qSession.file);
		return -ERR_SD_FILENOTEXISTED;
	}

	f = &qSession.file; //�ļ����

	f_lseek(f, qSession.u32Offset_Cur);	///��λ��ָ�뵽����ƫ��λ��

	res = f_read(f, pBuf, u16RecordSize, &already_read);  ///��ȡָ��������¼
	if (FR_OK != res) {
		f_close(f);
		return -ERR_SD_WRITE;
	}

	if (already_read != u16RecordSize) {	///ʵ�ʶ�ȡ�ֽ�������������
		f_close(f);
		return -ERR_SD_READ;
	}

	*pu16ReadBytes = u16RecordSize; 	///����ʵ�ʶ�ȡ�ֽ���

	if (qSession.u32Offset_Cur > 0) {
		Q_ASSERT(qSession.u32Offset_Cur >= u16RecordSize);
		qSession.u32Offset_Cur -= u16RecordSize;
	}
	else {
		/**��λ��ǰһ�ļ������һ����¼*/
		f_close(&qSession.file);
		if (strcmp_(qSession.fn_Cur, qSession.fn_Start))
			qSession.bAllBlocksFinishedReadout = 1;	///���ö�ȡ��ϱ�־
		else
			VDR_SeekPreviousFileLastRecord(u16RecordSize);
	}

	return 0;
}

/**
 * ��λ��ǰһ�ļ������һ����¼
 *
 * @param u16RecordSize		������¼����
 */
void VDR_SeekPreviousFileLastRecord(u16 u16RecordSize) {
	FRESULT res;
	u8 bFind = 0;

	/*ѭ����ѯ��¼�ļ���ֱ���ҵ�������ʼʱ��ĵ�һ���ļ�*/

	time_GetPrevDay(&qSession.t_Cur);	//����Сʱ��������

	while (!strcmp_(qSession.fn_Cur, qSession.fn_Start)) {

		//��ʱ��ת�����ļ���
		sprintf_(qSession.fn_Cur, "0:/%s/%d_%d_%d",
				Dir[qSession.u8SessionType], qSession.t_Cur.year, qSession.t_Cur.month, qSession.t_Cur.day);

		res = f_open(&qSession.file, qSession.fn_Cur, FA_OPEN_EXISTING | FA_READ);	//�������ļ�
		if ((FR_OK == res) && (qSession.file.fsize > 0)) {	//�ļ������ҷǿգ����ҳɹ�
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
 * �жϼ�¼�Ƿ�ȫ������
 *
 * @return	TRUE=��ɣ�FALSE=δ���
 */
BOOL VDR_IsReadFinished() {

	if(qSession.bAllBlocksFinishedReadout == 1)
		return TRUE;

	return FALSE;
}


/**
 * @Func	ʱ�俽��
 * @Param	t1			[in]	ʱ��1
 * 			t2			[in]	ʱ��2
 * @Ret		��
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
 * ������һ������
 *
 * @param t		��ǰ������
 */
void time_GetNextDay(TIME *t) {
	u8 u8DaysofMonth = 0;

	t->day++;

	u8DaysofMonth = MISC_DaysofMonth(t->year/* + 2000*/, t->month); //ȡ�õ�ǰ���ж�����
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
 * ����ǰһ������
 *
 * @param t		��ǰ������
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
 * @Func	������һ��Сʱ��ʾ
 * @Param	t1			[in]	ԭʱ��
 * @Ret		t1			[out]	����1Сʱ���ʱ��
 */
void time_nexthour(TIME *t)
{
	t->hour++;
	if( t->hour >= 24)
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
 * ����ǰһ��Сʱ��ʾ
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
				u8DaysofMonth = MISC_DaysofMonth(t->year, t->month); //ȡ�õ�ǰ���ж�����
				t->day = u8DaysofMonth;
			}
		}
	}
}
#endif

/**
 * @Func	ʱ��Ƚ�
 * @Param	t1			[in]	ʱ��1
 * 			t2			[in]	ʱ��2
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
 * ����������¼�����ļ�λ�ã��۰����
 *
 * @param file				�Ѵ򿪵��ļ����
 * @param tick				ʱ��
 * @param u16Recordsize		��λ��¼����
 * @param pu32Offset	OUT	���ļ��е�ƫ����
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

	/*��λ������¼λ��*/
	f_lseek(f, 0); //ָ���ļ�ͷ

	if (0 == tick) //��0��0�뿪ʼ���������
			{
		*pu32Offset = 0;
		return TRUE;
	}

	mid_ticks = GET_TICKS(time.hour, time.minute, time.second);

	if (tick < mid_ticks) //������¼��ʱ����������ѯʱ�䣬�����ٲ�����
			{
		f_lseek(f, 0);
		return TRUE;
	}

	/*��������£��۰����*/
	while (low + u16Recordsize < high) {
		mid = (high - low) / u16Recordsize;
		mid = low + (mid >> 1) * u16Recordsize;
		res = f_lseek(f, mid); //��ָ��������۰��¼��
		if (res != FR_OK) {
			f_close(f);
			return FALSE;
		}

		res = f_read(f, (u8*) &time, u16Recordsize, &read); //���ļ��۰봦�ļ�¼
		if (res != FR_OK) {
			f_close(f);
			return FALSE;
		}

		mid_ticks = GET_TICKS(time.hour, time.minute, time.second);

		if (tick > mid_ticks)
			low = mid;	//ʱ���С
		else if (tick < mid_ticks)
			high = mid;	//ʱ��ϴ�
		else if (tick == mid_ticks)
			low = mid;
	}

	f_lseek(f, mid); //�ļ���ָ������������¼λ��

	*pu32Offset = mid;

	return TRUE;
}

#if 0
/**
 * @Func	����洢ԭʼ֡��ע���ڽ��յ�һ֡��Ӧ�������øù���
 * @Param	pNode			[in]	�ڵ�
 * @Ret		��
 */
void SD_RequestDumpCANFrame(DATANODE* pNode)
{
	if(pNode)
	pNode->reference ++;
}

/**
 * @Func	�洢ԭʼ֡
 * @Param	pFrame			[in]	֡����
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
void SD_DoDumpCANFrame()
{
	DATANODE* pNode;
	u8 dump_times = 0;

	pNode = MP_GetAllocNodeHead(); //ȡ���������׽ڵ�

	while(pNode && dump_times < DUMP_MAXFRAMES_PERCALL)
	{
		SD_DumpCANFrame((CANRAWFRAME*)pNode->data); //�洢�������Ƿ�ɹ����ͷŽڵ�
		MP_DecountReference(pNode);
		MP_FreeNode(pNode);//�ͷŽڵ�

		pNode = pNode->next;//��������ĺ����ڵ�
		dump_times ++;
	}
}

/**
 * @Func	�洢ԭʼ֡
 * @Param	pFrame			[in]	֡����
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
int SD_DumpCANFrame(CANRAWFRAME* pFrame)
{
	char fn[50];
	TIME timer;
	FRESULT res;
	u32 bw = 0;

	SysTimer_GetRaw(&timer); //ȡ��ǰʱ��

	encode((u8*)pFrame, sizeof(CANRAWFRAME));//���ݼ���

	sprintf_(fn, "0:/CAN/%d_%d_%d_%d.can",
			timer.year, timer.month, timer.day, timer.hour);//��ʱ��ת�����ļ���
	TRACE_(QS_USER, NULL, "[SD] trying to Seek file: %s", fn);

	if(strcmp_(fn, can_wsession.fn_Cur))//�ļ��Ѵ���
	{
		if(NULL == can_wsession.file) //�״δ�
		{
			//���ļ��������ڴ������ļ�
			res = f_open(can_wsession.file, can_wsession.fn_Cur, FA_OPEN_ALWAYS | FA_WRITE);
			if(FR_OK == res)//�ļ����ڣ����ҳɹ�
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
		strcpy_(can_wsession.fn_Cur, fn); //������ǰ�ļ���

		if(can_wsession.file)//�رվ��ļ�
		{
			f_close(can_wsession.file);
		}

		//���ļ��������ڴ������ļ�
		res = f_open(can_wsession.file, can_wsession.fn_Cur, FA_OPEN_ALWAYS | FA_WRITE);
		if(FR_OK == res)//�ļ����ڣ����ҳɹ�
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
#endif


/**
 * ���浥����¼�������뻺����д�ļ�
 *
 * @param pTime			ʱ��
 * @param pRecord		������¼
 * @return	0=�ɹ�������������
 */
int VDR_DumpRecord_Speed(TIME *pTime, tSPEEDPERSEC *pRecord) {
	static tRECORD_SPEED tSpeedLogBuf;	///���ػ���
	tTIME time_BCD;
	int ret = 0;

	TIME2BCDTIME(pTime, &time_BCD);	///ʱ���ʽת����BCDʱ��

	/**������ʼʱ��*/
	if (GET_TICKS(pTime->hour, pTime->minute, pTime->second) == 0) {
		BCDTIME_CPY(&tSpeedLogBuf.time_Start, &time_BCD);
	}

	/**������¼�뻺��*/
	memcpy_((u8*)&tSpeedLogBuf.tSpeed[pTime->second], (u8*)pRecord, sizeof(tRECORD_SPEED));

	/**���һ����ʱд�ļ�*/
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
 * �����¹��ɵ��¼
 *
 * @param pTime			ʱ��
 * @param pRecord		������¼
 * @return
 */
int VDR_DumpRecord_Accident(TIME *pTime, tITEM_ACCIDENT *pRecord) {
	static tRECORD_ACCIDENT tLogBuf;	///���ػ���
	static u8 u8Cur = 0;	///�����¼����������
	tTIME time_BCD;
	int ret = 0;
	tLICENSE_ST *pLicense;

	TIME2BCDTIME(pTime, &time_BCD);	///ʱ���ʽת����BCDʱ��

	memcpy_((u8*)&tLogBuf.atItem[u8Cur++], (u8*)pRecord, sizeof(tITEM_ACCIDENT));

	/*��������100����¼��д���ļ�*/
	if(u8Cur == 100) {
		/**��丽����Ϣ*/
		BCDTIME_CPY(&tLogBuf.time_End, &time_BCD);	///����ʱ��
		VDR_GetGPSInfo(&tLogBuf.tLastValidPos);		///λ����Ϣ
		pLicense = (tLICENSE_ST*)VDR_GetParam(VDR_PARAM_LICENSE);	///���ƺ�
		memcpy_(tLogBuf.aLicense, pLicense->aLicense, 18);

		/**��¼��д���ļ�*/
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
 * �����޻���ĵ�����¼
 *
 * ��Щ���͵ļ�¼���ڲ��Ǻ�Ƶ�������ܵ�������д�ļ�����������ʱ��ʻ��¼��λ����Ϣ��¼����ʻ����Ϣ��¼���ⲿ�����¼�������޸ļ�¼���ٶ�״̬��¼
 *
 * @param pTime				ʱ��
 * @param pRecord			��¼
 * @param u16BlockLen		��¼����
 * @return	0=�ɹ�������������
 */
int VDR_DumpRecord_Instant2File(TIME *pTime, u8 *pRecord, u16 u16BlockLen) {

	return VDR_Dump2File(pTime, pRecord, u16BlockLen);
}


/**
 * �����¼������������׼���ã����������Ͳ���д�ļ���ͨ�ô洢�ӿ�
 *
 * @param eClass		��¼����
 * @param pTime			ʱ������
 * @param pBlock		��¼���ݿ�
 * @param u16BlockLen	��¼���ݿ鳤��
 * @return
 */
int VDR_Dump2File(TIME *pTime, u8 *pBlock, u16 u16BlockLen) {
	char fname[20];
	FRESULT res;
	FIL file;
	u32 bw;

	if ((pBlock != NULL ) && (u16BlockLen > 0))
		return -ERR_PARAM_INVALID;

	/**��ʱ��ת�����ļ���*/
	sprintf_(fname, "0:/%s/%d_%d_%d",
			Dir[eLOG_Accident], pTime->year, pTime->month, pTime->day);

	/**�������ļ���д���ļ�β*/
	res = f_open(&file, fname, FA_OPEN_EXISTING | FA_WRITE);
	if (FR_OK == res) {
		f_lseek(file, file.fsize);
		res = f_write(file, pBlock, u16BlockLen, &bw); //д��¼
		if (res || (bw != u16BlockLen)) {
			f_close(&file);
			return -ERR_SD_WRITE;
		}
	}
	else {
		/**�������ļ�����¼д���ļ�ͷ*/
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
 * @Func	����SD����������
 * @Param	free		[in]	����������С����λKB
 * @Ret		0=�ɹ�������������
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
 * @Func	����SD��������
 * @Param	volume		[in]	��������С����λKB
 * @Ret		0=�ɹ�������������
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
 * ����ʱ��ת����BCDʱ��
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
 * BCDʱ��ת��������ʱ��
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
 * BCD��ʽʱ�俽��
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
 * ��ȡ��ǰ��λ��Ϣ
 *
 * @param ptPos
 */
void VDR_GetGPSInfo(tPOSITION *ptPos) {


}

#if 0
/*
 * ��������
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

	/*������ȡ5����¼������*/
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

	/*��ѯ��ɣ��ͷŲ�ѯSESSION*/
	SD_ClearQuerySession(&qSession);

	/*������ȡ��¼������*/
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

	/*��ѯ��ɣ��ͷŲ�ѯSESSION*/
	SD_ClearQuerySession(&qSession);
}
#endif
