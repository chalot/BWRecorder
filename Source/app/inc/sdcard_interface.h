/**   
* @Title: sdcard.h  
* @Description: TODO(��һ�仰�������ļ���ʲô) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-5-27 ����11:28:38 
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
	eLOG_Accident, //�¹��ɵ�
	eLOG_OvertimeDrive, // ��ʱ��ʻ
	eLOG_PositionInfo, //λ����Ϣ
	eLOG_DriverInfo, //��ʻ�����
	eLOG_Distance, //���
	eLOG_InstallParam, //��װ������¼
	eLOG_Power, //�ⲿ�����¼
	eLOG_ParamChange, //�����޸ļ�¼
	eLOG_SpeedState, //�ٶ�״̬��־

} eLOGCLASS;

#if 0

/*CAN֡�洢��ʽ����ԭʼ֡��Ϣ����������˱�Ҫ���ֶΣ�dir����tickʱ�̣������ڴ洢*/
typedef struct _CANRAWFRAME
{
	u32 id;				/*ID*/
	u8	dir		:	1;	/*�շ�����0��Tx��1��Rx*/
	u8	len		:	7;	/*֡����*/
	u8  data[8];		/*֡����*/
	u32	tick;			/*�շ�ʱ�̣���λ0.1ms������ʱ�����ʵ�ʷ���ʱ��ֵ������ʱ�����ʵ�ʽ���ʱ��ֵ*/
} CANRAWFRAME;


#endif

//GPS�洢��Ϣ
typedef	struct
{
	u8		bInfoValid			:	4;		//��λ��Ч��1=��Ч��0=��Ч
	u8      bAntenaState		:	4;		//����״̬��1=�γ���0=����
	u8		Latitude[4];			//γ�ȣ���λ=10^-3�֣�ѹ��BCD��
	u8		Longtitude[4];			//���ȣ���λ=10^-3�֣�ѹ��BCD��
	u8		Altitude[2];			//���θ߶ȣ���λ=�ף�ѹ��BCD��
	u8		Speed[2];				//�ٶȣ���λ=10��/ʱ��ѹ��BCD��
	u8		Direction[2];				//���򣬵�λ=�֣�ѹ��BCD��
	u8		u8ValidSatellites;		//����ʹ��������
	u8		u8VisibleSatellites;	//�ɼ�������
//	u16		u16Year;				//��ǰȡ�õ�GPS��Ϣ-��
//	u8		u8Month;				//��ǰȡ�õ�GPS��Ϣ-��
//	u8		u8Day;					//��ǰȡ�õ�GPS��Ϣ-��
//	u8		u8Hour;					//��ǰȡ�õ�GPS��Ϣ-ʱ
//	u8		u8Minute;				//��ǰȡ�õ�GPS��Ϣ-��
//	u8		u8Second;				//��ǰȡ�õ�GPS��Ϣ-��

}	_tGPSSaveInfo;/*T_GPSINFO;*/


/*CAN֡��¼��ʽ*/
//typedef struct _CAN_DUMPRECORD
//{
//	u32 	tick;	//ʱ��ƫ��
//	CANRAWFRAME info;	//֡��Ϣ
//} T_CAN_DUMPRECORD;

#define T_CAN_DUMPRECORD	CANRAWFRAME

/*GPS��¼��ʽ*/
typedef struct _GPS_DUMPRECORD
{
	u32 	tick;	//ʱ��ƫ��
//	T_GPSINFO info;	//֡��Ϣ
} T_GPS_DUMPRECORD;

typedef union _DUMPRECORD
{
	uint8_t eCAN[sizeof(T_CAN_DUMPRECORD)];
	uint8_t eGPS[sizeof(T_GPS_DUMPRECORD)];
}T_DUMPRECORD;

#define SESSION_FILE_LEN	7		///�Ự�е��ļ����ȣ���ʽ����_��_��
#define SESSION_FILELIST_LEN	100	///�Ự�е��ļ��б�����

/*��ѯ�Ự*/
typedef struct _QSESSION
{
	u8		IsActive;		///�Ƿ�״̬��1=��ռ�ã�0=����
	u8 		u8SessionType;	///��ѯ����
	u8		u8Cmd;			//��ѯ����
	FIL		file;  			//���ļ����
//	char 	fn_Start[SESSION_FILE_LEN];	//��һ����¼�����ļ���
	u32 	u32Offset_Start;///��һ����¼���ļ��е�ƫ����
//	char 	fn_End[SESSION_FILE_LEN];		//���һ����¼�����ļ���
	u32 	u32Offset_End;	///���һ����¼���ļ��е�ƫ����
//	char 	fn_Cur[SESSION_FILE_LEN];		//��ǰ���ļ���
	u32		u32Offset_Cur; 	//��ǰ��ȡƫ��λ��
	char 	aFileList[100][7];	///���������������ļ��б�
	u8		u8FileAmt;		///�ļ�����
	u8		u8FileStart;		///��һ����¼�ļ����ļ��б��е�������
	u8		u8FileEnd;			///���һ����¼�ļ����ļ��б��е�������
	u8		u8FileCur;		///��ǰ���ļ����ļ��б��е�������
	TIME 	t_Start;		//��ѯ������ʼʱ��
	TIME 	t_End;			//��ѯ��������ʱ��
	TIME 	t_Cur;			//��ǰ����ʱ��
	u8 		bAllBlocksFinishedReadout;///�������ݶ�ȡ��ϱ�־
	u16 	u16BlocksToTrans;	///�����������ܿ���
	u16		u16BlocksTransed;	///�Ѵ����������ܿ���

} T_QSESSION;

