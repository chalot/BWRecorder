/**   
* @Title: upgrade.h  
* @Description: TODO(��һ�仰�������ļ���ʲô) 
* @author Zhengd.Gao zhengdgao@163.com   
* @date 2013-6-25 ����11:00:45 
* @version V1.0   
*/ 


#ifndef UPGRADE_H_
#define UPGRADE_H_

#include "type.h"

#pragma pack(1)


//�������̲�����
#define		UPDATE_COMMAND_START					0x01	//�������������·�������������
#define		UPDATE_COMMAND_DATA						0x02	//���ݣ��������·���������
#define		UPDATE_COMMAND_REFUSE					0x10	//�ܾ����ն˾ܾ���������
#define		UPDATE_COMMAND_REQUEST					0x11	//�����ն����������������
#define		UPDATE_COMMAND_ERROR					0x12	//�ն�֪ͨ��������������ֹ
#define		UPDATE_COMMAND_SUCCESS					0x13	//�ն�֪ͨ���������������
#define		UPDATE_COMMAND_OTHER					0xFF	//����������

//����Ŀ������
#define		UPDATE_TARGET_PROGRAM					0x01	//�ն���������
#define		UPDATE_TARGET_MODE_CHINESE				0x02	//�ն��ֿ�-����
#define		UPDATE_TARGET_MODE_ASCII				0x03	//�ն��ֿ�-ASCII
#define		UPDATE_TARGET_MODE_SYMBOL				0x04	//�ն��ֿ�-����
#define		UPDATE_TARGET_VOICE						0x11	//�����ļ�
#define		UPDATE_TARGET_DISPATCHER				0x21	//�ն��ֿ�-�������̼�
#define		UPDATE_TARGET_LED_AD					0x31	//�ն��ֿ�-LED������̼�
#define		UPDATE_TARGET_DAB_RECIEVER				0x41	//�ն��ֿ�-DAB���ջ�
#define		UPDATE_TARGET_OTHER						0x41	//����

#define		UPDATE_TARGET_TYPE_AMOUNT				0x09	//����Ŀ����������

#define		UPD_BLOCK_SIZE		512				//�����ļ��ֿ��С
#define		UPD_PAGE_BLOCKS		(EF_PAGE_SIZE / UPD_BLOCK_SIZE)	//һҳ��ŷֿ����
#define		UPD_SECTOR_BLOCKS		(EF_BLOCK_SIZE / UPD_BLOCK_SIZE)	//һҳ��ŷֿ����



//������ɺ�����������3��32λ��������Ч��־���´ο�����ʱ����������־�Ƿ���Ч��
//�����Ч�Ļ������Ƭ��FLASH�а����������뵽Ƭ��FLASH�̶���λ�ã�Ȼ����λϵͳ
//����������ϵͳ�����еľ���������ĳ��������
#define		_EFLASH_UPPROG_TAG1_		0x11223344
#define		_EFLASH_UPPROG_TAG2_		0xFEFDFCFB
#define		_EFLASH_UPPROG_TAG3_		0x55AAAA55

//Э����Ϣ�ṹ
typedef struct
{
	u8 		u8FactoryCode;						  		//���̴���
	u16		u16ProtocolVersion;							//Э��汾
	u16		u16ContentLength;							//�����ֽڳ���
	u16		fCommand				:	12;				//������
	u16		fForceSend				:	1;				//�Ƿ�ǿ�Ʒ��ͣ�1=�ǣ�0=��
	u16		fFeedback				:	1;				//�Ƿ���Ҫ������1=�ǣ�0=��
	u16		fResponse				:	1;				//�Ƿ���ҪӦ��1=�ǣ�0=��
	u16		fLast					:	1;				//�Ƿ�β֡��1=�ǣ�0=��
	u8		u8PackId;									//��ID��0x00-0xFE
	u32		u32DeviceID;								//�ն�ID��
} _GPRSPROTOCOL_INFOType;

