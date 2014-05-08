/** @file qevent.h
  * @brief brief description
  * @author <gao zhengdong, zhengdgao@163.com>
  * @date 2014��2��24������2:53:16
  * @version V1.0
  * @note
  * detailed description
  */
#ifndef EVENT_H_
#define EVENT_H_

#include <stm32f2xx.h>
#include <protocol.h>
#include <qp_port.h>

/** ϵͳ�ź� =====================================================*/
enum Signals {

/*------- �㲥��Ϣ -----------*/
    Q_TIMEOUT_SIG = Q_USER_SIG,

    ACC_ON_SIG,              /* ACC�ϵ� */
    ACC_OFF_SIG,             /* ACC���� */
    SLEEP_REQ_SIG,    //���������ź�
    WAKEUP_REQ_SIG,   //���������ź�

    SYS_POWEROFF,	///�ػ�
    SYS_RESET,	///��λ
    SYS_RECOVERFACTORYSETTING, ///�ն˻ָ���������


    MAX_PUB_SIG,             /* 7���һ���㲥�źţ����㲥�ź����� */

    ACC_ONCHANGE_SIG,        /* 8ACC״̬�仯 */

    PER_SECOND_SIG,          //�붨ʱ�ź�

/*------- CAN�������Ϣ -----------*/
    CAN_NEWSENDMSG_SIG,    /* 9�µ�һ������֡ */
    CAN_NEWRECVMSG_SIG,    /* 10�µ�һ������֡ */
    CAN_WAKEUP_SIG, //11
    CAN_OK_SIG,
    CAN_LED_TIMEOUT_SIG,
    CAN_SLEEP_REQ_SIG,
    CAN_RETRIEVE_TIMEOUT_SIG,
    CAN_INT_NEWMSG_SIG,
    CAN_RXIDLE_TIMEOUT_SIG,
    CAN_CONFLICT_TIMEOUT_SIG,
    DATA_READY_SIG,                        /* 17֡���� */

/* AO_Modem */

    AT_OK_SIG,              /* 18ATӦ�� */
    AT_REGGED_SIG,  //19
    AT_PINREADY_SIG,    //20                                            /*19 PIN READY */
    AT_NETREGGED_SIG,     //21                                   /* 20�ź����ͣ�ע�������� */
    AT_NETDISREG_SIG,     //22                                        /* 21�ź����ͣ����� */
    AT_CSQ_SIG,          //23                                      /* 22�ź����ͣ��ź�ǿ�� */
    AT_CONNECTED_SIG,   //24
    AT_DISCONNECTED_SIG,//25
    AT_DATAREADY_SIG,   //26                                         /* 23����֡���� */
   AT_SENDREADY_SIG,     //27                                       /* 24��Ϣ�������� */
    AT_SENDFILLDATA_SIG,  //28                              /* 25����Ӧ��������䷢������ */
    AT_ERROR_SIG,          //29                                  /* 26�ź����ͣ�AT��Ӧ���� */
    AT_CMSERROR_SIG,        //30                                /* 27�ź����ͣ�CMS Error */
    AT_CMEERROR_SIG,         //31                               /* 28�ź����ͣ�CME Error */
    AT_SENDERROR_SIG,    //32
    AT_FILLDATATIP_SIG,      //33                                   /* 29����������ʾ'>' */
    AT_SENDOK_SIG,        //34
    AT_DATAQUERY_TIMEOUT_SIG, //35                                 /* 30�������ݲ�ѯ��ʱ */
    AT_DATAQUERY_SIG,  //36

/*------- GPRS��Ϣ -----------*/
    GPRS_COMMINGMSG_SIG, //37
    GPRS_SERVER_CONNECTED_SIG, //38/* �����Ϸ����� */
    GPRS_PING_TIMEOUT_SIG, //39
    GPRS_SENDREQ_SIG,     /* 40������ */
    GPRS_LED_TIMEOUT_SIG, /* GPRS״̬�ź� */
    GPRS_RETRIEVE_TIMEOUT_SIG,
    NEW_TASKSENDREQ_SIG,
    GPRS_SLEEPREQ_TIMEOUT_SIG,
    GPRS_PHONECALL_HANDLED_SIG,
    GPRS_SMS_HANDLED_SIG,
    GPRS_COMMINGCALL_SIG,
    GPRS_COMMINGSMS_SIG,
    GPRS_RESET_SIG,
    SIMCARD_ONCHANGE_SIG,
    GPRS_SACK_SIG,    ///������Ӧ��
    SACK_TERMREG_OK_SIG,    ///�ն�ע��ɹ�Ӧ��
    SACK_TERMREG_ERR_SIG,    ///�ն�ע��ʧ��Ӧ��

