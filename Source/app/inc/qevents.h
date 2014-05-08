/** @file qevent.h
  * @brief brief description
  * @author <gao zhengdong, zhengdgao@163.com>
  * @date 2014年2月24日下午2:53:16
  * @version V1.0
  * @note
  * detailed description
  */
#ifndef EVENT_H_
#define EVENT_H_

#include <stm32f2xx.h>
#include <protocol.h>
#include <qp_port.h>

/** 系统信号 =====================================================*/
enum Signals {

/*------- 广播消息 -----------*/
    Q_TIMEOUT_SIG = Q_USER_SIG,

    ACC_ON_SIG,              /* ACC上电 */
    ACC_OFF_SIG,             /* ACC掉电 */
    SLEEP_REQ_SIG,    //休眠请求信号
    WAKEUP_REQ_SIG,   //唤醒请求信号

    SYS_POWEROFF,	///关机
    SYS_RESET,	///复位
    SYS_RECOVERFACTORYSETTING, ///终端恢复出厂设置


    MAX_PUB_SIG,             /* 7最后一个广播信号，即广播信号总数 */

    ACC_ONCHANGE_SIG,        /* 8ACC状态变化 */

    PER_SECOND_SIG,          //秒定时信号

/*------- CAN物理层消息 -----------*/
    CAN_NEWSENDMSG_SIG,    /* 9新到一个发送帧 */
    CAN_NEWRECVMSG_SIG,    /* 10新到一个接收帧 */
    CAN_WAKEUP_SIG, //11
    CAN_OK_SIG,
    CAN_LED_TIMEOUT_SIG,
    CAN_SLEEP_REQ_SIG,
    CAN_RETRIEVE_TIMEOUT_SIG,
    CAN_INT_NEWMSG_SIG,
    CAN_RXIDLE_TIMEOUT_SIG,
    CAN_CONFLICT_TIMEOUT_SIG,
    DATA_READY_SIG,                        /* 17帧就绪 */

/* AO_Modem */

    AT_OK_SIG,              /* 18AT应答 */
    AT_REGGED_SIG,  //19
    AT_PINREADY_SIG,    //20                                            /*19 PIN READY */
    AT_NETREGGED_SIG,     //21                                   /* 20信号类型：注册上网络 */
    AT_NETDISREG_SIG,     //22                                        /* 21信号类型：断网 */
    AT_CSQ_SIG,          //23                                      /* 22信号类型：信号强度 */
    AT_CONNECTED_SIG,   //24
    AT_DISCONNECTED_SIG,//25
    AT_DATAREADY_SIG,   //26                                         /* 23串口帧就绪 */
   AT_SENDREADY_SIG,     //27                                       /* 24消息：允许发送 */
    AT_SENDFILLDATA_SIG,  //28                              /* 25命令应答，允许填充发送数据 */
    AT_ERROR_SIG,          //29                                  /* 26信号类型：AT响应错误 */
    AT_CMSERROR_SIG,        //30                                /* 27信号类型：CMS Error */
    AT_CMEERROR_SIG,         //31                               /* 28信号类型：CME Error */
    AT_SENDERROR_SIG,    //32
    AT_FILLDATATIP_SIG,      //33                                   /* 29发送数据提示'>' */
    AT_SENDOK_SIG,        //34
    AT_DATAQUERY_TIMEOUT_SIG, //35                                 /* 30网络数据查询定时 */
    AT_DATAQUERY_SIG,  //36

/*------- GPRS消息 -----------*/
    GPRS_COMMINGMSG_SIG, //37
    GPRS_SERVER_CONNECTED_SIG, //38/* 连接上服务器 */
    GPRS_PING_TIMEOUT_SIG, //39
    GPRS_SENDREQ_SIG,     /* 40请求发送 */
    GPRS_LED_TIMEOUT_SIG, /* GPRS状态信号 */
    GPRS_RETRIEVE_TIMEOUT_SIG,
    NEW_TASKSENDREQ_SIG,
    GPRS_SLEEPREQ_TIMEOUT_SIG,
    GPRS_PHONECALL_HANDLED_SIG,
    GPRS_SMS_HANDLED_SIG,
    GPRS_COMMINGCALL_SIG,
    GPRS_COMMINGSMS_SIG,
    GPRS_RESET_SIG,
    SIMCARD_ONCHANGE_SIG,
    GPRS_SACK_SIG,    ///服务器应答
    SACK_TERMREG_OK_SIG,    ///终端注册成功应答
    SACK_TERMREG_ERR_SIG,    ///终端注册失败应答

