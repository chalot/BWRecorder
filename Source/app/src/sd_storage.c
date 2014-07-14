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
#include <vdr.h>

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
		sizeof(tRECORD_INSTALLPARAM),
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
static void VDR_ConfigFileName(char *pFName, u8 u8Year, u8 u8Month, u8 u8Day);
static int DATACMP(char *s1, char *s2);
static int VDR_USB_DumpSession(eLOGCLASS eClass, FIL *f, u8 direct);
static void VDR_GetGPSInfo(tPOSITION *ptPos);
static int VDR_ScanLogFiles(eLOGCLASS eClass);

/*----------������������������---------------------------------------------------------------------*/

/**
 * @Func	SD����ʼ��
 * @Param	��
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
int VDR_SDInit() {
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
void VDR_ClearQuerySession() {
	f_close(&qSession.file);
	ZeroMem((u8*) &qSession, sizeof(T_QSESSION));
}

/**
 * ����һ���µĲ�ѯ����
 *
 * @param eClass			��ѯ����
 * @param pTime_Start		��ʼʱ��
 * @param pTime_End			����ʱ��
 * @return
 */
int VDR_NewQuerySession(eLOGCLASS eClass, TIME *pTime_Start, TIME *pTime_End) {
	int iRet;

	TRACE_(QS_USER, NULL, "[VDR] ******** NEW Query Session **********");
	TRACE_(QS_USER, NULL, "[VDR] type = %s, from %d-%d-%d:%d-%d-%d to %d-%d-%d:%d-%d-%d ", Dir[eClass],
			pTime_Start->year, pTime_Start->month, pTime_Start->day, pTime_Start->hour, pTime_Start->minute,
			pTime_Start->second, pTime_End->year, pTime_End->month, pTime_End->day, pTime_End->hour, pTime_End->minute,
			pTime_End->second);

	/**����ɵĲ�ѯ״̬*/
	SD_ClearQuerySession(&qSession);

	/**����Ự���ͣ�����¼����*/
	qSession.u8SessionType = eClass;

	/**ȷ������ʱ��Ӧ���ڵ��ڿ�ʼʱ��*/
	if(-1 != time_compare(pTime_Start, pTime_End))
	return -ERR_PARAM_INVALID;

	/**������¼�ļ�����Ŀ¼�������ļ����õ��ļ��б�*/
	iRet = VDR_ScanLogFiles(eClass);
	if(iRet < 0)
	return iRet;

	/**������ʼ�ļ�����һ����¼λ��*/
	iRet = VDR_FindFileStartFrom(pTime_Start);
	if(iRet < 0)
	return iRet;

	iRet = VDR_FindRecord(eClass, pTime_Start, qSession.u8FileStart, &qSession.u32Offset_Start);
	if(iRet < 0)
	return iRet;

	/**���ҽ����ļ������һ����¼λ��*/
	iRet = VDR_FindFileEndTo(eClass, pTime_End);
	if(iRet < 0)
	return iRet;

	iRet = VDR_FindRecord(eClass, pTime_End, &qSession.u8FileEnd, &qSession.u32Offset_End);
	if(iRet < 0)
	return iRet;

	/**���Ự��Ϣ*/
	time_copy(&qSession.t_Start, pTime_Start);
	time_copy(&qSession.t_End, pTime_End);

	qSession.u8FileCur = qSession.u8FileStart;
	qSession.u32Offset_Cur = qSession.u32Offset_Start;

	/**�ļ�������������ر�*/
	f_close(&qSession.file);

	TRACE_(QS_USER, NULL, "[VDR] ========= Query Session Result ===========");
	TRACE_(QS_USER, NULL, "[VDR] start file = %s, offset = %d",
			qSession.aFileList[qSession.u8FileStart], qSession.u32Offset_Start);
	TRACE_(QS_USER, NULL, "[VDR] end file = %s, offset = %d",
			qSession.aFileList[qSession.u8FileStart], qSession.u32Offset_End);

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

	if (NULL == pBuf)
		return -ERR_PARAM_INVALID;

	if(NULL == qSession.file.fs)
		return -ERR_VDR_FILE_NOTEXIST;

	if(qSession.file.fs == NULL) {
		/*���ļ�*/
		res = f_open(&qSession.file, qSession.aFileList[qSession.u8FileCur], FA_OPEN_EXISTING | FA_READ);
		if (FR_OK != res) //�ļ����ڣ����ҳɹ�
				{
			f_close(&qSession.file);
			return -ERR_SD_FILENOTEXISTED;
		}
	}

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
		if (qSession.u8FileCur == qSession.u8FileEnd) {
			qSession.bAllBlocksFinishedReadout = 1;	///���ö�ȡ��ϱ�־
		}
		else {
			VDR_SeekNextFileBeginRecord();	///��ȡ�м��ļ�
		}
	}

	return 0;
}

