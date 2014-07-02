#ifndef __BRAINWARE_ELECTRONICS_NANDFLASH_H__
#define __BRAINWARE_ELECTRONICS_NANDFLASH_H__

#include "stm32f2xx.h"
#include	"nandflash.h"

#if 0
// ----------------------------------------------------------------------------
// �����
#define			NF_READ1				0x00
#define			NF_READ2				0x30
#define			NF_READCOPY1			0x00
#define			NF_READCOPY2			0x35
#define			NF_READID				0x90
#define			NF_RESET				0xFF
#define			NF_PAGEWRITE1			0x80
#define			NF_PAGEWRITE2			0x10
#define			NF_CACHEWRITE1			0x80
#define			NF_CAHCEWRITE2			0x15
#define			NF_COPYWRITE1			0x85
#define			NF_COPYWRITE2			0x10
#define			NF_BLOCKERASE1			0x60
#define			NF_BLOCKERASE2			0xD0
#define			NF_RANDOMINPUT1			0x85
#define			NF_RANDOMOUTPUT1		0x05
#define			NF_RANDOMOUTPUT2		0xE0
#define			NF_STATUS				0x70

#define			NF_CMD				*(__IO uint8_t *)0x70010000			// ����״̬��CLE(A16)=1 ALE(A17)=0��P2.0ΪR/B
#define			NF_ADDR				*(__IO uint8_t *)0x70020000			// ��ַ״̬��CLE(A16)=0 ALE(A17)=1
#define			NF_DATA				*(__IO uint8_t *)0x70000000			// ����״̬��CLE(A16)=0 ALE(A17)=0

#define			NF_FLAG					0xEC							// ���̱���
#define			NF_ID1					0xF1							// �豸����
#define			NF_ID2					0x95							// ҳ/����Ϣ��D[1:0]=0 ҳ1KB��=1 ҳ2KB����������
// 			  D[5:4]=0 ��64KB��=1 ��128KB��=2 ��256KB����������
//			  D[2]=0 ÿҳ��8B��=1 ÿҳ��16B
//			  D[6]=0 8λ��=1 16λ
//			  D[3][7] ���з�����Сʱ��

//״̬�ֽ�λ��־����
#define			_NF_STATUS_ERASE_FAILURE_		0x01					//����ʧ��
#define			_NF_STATUS_BUFWR_FAILURE_		0x02					//������дʧ��
#define			_NF_STATUS_NO_WP_				0x80					//û��д����
#define			_NF_STATUS_READY_				0x40					//��æ
#define			_NF_STATUS_WRITE_PERMIT_		0xC0					//����д
#define			_NF_STATUS_MASK_				0x7F

// ��δʹ�ñ�־
#define			NF_UNUSED						0xFF

//��ַ��Χ����
#define			_NF_MAX_ADDR_					(u32)(128 << _NF_BLOCK_OFFSET_)

//���ݿ��С����
#define			_NF_BLOCK_SIZE_					(u32)(128 << 10)
//ÿ��д�������ҳ��С
#define			_NF_PAGE_SIZE_					2048	//2048B(Data) + 1B(CRC)
//FLASH�ܵ����ݿ鶨��
#define			_NF_MAX_BLOCK_NUM_					1024
#define			_NF_MAX_PAGE_NUM_					64	//ÿ�����ڵ�ҳ��ĸ��64ҳ

#define			_NF_BLOCK_OFFSET_					22
#define			_NF_PAGE_OFFSET_  					16
//
#define			_NF_PAGE_ALREADY_WRITE_TAG1_		0x12345678
#define			_NF_PAGE_ALREADY_WRITE_TAG2_		0x55AAAA55

#define			_NF_MAX_WRITE_SIZE_					0x800			//ÿ�����д��Ϊ1ҳ2K�ֽ�
#define			_NFLASH_WRITE_SIZE_					_NF_PAGE_SIZE_	//2048 (256)
//��������ռ�ݵ���BLOCK1��BLOCK2
#define			_NF_UPGRADE_BASEADDR_				0		      //����������ʼλ��
#define			_NF_UPGRADE_BLOCKNUM				2			//��������ռ������BLOCK����256K�ֽ�

/**********************************************��������**********************************************/
//����������BLOCK0~BLOCK3
#define		_NFLASH_UPGRADE_SRC_BLOCK_0				0				//����������ʼ���
#define		_NFLASH_UPGRADE_SRC_BLOCK_1				1				//����������ʼ���
#define		_NFLASH_UPGRADE_BAK_BLOCK_2				2				//����������ʼ���ݿ��
#define		_NFLASH_UPGRADE_BAK_BLOCK_3				3				//����������ʼ���ݿ��
#define		 NFLASH_UPG_FLAG_PAGE					62				//������־���λ��

//���ò������BLOCK4~BLOCK5
#define		_EFLASH_BLOCK_PARAM_					4				//����ģ����
#define		_EFLASH_BLOCK_PARAM_BAK_				5				//����ģ�屸�ݿ��

//����դ�����BLOCK6~BLOCK7
#define		_EFLASH_BLOCK_BARRIER_					6				//����դ�����
#define		_EFLASH_BLOCK_BARRIER_BAK_				7				//����դ�����ݿ��

