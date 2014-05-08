/**   
* @Title: upgrade.h  
* @Description: TODO(用一句话描述该文件做什么) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-25 上午11:00:45 
* @version V1.0   
*/ 


#ifndef UPGRADE_H_
#define UPGRADE_H_

#include "type.h"

#pragma pack(1)


//升级过程操作码
#define		UPDATE_COMMAND_START					0x01	//启动，服务器下发升级启动命令
#define		UPDATE_COMMAND_DATA						0x02	//数据，服务器下发升级数据
#define		UPDATE_COMMAND_REFUSE					0x10	//拒绝，终端拒绝升级命令
#define		UPDATE_COMMAND_REQUEST					0x11	//请求，终端向服务器请求数据
#define		UPDATE_COMMAND_ERROR					0x12	//终端通知服务器，升级中止
#define		UPDATE_COMMAND_SUCCESS					0x13	//终端通知服务器，升级完成
#define		UPDATE_COMMAND_OTHER					0xFF	//其他，保留

//升级目标类型
#define		UPDATE_TARGET_PROGRAM					0x01	//终端主机程序
#define		UPDATE_TARGET_MODE_CHINESE				0x02	//终端字库-汉字
#define		UPDATE_TARGET_MODE_ASCII				0x03	//终端字库-ASCII
#define		UPDATE_TARGET_MODE_SYMBOL				0x04	//终端字库-符号
#define		UPDATE_TARGET_VOICE						0x11	//语音文件
#define		UPDATE_TARGET_DISPATCHER				0x21	//终端字库-调度屏固件
#define		UPDATE_TARGET_LED_AD					0x31	//终端字库-LED广告屏固件
#define		UPDATE_TARGET_DAB_RECIEVER				0x41	//终端字库-DAB接收机
#define		UPDATE_TARGET_OTHER						0x41	//其他

#define		UPDATE_TARGET_TYPE_AMOUNT				0x09	//升级目标类型总数

#define		UPD_BLOCK_SIZE		512				//升级文件分块大小
#define		UPD_PAGE_BLOCKS		(EF_PAGE_SIZE / UPD_BLOCK_SIZE)	//一页存放分块个数
#define		UPD_SECTOR_BLOCKS		(EF_BLOCK_SIZE / UPD_BLOCK_SIZE)	//一页存放分块个数



//升级完成后设置连续的3个32位的升级有效标志，下次开机的时候检测升级标志是否有效，
//如果有效的话，则从片外FLASH中把升级程序导入到片内FLASH固定的位置，然后软复位系统
//重新启动后，系统中运行的就是升级后的程序代码了
#define		_EFLASH_UPPROG_TAG1_		0x11223344
#define		_EFLASH_UPPROG_TAG2_		0xFEFDFCFB
#define		_EFLASH_UPPROG_TAG3_		0x55AAAA55

//协议信息结构
typedef struct
{
	u8 		u8FactoryCode;						  		//厂商代码
	u16		u16ProtocolVersion;							//协议版本
	u16		u16ContentLength;							//内容字节长度
	u16		fCommand				:	12;				//命令字
	u16		fForceSend				:	1;				//是否强制发送：1=是，0=否
	u16		fFeedback				:	1;				//是否需要反馈：1=是，0=否
	u16		fResponse				:	1;				//是否需要应答：1=是，0=否
	u16		fLast					:	1;				//是否尾帧：1=是，0=否
	u8		u8PackId;									//包ID，0x00-0xFE
	u32		u32DeviceID;								//终端ID号
} _GPRSPROTOCOL_INFOType;

//远程软件升级启动命令信息
typedef		struct
{
	u8		u8Target;									//升级目标类型
	u8		u8Command;									//升级过程操作码，=0x01
	u16		u16Version;									//升级文件版本号
	u32		u32FileSize;								//升级文件总字节长度
	u32		u32BlockAmount;								//升级文件总分块数

}	_tUPDATE_HEADINFO;

//文件块信息
typedef		struct
{
	_tUPDATE_HEADINFO	tHeadInfo;

	u32		u32CurBlockId;
	u16		u16CurBlockLen;

}	_tBLOCKINFO;

//终端反馈请求数据信息
typedef		struct
{
	u8		u8Target;									//升级目标类型
	u8		u8Command;									//升级过程操作码，0x11/0x10/0x12/0x13
	u8		u8ResPackId;								//被响应的下行包ID
	u32		u32Block;									//分块序号

}	_tUPDATE_REQUESTINFO;



