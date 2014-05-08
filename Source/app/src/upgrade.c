/**   
 * @Title: upgrade.c  
 * @Description: TODO(用一句话描述该文件做什么) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-6-25 上午11:00:57 
 * @version V1.0   
 */
#include "upgrade.h"
//#include "ext_flash.h"
//#include "ff.h"
#include "utility.h"
#include "parameter.h"
//#include "sdcard.h"

#define	 SD_UPGRADE_FILE_PATH 	"gps.bin"

#define SDC		0
//static FATFS 	fs;
//static FIL     fsrc;

//程序文件读写控制
//static _tUPGRADType			tUpgrade;
//static _tUPDATEINFO			tUpdInfo;  //升级信息
//static _tEFLASH_UPG_INFO  	tUpdFlag;	//升级成功标志

//static 	u8 pBuffer[_UPG_PACKLEN_]; //临时缓区

/**
 * @Func	SD卡检测
 * @Param	无
 * @Ret		0=无卡，1=有卡
 */
BOOL UPG_SD_Detect() {
//	if(SD_PRESENT == SD_Detect())
//		return TRUE;
	return FALSE;
}

/**
 * @Func	SD卡初始化
 * @Param	无
 * @Ret		0=失败，1=成功
 */
BOOL UPG_SD_Init() {
//	if(SD_OK == SD_Init())
//	{
////		SD_SetDeviceMode(SD_DMA_MODE);
//
//		f_mount(SDC,&fs);
//		return TRUE;
//	}
	return FALSE;
}

/**
 * @Func	向平台上报SD卡错误状态
 * @Param	无
 * @Ret		0=失败，1=成功
 */
void UPG_ReportSDCardState(u8 err) {

}

/**
 * @Func	检查SD卡中升级文件是否存在
 * @Param	无
 * @Ret		0=失败，1=成功
 */
BOOL Upg_CheckUpgradeFileExisted() {
//	u8		au8Path[120];
//	FRESULT	result = FR_OK;
//
//	sprintf_(au8Path, "%d:/%s", SDC, SD_UPGRADE_FILE_PATH);
//   	result = f_open(&fsrc, au8Path, FA_READ );	//打开升级文件
//   	if (result != FR_OK)  	//打开失败
//	{
//		return FALSE;
//	}
//
//	result = f_lseek(&fsrc,0);	  //打开升级文件成功，但索引文件失败
//	if (result != FR_OK )
//	{
//		f_close(&fsrc);	  //关闭升级文件
//		return FALSE;
//	}

	return TRUE;
}

/**
 * @Func	检查升级文件有效性，包括内容及版本检查
 * @Param	无
 * @Ret		0=失败，1=成功
 */
BOOL UPG_CheckUpgFileValidation() {
#if 0
	FRESULT result = FR_OK;
	u32 u32Read = 0;
	PUPGRADE_FILE_FORMATE_HEADER pFileHeadInfo;
	PUPGRADE_FILE_VERSION_FORMATE pFileVersionInfo;

	//1）先读取文件头和版本信息，判断升级信息是否正确
	result = f_read(&fsrc, pBuffer,_UPG_HEAD_VERSION_INFO_LEN_ , &u32Read);
	if (result != FR_OK )
	{
		f_close(&fsrc);	  //关闭升级文件
		return FALSE;
	}
	pFileHeadInfo = (PUPGRADE_FILE_FORMATE_HEADER)pBuffer;
	pFileVersionInfo = (PUPGRADE_FILE_VERSION_FORMATE)(pBuffer + _UPG_VERSION_INFO_LEN_);
	//文件头校验
	if (pFileHeadInfo->upgrade_file_crc != CalculateCRC8((u8*)pFileHeadInfo,_UPG_HEAD_INFO_LEN_ - 1))
	{
		f_close(&fsrc);	  //关闭升级文件
		return FALSE;
	}
//文件版本校验
//	if (pFileVersionInfo->upgrade_file_crc != CalculateCRC8((u8*)pFileVersionInfo,_UPG_VERSION_INFO_LEN_ - 1))
//	{
//		tSDIOAppFSM.eUpdFileReadResult = UPGRADE_FILE_VERSION_CRC_CRR;
//	    f_close(&fsrc);	  //关闭升级文件
//		return;
//	}

	//清除升级状态
	ZeroMem((u8*)&tUpdInfo, sizeof(_tEFLASH_UPG_INFO));

	//保存升级文件信息
	tUpdInfo.u8UpdateTarget = pFileHeadInfo->upgrade_file_target;
	tUpdInfo.u16Version = pFileVersionInfo->upgrade_file_version;
	tUpdInfo.u32FileSize = pFileHeadInfo->upgrade_file_bytes_count;
	tUpdInfo.u32BlockAmount = pFileHeadInfo->upgrade_file_blocks_count;
	tUpdInfo.u32AvailBlock = 0;
	tUpdInfo.pBlockContent = pBuffer;

	//填充升级状态信息
	tUpgrade.u16SrcBlock = _EFLASH_UPGRADE_SRC_BLOCK_0;
	tUpgrade.u8SrcPage = 0;
	tUpgrade.u16BakBlock = _EFLASH_UPGRADE_BAK_BLOCK_0;
	tUpgrade.u8BakPage = 0;
	tUpgrade.bSrcError = FALSE;
	tUpgrade.bBakError = FALSE;

	//清除升级状态
//	ZeroMem((u8*)&tUpdInfo, sizeof(_tEFLASH_UPG_INFO));

	//不能擦整块，时间太长，造成CAN状态机CAN帧内存分配失败，QK是可抢占的
	//擦除源块
//	EFLASH_BlockErase(_EFLASH_UPGRADE_SRC_BLOCK_0);
//	EFLASH_BlockErase(_EFLASH_UPGRADE_SRC_BLOCK_1);

	//擦除备份块
//	EFLASH_BlockErase(_EFLASH_UPGRADE_BAK_BLOCK_0);
//	EFLASH_BlockErase(_EFLASH_UPGRADE_BAK_BLOCK_1);
#endif
	return TRUE;
}