//����ģ����BLOCK8~BLOCK9
#define		_EFLASH_BLOCK_MSGTMPL_					8				//����ģ����
#define		_EFLASH_BLOCK_MSGTMPL_BAK_				9				//����ģ�屸�ݿ��

//360Сʱ���ٶ���Ϣ����Ŀ��BLOCK10~BLOCK12
#define			NF_SPEEDOF360HRS_START_BLOCK		10
#define			NF_SPEEDOF360HRS_END_BLOCK			12

//GSM����ʱ��λ��Ϣ����Ŀ��BLOCK13~BLOCK15
#define			NF_GPSINFO_START_BLOCK				13
#define			NF_GPSINFO_END_BLOCK				15

//ͼƬ
#define		_JPEG_EFLASH_BASEBLOCK_					20				//JPEGͼƬ�Ļ���
#define		_JPEG_EFLASH_LASTBLOCK_					(_NF_MAX_BLOCK_NUM_ - 2)//JPEGͼƬ��β�飬
#define		_JPEG_EFLASH_HEADBLOCK_					(_NF_MAX_BLOCK_NUM_ - 1)//JPEGͼƬͷ��Ϣ��ʼλ�ã�β��

#define			_NF_PAGE_PER_BLOCK_					64
#define			_NF_TAG_OFFSET_						0
#define			_NF_TAG_SIZE_						8
#define			_NF_PARAM_OFFSET_					1	  			//���ò���FLASH��ƫ�� (1ҳ��
#define			_NF_DIANZIZHALAN_OFFSET_			0x1000			//����դ��FLASH��ƫ��
#define			_NF_BLOCK_PARTITION_NUM_			32				//128K�Ŀ�ռ䣬�ֳ�8��16K�Ĳ���������
#define			_NF_BLOCK_PARTITION_PAGES_			(_NF_PAGE_PER_BLOCK_ / _NF_BLOCK_PARTITION_NUM_)			//16K��С��ÿ����������

#define			_NF_PARAM_LEN_						0x1000	   		//4K���ò�����С
#define			_NF_BARRIER_LEN_					0x1000	  		//4K����դ������

#define			_NF_CONFIG_START_BLOCK_				4				//���ò�����ʼ��ռ�
#define			_NF_CONFIG_BACKUP_BLOCK_			5				//���ò������ݲ������
#define			_NF_CONFIG_BLOCK_NUM_				2				//���ò����ռ���

//�̻����Ŵ洢�����ݿռ���BLOCK5
#define			_NF_SOLIDMSG_START_BLOCK_			5				//�̻����Ŵ洢�Ŀ���ʼ��ַ
#define			_NF_SOLIDMSG_NUM_BLOCK_				1				//�̻����Ŵ洢ռ�ݵĿ���Ŀ

/******************************************END of��������********************************************/

/**ä��������λ���ݴ洢�� 10��000 * 100B ~ 1MB */
#define NFLASH_BLINDPOS_START_BLOCK		2  		///ä��������λ��Ϣ��ʼ���
#define NFLASH_BLINDPOS_END_BLOCK		10  	///ä��������λ��Ϣ�������



//����ϵͳ��������������
#define		_EFLASH_ERASE_TIMEMAX_		3

#pragma pack(1)

//Ƭ��FLASH��д˫����
typedef struct {
	u16 u16SrcBlock;
	u16 u16BakBlock;
	u8 u8SrcPage;
	u8 u8BakPage;

} _tEFLASH_DBAK;
typedef struct {
//������йص���Ϣ
	u8 u8CurPartitionNum;			//��ǰ�洢�Ĳ���λ�ú�(��0�ŵ�7�ţ�����128K��Block�ֳ�8�ȷ�)

//���λ��Ϣ�йر�������
	u32 u32PointsArrCnt;			//��λ��Ŀ
	u32 u32PointsBaseAddr;			//��λ���ݻ���ַ
	u8 u8PointsSize;				//ÿ����λ���ݴ�С

//�����������йر�������
	u32 u32polygonCnt;				//������������
	u32 u32polygonBaseAddr;			//������������ַ
	u8 u8polygonSize;				//������������ݴ�С

//�����ģ���й���Ϣ����
	u32 u32ShtMsgTemplateCnt;		//����ģ����Ŀ
	u32 u32ShtMsgTemplateBaseAddr;	//����ģ��洢����ַ
	u8 u8ShtMsgSize;				//ÿ������ģ���С

//��ͼƬ�й���Ϣ����
	u32 u32JpegCnt;					//JPEGͼƬ��Ŀ
	u32 u32JpegBaseAddr;			//JPEGͼƬ����ַ
	u16 u16CurBlock;				//��ǰʹ�õĿ�

//��NANDFLASH�йض���
	u32 u32ID;
	u32 u32BlockSize;				//���С
	u32 u32PageSize;				//ҳ��С
} tNANDFLASHFSMType;

typedef struct {
	u16 u16Block;
	u16 u16Page;
} tNANDFLASHPOSTypeDef;