    GPRS_SHUTDATACOMM, /**�ر�����ͨ��*/
    GPRS_SHUTALLCOMM, /**�ر���������ͨ��*/
    GPRS_CONNECTDEFAULTSERVER, ///�л���ԭȱʡ������

    GPRS_DIAL_NORMAL_SIG,	///����ͨ��
    GPRS_DIAL_MONITOR_SIG,	///����



/*------- GPS��Ϣ -----------*/
    GPS_RPT_TIMEOUT_SIG, //��ʱ�ϴ�
    GPS_ANTENA_ONCHANGE_SIG, //GPS���߰β�״̬�仯
    GPS_TRACK_SIG, ///����

/*------- ������Ϣ -----------*/
    SYS_UPGRAGE_SIG,
    UPG_REQUEST_SIG, ///��������
    UPG_START_SIG,
    UPG_OK_SIG,
    UPG_ERR_SIG,


/*------- �ⲿ�ź�����Ϣ -----------*/
    CHARGE_ONCHANGE_SIG, //�����



    SYS_MANUALVERIFYWARN_SIG, ///�˹�ȷ�ϱ�����Ϣ



    /*------- ��ʾ����Ϣ -----------*/


    UI_SHOWMSG_SIG, 	///�·���Ϣ
    UI_REFRESH_EVENTS_SIG, ///����¼��б�
    UI_REFRESH_MSGODMENU_SIG, ///���µ㲥�˵�
    UI_REFRESH_MSGODMSG_SIG, ///���µ㲥��Ϣ
    UI_REFRESH_PHONEBOOK_SIG, ///���µ绰��

    /*------- ������Ϣ -----------*/
    KEY_OK_SIG,		///��ȷ��"��Ϣ
    KEY_UP_SIG,		///���Ϸ�"��Ϣ
    KEY_DOWN_SIG,	///���·�"��Ϣ
    KEY_MENU_SIG,	///���˵�"��Ϣ

    /*------- TTS��Ϣ -----------*/
    TTS_READMSG_SIG,
    TTS_OK_SIG,


    /*------- ����������Ϣ -----------*/
    VECHICAL_CTRL_LOCK_SIG,		///����
    VECHICAL_CTRL_UNLOCK_SIG,	///����


    /*------- ��ʻ��¼����Ϣ -----------*/
    RECORDER_START_SIG,	///���ݲɼ�����


    /*------- ��ý������Ϣ -----------*/
    MULTIMEDIA_UPLOADPACK_ACK_SIG,	///��ý������Ӧ��
    MEDIADATA_RETRIEVE_SIG,///�洢��ý�����ݼ���
    MEDIADATA_UPLOAD_SIG,///�洢��ý�������ϴ�
    MEDIADATA_SINGAL_UPLOAD_SIG,///�����洢��ý�������ϴ�

    /*------- ������Ϣ -----------*/
    CAMERA_TAKEPHOTO_SIG, ///����

    /*------- ¼����Ϣ -----------*/
    AUDIO_RECORD_START_SIG,	///��ʼ¼��
    AUDIO_RECORD_STOP_SIG,  ///ֹͣ¼��

    /*------- ͸����Ϣ -----------*/
    TRANSPARENT_UPLOAD_SIG,///��������͸��

    /*------- ADC��Ϣ -----------*/
    ADC_ONCHANGE_SIG, 	///ADC

    MAX_SIG,               /* ����ź� */

};

/** ϵͳ�¼� =====================================================*/

///���������¼�
/* @(/2/0) .................................................................*/
typedef struct TaskEvtTag {
/* protected: */
    QEvt super;

/* public: */
    uint16_t cmd;
    uint16_t sequence;
    int8_t ret;
    uint16_t resCmd;
    uint16_t resSeqence;
    union {
    	struct {
    	    u32 address;
    	    uint16_t dataSize;
    	} mem;
     	u8 arr[10];
    } U;
} TaskEvt;

///������Ӧ���¼�
/* @(/2/1) .................................................................*/
typedef struct SAckEvtTag {
/* protected: */
    QEvt super;

/* public: */
    uint16_t u16RespSeqId;
    uint16_t u16RespMsgId;
    uint8_t ret;
} SAckEvt;