/**
 * @Func	将SD卡一个升级数据块写入EFLASH
 * @Param	无
 * @Ret		0=失败，1=成功
 */
BOOL UPG_TransferBlocks2Flash(u16 curBlockId) {
//	FRESULT	result = FR_OK;
//	u32 u32Read = 0;
//	BLOCKINFO *pBlock = NULL;
//	u8 crc = 0;
//	BOOL ret;
//
//	if(curBlockId >= tUpdInfo.u32BlockAmount)
//		return FALSE;
//
//	//指针跳到当前数据块位置
//	result = f_lseek(&fsrc,_UPG_HEAD_VERSION_INFO_LEN_ + (curBlockId * _UPG_PACKLEN_));
//	if (result != FR_OK )
//	{
//		f_close(&fsrc);	  //关闭升级文件
//		return FALSE;
//	}
//
//	result = f_read(&fsrc, pBuffer, _UPG_PACKLEN_ , &u32Read);	//读取一个数据块
//	if (result != FR_OK )
//	{
//		return FALSE;
//	}
//
//	pBlock = (BLOCKINFO *)pBuffer;
//	if(pBlock->blockId != curBlockId) //块号不一致，错误
//	{
//		f_close(&fsrc);	  //关闭升级文件
//		return FALSE;
//	}
//
//	crc = CalculateCRC8((u8*)pBuffer, (_UPG_PACKLEN_ -1));	//块数据CRC校验
//	if(crc != pBlock->checksum)
//	{
//		f_close(&fsrc);	  //关闭升级文件
//		return FALSE;
//	}
//
//	tUpdFlag.u8Block_CRC[curBlockId] = CalculateCRC8(pBlock->block, 512);	//保存块CRC码，供升级时校验
//
//	ret = UPD_SaveUpdBlock(curBlockId, pBlock->block, 512, tUpdFlag.u8Block_CRC[curBlockId]);	//块写FLASH
//	if(!ret)
//	{
//		f_close(&fsrc);	  //关闭升级文件
//		return FALSE;
//	}
//
//	if(curBlockId == (tUpdInfo.u32BlockAmount - 1)) //文件写完
//	{
//		f_close(&fsrc);	  //关闭升级文件
//	}
//
	return TRUE;
}

/**
 * @Func	返回本次升级总块数
 * @Param	无
 * @Ret
 */
u32 UPG_GetBlocksAmount() {
//	return tUpdInfo.u32BlockAmount;
}

/**
 * @Func	置升级成功标志位
 * @Param	无
 * @Ret
 */