//升级信息
typedef		struct
{
	u8	u8UpdateTarget;
	u16	u16Version;
	u32	u32FileSize;
	u32	u32BlockAmount;
	u32	u32AvailBlock;
	u8	*pBlockContent;

}	_tUPDATEINFO;


//片外FLASH读写双备份
typedef	struct
{
	u16		u16SrcBlock;
	u16		u16BakBlock;
	u8		u8SrcPage;
	u8		u8BakPage;
	u8		bSrcError;
	u8		bBakError;

}	_tUPGRADType;


//升级程序数据结构体定义
typedef struct
{
	u32 	u32Tag1;					//标志1
	u32 	u32Tag2;					//标志2
	u32 	u32Tag3;					//标志3
	u16		u16Version;					//程序版本号
	u32		u32PackLen;					//升级程序大小
	u8		u8Block_CRC[512];			//页校验码
	u8		u8CRC;						//以上所有字节CRC

}_tEFLASH_UPG_INFO;


#define UPGRADE_FILE_BLOCK_SIZE		512		//每个升级文件的块大小

//升级文件头
typedef struct Upgrade_File_Formate_Header_Tag
{
	//文件字节大小
	u32	upgrade_file_bytes_count;
	//版本信息字节大小
	u16	upgrade_file_version_bytes_count;
	//升级目标
	u8	upgrade_file_target;
	//分块个数
	u32	upgrade_file_blocks_count;
	//分块字节数，默认256，尾块空白处填充0xFF
	u32	upgrade_file_block_bytes_count;
	//文件头校验CRC-8
	u8	upgrade_file_crc;
}UPGRADE_FILE_FORMATE_HEADER,*PUPGRADE_FILE_FORMATE_HEADER;

//版本信息
typedef struct Upgrade_File_Version_Formate_Tag
{
	//版本信息
	u16	upgrade_file_version;
	//发布日期，年
	u16	upgrade_file_release_year;
	//发布日期，月
	u8	upgrade_file_release_month;
	//发布日期，日
	u8	upgrade_file_release_day;
	//语言 0x00中文，0x01英文，其余保留
	u8	upgrade_file_language;
	//文件信息校验CRC-8
	u8	upgrade_file_crc;
}UPGRADE_FILE_VERSION_FORMATE,*PUPGRADE_FILE_VERSION_FORMATE;

//块信息
typedef struct Upgrade_File_Block_Formate_Tag
{
	//分块序号
	u32	upgrade_file_block_sequence;
	//分块数据内容,没有数据添0XFF
	u8	upgrade_file_block[UPGRADE_FILE_BLOCK_SIZE];
	//分块校验CRC-8
	u8	upgrade_file_crc;
}UPGRADE_FILE_BLOCK_FORMATE,*PUPGRADE_FILE_BLOCK_FORMATE;

//分块存储信息
typedef struct BLOCK_INFO
{
	u32	blockId;
	u8 block[UPGRADE_FILE_BLOCK_SIZE];
	u8 checksum;
} BLOCKINFO;

extern		UPGRADE_FILE_FORMATE_HEADER		tUsbUpgrdeFileHeadInfo;

#define	_UPG_HEAD_INFO_LEN_				(sizeof(UPGRADE_FILE_FORMATE_HEADER))
#define	_UPG_VERSION_INFO_LEN_			(sizeof(UPGRADE_FILE_VERSION_FORMATE))
#define _UPG_HEAD_VERSION_INFO_LEN_		(sizeof(UPGRADE_FILE_FORMATE_HEADER) + sizeof(UPGRADE_FILE_VERSION_FORMATE))

#define	_UPG_PACKLEN_					(sizeof(UPGRADE_FILE_BLOCK_FORMATE))
#define	_UPG_ASS_LEN_					(sizeof(UPGRADE_FILE_BLOCK_FORMATE) - UPGRADE_FILE_BLOCK_SIZE)

#define	_CRC_STORE_OFFSET_				4						//CRC校验和在校验和数组中偏移位置
#define	_PACK_DATA_OFFSET_				4						//数据包中数据的偏移


BOOL UPG_SD_Detect();
BOOL UPG_SD_Init();
void UPG_ReportSDCardState(u8 err);
BOOL Upg_CheckUpgradeFileExisted();
BOOL UPG_CheckUpgFileValidation();
BOOL UPG_TransferBlocks2Flash(u16 curBlockId);
u32 UPG_GetBlocksAmount();
void UPD_MarkFileReady();
BOOL UPD_SaveUpdBlock(u16 blockId, u8 *pBlock, u16 blockLen, u8 checksum);

#pragma pack()
#endif /* UPGRADE_H_ */
