/**   
 * @Title: upgrade.c  
 * @Description: TODO(��һ�仰�������ļ���ʲô) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-6-25 ����11:00:57 
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

//�����ļ���д����
//static _tUPGRADType			tUpgrade;
//static _tUPDATEINFO			tUpdInfo;  //������Ϣ
//static _tEFLASH_UPG_INFO  	tUpdFlag;	//�����ɹ���־

//static 	u8 pBuffer[_UPG_PACKLEN_]; //��ʱ����

/**
 * @Func	SD�����
 * @Param	��
 * @Ret		0=�޿���1=�п�
 */
BOOL UPG_SD_Detect() {
//	if(SD_PRESENT == SD_Detect())
//		return TRUE;
	return FALSE;
}

/**
 * @Func	SD����ʼ��
 * @Param	��
 * @Ret		0=ʧ�ܣ�1=�ɹ�
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
 * @Func	��ƽ̨�ϱ�SD������״̬
 * @Param	��
 * @Ret		0=ʧ�ܣ�1=�ɹ�
 */
void UPG_ReportSDCardState(u8 err) {

}

/**
 * @Func	���SD���������ļ��Ƿ����
 * @Param	��
 * @Ret		0=ʧ�ܣ�1=�ɹ�
 */
BOOL Upg_CheckUpgradeFileExisted() {
//	u8		au8Path[120];
//	FRESULT	result = FR_OK;
//
//	sprintf_(au8Path, "%d:/%s", SDC, SD_UPGRADE_FILE_PATH);
//   	result = f_open(&fsrc, au8Path, FA_READ );	//�������ļ�
//   	if (result != FR_OK)  	//��ʧ��
//	{
//		return FALSE;
//	}
//
//	result = f_lseek(&fsrc,0);	  //�������ļ��ɹ����������ļ�ʧ��
//	if (result != FR_OK )
//	{
//		f_close(&fsrc);	  //�ر������ļ�
//		return FALSE;
//	}

	return TRUE;
}

/**
 * @Func	��������ļ���Ч�ԣ��������ݼ��汾���
 * @Param	��
 * @Ret		0=ʧ�ܣ�1=�ɹ�
 */
BOOL UPG_CheckUpgFileValidation() {
#if 0
	FRESULT result = FR_OK;
	u32 u32Read = 0;
	PUPGRADE_FILE_FORMATE_HEADER pFileHeadInfo;
	PUPGRADE_FILE_VERSION_FORMATE pFileVersionInfo;

	//1���ȶ�ȡ�ļ�ͷ�Ͱ汾��Ϣ���ж�������Ϣ�Ƿ���ȷ
	result = f_read(&fsrc, pBuffer,_UPG_HEAD_VERSION_INFO_LEN_ , &u32Read);
	if (result != FR_OK )
	{
		f_close(&fsrc);	  //�ر������ļ�
		return FALSE;
	}
	pFileHeadInfo = (PUPGRADE_FILE_FORMATE_HEADER)pBuffer;
	pFileVersionInfo = (PUPGRADE_FILE_VERSION_FORMATE)(pBuffer + _UPG_VERSION_INFO_LEN_);
	//�ļ�ͷУ��
	if (pFileHeadInfo->upgrade_file_crc != CalculateCRC8((u8*)pFileHeadInfo,_UPG_HEAD_INFO_LEN_ - 1))
	{
		f_close(&fsrc);	  //�ر������ļ�
		return FALSE;
	}
//�ļ��汾У��
//	if (pFileVersionInfo->upgrade_file_crc != CalculateCRC8((u8*)pFileVersionInfo,_UPG_VERSION_INFO_LEN_ - 1))
//	{
//		tSDIOAppFSM.eUpdFileReadResult = UPGRADE_FILE_VERSION_CRC_CRR;
//	    f_close(&fsrc);	  //�ر������ļ�
//		return;
//	}

	//�������״̬
	ZeroMem((u8*)&tUpdInfo, sizeof(_tEFLASH_UPG_INFO));

	//���������ļ���Ϣ
	tUpdInfo.u8UpdateTarget = pFileHeadInfo->upgrade_file_target;
	tUpdInfo.u16Version = pFileVersionInfo->upgrade_file_version;
	tUpdInfo.u32FileSize = pFileHeadInfo->upgrade_file_bytes_count;
	tUpdInfo.u32BlockAmount = pFileHeadInfo->upgrade_file_blocks_count;
	tUpdInfo.u32AvailBlock = 0;
	tUpdInfo.pBlockContent = pBuffer;

	//�������״̬��Ϣ
	tUpgrade.u16SrcBlock = _EFLASH_UPGRADE_SRC_BLOCK_0;
	tUpgrade.u8SrcPage = 0;
	tUpgrade.u16BakBlock = _EFLASH_UPGRADE_BAK_BLOCK_0;
	tUpgrade.u8BakPage = 0;
	tUpgrade.bSrcError = FALSE;
	tUpgrade.bBakError = FALSE;

	//�������״̬
//	ZeroMem((u8*)&tUpdInfo, sizeof(_tEFLASH_UPG_INFO));

	//���ܲ����飬ʱ��̫�������CAN״̬��CAN֡�ڴ����ʧ�ܣ�QK�ǿ���ռ��
	//����Դ��
//	EFLASH_BlockErase(_EFLASH_UPGRADE_SRC_BLOCK_0);
//	EFLASH_BlockErase(_EFLASH_UPGRADE_SRC_BLOCK_1);

	//�������ݿ�
//	EFLASH_BlockErase(_EFLASH_UPGRADE_BAK_BLOCK_0);
//	EFLASH_BlockErase(_EFLASH_UPGRADE_BAK_BLOCK_1);
#endif
	return TRUE;
}

