#ifndef __BRAINWARE_ELECTRONICS_NANDFLASH_H__
#define __BRAINWARE_ELECTRONICS_NANDFLASH_H__

#include "stm32f2xx.h"
#include	"nandflash.h"

#if 0
// ----------------------------------------------------------------------------
// 命令定义
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

#define			NF_CMD				*(__IO uint8_t *)0x70010000			// 命令状态，CLE(A16)=1 ALE(A17)=0，P2.0为R/B
#define			NF_ADDR				*(__IO uint8_t *)0x70020000			// 地址状态，CLE(A16)=0 ALE(A17)=1
#define			NF_DATA				*(__IO uint8_t *)0x70000000			// 数据状态，CLE(A16)=0 ALE(A17)=0

#define			NF_FLAG					0xEC							// 厂商编码
#define			NF_ID1					0xF1							// 设备编码
#define			NF_ID2					0x95							// 页/块信息，D[1:0]=0 页1KB、=1 页2KB、其他保留
// 			  D[5:4]=0 块64KB、=1 块128KB、=2 块256KB、其他保留
//			  D[2]=0 每页多8B、=1 每页多16B
//			  D[6]=0 8位、=1 16位
//			  D[3][7] 串行访问最小时间

//状态字节位标志定义
#define			_NF_STATUS_ERASE_FAILURE_		0x01					//擦除失败
#define			_NF_STATUS_BUFWR_FAILURE_		0x02					//缓冲区写失败
#define			_NF_STATUS_NO_WP_				0x80					//没有写保护
#define			_NF_STATUS_READY_				0x40					//不忙
#define			_NF_STATUS_WRITE_PERMIT_		0xC0					//允许写
#define			_NF_STATUS_MASK_				0x7F

// 块未使用标志
#define			NF_UNUSED						0xFF

//地址范围定义
#define			_NF_MAX_ADDR_					(u32)(128 << _NF_BLOCK_OFFSET_)

//数据块大小定义
#define			_NF_BLOCK_SIZE_					(u32)(128 << 10)
//每次写入的数据页大小
#define			_NF_PAGE_SIZE_					2048	//2048B(Data) + 1B(CRC)
//FLASH总的数据块定义
#define			_NF_MAX_BLOCK_NUM_					1024
#define			_NF_MAX_PAGE_NUM_					64	//每个块内的页数母，64页

#define			_NF_BLOCK_OFFSET_					22
#define			_NF_PAGE_OFFSET_  					16
//
#define			_NF_PAGE_ALREADY_WRITE_TAG1_		0x12345678
#define			_NF_PAGE_ALREADY_WRITE_TAG2_		0x55AAAA55

#define			_NF_MAX_WRITE_SIZE_					0x800			//每次最大写入为1页2K字节
#define			_NFLASH_WRITE_SIZE_					_NF_PAGE_SIZE_	//2048 (256)
//升级程序占据的是BLOCK1和BLOCK2
#define			_NF_UPGRADE_BASEADDR_				0		      //升级程序起始位置
#define			_NF_UPGRADE_BLOCKNUM				2			//升级程序占据两个BLOCK，共256K字节

/**********************************************升级程序**********************************************/
//升级程序块号BLOCK0~BLOCK3
#define		_NFLASH_UPGRADE_SRC_BLOCK_0				0				//升级程序起始块号
#define		_NFLASH_UPGRADE_SRC_BLOCK_1				1				//升级程序起始块号
#define		_NFLASH_UPGRADE_BAK_BLOCK_2				2				//升级程序起始备份块号
#define		_NFLASH_UPGRADE_BAK_BLOCK_3				3				//升级程序起始备份块号
#define		 NFLASH_UPG_FLAG_PAGE					62				//升级标志存放位置

//配置参数块号BLOCK4~BLOCK5
#define		_EFLASH_BLOCK_PARAM_					4				//短信模板块号
#define		_EFLASH_BLOCK_PARAM_BAK_				5				//短信模板备份块号

//电子栅栏块号BLOCK6~BLOCK7
#define		_EFLASH_BLOCK_BARRIER_					6				//电子栅栏块号
#define		_EFLASH_BLOCK_BARRIER_BAK_				7				//电子栅栏备份块号

//短信模板块号BLOCK8~BLOCK9
#define		_EFLASH_BLOCK_MSGTMPL_					8				//短信模板块号
#define		_EFLASH_BLOCK_MSGTMPL_BAK_				9				//短信模板备份块号

//360小时内速度信息保存的块号BLOCK10~BLOCK12
#define			NF_SPEEDOF360HRS_START_BLOCK		10
#define			NF_SPEEDOF360HRS_END_BLOCK			12

//GSM离线时点位信息保存的块号BLOCK13~BLOCK15
#define			NF_GPSINFO_START_BLOCK				13
#define			NF_GPSINFO_END_BLOCK				15

//图片
#define		_JPEG_EFLASH_BASEBLOCK_					20				//JPEG图片的基块
#define		_JPEG_EFLASH_LASTBLOCK_					(_NF_MAX_BLOCK_NUM_ - 2)//JPEG图片的尾块，
#define		_JPEG_EFLASH_HEADBLOCK_					(_NF_MAX_BLOCK_NUM_ - 1)//JPEG图片头信息起始位置，尾块