///��ʱλ�ø����¼�
typedef struct TrackEvtTag {
/* protected: */
    QEvt super;

/* public: */
    uint16_t u16Period; ///��λΪ�루s����0 ��ֹͣ���١�ֹͣ�������������ֶ�
    uint32_t u32Timetill; ///��λΪ�루s�����ն��ڽ��յ�λ�ø��ٿ�����Ϣ������Ч�ڽ�ֹʱ��֮ǰ��
    						///������Ϣ�е�ʱ��������λ�û㱨
} TrackEvt;

///�ֶ�ȷ�ϱ���
typedef struct ManualVerifyEvtTag {
/* protected: */
    QEvt super;

/* public: */
    uint16_t u16VerifyWarnSeq;	///ȷ�ϱ�����Ϣ��ˮ��
    tManualVerifyWarnType tWarnInfo; ///ȷ�ϱ�����Ϣ
} ManualVerifyEvt;

///�·��ı���Ϣ
typedef struct DisplayMsgEvtTag {
/* protected: */
    QEvt super;

/* public: */
    char msg[1024]; ///��Ϣ����
} DisplayMsgEvt;

///TTS�����ı���Ϣ
typedef struct TTSMsgEvtTag {
/* protected: */
    QEvt super;

/* public: */
    char msg[1024]; ///��Ϣ����
} TTSMsgEvt;

///TTS�����ı���Ϣ
typedef struct QuestionMsgEvtTag {
/* protected: */
    QEvt super;

/* public: */
    char msg[1024]; ///��Ϣ����
} QuestionMsgEvt;

///ͨ��
typedef struct DialEvtTag {
/* protected: */
    QEvt super;

/* public: */
    char phone[20]; ///����
} DialEvt;

///��ʻ��¼�����ݲɼ�
typedef struct RecorderStartEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u8 u8Cmd;	///������
    u16 u16CmdSeq;	///������ˮ�ţ�Ҳ���ϴ�����Ӧ����ˮ��
    u8	param[100];	///�������ݣ�������

} RecorderStartEvt;

///��ý�������ϴ�Ӧ��
typedef struct MultimediaDataAckEvtTag {
/* protected: */
    QEvt super;

/* public: */
	u32	u32Id;	///��ý��ID DWORD >0�����յ�ȫ�����ݰ���û�к����ֶ�
	u8	u8Amount;	///�ش������� BYTE n
	u16	IDList[256];	///�ش���ID �б� BYTE[2*n]	�ش������˳�����У��硰��ID1 ��ID2......��IDn��

} MultimediaDataAckEvt;

///����ͷ������������
typedef struct InstantPhotoEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u16 u16CmdSeq;	///������ˮ�ţ�Ҳ���ϴ�����Ӧ����ˮ��
    tMsg_SCMD_InstantPhoto tParam;
} InstantPhotoEvt;

///�洢��ý�����ݼ���
typedef struct MediaDataRetrieveEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u16 u16CmdSeq;	///������ˮ�ţ�Ҳ���ϴ�����Ӧ����ˮ��
    tMsg_SCMD_MediaDataRetrieve tParam;
} MediaDataRetrieveEvt;

//�洢��ý�������ϴ�����
typedef struct MediaDataUploadEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u16 u16CmdSeq;	///������ˮ�ţ�Ҳ���ϴ�����Ӧ����ˮ��
    tMsg_SCMD_MediaDataUpload tParam;
} MediaDataUploadEvt;

///¼����ʼ����
typedef struct AudioRecordEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u16 u16CmdSeq;	///������ˮ�ţ�Ҳ���ϴ�����Ӧ����ˮ��
    tMsg_SCMD_RecordAudio tParam;
} AudioRecordEvt;

///�����洢��ý�����ݼ����ϴ�����
typedef struct SingleMediaDataUploadEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u16 u16CmdSeq;	///������ˮ�ţ�Ҳ���ϴ�����Ӧ����ˮ��
    tMsg_SCMD_SingleStoredMediaUpload tParam;
} SingleMediaDataUploadEvt;

///��������͸��
typedef struct TransparentUploadEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u8 u8Type;	///��Ϣ����
    u8 data[100];///͸����Ϣ����
} TransparentUploadEvt;



/*��֡��Ϣ*/
typedef struct CSQEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u8 csq;  /*�ź�����*/
} CSQEvt;




#endif /* QEVENT_H_ */