/**
 * @Func	��SD��һ���������ݿ�д��EFLASH
 * @Param	��
 * @Ret		0=ʧ�ܣ�1=�ɹ�
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
//	//ָ��������ǰ���ݿ�λ��
//	result = f_lseek(&fsrc,_UPG_HEAD_VERSION_INFO_LEN_ + (curBlockId * _UPG_PACKLEN_));
//	if (result != FR_OK )
//	{
//		f_close(&fsrc);	  //�ر������ļ�
//		return FALSE;
//	}
//
//	result = f_read(&fsrc, pBuffer, _UPG_PACKLEN_ , &u32Read);	//��ȡһ�����ݿ�
//	if (result != FR_OK )
//	{
//		return FALSE;
//	}
//
//	pBlock = (BLOCKINFO *)pBuffer;
//	if(pBlock->blockId != curBlockId) //��Ų�һ�£�����
//	{
//		f_close(&fsrc);	  //�ر������ļ�
//		return FALSE;
//	}
//
//	crc = CalculateCRC8((u8*)pBuffer, (_UPG_PACKLEN_ -1));	//������CRCУ��
//	if(crc != pBlock->checksum)
//	{
//		f_close(&fsrc);	  //�ر������ļ�
//		return FALSE;
//	}
//
//	tUpdFlag.u8Block_CRC[curBlockId] = CalculateCRC8(pBlock->block, 512);	//�����CRC�룬������ʱУ��
//
//	ret = UPD_SaveUpdBlock(curBlockId, pBlock->block, 512, tUpdFlag.u8Block_CRC[curBlockId]);	//��дFLASH
//	if(!ret)
//	{
//		f_close(&fsrc);	  //�ر������ļ�
//		return FALSE;
//	}
//
//	if(curBlockId == (tUpdInfo.u32BlockAmount - 1)) //�ļ�д��
//	{
//		f_close(&fsrc);	  //�ر������ļ�
//	}
//
	return TRUE;
}

/**
 * @Func	���ر��������ܿ���
 * @Param	��
 * @Ret
 */
u32 UPG_GetBlocksAmount() {
//	return tUpdInfo.u32BlockAmount;
}

/**
 * @Func	�������ɹ���־λ
 * @Param	��
 * @Ret
 */