#define			_NF_PAGE_PER_BLOCK_					64
#define			_NF_TAG_OFFSET_						0
#define			_NF_TAG_SIZE_						8
#define			_NF_PARAM_OFFSET_					1	  			//配置参数FLASH内偏移 (1页）
#define			_NF_DIANZIZHALAN_OFFSET_			0x1000			//电子栅栏FLASH内偏移
#define			_NF_BLOCK_PARTITION_NUM_			32				//128K的块空间，分成8个16K的参数缓冲区
#define			_NF_BLOCK_PARTITION_PAGES_			(_NF_PAGE_PER_BLOCK_ / _NF_BLOCK_PARTITION_NUM_)			//16K大小的每个缓冲区块

#define			_NF_PARAM_LEN_						0x1000	   		//4K配置参数大小
#define			_NF_BARRIER_LEN_					0x1000	  		//4K电子栅栏数据

#define			_NF_CONFIG_START_BLOCK_				4				//配置参数起始块空间
#define			_NF_CONFIG_BACKUP_BLOCK_			5				//配置参数备份参数块号
#define			_NF_CONFIG_BLOCK_NUM_				2				//配置参数空间数

//固话短信存储的数据空间是BLOCK5
#define			_NF_SOLIDMSG_START_BLOCK_			5				//固话短信存储的块起始地址
#define			_NF_SOLIDMSG_NUM_BLOCK_				1				//固话短信存储占据的块数目

/******************************************END of升级程序********************************************/

/**盲区补传定位数据存储， 10，000 * 100B ~ 1MB */
#define NFLASH_BLINDPOS_START_BLOCK		2  		///盲区补传定位信息起始块号
#define NFLASH_BLINDPOS_END_BLOCK		10  	///盲区补传定位信息结束块号



//定义系统擦除操作最大次数
#define		_EFLASH_ERASE_TIMEMAX_		3

#pragma pack(1)

//片外FLASH读写双备份
typedef struct {
	u16 u16SrcBlock;
	u16 u16BakBlock;
	u8 u8SrcPage;
	u8 u8BakPage;

} _tEFLASH_DBAK;
typedef struct {
//与参数有关的信息
	u8 u8CurPartitionNum;			//当前存储的参数位置号(从0号到7号，代表128K的Block分成8等分)

//与点位信息有关变量定义
	u32 u32PointsArrCnt;			//点位数目
	u32 u32PointsBaseAddr;			//点位数据基地址
	u8 u8PointsSize;				//每个点位数据大小

//与多边形区域有关变量定义
	u32 u32polygonCnt;				//多边形区域个数
	u32 u32polygonBaseAddr;			//多边形区域基地址
	u8 u8polygonSize;				//多边形区域数据大小

//与短信模版有关信息定义
	u32 u32ShtMsgTemplateCnt;		//短信模板数目
	u32 u32ShtMsgTemplateBaseAddr;	//短信模板存储基地址
	u8 u8ShtMsgSize;				//每个短信模版大小

//与图片有关信息定义
	u32 u32JpegCnt;					//JPEG图片数目
	u32 u32JpegBaseAddr;			//JPEG图片基地址
	u16 u16CurBlock;				//当前使用的块

//与NANDFLASH有关定义
	u32 u32ID;
	u32 u32BlockSize;				//块大小
	u32 u32PageSize;				//页大小
} tNANDFLASHFSMType;

typedef struct {
	u16 u16Block;
	u16 u16Page;
} tNANDFLASHPOSTypeDef;

//定义操作错误返回代码
typedef enum {
	_NO_ERR_ = 0,			//没有错误，正确返回
	_INVALID_PARAM_ = -1,			//参数错误
	_INVALID_ADDR_ = -2,			//地址越界
	_READ_ERR_ = -3,			//读错误
	_WRITE_ERR_ = -4,			//写错误
	_ERASE_ERR_ = -5,			//擦除错误
	_ERASE_TIMEOUT_ = -6,			//擦除超时
	_READ_STATUS_ERR_ = -7,			//读状态错误
	_OP_NOT_FINISHED_ = -8,			//操作未完成
	_NOSPACE_ERR_ = -9
} _eERRType;

// 局部函数定义
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
//外部函数接口
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

//读写系统参数接口函数
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


//定义操作错误返回代码
typedef enum {
	_NO_ERR_ = 0,			//没有错误，正确返回
	_INVALID_PARAM_ = -1,			//参数错误
	_INVALID_ADDR_ = -2,			//地址越界
	_READ_ERR_ = -3,			//读错误
	_WRITE_ERR_ = -4,			//写错误
	_ERASE_ERR_ = -5,			//擦除错误
	_ERASE_TIMEOUT_ = -6,			//擦除超时
	_READ_STATUS_ERR_ = -7,			//读状态错误
	_OP_NOT_FINISHED_ = -8,			//操作未完成
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
 * 坏块管理
 */
void NF_ScanErrBlock();
BOOL NF_IsErrBlock(u16 u16BlockId);
void NF_MarkErrBlock(u16 u16BlockId);



#endif //__BRAINWARE_ELECTRONICS_NANDFLASH_H__