/*д�Ự*/
//typedef struct _WSESSION
//{
//	FIL	*file;  	//�ļ����
//	char fn_cur[20];	//��ǰ��ѯ�ļ���
//
//} T_WSESSION;

/**
 * @Func	SD����ʼ��
 * @Param	��
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
//int SDCard_Init();

/**
 * @Func	�洢ԭʼ֡
 * @Param	pFrame			[in]	֡����
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
//int SD_DumpFrame(CANRAWFRAME* pFrame);

/**
 * @Func 	��SD���и�����ʼʱ��ͽ���ʱ�������ʷ��¼����ȷ����
 *			�ύһ�β�ѯ�����Ӧ����һ���µĲ�ѯ�����ţ����ݴ�����������ͬ�Ĳ�ѯ���󣬺Ͷ�λ���ݶΣ�
 *			һ��ͬһʱ��ֻ�ܴ���һ����Ч��ѯ����
 * @Param	tick_Begin		[in]	��ʼʱ��
 * 			tick_End		[in]	����ʱ��
 * 			rId				[in]	��Ч��ѯ������
 * @Ret		0=�ɹ�������ʧ�ܣ����ش�����루RC��
  */
//int SD_StartNewQuery(eLOGCLASS class, TIME *tick_Begin, TIME *tick_End/*, u8 *rId*/);

/**
 * @Func 	��ȡ֡�����ݣ����ݲ�ѯ����ID��Ψһȷ����ѯ���󣬿��ܶ�ͬһ����IDִ�ж�β�ѯ�����ⷢ��
 * 			������������Ҫ�ֿ��ȡ������¡�
 * @Param	rId				[in]	��ѯ������
 * 			pMsgBuf			[in]	���ջ���
 * 			u16MsgBufSize	[in]	���ջ�������
 * @Ret		u16��ʵ�ʶ�ȡ�ֽ�����=0˵����ȡ����
 */
//u16 SD_RetrieveFrameBlock(eLOGCLASS class, u8 *pMsgBuf, u16 u16MsgBufSize);

/**
 * @Func	֪ͨ����������ʼ���״̬������Э��Ҫ����Ϊ��ʼһ���µļ�ʻѭ������ѭ����
 * 			�贴��һ���µĴ洢�ļ�����CAN֡����
 * @Param	time			[in]	��ǰʱ�̣�������Ϊ�ļ�����������
 * @Ret		��
 */
//void SD_NotifyDriveOrChargeSessionStart(TIME *time);

/**
 * @Func	֪ͨ����ͣ����������״̬������Э��Ҫ����Ϊһ����ʻѭ������ѭ��������
 * 			��ֹ�ļ��洢
 * @Param	time			[in]	��ǰʱ��
 * @Ret		��
 */
//void SD_NotifyDriveOrChargeSessionTerminal(TIME *time);

/**
 * @Func	�洢GPS����
 * @Param	pGpsInfo			[in]	��λ����
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
//int SD_DumpGpsInfo(_tGPSSaveInfo* pGpsInfo);
//int SD_DumpGpsInfo(_tGPSInfo* pGpsInfo);

/**
 * @Func	����洢ԭʼ֡��ע���ڽ��յ�һ֡��Ӧ�������øù���
 * @Param	pNode			[in]	�ڵ�
 * @Ret		��
 */
//void SD_RequestDumpFrame(DATANODE* pNode);

/**
 * @Func	�洢ԭʼ֡
 * @Param	pFrame			[in]	֡����
 * @Ret		0=�ɹ�������Ϊ�������(RC)
 */
//void SD_DoDumpFrame();

/**
 * @Func	����SD����������
 * @Param	free		[in]	����������С����λKB
 * @Ret		0=�ɹ�������������
 */
int SD_GetFree(u32 *free);

/**
 * @Func	����SD��������
 * @Param	free		[in]	����������С����λKB
 * @Ret		0=�ɹ�������������
 */
int SDCard_Getvolume(u32 *volume);

BOOL SDCard_Clear();
u8 SDCard_GetExistedState();//SD���β�״̬
u8 SDCard_GetStorageState();//SD���洢״̬
u8 SDCard_GetRWState();//SD����д״̬


#pragma pack()

#endif /* SDCARD_H_ */