    GPRS_SHUTDATACOMM, /**关闭数据通信*/
    GPRS_SHUTALLCOMM, /**关闭所有无线通信*/
    GPRS_CONNECTDEFAULTSERVER, ///切换回原缺省服务器

    GPRS_DIAL_NORMAL_SIG,	///正常通话
    GPRS_DIAL_MONITOR_SIG,	///监听



/*------- GPS消息 -----------*/
    GPS_RPT_TIMEOUT_SIG, //定时上传
    GPS_ANTENA_ONCHANGE_SIG, //GPS天线拔插状态变化
    GPS_TRACK_SIG, ///跟踪

/*------- 升级消息 -----------*/
    SYS_UPGRAGE_SIG,
    UPG_REQUEST_SIG, ///升级请求
    UPG_START_SIG,
    UPG_OK_SIG,
    UPG_ERR_SIG,


/*------- 外部信号量消息 -----------*/
    CHARGE_ONCHANGE_SIG, //充电检测



    SYS_MANUALVERIFYWARN_SIG, ///人工确认报警消息



    /*------- 显示屏消息 -----------*/


    UI_SHOWMSG_SIG, 	///下发消息
    UI_REFRESH_EVENTS_SIG, ///清空事件列表
    UI_REFRESH_MSGODMENU_SIG, ///更新点播菜单
    UI_REFRESH_MSGODMSG_SIG, ///更新点播信息
    UI_REFRESH_PHONEBOOK_SIG, ///更新电话本

    /*------- 按键消息 -----------*/
    KEY_OK_SIG,		///“确定"消息
    KEY_UP_SIG,		///“上翻"消息
    KEY_DOWN_SIG,	///“下翻"消息
    KEY_MENU_SIG,	///“菜单"消息

    /*------- TTS消息 -----------*/
    TTS_READMSG_SIG,
    TTS_OK_SIG,


    /*------- 车辆控制消息 -----------*/
    VECHICAL_CTRL_LOCK_SIG,		///锁车
    VECHICAL_CTRL_UNLOCK_SIG,	///解锁


    /*------- 行驶记录仪消息 -----------*/
    RECORDER_START_SIG,	///数据采集命令


    /*------- 多媒体类消息 -----------*/
    MULTIMEDIA_UPLOADPACK_ACK_SIG,	///多媒体数据应答
    MEDIADATA_RETRIEVE_SIG,///存储多媒体数据检索
    MEDIADATA_UPLOAD_SIG,///存储多媒体数据上传
    MEDIADATA_SINGAL_UPLOAD_SIG,///单条存储多媒体数据上传

    /*------- 拍照消息 -----------*/
    CAMERA_TAKEPHOTO_SIG, ///拍照

    /*------- 录音消息 -----------*/
    AUDIO_RECORD_START_SIG,	///开始录音
    AUDIO_RECORD_STOP_SIG,  ///停止录音

    /*------- 透传消息 -----------*/
    TRANSPARENT_UPLOAD_SIG,///数据下行透传

    /*------- ADC消息 -----------*/
    ADC_ONCHANGE_SIG, 	///ADC

    MAX_SIG,               /* 最后信号 */

};

/** 系统事件 =====================================================*/

///发送任务事件
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

///服务器应答事件
/* @(/2/1) .................................................................*/
typedef struct SAckEvtTag {
/* protected: */
    QEvt super;

/* public: */
    uint16_t u16RespSeqId;
    uint16_t u16RespMsgId;
    uint8_t ret;
} SAckEvt;