/**
 * ��λ����һ�ļ��ĵ�һ����¼����һ�ļ���ָ�뵱ǰ�����ļ�����������ļ�
 */
void VDR_SeekNextFileBeginRecord() {
	FRESULT res;
	u8 bFind = 0;
	char fn[7];
	u8 i;
	u8 fCur = 0;

	fCur = qSession.u8FileCur;

	i = 0;
	while(i < qSession.u8FileAmt) {
		if(DATACMP(qSession.aFileList[i], qSession.aFileList[qSession.u8FileCur]) && (i != qSession.u8FileCur)) {
			if(DATACMP(qSession.aFileList[fCur], qSession.aFileList[i])) {
				fCur = i;
			}
		}
		i++;
	}

	qSession.u8FileCur = fCur;
	qSession.u32Offset_Cur = 0;
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
int VDR_RetrieveSingleRecordBackward(u8 *pBuf, u16 bufSize) {

	FIL *f;
	FILINFO *fno;
	FRESULT res;
	u32 record_size;
	u32 already_read;
	u16 u16RecordSize = 0;

	if (NULL == pBuf)
		return -ERR_PARAM_INVALID;

	if(qSession.file.fs == NULL) {
		/*���ļ�*/
		res = f_open(&qSession.file, qSession.aFileList[qSession.u8FileCur], FA_OPEN_EXISTING | FA_READ);
		if (FR_OK != res) //�ļ����ڣ����ҳɹ�
				{
			f_close(&qSession.file);
			return -ERR_SD_FILENOTEXISTED;
		}
	}

	u16RecordSize = RECORD_SIZE(qSession.u8SessionType);

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

	if (qSession.u32Offset_Cur > u16RecordSize) {
		qSession.u32Offset_Cur -= u16RecordSize;
	}
	else {
		/**��λ��ǰһ�ļ������һ����¼*/
		f_close(&qSession.file);
		if (qSession.u8FileCur == qSession.u8FileStart)
			qSession.bAllBlocksFinishedReadout = 1;	///���ö�ȡ��ϱ�־
		else
			VDR_SeekPreviousFileLastRecord();
	}

	return 0;
}

/**
 * ��λ��ǰһ�ļ������һ����¼
 *
 * @param u16RecordSize		������¼����
 */
void VDR_SeekPreviousFileLastRecord() {

	FRESULT res;
	u8 bFind = 0;
	char fn[7];
	u8 i;
	u8 fCur = 0;

	fCur = qSession.u8FileCur;

	i = 0;
	while(i < qSession.u8FileAmt) {
		if(DATACMP(qSession.aFileList[qSession.u8FileCur], qSession.aFileList[i]) && (i != qSession.u8FileCur)) {
			if(DATACMP(qSession.aFileList[i], qSession.aFileList[fCur])) {
				fCur = i;
			}
		}
		i++;
	}

	qSession.u8FileCur = fCur;
	qSession.u32Offset_Cur = qSession.file.fsize - RECORD_SIZE(qSession.u8SessionType);
}

/**
 * �жϼ�¼�Ƿ�ȫ������
 *
 * @return	TRUE=��ɣ�FALSE=δ���
 */
BOOL VDR_IsReadFinished() {
	return (qSession.bAllBlocksFinishedReadout == 1);
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

/**
 * �������ڹ����ļ�����14��2��12�գ�ת��Ϊ140212
 *
 * @param pFName		OUT		�ļ���
 * @param u8Year		��
 * @param u8Month		��
 * @param u8Day			��
 */
void VDR_ConfigFileName(char *pFName, u8 u8Year, u8 u8Month, u8 u8Day) {
	Q_ASSERT(pFName != NULL);

	pFName[0] = (u8Year / 10) + 0x30;
	pFName[1] = (u8Year % 10) + 0x30;
	pFName[2] = (u8Month / 10) + 0x30;
	pFName[3] = (u8Month % 10) + 0x30;
	pFName[4] = (u8Day / 10) + 0x30;
	pFName[5] = (u8Day % 10) + 0x30;

	pFName[6] = '\0';
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

#if 0
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


#if 0
/**
 * �����¹��ɵ㵥����¼
 *
 * @param pTime			ʱ��
 * @param pRecord		������¼
 * @return	0=�ɹ�������������
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
#endif

/**
 * �����¹��ɵ��¼����¼������Ӧ�Ѿ�׼����
 *
 * @param pBlock
 * @return
 */
int VDR_DumpAccident(tRECORD_ACCIDENT *pBlock) {
	int ret = 0;

	ret = VDR_Dump2File(eLOG_Accident, pBlock->time_End, (u8*)pBlock, sizeof(tRECORD_ACCIDENT));
	if(ret < 0) {
		return ret;
	}
	return 0;
}

/**
 * �����ٶ�״̬��¼����¼������Ӧ�Ѿ�׼����
 *
 * @param pBlock
 * @return
 */
int VDR_DumpSpeedStatus(tRECORD_SPEEDSTATUS *pBlock) {
	int ret = 0;

	ret = VDR_Dump2File(eLOG_SpeedState, pBlock->time_Begin, (u8*)pBlock, sizeof(tRECORD_SPEEDSTATUS));
	if(ret < 0) {
		return ret;
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
int VDR_DumpRecord_Instant2File(eLOGCLASS eClass, TIME *pTime, u8 *pRecord, u16 u16RecordLen) {

	return VDR_Dump2File(eClass, pTime, pRecord, u16RecordLen);
}
#endif

/**
 * �����¼������������׼���ã����������Ͳ���д�ļ���ͨ�ô洢�ӿ�
 *
 * @param eClass		��¼����
 * @param pTime			ʱ������
 * @param pBlock		��¼���ݿ�
 * @param u16BlockLen	��¼���ݿ鳤��
 * @return
 */
int VDR_Dump2File(eLOGCLASS eClass, TIME *pTime, u8 *pBlock, u16 u16BlockLen) {
	char fname[20];
	FRESULT res;
	FIL file;
	u32 bw;
	char fn[7];

	if ((pBlock != NULL ) && (u16BlockLen > 0))
		return -ERR_PARAM_INVALID;

	/**��ʱ��ת�����ļ���*/
	VDR_ConfigFileName(fn, pTime->year, pTime->month, pTime->day);
	sprintf_(fname, "0:/%s/%s",	Dir[(u8)eClass], fn);

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


/**
 * ������¼�ļ�����Ŀ¼�������ļ����õ��ļ��б�
 *
 * @param eClass		��¼����
 * @return	0=�ɹ�������������
 */
int VDR_ScanLogFiles(eLOGCLASS eClass) {
	char path[10];
	DIR dir;
	FRESULT ret;
	FILINFO fno;

	/**����Ŀ¼*/
	sprintf_(path, "0:/%s",	Dir[(u8)eClass]);

	ret = f_opendir(&dir, path);
	if (ret) {
		TRACE_(QS_USER, NULL, "Open directory error, %s\n", path);
		return -ERR_SD_FILENOTEXISTED;
	}
	else {
		qSession.u8FileAmt = 0;
		/**ѭ����ȡĿ¼�µ������ļ�*/
		for (;;) {
			ret = f_readdir(&dir, &fno);		/** Read a directory item */
			if (ret || !fno.fname[0]) {
				break;	/** Error or end of dir */
			}

			/**�ҵ���һ���ļ�*/
			TRACE_(QS_USER, NULL, "%s\n", fno.fsize, fno.fname);

			/**�����ļ����ļ��б�*/
			strcpy_(qSession.aFileList[qSession.u8FileAmt++], fno.fname);
		}
		if (ret) {
			TRACE_(QS_USER, NULL, "Read a directory error\n");
			return -ERR_SD_FILENOTEXISTED;
		}
	}

	return 0;
}



/**
 * ������ʼ�ļ�����һ����¼λ��
 *
 * @param pTime_Start		��ʼʱ��
 * @return	0=�ɹ�������������
 */
int VDR_FindFileStartFrom(TIME *pTime_Start) {
	u8 i;
	u8 iFirst = 0;
	char fn[7];

	if(qSession.u8FileAmt == 0)
		return -1;

	VDR_ConfigFileName(fn, pTime_Start->year, pTime_Start->month, pTime_Start->day);

	i = 0;
	iFirst = 0;
	while(i < qSession.u8FileAmt) {
		if(DATACMP(qSession.aFileList[i], fn) && DATACMP(qSession.aFileList[iFirst], fn))
			iFirst = i;
		i ++;
	}

	qSession.u8FileStart = iFirst;

	return 0;
}

/**
 * ������ʼ�ļ�����һ����¼λ��
 *
 * @param pTime_Start		��ʼʱ��
 * @return	0=�ɹ�������������
 */
int VDR_FindFileEndTo(TIME *pTime_End) {
	u8 i;
	u8 iLast = 0;
	char fn[7];

	if(qSession.u8FileAmt == 0)
		return -1;

	VDR_ConfigFileName(fn, pTime_End->year, pTime_End->month, pTime_End->day);

	i = 0;
	iLast = 0;
	while(i < qSession.u8FileAmt) {
		if(DATACMP(fn, qSession.aFileList[i]) && DATACMP(qSession.aFileList[iLast], fn))
			iLast = i;
		i ++;
	}

	qSession.u8FileEnd = iLast;

	return 0;
}


/**
 * ���Ҽ�¼�����ļ�λ��
 *
 * @param eClass
 * @param pTime_Start
 * @param u8FileIndex
 */
int VDR_FindRecord(eLOGCLASS eClass, TIME *pTime, u8 u8FileIndex, u32 *pu32Offset) {
	FIL *f;
	FRESULT res;
	UINT read;
	TIME time;
	tTIME bcdTime;
	TIME startTime;
	u32 u32Offset = 0;
	char fn[7];
	BOOL bFind = FALSE;
	u32 tick = 0;
	u32 start_tick = 0;
	u16 u16Recordsize = 0;

	u32 low = 0, high = 0, mid = 0;
	u32 mid_ticks = 0;

	Q_ASSERT(u8FileIndex < qSession.u8FileAmt);

	/*���ļ�*/
	sprintf_(fn, "0:/%s/%s", Dir[eClass], qSession.aFileList[u8FileIndex]);
	TRACE_(QS_USER, NULL, "[VDR] Seeking file: %s", fn);

	res = f_open(&qSession.file, fn, FA_OPEN_EXISTING | FA_READ);	//�������ļ�
	if ((FR_OK == res) && (qSession.file.fsize > 0)) {	//�ļ������ҷǿգ����ҳɹ�
		bFind = 1;
		break;
	}
	else {
		f_close(&qSession.file);
		if(qSession.file.fsize == 0) {
			TRACE_(QS_USER, NULL, "ERR,file is empty.");
			return -ERR_SD_FILEISEMPTY;	///���ؿ��ļ�����
		}
		else
			return -ERR_SD_FILENOTEXISTED;
	}

	f = qSession.file;

	low = 0;
	high = qSession.file.fsize;
	u16Recordsize = qSession.file.fsize;

	/*��λ������¼λ��*/

	/*����һ����¼��ʱ���Ƿ��Ѵ��ڿ�ʼʱ��*/
	f_lseek(f, 0);
	res = f_read(f, (u8*) &bcdTime, sizeof(tTIME), &read);
	if (res != FR_OK) {
		f_close(f);
		return -ERR_SD_READ;
	}

	BCDTIME2TIME(&bcdTime, &startTime);

	start_tick = GET_TICKS(startTime.hour, startTime.minute, startTime.second);
	tick = GET_TICKS(pTime->hour, pTime->minute, pTime->second);
	if(start_tick >= tick) {
		*pu32Offset = 0;
		return TRUE;
	}

	/*��������£��۰����*/
	mid_ticks = tick;
	while (low + u16Recordsize < high) {
		mid = (high - low) / u16Recordsize;
		mid = low + (mid >> 1) * u16Recordsize;
		res = f_lseek(f, mid); //��ָ��������۰��¼��
		if (res != FR_OK) {
			f_close(f);
			return FALSE;
		}

		res = f_read(f, (u8*) &time, sizeof(tTIME), &read); //���ļ��۰봦�ļ�¼
		if (res != FR_OK) {
			f_close(f);
			return FALSE;
		}

		BCDTIME2TIME(&bcdTime, &time);
		mid_ticks = GET_TICKS(time.hour, time.minute, time.second);

		if (tick > mid_ticks)
			low = mid;	//ʱ���С
		else if (tick < mid_ticks)
			high = mid;	//ʱ��ϴ�
		else if (tick == mid_ticks)
			low = mid;
	}

	*pu32Offset = mid;	///����������¼λ��

	f_close(f);

	return 0;
}



/**
 *
 *
 */
/**
 * �Ƚ��ļ�����Ӧ���ڴ�С
 *
 * @param s1		����1
 * @param s2		����2
 * @return	0����ȣ�1������1>����2,-1������1<����2
 */
int DATACMP(char *s1, char *s2) {
	u8 i= 0;

	while(i < 6) {
		if(s1[i] > s2[i])
			return 1;
		if(s1[i] < s2[i])
			return -1;
		i++;
	}

	return 0;
}

/**
 * �����ʱ�ļ�
 */
void VDR_RemoveOutdatedLogFiles() {





}

/**
 * ����USB���ݵ����ļ���
 *
 * @param pTime			ʱ��
 * @param pName		OUT	�ļ���
 */
void VDR_USB_CreateFileName(TIME *pTime, u8 *pName) {
	char aLicense[9];
	tCARINFO_ST* pLicense;
	ZeroMem(aLicense, 9);
	pLicense = ((tCARINFO_ST*)VDR_GetParam(VDR_PARAM_CARINFO))->tCarInfo.aLicense;
	if(memcmp_(pLicense, aLicense)) {
		memcpy_(aLicense, "δ֪����", 8);
	}
	else {
		memcpy_(aLicense, pLicense, 8);
		aLicense[8] = '\0';
	}

	sprintf_(pName, "D%d%d%d%d%d%d_%d%d%d%d_%s.VDR",
			(pTime->year - 2000) / 10, (pTime->year - 2000) % 10,
			pTime->month / 10, pTime->month % 10, pTime->day / 10, pTime->day % 10,
			pTime->hour / 10, pTime->hour % 10, pTime->minute / 10, pTime->minute % 10,
			aLicense);
}

/**
 * ��¼��USB���ݵ���
 *
 * @param f				�Ѵ򿪵Ĵ�д���ļ����
 * @param pTime			����ʱ��
 * @return	0=�ɹ�������������
 */
int VDR_DumpFiles2USBStorage(FIL *f, TIME *pTime) {
	u8 i = 0;
	u16 u16BlockAmt = 0;
	u16 bytesWritten = 0;
	u16 bytesToWrite = 0;
	u32 u32Offset = 0;
	FRESULT res;
	VDRUSBBLOCKHEAD tBlock;
	u8 *pData;
	TIME time_start;
	TIME time_end;
	int ret;

	if((f == NULL) || (f->fs == NULL))
		return -ERR_PARAM_INVALID;

	TRACE_(QS_USER, NULL, "************* [VDR] USB: Export Data To Device ********** ");

    TRACE_(QS_USER, NULL, "[VDR] USB: write [blocks amount], %d", u16BlockAmt);

	/**д�����ݿ�����*/
	u16BlockAmt = VDR_USB_BLOCK_AMOUNT;
	ENDIAN_U16(u16BlockAmt);

	f_lseek(f, 0);
    res= f_write (f, &u16BlockAmt, 2, (void *)&bytesWritten);
    if((bytesWritten == 0) || (res != FR_OK)) /*EOF or Error*/
    {
		f_close(f);
        TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
        return -ERR_SD_WRITE;
    }
    u32Offset = 2;

    /**˳���ȡ��д���¼��*/
    i = 0;
	while(i < VDR_USB_BLOCK_AMOUNT) {
		switch(VDR_BLOCK[i].code) {
		case	VDR_USB_CODE_VERSION: 		/// ִ�б�׼�汾��� ����A.6
		{	tVERSION *ptVersion;

			tBlock.u8Code = VDR_BLOCK[i].code;
			memcpy_(tBlock.aName, VDR_BLOCK[i].name, strlen_(VDR_BLOCK[i].name));
			tBlock.u32Len = sizeof(tVERSION);

			ptVersion = (tVERSION*)tBlock.pData;
			ptVersion->u8Version = 0x13;	///��¼��ִ�б�׼��ź�2λ,BCD
			ptVersion->u8FixID = 0;	///�޸ĵ���

			/**�����д���ֽڳ���*/
			bytesToWrite = sizeof(VDRUSBBLOCKHEAD) + tBlock.u32Len;

			/**дһ����¼���ļ�*/
			res= f_write (f, (u8*)&tBlock, sizeof(VDRUSBBLOCKHEAD) + tBlock.u32Len, (void *)&bytesWritten);
		    if((bytesWritten != bytesToWrite) || (res != FR_OK))
		    {
				f_close(f);
		        TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
		        return -ERR_SD_WRITE;
		    }
		}
			break;
		case	VDR_USB_CODE_DRIVERINFO:	/// ��ǰ��ʻ����Ϣ ����A.7
		{
			tDRIVERLICENSEINFO *pInfo;

			tBlock.u8Code = VDR_BLOCK[i].code;
			memcpy_(tBlock.aName, VDR_BLOCK[i].name, strlen_(VDR_BLOCK[i].name));
			tBlock.u32Len = sizeof(tDRIVERLICENSEINFO);

			pInfo = (tDRIVERLICENSEINFO*)tBlock.pData;
			memcpy_(pInfo->aDriverLicense, ((tLICENSE_ST*)VDR_GetParam(VDR_PARAM_LICENSE))->aLicense, 18) ;

			/**дһ����¼���ļ�*/
			res= f_write (f, (u8*)&tBlock, sizeof(VDRUSBBLOCKHEAD) + tBlock.u32Len, (void *)&bytesWritten);
		    if((bytesWritten != bytesToWrite) || (res != FR_OK))
		    {
				f_close(f);
		        TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
		        return -ERR_SD_WRITE;
		    }
		}
			break;
		case	VDR_USB_CODE_RTC: 			///ʵʱʱ�� ����A.8
		{
			tTIME *pTime;

			tBlock.u8Code = VDR_BLOCK[i].code;
			memcpy_(tBlock.aName, VDR_BLOCK[i].name, strlen_(VDR_BLOCK[i].name));
			tBlock.u32Len = sizeof(tTIME);

			pTime = (tTIME*)tBlock.pData;
			RTC_Get(time);
			TIME2BCDTIME(pTime, &time);

			/**дһ����¼���ļ�*/
			res= f_write (f, (u8*)&tBlock, sizeof(VDRUSBBLOCKHEAD) + tBlock.u32Len, (void *)&bytesWritten);
		    if((bytesWritten != bytesToWrite) || (res != FR_OK))
		    {
				f_close(f);
		        TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
		        return -ERR_SD_WRITE;
		    }
		}
			break;
		case	VDR_USB_CODE_DIST:			/// �ۼ���ʻ��� ����A.9
		{
			TIME time;
			tTIME *pTime;

			tBlock.u8Code = VDR_BLOCK[i].code;
			memcpy_(tBlock.aName, VDR_BLOCK[i].name, strlen_(VDR_BLOCK[i].name));
			tBlock.u32Len = sizeof(tTIME);

			pTime = (tTIME*)tBlock.pData;
			RTC_Get(time);
			TIME2BCDTIME(pTime, &time);

			/**дһ����¼���ļ�*/
			res= f_write (f, (u8*)&tBlock, sizeof(VDRUSBBLOCKHEAD) + tBlock.u32Len, (void *)&bytesWritten);
		    if((bytesWritten != bytesToWrite) || (res != FR_OK))
		    {
				f_close(f);
		        TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
		        return -ERR_SD_WRITE;
		    }
		}
			break;
		case	VDR_USB_CODE_PULSE:		 	///����ϵ�� ����A.10
		{
			tPULSE *pPulse;

			tBlock.u8Code = VDR_BLOCK[i].code;
			memcpy_(tBlock.aName, VDR_BLOCK[i].name, strlen_(VDR_BLOCK[i].name));
			tBlock.u32Len = sizeof(tPULSE);

			pPulse = ((tPULSE_ST*)VDR_GetParam(VDR_PARAM_PULSE));
			memcpy_(tBlock.pData, (u8*)pPulse, sizeof(tPULSE));

			/**дһ����¼���ļ�*/
			res= f_write (f, (u8*)&tBlock, sizeof(VDRUSBBLOCKHEAD) + tBlock.u32Len, (void *)&bytesWritten);
		    if((bytesWritten != bytesToWrite) || (res != FR_OK))
		    {
				f_close(f);
		        TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
		        return -ERR_SD_WRITE;
		    }
		}
			break;
		case	VDR_USB_CODE_CARINFO:		/// ������Ϣ ����A.11
		{
			tPULSE *pPulse;

			tBlock.u8Code = VDR_BLOCK[i].code;
			memcpy_(tBlock.aName, VDR_BLOCK[i].name, strlen_(VDR_BLOCK[i].name));
			tBlock.u32Len = sizeof(tPULSE);

			pPulse = ((tPULSE_ST*)VDR_GetParam(VDR_PARAM_PULSE));
			memcpy_(tBlock.pData, (u8*)pPulse, sizeof(tPULSE));

			/**дһ����¼���ļ�*/
			res= f_write (f, (u8*)&tBlock, sizeof(VDRUSBBLOCKHEAD) + tBlock.u32Len, (void *)&bytesWritten);
		    if((bytesWritten != bytesToWrite) || (res != FR_OK))
		    {
				f_close(f);
		        TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
		        return -ERR_SD_WRITE;
		    }
		}
			break;
		case	VDR_USB_CODE_CONFIG:			/// ״̬�ź�������Ϣ ����A.12
		{
			tACK_STATECONFIG *pInfo;

			tBlock.u8Code = VDR_BLOCK[i].code;
			memcpy_(tBlock.aName, VDR_BLOCK[i].name, strlen_(VDR_BLOCK[i].name));
			tBlock.u32Len = sizeof(tACK_STATECONFIG);

			pInfo = ((tSTATECONFIG_ST*)VDR_GetParam(VDR_PARAM_STATECONFIG));
			memcpy_(tBlock.pData, (u8*)pInfo, sizeof(tACK_STATECONFIG));

			/**дһ����¼���ļ�*/
			res= f_write (f, (u8*)&tBlock, sizeof(VDRUSBBLOCKHEAD) + tBlock.u32Len, (void *)&bytesWritten);
		    if((bytesWritten != bytesToWrite) || (res != FR_OK))
		    {
				f_close(f);
		        TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
		        return -ERR_SD_WRITE;
		    }
		}
			break;
		case	VDR_USB_CODE_UNIQID:	 	///��¼��Ψһ�Ա�� ����A.14
		{
			tUNIQID_ST *pInfo;

			tBlock.u8Code = VDR_BLOCK[i].code;
			memcpy_(tBlock.aName, VDR_BLOCK[i].name, strlen_(VDR_BLOCK[i].name));
			tBlock.u32Len = sizeof(tVDRID);

			pInfo = ((tUNIQID_ST*)VDR_GetParam(VDR_PARAM_ID));
			memcpy_(tBlock.pData, (u8*)pInfo->tID, sizeof(tVDRID));

			/**дһ����¼���ļ�*/
			res= f_write (f, (u8*)&tBlock, sizeof(VDRUSBBLOCKHEAD) + tBlock.u32Len, (void *)&bytesWritten);
		    if((bytesWritten != bytesToWrite) || (res != FR_OK))
		    {
				f_close(f);
		        TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
		        return -ERR_SD_WRITE;
		    }
		}
			break;
		case	VDR_USB_CODE_SPEED:			///08H ��ʻ�ٶȼ�¼ ����A.16
		{
			ret = VDR_USB_DumpSession(eLOG_Speed, f, BACKWARD);
			if(ret < 0)
				return ret;
			TRACE_(QS_USER, NULL, "[USB] dump Speed info OK.");
		}
			break;
		case	VDR_USB_CODE_POSITION:		//09H λ����Ϣ��¼ ����A.18
		{
			ret = VDR_USB_DumpSession(eLOG_PositionInfo, f, FORWARD);
			if(ret < 0)
				return ret;
			TRACE_(QS_USER, NULL, "[USB] dump POSITION info OK.");
		}
			break;
		case	VDR_USB_CODE_ACCIDENT:		//10H �¹��ɵ��¼ ����A.21 ȫ����¼
		{
			ret = VDR_USB_DumpSession(eLOG_Accident, f, BACKWARD);
			if(ret < 0)
				return ret;
			TRACE_(QS_USER, NULL, "[USB] dump ACCIDENT info OK.");
		}
			break;
		case	VDR_USB_CODE_OTDRIVE:		//11H ��ʱ��ʻ��¼ ����A.23 ȫ����¼
		{
			ret = VDR_USB_DumpSession(eLOG_OvertimeDrive, f, BACKWARD);
			if(ret < 0)
				return ret;
			TRACE_(QS_USER, NULL, "[USB] dump OVERTIME DRIVING info OK.");
		}

			break;
		case	VDR_USB_CODE_DRIVERLOG:		//12H ��ʻ����ݼ�¼ ����A.25 ȫ����¼
		{
			ret = VDR_USB_DumpSession(eLOG_DriverInfo, f, BACKWARD);
			if(ret < 0)
				return ret;
			TRACE_(QS_USER, NULL, "[USB] dump DRIVER IDENTIFY info OK.");
		}
			break;
		case	VDR_USB_CODE_POWER:			//13H �ⲿ�����¼ ����A.26 ȫ����¼
		{
			ret = VDR_USB_DumpSession(eLOG_Power, f, BACKWARD);
			if(ret < 0)
				return ret;
			TRACE_(QS_USER, NULL, "[USB] dump POWER info OK.");
		}
			break;
		case	VDR_USB_CODE_PARAMCHG:		//14H �����޸ļ�¼ ����A.29 ȫ����¼
		{
			ret = VDR_USB_DumpSession(eLOG_ParamChange, f, BACKWARD);
			if(ret < 0)
				return ret;
			TRACE_(QS_USER, NULL, "[USB] dump PARAM CHANGE info OK.");
		}
			break;
		case	VDR_USB_CODE_SPEEDSTATUS:	//15H �ٶ�״̬��־ ����A.31 ȫ����¼
		{
			ret = VDR_USB_DumpSession(eLOG_SpeedState, f, BACKWARD);
				if(ret < 0)
					return ret;
				TRACE_(QS_USER, NULL, "[USB] dump SPEED STATES info OK.");
		}
			break;

		default:
			TRACE_(QS_USER, NULL, "[USB] Unknown dump command.");
			return -ERR_PARAM_INVALID;
			break;
		}

		i++;
	}

	f_close(f);
    TRACE_(QS_USER, NULL, "[VDR] USB: *** All files has been dumped to USB device Successfully. ***");

	return 0;
}

/**
 * ���ֿ��¼����ת����USB�豸
 *
 * @param eClass		��¼����
 * @param f				�򿪵��ļ����
 * @param direct		��¼��ȡ����
 * @return
 */
int VDR_USB_DumpSession(eLOGCLASS eClass, FIL *f, u8 direct) {
	FRESULT res;
	int ret;
	u8 i;
	FIL _f;
	TIME time_start;
	TIME time_end;
	VDRUSBBLOCKHEAD tBlock;
	u16 bytesToWrite = 0;
	u16 bytesWritten = 0;
	u8 data[512];

	Q_ASSERT(f->fs != NULL);
	Q_ASSERT((direct == FORWARD) || (direct == BACKWARD));

	/**�ر�֮ǰ�Ĳ�ѯ*/
	VDR_CloseQuerySession();

	ZeroMem((u8*)&time_start, 6);
	ZeroMem((u8*)&time_end, 6);
	ret = VDR_NewQuerySession(eLOG_Speed, &time_start, &time_end);

	/**���ͷ��Ϣ*/
	tBlock.u8Code = VDR_BLOCK[i].code;
	memcpy_(tBlock.aName, VDR_BLOCK[i].name, strlen_(VDR_BLOCK[i].name));
	tBlock.u32Len = 0;

	/**�������ݳ���*/
	i = 0;
	while(i < qSession.u8FileAmt) {
		/*���ļ�*/
		res = f_open(_f, qSession.aFileList[i], FA_OPEN_EXISTING | FA_READ);
		if (FR_OK != res) {
			f_close(_f);
			return -ERR_SD_FILENOTEXISTED;
		}

		tBlock.u32Len += _f.fsize;
		f_close(_f);
		i++;
	}

	/**��дͷ*/
	bytesToWrite = sizeof(VDRUSBBLOCKHEAD);
	f_lseek(f, f->fsize); ///������ӵ��ļ�β
	res = f_write (f, (u8*)&tBlock, sizeof(VDRUSBBLOCKHEAD), (void *)&bytesWritten);
	if((bytesWritten != bytesToWrite) || (res != FR_OK))
	{
		f_close(f);
		TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
		return -ERR_SD_WRITE;
	}

	/**ѭ����ȡ��д�뵥����¼*/
	while(!VDR_IsReadFinished()) {
		if(direct == FORWARD)
			ret = VDR_RetrieveSingleRecordForward(data, RECORD_SIZE[eClass]);
		else
			ret = VDR_RetrieveSingleRecordBackward(data, RECORD_SIZE[eClass]);
		if(ret < 0) {
			return -ERR_USB_WRITE;
		}

		/**�����д���ֽڳ���*/
		bytesToWrite = RECORD_SIZE[eClass];

		/**дһ����¼���ļ�*/
		res= f_write (f, data, bytesToWrite, (void *)&bytesWritten);
		if((bytesWritten == 0) || (res != FR_OK))
		{
			f_close(f);
			TRACE_(QS_USER, NULL, "[VDR] USB: file write failed.");
			return -ERR_SD_WRITE;
		}
		i++;
	}

	/**�رյ�ǰ�Ĳ�ѯ*/
	VDR_CloseQuerySession();
	TRACE_(QS_USER, NULL, "[USB] dump Speed info OK.");

	return 0;
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