void UPD_MarkFileReady() {
#if 0
//������ɺ��ڵ�2�����һҳд��������3��32λ��������Ч��־���´ο�����ʱ����������־�Ƿ���Ч��
//�����Ч�Ļ������Ƭ��FLASH�а����������뵽Ƭ��FLASH�̶���λ�ã�Ȼ����λϵͳ
//����������ϵͳ�����еľ���������ĳ��������
	BOOL bSrcUpgFlag;
	BOOL bBakUpgFlag;

	//���������־��Ϣ���������зֿ�CRC��
	tUpdFlag.u32Tag1 = _EFLASH_UPPROG_TAG1_;
	tUpdFlag.u32Tag2 = _EFLASH_UPPROG_TAG2_;
	tUpdFlag.u32Tag3 = _EFLASH_UPPROG_TAG3_;
	tUpdFlag.u16Version = tUpdInfo.u16Version;
	tUpdFlag.u32PackLen = tUpdInfo.u32FileSize;
	tUpdFlag.u8CRC = CalculateCRC8((u8*)&tUpdInfo, sizeof(_tEFLASH_UPG_INFO) - 1);

	//Դ��д��ɹ�
	if(!tUpgrade.bSrcError)
	{
		//д��Դ��������־
		bSrcUpgFlag = EFLASH_PageWrite( _EFLASH_UPGRADE_SRC_BLOCK_1,
				EFLASH_UPG_FLAG_PAGE,
				0,
				sizeof(_tEFLASH_UPG_INFO),
				(u8*)&tUpdInfo
		);

		//дʧ�ܣ�����������־��ϵͳ������ԭ����
		if(TRUE != bSrcUpgFlag)
		{
			EFLASH_PageErase(_EFLASH_UPGRADE_SRC_BLOCK_1, EFLASH_UPG_FLAG_PAGE);
		}
	}

	//���ݿ�д��ɹ�
	if(!tUpgrade.bBakError)
	{
		//д�뱸�ݿ�������־
		bBakUpgFlag = EFLASH_PageWrite( _EFLASH_UPGRADE_BAK_BLOCK_1,
				EFLASH_UPG_FLAG_PAGE,
				0,
				sizeof(_tEFLASH_UPG_INFO),
				(u8*)&tUpdInfo
		);

		//дʧ�ܣ�����������־��ϵͳ������ԭ����
		if(TRUE != bBakUpgFlag)
		{
			EFLASH_PageErase(_EFLASH_UPGRADE_BAK_BLOCK_1, EFLASH_UPG_FLAG_PAGE);
		}
	}

	//Դ��ͱ��ݿ鶼д��ʧ�ܣ������֮ǰд�����ݣ�
	if((TRUE != bSrcUpgFlag) && (TRUE != bBakUpgFlag))
	{

		EFLASH_DoubleBakWrite(_EFLASH_UPGRADE_BAK_BLOCK_1,EFLASH_UPG_FLAG_PAGE,
				_EFLASH_UPGRADE_SRC_BLOCK_1,EFLASH_UPG_FLAG_PAGE,
				(u8*)&tUpdInfo,sizeof(_tEFLASH_UPG_INFO));

		EFLASH_DoubleBakWrite(_EFLASH_UPGRADE_BAK_BLOCK_1,EFLASH_UPG_FLAG_PAGE - 1,
				_EFLASH_UPGRADE_SRC_BLOCK_1,EFLASH_UPG_FLAG_PAGE - 1,
				(u8*)&tUpdInfo,sizeof(_tEFLASH_UPG_INFO));

		//ϵͳ��λ����������ϵͳ
		NVIC_SystemReset();
	}
	else
	{
		//ϵͳ��λ����������ϵͳ
		NVIC_SystemReset();
	}
#endif
}

/**
 * @Func	�������ɹ���־λ
 * @Param	blockId,		[IN]  ��ID
 * 			pBlock,			[IN]  ������
 * 			blockLen,		[IN]  �鳤��
 * 			checksum,		[IN]  ��У��
 * @Ret		0��ʧ�ܣ�1���ɹ�
 */
BOOL UPD_SaveUpdBlock(u16 blockId, u8 *pBlock, u16 blockLen, u8 checksum) {
#if 0
	BOOL ret;
	u32 writeAddress = _EFLASH_UPGRADE_SRC_BLOCK_0 * EF_BLOCK_SIZE + blockId * UPGRADE_FILE_BLOCK_SIZE;

	if(0 == (blockId % UPD_PAGE_BLOCKS)) //����ҳ��
	{
		if(blockId >= UPD_SECTOR_BLOCKS)
		ret = EFLASH_PageErase(_EFLASH_UPGRADE_SRC_BLOCK_1, blockId / UPD_PAGE_BLOCKS);
		else
		ret = EFLASH_PageErase(_EFLASH_UPGRADE_SRC_BLOCK_0, blockId / UPD_PAGE_BLOCKS);

		if(!ret) //����ʧ��
		{
			return FALSE;
		}
	}

	//д��
	EFLASH_WriteData(writeAddress, pBlock, blockLen);

	ZeroMem(pBlock, blockLen);

	//����������У�飬ȷ��������ȷ
	EFLASH_WriteData(writeAddress, pBlock, blockLen);

	if(checksum == CalculateCRC8(pBlock, blockLen))
	{
		return FALSE;
	}
#endif
	return TRUE;
}