void UPD_MarkFileReady() {
#if 0
//升级完成后，在第2块最后一页写入连续的3个32位的升级有效标志，下次开机的时候检测升级标志是否有效，
//如果有效的话，则从片外FLASH中把升级程序导入到片内FLASH固定的位置，然后软复位系统
//重新启动后，系统中运行的就是升级后的程序代码了
	BOOL bSrcUpgFlag;
	BOOL bBakUpgFlag;

	//填充升级标志信息，包含所有分块CRC码
	tUpdFlag.u32Tag1 = _EFLASH_UPPROG_TAG1_;
	tUpdFlag.u32Tag2 = _EFLASH_UPPROG_TAG2_;
	tUpdFlag.u32Tag3 = _EFLASH_UPPROG_TAG3_;
	tUpdFlag.u16Version = tUpdInfo.u16Version;
	tUpdFlag.u32PackLen = tUpdInfo.u32FileSize;
	tUpdFlag.u8CRC = CalculateCRC8((u8*)&tUpdInfo, sizeof(_tEFLASH_UPG_INFO) - 1);

	//源块写入成功
	if(!tUpgrade.bSrcError)
	{
		//写入源块升级标志
		bSrcUpgFlag = EFLASH_PageWrite( _EFLASH_UPGRADE_SRC_BLOCK_1,
				EFLASH_UPG_FLAG_PAGE,
				0,
				sizeof(_tEFLASH_UPG_INFO),
				(u8*)&tUpdInfo
		);

		//写失败，擦除升级标志，系统仍运行原程序
		if(TRUE != bSrcUpgFlag)
		{
			EFLASH_PageErase(_EFLASH_UPGRADE_SRC_BLOCK_1, EFLASH_UPG_FLAG_PAGE);
		}
	}

	//备份块写入成功
	if(!tUpgrade.bBakError)
	{
		//写入备份块升级标志
		bBakUpgFlag = EFLASH_PageWrite( _EFLASH_UPGRADE_BAK_BLOCK_1,
				EFLASH_UPG_FLAG_PAGE,
				0,
				sizeof(_tEFLASH_UPG_INFO),
				(u8*)&tUpdInfo
		);

		//写失败，擦除升级标志，系统仍运行原程序
		if(TRUE != bBakUpgFlag)
		{
			EFLASH_PageErase(_EFLASH_UPGRADE_BAK_BLOCK_1, EFLASH_UPG_FLAG_PAGE);
		}
	}

	//源块和备份块都写入失败，则擦除之前写入数据，
	if((TRUE != bSrcUpgFlag) && (TRUE != bBakUpgFlag))
	{

		EFLASH_DoubleBakWrite(_EFLASH_UPGRADE_BAK_BLOCK_1,EFLASH_UPG_FLAG_PAGE,
				_EFLASH_UPGRADE_SRC_BLOCK_1,EFLASH_UPG_FLAG_PAGE,
				(u8*)&tUpdInfo,sizeof(_tEFLASH_UPG_INFO));

		EFLASH_DoubleBakWrite(_EFLASH_UPGRADE_BAK_BLOCK_1,EFLASH_UPG_FLAG_PAGE - 1,
				_EFLASH_UPGRADE_SRC_BLOCK_1,EFLASH_UPG_FLAG_PAGE - 1,
				(u8*)&tUpdInfo,sizeof(_tEFLASH_UPG_INFO));

		//系统软复位，重新引导系统
		NVIC_SystemReset();
	}
	else
	{
		//系统软复位，重新引导系统
		NVIC_SystemReset();
	}
#endif
}

/**
 * @Func	置升级成功标志位
 * @Param	blockId,		[IN]  块ID
 * 			pBlock,			[IN]  块数据
 * 			blockLen,		[IN]  块长度
 * 			checksum,		[IN]  块校验
 * @Ret		0：失败，1：成功
 */
BOOL UPD_SaveUpdBlock(u16 blockId, u8 *pBlock, u16 blockLen, u8 checksum) {
#if 0
	BOOL ret;
	u32 writeAddress = _EFLASH_UPGRADE_SRC_BLOCK_0 * EF_BLOCK_SIZE + blockId * UPGRADE_FILE_BLOCK_SIZE;

	if(0 == (blockId % UPD_PAGE_BLOCKS)) //擦除页首
	{
		if(blockId >= UPD_SECTOR_BLOCKS)
		ret = EFLASH_PageErase(_EFLASH_UPGRADE_SRC_BLOCK_1, blockId / UPD_PAGE_BLOCKS);
		else
		ret = EFLASH_PageErase(_EFLASH_UPGRADE_SRC_BLOCK_0, blockId / UPD_PAGE_BLOCKS);

		if(!ret) //擦除失败
		{
			return FALSE;
		}
	}

	//写入
	EFLASH_WriteData(writeAddress, pBlock, blockLen);

	ZeroMem(pBlock, blockLen);

	//立即读出并校验，确保数据正确
	EFLASH_WriteData(writeAddress, pBlock, blockLen);

	if(checksum == CalculateCRC8(pBlock, blockLen))
	{
		return FALSE;
	}
#endif
	return TRUE;
}