///临时位置跟踪事件
typedef struct TrackEvtTag {
/* protected: */
    QEvt super;

/* public: */
    uint16_t u16Period; ///单位为秒（s），0 则停止跟踪。停止跟踪无需带后继字段
    uint32_t u32Timetill; ///单位为秒（s），终端在接收到位置跟踪控制消息后，在有效期截止时间之前，
    						///依据消息中的时间间隔发送位置汇报
} TrackEvt;

///手动确认报警
typedef struct ManualVerifyEvtTag {
/* protected: */
    QEvt super;

/* public: */
    uint16_t u16VerifyWarnSeq;	///确认报警消息流水号
    tManualVerifyWarnType tWarnInfo; ///确认报警信息
} ManualVerifyEvt;

///下发文本消息
typedef struct DisplayMsgEvtTag {
/* protected: */
    QEvt super;

/* public: */
    char msg[1024]; ///消息内容
} DisplayMsgEvt;

///TTS播报文本消息
typedef struct TTSMsgEvtTag {
/* protected: */
    QEvt super;

/* public: */
    char msg[1024]; ///消息内容
} TTSMsgEvt;

///TTS播报文本消息
typedef struct QuestionMsgEvtTag {
/* protected: */
    QEvt super;

/* public: */
    char msg[1024]; ///消息内容
} QuestionMsgEvt;

///通话
typedef struct DialEvtTag {
/* protected: */
    QEvt super;

/* public: */
    char phone[20]; ///号码
} DialEvt;

///行驶记录仪数据采集
typedef struct RecorderStartEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u8 u8Cmd;	///命令字
    u16 u16CmdSeq;	///命令流水号，也是上传包的应答流水号
    u8	param[100];	///参数数据，待补充

} RecorderStartEvt;

///多媒体数据上传应答
typedef struct MultimediaDataAckEvtTag {
/* protected: */
    QEvt super;

/* public: */
	u32	u32Id;	///多媒体ID DWORD >0，如收到全部数据包则没有后续字段
	u8	u8Amount;	///重传包总数 BYTE n
	u16	IDList[256];	///重传包ID 列表 BYTE[2*n]	重传包序号顺序排列，如“包ID1 包ID2......包IDn”

} MultimediaDataAckEvt;

///摄像头立即拍摄命令
typedef struct InstantPhotoEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u16 u16CmdSeq;	///命令流水号，也是上传包的应答流水号
    tMsg_SCMD_InstantPhoto tParam;
} InstantPhotoEvt;

///存储多媒体数据检索
typedef struct MediaDataRetrieveEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u16 u16CmdSeq;	///命令流水号，也是上传包的应答流水号
    tMsg_SCMD_MediaDataRetrieve tParam;
} MediaDataRetrieveEvt;

//存储多媒体数据上传命令
typedef struct MediaDataUploadEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u16 u16CmdSeq;	///命令流水号，也是上传包的应答流水号
    tMsg_SCMD_MediaDataUpload tParam;
} MediaDataUploadEvt;

///录音开始命令
typedef struct AudioRecordEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u16 u16CmdSeq;	///命令流水号，也是上传包的应答流水号
    tMsg_SCMD_RecordAudio tParam;
} AudioRecordEvt;

///单条存储多媒体数据检索上传命令
typedef struct SingleMediaDataUploadEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u16 u16CmdSeq;	///命令流水号，也是上传包的应答流水号
    tMsg_SCMD_SingleStoredMediaUpload tParam;
} SingleMediaDataUploadEvt;

///数据下行透传
typedef struct TransparentUploadEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u8 u8Type;	///消息类型
    u8 data[100];///透传消息内容
} TransparentUploadEvt;



/*新帧消息*/
typedef struct CSQEvtTag {
/* protected: */
    QEvt super;

/* public: */
    u8 csq;  /*信号质量*/
} CSQEvt;




#endif /* QEVENT_H_ */