//Զ�������������������Ϣ
typedef		struct
{
	u8		u8Target;									//����Ŀ������
	u8		u8Command;									//�������̲����룬=0x01
	u16		u16Version;									//�����ļ��汾��
	u32		u32FileSize;								//�����ļ����ֽڳ���
	u32		u32BlockAmount;								//�����ļ��ֿܷ���

}	_tUPDATE_HEADINFO;

//�ļ�����Ϣ
typedef		struct
{
	_tUPDATE_HEADINFO	tHeadInfo;

	u32		u32CurBlockId;
	u16		u16CurBlockLen;

}	_tBLOCKINFO;

//�ն˷�������������Ϣ
typedef		struct
{
	u8		u8Target;									//����Ŀ������
	u8		u8Command;									//�������̲����룬0x11/0x10/0x12/0x13
	u8		u8ResPackId;								//����Ӧ�����а�ID
	u32		u32Block;									//�ֿ����

}	_tUPDATE_REQUESTINFO;



//������Ϣ
typedef		struct
{
	u8	u8UpdateTarget;
	u16	u16Version;
	u32	u32FileSize;
	u32	u32BlockAmount;
	u32	u32AvailBlock;
	u8	*pBlockContent;

}	_tUPDATEINFO;


//Ƭ��FLASH��д˫����
typedef	struct
{
	u16		u16SrcBlock;
	u16		u16BakBlock;
	u8		u8SrcPage;
	u8		u8BakPage;
	u8		bSrcError;
	u8		bBakError;

}	_tUPGRADType;


//�����������ݽṹ�嶨��
typedef struct
{
	u32 	u32Tag1;					//��־1
	u32 	u32Tag2;					//��־2
	u32 	u32Tag3;					//��־3
	u16		u16Version;					//����汾��
	u32		u32PackLen;					//���������С
	u8		u8Block_CRC[512];			//ҳУ����
	u8		u8CRC;						//���������ֽ�CRC

}_tEFLASH_UPG_INFO;


#define UPGRADE_FILE_BLOCK_SIZE		512		//ÿ�������ļ��Ŀ��С

//�����ļ�ͷ
typedef struct Upgrade_File_Formate_Header_Tag
{
	//�ļ��ֽڴ�С
	u32	upgrade_file_bytes_count;
	//�汾��Ϣ�ֽڴ�С
	u16	upgrade_file_version_bytes_count;
	//����Ŀ��
	u8	upgrade_file_target;
	//�ֿ����
	u32	upgrade_file_blocks_count;
	//�ֿ��ֽ�����Ĭ��256��β��հ״����0xFF
	u32	upgrade_file_block_bytes_count;
	//�ļ�ͷУ��CRC-8
	u8	upgrade_file_crc;
}UPGRADE_FILE_FORMATE_HEADER,*PUPGRADE_FILE_FORMATE_HEADER;

//�汾��Ϣ
typedef struct Upgrade_File_Version_Formate_Tag
{
	//�汾��Ϣ
	u16	upgrade_file_version;
	//�������ڣ���
	u16	upgrade_file_release_year;
	//�������ڣ���
	u8	upgrade_file_release_month;
	//�������ڣ���
	u8	upgrade_file_release_day;
	//���� 0x00���ģ�0x01Ӣ�ģ����ౣ��
	u8	upgrade_file_language;
	//�ļ���ϢУ��CRC-8
	u8	upgrade_file_crc;
}UPGRADE_FILE_VERSION_FORMATE,*PUPGRADE_FILE_VERSION_FORMATE;

//����Ϣ
typedef struct Upgrade_File_Block_Formate_Tag
{
	//�ֿ����
	u32	upgrade_file_block_sequence;
	//�ֿ���������,û��������0XFF
	u8	upgrade_file_block[UPGRADE_FILE_BLOCK_SIZE];
	//�ֿ�У��CRC-8
	u8	upgrade_file_crc;
}UPGRADE_FILE_BLOCK_FORMATE,*PUPGRADE_FILE_BLOCK_FORMATE;

//�ֿ�洢��Ϣ
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

#define	_CRC_STORE_OFFSET_				4						//CRCУ�����У���������ƫ��λ��
#define	_PACK_DATA_OFFSET_				4						//���ݰ������ݵ�ƫ��


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