//����������󷵻ش���
typedef enum {
	_NO_ERR_ = 0,			//û�д�����ȷ����
	_INVALID_PARAM_ = -1,			//��������
	_INVALID_ADDR_ = -2,			//��ַԽ��
	_READ_ERR_ = -3,			//������
	_WRITE_ERR_ = -4,			//д����
	_ERASE_ERR_ = -5,			//��������
	_ERASE_TIMEOUT_ = -6,			//������ʱ
	_READ_STATUS_ERR_ = -7,			//��״̬����
	_OP_NOT_FINISHED_ = -8,			//����δ���
	_NOSPACE_ERR_ = -9
} _eERRType;

// �ֲ���������
void NFLASH_LowLevel_DeInit(void);
void NFLASH_LowLevel_Init(void);
void NAND_FSMCConfig(void);

u8 NandFlashReadId(u8 *pBuf);
u8 NandFlashStatus(void);
_eERRType NF_RawPageRead(u32 u32Addr, u16 u16Length, u8 *pu8Buf);
_eERRType NF_RawPageWrite(u32 u32Addr, u16 u16Length, u8 *pu8Buf);
_eERRType NF_BlockErase(u16 u16Block);
_eERRType EFLASH_EraseProgram(void);
_eERRType NF_CopyBack(u32 u32SrcAddr, u32 u32DestAddr);

_eERRType NFLASH_EraseChip(void);
_eERRType NFLASH_Program(u16 u16Block, u8 u8Page, u8* pDataArr, u16 u16Size);
u8 NF_FindEmtpyParamBlock(void);
u8 NF_FindEmtpyParamBlockBackup(void);
_eERRType NFLASH_WriteParamTag(u16 u16Block, u8 u8Page, u16 u16Offset);

u32 NF_GetAddrByBlockPageAndOffset(u16 u16Block, u8 u8Page, u16 u16Offset);
//�ⲿ�����ӿ�
//_eERRType		NFLASH_WriteParam(u32 u32FlashBaseAddress, u8* pDataArr, u16 u16Size);
//_eERRType		NFLASH_WriteMsg(_eMSG_TYPE msgtype,u32 u32FlashBaseAddr, u8* pDataArr, u16 u16Size);
_eERRType NFLASH_BlockCopy(u32 u32SourceAddress, u32 u32DestinyAddress,	u16 u16Size);
_eERRType NFLASH_BlockCopyChange(u16 u16SourceBlock, u16 u16DestBlock,
		u8 u8SourceStartPage, u8 u8DestStartPage, u8 u8PageNum, u16 u16Offset,
		u8* pDataArr, u16 u16DataSize);
_eERRType NFLASH_ReadArr(u32 u32Addr, u8* pDataArr, u32 u32Size);

//BOOL			NFLASH_WriteBarrier(u8* pData, u16 u16Size);
//BOOL			NFLASH_ReadBarrier(void);
//BOOL			NFLASH_WriteMsgTemplate(u8* pData, u16 u16Size);
//BOOL			NFLASH_ReadMsgTemplate(void);

//��дϵͳ�����ӿں���
_eERRType NF_PageRead(u16 u16Block, u8 u8Page, u16 u16Offset, u16 u16Size, u8* pData);
_eERRType NF_PageWrite(u16 u16Block, u8 u8Page, u16 u16Offset, u16 u16Size,	u8* pData);

//void		NFLASH_WriteAfterUpgFinish(u32 u32FrameNum);

#endif

#include <fsmc_nand.h>

#define NF_BLOCK_AMOUNT	1024
#define NF_PAGE_AMOUNT	64
#define NF_BLOCK_SIZE	64 * 2048
#define NF_PAGE_SIZE	2048
#define NF_BLOCK_INVALID	0xFFFF


//����������󷵻ش���
typedef enum {
	_NO_ERR_ = 0,			//û�д�����ȷ����
	_INVALID_PARAM_ = -1,			//��������
	_INVALID_ADDR_ = -2,			//��ַԽ��
	_READ_ERR_ = -3,			//������
	_WRITE_ERR_ = -4,			//д����
	_ERASE_ERR_ = -5,			//��������
	_ERASE_TIMEOUT_ = -6,			//������ʱ
	_READ_STATUS_ERR_ = -7,			//��״̬����
	_OP_NOT_FINISHED_ = -8,			//����δ���
	_NOSPACE_ERR_ = -9,
	_BLOCK_ERR_,
} _eERRType;


void NFLASH_LowLevel_DeInit(void);
void NFLASH_LowLevel_Init(void);

_eERRType NF_PageRead(u16 u16Block, u8 u8Page, u16 u16Size, u8* pData);
_eERRType NF_PageWrite(u16 u16Block, u8 u8Page, u16 u16Size, u8* pData);
_eERRType NF_EraseBlock(u16 u16Block);
_eERRType NF_WriteSpareArea(u16 u16Block, u8 u8Page, u8 u8Size, u8 *pData);

/**
 * �������
 */
void NF_ScanErrBlock();
BOOL NF_IsErrBlock(u16 u16BlockId);
void NF_MarkErrBlock(u16 u16BlockId);



#endif //__BRAINWARE_ELECTRONICS_NANDFLASH_H__
