/***********************************************************************************************************
* ��    ����	NANDFLASH.C
* ��    ����	Ƭ��FLASH�����ӿڼ�Ӧ�ô���
* �޸ļ�¼��	1)	��  �ڣ�2009-01-19
*					��  ����V1.0
*					��  ����
*               2)  ��  �ڣ�
*					��  ����
*					�޸��ߣ�
*					��  �⣺
*					��  �ģ�
***********************************************************************************************************/

#include "NandFlash.h"
#include "parameter.h"
#include "utility.h"
#include "board.h"
//#include	"periph.h"

//extern	BARRIER			tBarrier;									//����դ��
//extern	MSGTEMPLATE		tMsgTmpl;									//����ģ��
//
////�����ļ���д����
//_tEFLASH_DBAK	tUpdProgram;

/***********************************************************************************************************
 �����������������Щ�������������������������������������������������������������������������
 �� �ܽ�����     ���ܽŹ���												                   ��
 �����������������੤������������������������������������������������������������������������
 ��IO0~IO7       ��IO�ܽ����������������ַ�����ݵģ�Ҳ�����ڶ�ʱ���������              ��
 �����������������੤������������������������������������������������������������������������
 �� CLE          ��CLE����������ʹ�ܣ�����Ч����WE�������ذ�����ͨ��IO��д�뵽����Ĵ����� ��
 �����������������੤������������������������������������������������������������������������
 �� ALE          ��ALE�ǵ�ַ����ʹ�ܣ���ALEΪ��ʱ����WE������������                        �� 
 �����������������੤������������������������������������������������������������������������
 �� CE           ��Ƭѡʹ�ܣ�����Ч                                                        ��
 �����������������੤������������������������������������������������������������������������
 �� RE           ����ʹ�ܣ�����Ч	                                                       ��
 �����������������੤������������������������������������������������������������������������
 �� WE           ��дʹ�ܣ�����Ч                                                          ��
 �����������������੤������������������������������������������������������������������������
 �� WP           ��д����������Ч                                                          ��
 �����������������੤������������������������������������������������������������������������
 �� R/B          ������/æ�������ʱ����ʾ�ڽ��б�̡��������������ȡ��������ʱ��ʾ������ ��
 �����������������੤������������������������������������������������������������������������
 �� Vcc          ������												                       ��
 �����������������੤������������������������������������������������������������������������
 �� Vss          ���� 												                       �� 
 �����������������੤������������������������������������������������������������������������
 �� NC           ������												                       ��
 �����������������ة�������������������������������������������������������������������������


 �����������������Щ����������������������Щ�������������������
 �� ��ַ/ƫ��    ��       ��С           ��        ����      ��
 �����������������੤���������������������੤������������������
 ��  0x000000    ��       256K           ��    ��������      ��
 �����������������੤���������������������੤������������������
 ��  0x040000    ��      4K              ��   ���ò���       ��
 ��  0x040000    ��      256�ֽ�         ��   �豸����       ��
 ��  0x040100    ��      256�ֽ�         ��   ��������       ��
 ��  0x040200    ��      256�ֽ�         ��   ͨѶ����       ��
 ��  0x040300    ��      256�ֽ�         ��   ͳ�Ʋ���       ��
 ��  0x040400    ��      256�ֽ�         ��   ���в���       ��
 �����������������੤���������������������੤������������������
 ��  0x04100     ��      4K              ��   ����դ��       ��
 �����������������ة����������������������ة�������������������
*************************************************д������дդ����������ƿ���*****************************************
1.NAND FLASH�Ĳ����ص㿼�ǣ�
1)оƬ���õ���K9F1G08U0B��оƬ��СΪ128MB����Ϊ1024��BLOCK(��)��ÿ���СΪ128KB��ÿ��ֳ�64ҳ��ÿҳ��СΪ2KB��
2)оƬ�Ĳ����������Կ�Ϊ��λ�ġ�
3)оƬ��д�������ҳΪ��λ��
4)NAND FLASH����Ҳ�д������ƣ������ٲ�����

2.�洢���ݽṹ�ص㣺
1)�洢�Ĳ�����Ҫ�У����ò���(�����豸����������������ͨѶ������ͳ�Ʋ��������в�����)����λ��Ϣ�ȡ�
2)���ò�����Ҫ�ڳ������й��������޸ġ�

�������Ǹ��ݲ�����С�Ŀ��ǣ�Ϊ������Ϣ��������BLOCK����256K�ֽڴ�С(��ʵ���󲿷ֵĿռ䶼�����õ�)��
BLOCK1Ϊ���ݻ�������BLOCK2Ϊ���ݻ�������
��һ��BLOCK(128K)�ֳ�8���ӿ飬ÿ���СΪ16K(ʵ����Ҳ���Կ��Ƿ�Ϊ16���ӿ飬ÿ���ӿ��С8K)��
1)	ÿ���ӿ�ǰ0x100���ֽڱ�����������ǰ8���ֽ�λ�ô洢���Ƿ�д���־�����д�����ò����ˣ���ǰ8���ֽ���ϢΪ0x12345678��0x55AAAA55��
��һ�ε����ò�������д���ӿ�λ��0����һ��Ҫ�޸�ĳд���ݵĻ�������ӿ�0��δ�޸ĵ����ݶ�������д�뵽�ӿ�1�У�Ҫ�޸ĵĲ�������
�޸ĺ�д�뵽�ӿ�1�У�д����ɣ������ӿ�1ռ�ñ�־���������ơ�
2)	�����е��ӿ鶼��ռ�ú�����BLOCK2�в���һ�����е��ӿ飬Ȼ���BLOCK1�����һ���ӿ�����ݿ�����BLOCK2�У��޸ĵ�����Ҳ������BLOCK2
�еĿ����ӿ鴦��Ȼ�����BLOCK1���ٰ�BLOCK2��д�������д�ص�BLOCK1�ĵ�һ���ӿ�λ�ã�Ȼ������д��ɱ�־��
3)	��BLOCK2�еĿ����ӿ�Ҳ����ռ�ú������Ȳ���BLOCK2��Ȼ���ٽ���2)������

**************************************************д������дդ����������ƿ���**********************************/


/****************************************************************************************************************/



/**
  * @brief  DeInitializes the peripherals used by the NAND FLASH FLASH driver.
  * @param  None
  * @retval None
  */
void NFLASH_LowLevel_DeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	/*!< Configure SFLASH_SPI pins: SCK */
	GPIO_InitStructure.GPIO_Pin = GPIOD_NF_RB | GPIOD_NF_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	

}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void NFLASH_LowLevel_Init(void)	 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//DO-D7 RD WR ALE CLE��IO���Ѿ���LED��ʼ��������
	//NFLASH_LowLevel_Initֻ��Ҫ����FLASH����ʹ�õ�IO��
	
	/* Enable GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE |
	                     RCC_AHB1Periph_GPIOF, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);	//D2
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);	//D3
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);	//RD
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);	//WR
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);	//ALE
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);	//CLE
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);	//D0
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);	//D1
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC); 	
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);		//D4	  
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);		//D5 
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);		//D6
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);		//D7	
	
	
	
	/*-- GPIO Configuration ------------------------------------------------------*/
	/* SRAM Data lines,  NOE and NWE configuration */
	GPIO_InitStructure.GPIO_Pin = GPIOD_FSMC_D0 | GPIOD_FSMC_D1 | GPIOD_FSMC_D2 | GPIOD_FSMC_D3 |
	                            GPIOD_FSMC_ALE | GPIOD_FSMC_CLE | GPIOD_NF_RB   | GPIOD_NF_CS   |              
	                            GPIOD_FSMC_RD |GPIOD_FSMC_WR;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
#if			0	
	GPIO_InitStructure.GPIO_Pin = GPIOD_NF_RB;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	 //CS
	GPIO_InitStructure.GPIO_Pin = GPIOD_NF_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
#endif

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIOE_FSMC_D4 | GPIOE_FSMC_D5 | GPIOE_FSMC_D6 | GPIOE_FSMC_D7 ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

}


/**
  * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
  * @param  None
  * @retval None
  */
void NAND_FSMCConfig(void)
{
	FSMC_NANDInitTypeDef FSMC_NANDInitStruct;
	FSMC_NAND_PCCARDTimingInitTypeDef p;

	/* Enable FSMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	FSMC_NANDDeInit(FSMC_Bank2_NAND);

	/*-- FSMC Configuration ------------------------------------------------------*/
	FSMC_NANDStructInit(&FSMC_NANDInitStruct);

	p.FSMC_SetupTime = 0x10;
    p.FSMC_WaitSetupTime = 0x5;
    p.FSMC_HoldSetupTime = 0x10;
    p.FSMC_HiZSetupTime = 0x5;

	FSMC_NANDInitStruct.FSMC_CommonSpaceTimingStruct = &p;
    FSMC_NANDInitStruct.FSMC_AttributeSpaceTimingStruct = &p;

	FSMC_NANDInit(&FSMC_NANDInitStruct);

	/* Enable FSMC NAND Bank2 */
	FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);

}

/***********************************************************************************************
* ����������	��ȡNAND FLASH ��ID
* ��    �ڣ�	��			
* ��    �ڣ�	pu8Buf,���ص�ID��Ϣ����ʼ��ַ�����ص�ID��Ϣ�����ĸ��ֽڣ�����Ϊ
				��һ�ֽ�		�����̴���
				�ڶ��ֽ�		�豸����
				�����ֽ�		�޹ؽ�Ҫ
				�����ֽ�		����ҳ��С�����С������ռ��С�Լ���ȡʱ�����Ϣ
				��δ����Ϊ  
				Bit 1��Bit 0   ����ҳ��С
				Bit  1    0  	0    0  ��ʾҳ��СΪ1K
				                0    1  ��ʾҳ��СΪ2K
				                1    0  ����
				                1    1  ����
				Bit 5��Bit 4���ƿ��С
				Bit  5    4     0    0  ��ʾ���СΪ64K
								0    1  ��ʾ���СΪ128K
								1    0  ��ʾ���СΪ256K
								1	 1  ����
			    Bit 2�������������С(ÿ512�ֽ�)
			    Bit 2           0  	     ���������СΪ8�ֽ�
			    				1		 ���������СΪ16�ֽ�
			    
			    Bit 6�������߿��
			    Bit 6			0        8λ
			    				1		 16λ
			    Bit 7 ��Bit 3��ʾ���л�ȡʱ��
*************************************************************************************************/
u8	NandFlashReadId(u8 *pu8Buf)
{
   u8 i = 0;
	//Ƭѡ							 
//	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
//	while(i++ < 10)
	NF_CMD = NF_READID;
	
	NF_ADDR = 0x00;


	*pu8Buf = NF_DATA;
	*(pu8Buf + 1) = NF_DATA;
	*(pu8Buf + 2) = NF_DATA;
	*(pu8Buf + 3) = NF_DATA;
	*(pu8Buf + 4) = NF_DATA;	
	//ȥƬѡ	
//	GPIO_SetBits(GPIOD,GPIOD_NF_CS);

	return TRUE;
}


/*************************************************************************************************
* ����������	��״̬�����ز�����״̬��Ϣ
* ��    �ڣ�	��			
* ��    �ڣ�	���ص�״̬�ֽ� D[0]=1��дʧ�ܡ�D[1]=1������дʧ�ܡ�D[6]=1��æ��D[7]=1û��д����
*************************************************************************************************/
u8	NandFlashStatus(void)
{
	u8	u8status;

	//Ƭѡ
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);
		
	NF_CMD = NF_STATUS;
	
	u8status = NF_DATA;

	//ȥƬѡ	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);
		
	return u8status;
}


u32 NF_GetAddrByBlockPageAndOffset(u16 u16Block,u8 u8Page,u16 u16Offset)
{
	u32 u32Addr = 0;

	//���ݿ�š�ҳ�š�ҳ��ƫ�Ƶ�ַ�����Ҫ��ȡ�����ݵ�ַ
	u32Addr =
		(((u32)u16Block) << _NF_BLOCK_OFFSET_) | (((u32)u8Page) << _NF_PAGE_OFFSET_) | u16Offset;

	return  u32Addr;
}

/************************************************************************************************
* ����������	ҳ������
* ��    �ڣ�	u16Block,Ҫ��ȡ��ҳ���
				u8Page,Ҫ��ȡ��ҳҳ��
				u16Offset��ҳ��ƫ�Ƶ�ַ
				u16Size��Ҫ��ȡ�����ݳ��ȡ�
				pData[OUT]����ȡ��������RAM�б������ʼ��ַ				
* ��    �ڣ�	_eERRType ���ص�״̬�ֽ� ��0��ʾ�����ɹ���������ʾ����
* ˵    ����	ҳ������������д��������0x00,Ȼ��д��32λ�ĵ�ַ����д��������0x30,�ȴ��������Tr(С��25us)
				��(ͨ����ȡR/B�ܽ�״̬��ȷ���Ƿ�������)���ٴ������϶�ȡ����
*************************************************************************************************/
_eERRType		NF_PageRead(u16 u16Block,u8 u8Page,u16 u16Offset,u16 u16Size,u8* pData)
{
	u32 u32Addr = 0;
	//�жϲ����Ƿ���Ч
	if(pData == 0 || u16Block >= _NF_MAX_BLOCK_NUM_ || u8Page >= _NF_MAX_PAGE_NUM_ ||
		u16Offset > _NF_PAGE_SIZE_	|| u16Size > _NF_PAGE_SIZE_)
	{
		return _INVALID_PARAM_;
	}	
	//���ݿ�š�ҳ�š�ҳ��ƫ�Ƶ�ַ�����Ҫ��ȡ�����ݵ�ַ
	u32Addr =
		(((u32)u16Block) << _NF_BLOCK_OFFSET_) | (((u32)u8Page) << _NF_PAGE_OFFSET_) | u16Offset;

	return NF_RawPageRead(u32Addr, u16Size, pData)	;

}

 /************************************************************************************************
* ����������	ҳд����
* ��    �ڣ�	u32Addr,Ҫд���������NAND FLASH��ŵĵ�ַ
				u16Length,Ҫд��������ֽ���Ŀ	
				pu8Buf����д������ݻ������׵�ַ		
* ��    �ڣ�	_eERRType ���ص�״̬�ֽ� ��0��ʾ�����ɹ���������ʾ����
* ˵    ����	ҳд�����������д��������0x80,Ȼ��д��32λ�ĵ�ַ����д���д�������
				��д��������0x10,Ȼ����R/B����״̬���жϲ����Ƿ���ɣ�Ȼ���ȡ״̬�ֽ�
				�жϴ˲����Ƿ�ɹ�
*************************************************************************************************/
_eERRType		NF_PageWrite(u16 u16Block,u8 u8Page,u16 u16Offset,u16 u16Size,u8* pData)
{
	u32 u32Addr = 0;
	//�жϲ����Ƿ���Ч
	if(pData == 0 || u16Block >= _NF_MAX_BLOCK_NUM_ || u8Page >= _NF_MAX_PAGE_NUM_ ||
		u16Offset > _NF_PAGE_SIZE_	|| u16Size > _NF_PAGE_SIZE_)
	{
		return _INVALID_PARAM_;
	}	
	//���ݿ�š�ҳ�š�ҳ��ƫ�Ƶ�ַ�����Ҫ��ȡ�����ݵ�ַ
	u32Addr =
		(((u32)u16Block) << _NF_BLOCK_OFFSET_) | (((u32)u8Page) << _NF_PAGE_OFFSET_) | u16Offset;

	return NF_RawPageWrite(u32Addr, u16Size, pData);
}

/*************************************************************************************************
* ����������	ҳ������
* ��    �ڣ�	u32Addr,Ҫ��ȡ��������NAND FLASH��ŵĵ�ַ
				u16Length,Ҫ��ȡ�������ֽ���Ŀ			
* ��    �ڣ�	_eERRType ���ص�״̬�ֽ� ��0��ʾ�����ɹ���������ʾ����
				pu8Buf,Ҫ���������ݱ���Ļ�������ʼ��ַ
* ˵    ����	ҳ������������д��������0x00,Ȼ��д��32λ�ĵ�ַ����д��������0x30,�ȴ��������Tr(С��25us)
				��(ͨ����ȡR/B�ܽ�״̬��ȷ���Ƿ�������)���ٴ������϶�ȡ����
*************************************************************************************************/
_eERRType	NF_RawPageRead(u32 u32Addr, u16 u16Length, u8 *pu8Buf)
{
	u8	u8Tmp = 0;
	
	//��������
	if(u16Length == 0 || pu8Buf == 0 )
	{
		return _INVALID_PARAM_;
	}
	//��ַԽ��
	if(u32Addr >= _NF_MAX_ADDR_)
	{
		return _INVALID_ADDR_;
	}
	
	for(u8Tmp = 0; u8Tmp < 25; u8Tmp++)													
	{
		// ��ȡ״̬��־����æ��ʼ����������ȴ�
		if(_NF_STATUS_READY_ == (NandFlashStatus() & _NF_STATUS_MASK_))			
		{
			break;
		}
		Delay_1ms(1);
	}
		
	if(u8Tmp > 4)
	{
//		return _READ_STATUS_ERR_;
	}

	Delay_1ms(1);

	//Ƭѡ
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);

	//EIC_IRQCommand(DISABLE);		
	// ҳ������
	NF_CMD = NF_READ1;
															
//	NF_ADDR = u32Addr&0x000000FF;	
//	NF_ADDR = (u32Addr>>8)&0x0000000F;

	NF_ADDR = 0;
	NF_ADDR = 0;
	NF_ADDR = (u32Addr>>16)&0x000000FF;
	NF_ADDR = (u32Addr>>24)&0x000000FF;
	
	NF_CMD = NF_READ2;
	
	for(u8Tmp = 0; u8Tmp < 25; u8Tmp++)	
	{
		//�ж�R/B�ܽ��Ƿ���ʾ�������
		if(GPIO_ReadInputDataBit(GPIOD,GPIOD_NF_RB))
		{
			break;
		}
		
		Delay_1us(2);
	}

	if(u8Tmp >= 25)
	{
///		EIC_IRQCommand(ENABLE);
		//ȥƬѡ	
		GPIO_SetBits(GPIOD,GPIOD_NF_CS);
		return _READ_ERR_;
	}
	while(u16Length-- > 0)
		*(pu8Buf++) = NF_DATA;

///   	EIC_IRQCommand(ENABLE);

	//ȥƬѡ	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);
		
	return _NO_ERR_;
}


/***********************************************************************************************
* ����������	ҳд����
* ��    �ڣ�	u32Addr,Ҫд���������NAND FLASH��ŵĵ�ַ
				u16Length,Ҫд��������ֽ���Ŀ	
				pu8Buf����д������ݻ������׵�ַ		
* ��    �ڣ�	_eERRType ���ص�״̬�ֽ� ��0��ʾ�����ɹ���������ʾ����
* ˵    ����	ҳд�����������д��������0x80,Ȼ��д��32λ�ĵ�ַ����д���д�������
				��д��������0x10,Ȼ����R/B����״̬���жϲ����Ƿ���ɣ�Ȼ���ȡ״̬�ֽ�
				�жϴ˲����Ƿ�ɹ�
*************************************************************************************************/
_eERRType	NF_RawPageWrite(u32 u32Addr, u16 u16Length, u8 *pu8Buf)
{
	u8	u8Tmp = 0;
	u16	u16Index = 0;
	//��������
	if(u16Length == 0 || pu8Buf == 0 )
	{
		return _INVALID_PARAM_;
	}
	//��ַԽ��
	if(u32Addr >= _NF_MAX_ADDR_)
	{
		return _INVALID_ADDR_;
	}
	
	for(u8Tmp = 0; u8Tmp < 25; u8Tmp++)													
	{
		// ��ȡ״̬��־��������ʼ����������ȴ�
		if(_NF_STATUS_WRITE_PERMIT_ == NandFlashStatus())			
		{
			break;
		}
		Delay_1ms(1);
	}

		
	if(u8Tmp >= 5)
	{
///		return _READ_STATUS_ERR_;
	}
	 
//	Delay_1ms(1);
	//Ƭѡ
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);

///	EIC_IRQCommand(DISABLE);

	// ҳд����
	NF_CMD = NF_PAGEWRITE1;	
												
	NF_ADDR = u32Addr&0x000000FF;	
	NF_ADDR = (u32Addr>>8)&0x0000000F;

//	NF_ADDR = 0;
//	NF_ADDR = 0;
	NF_ADDR = (u32Addr>>16)&0x000000FF;
	NF_ADDR = (u32Addr>>24)&0x000000FF;
	
	for(u16Index = 0; u16Index < u16Length; u16Index++)
		NF_DATA = *(pu8Buf + u16Index);
	NF_CMD = NF_PAGEWRITE2;

///	EIC_IRQCommand(ENABLE);

	// ���ȴ�5ms
	for(u8Tmp = 0;u8Tmp < 5;u8Tmp ++)														
	{
		//�ж�R/B�ܽ��Ƿ���ʾ�������
		if(GPIO_ReadInputDataBit(GPIOD,GPIOD_NF_RB))
		{
			break;
		}
		
		Delay_1ms(1);
	}

	//ȥƬѡ	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);	

	if(u8Tmp > 4)
		return _WRITE_ERR_;
		
	if((NandFlashStatus() & _NF_STATUS_ERASE_FAILURE_) == _NF_STATUS_ERASE_FAILURE_)
		return _WRITE_ERR_;

	//Ƭѡ
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);

///	EIC_IRQCommand(DISABLE);
	// ҳ������
	NF_CMD = NF_READ1;
															
//	NF_ADDR = u32Addr&0x000000FF;	
//	NF_ADDR = (u32Addr>>8)&0x0000000F;

	NF_ADDR = 0;
	NF_ADDR = 0;
	NF_ADDR = (u32Addr>>16)&0x000000FF;
	NF_ADDR = (u32Addr>>24)&0x000000FF;
	
	NF_CMD = NF_READ2;
	
	for(u8Tmp = 0; u8Tmp < 25; u8Tmp++)	
	{
		//�ж�R/B�ܽ��Ƿ���ʾ�������
		if(GPIO_ReadInputDataBit(GPIOD,GPIOD_NF_RB))
		{
			break;
		}
		
		Delay_1us(1);
	}

	if(u8Tmp >= 25)
	{
///		EIC_IRQCommand(ENABLE);
		return _READ_ERR_;
	}
	
	for(u16Index = 0; u16Index < u16Length; u16Index++)
	{
		if(*(pu8Buf++) != NF_DATA)
		{

///			EIC_IRQCommand(ENABLE);
			//ȥƬѡ	
			GPIO_SetBits(GPIOD,GPIOD_NF_CS);	
			return _READ_ERR_;
		}
	}
///	EIC_IRQCommand(ENABLE);
	//ȥƬѡ	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);	

	return _NO_ERR_;
}

/*************************************************************************************************
* ����������	���������
* ��    �ڣ�	u32Addr,Ҫ�����Ŀ��ַ		
* ��    �ڣ�	_eERRType ���ص�״̬�ֽ� ��0��ʾ�����ɹ���������ʾ����
* ˵    ����	���������ֻ��Ҫд���е�ַ���е�ַ����Ҫ
*************************************************************************************************/
_eERRType	NF_BlockErase(u16 u16Block)
{
	u8	u8Tmp = 0;

 	u32  u32Addr = ((u32)u16Block) << _NF_BLOCK_OFFSET_;

  	//��ַԽ��
	if(u16Block >= _NF_MAX_BLOCK_NUM_)
	{
		return _INVALID_ADDR_;
	}

	for(u8Tmp = 0; u8Tmp < 5; u8Tmp++)													
	{
		// ��ȡ״̬��־��������ʼ����������ȴ�
		if(_NF_STATUS_WRITE_PERMIT_ == NandFlashStatus())			
		{
			break;
		}
		Delay_1ms(1);
	}
		
	if(u8Tmp > 4)
	{
		return _READ_STATUS_ERR_;
	}

 //	Delay_1ms(1);

	//Ƭѡ
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);

///	EIC_IRQCommand(DISABLE);
	
	// ���������	
	NF_CMD = NF_BLOCKERASE1;
												
	NF_ADDR = (u32Addr>>16) &0x000000FF;
	NF_ADDR = (u32Addr>>24) &0x000000FF;
	
	NF_CMD = NF_BLOCKERASE2;

///	EIC_IRQCommand(ENABLE);

	// ���ȴ�5ms
	for(u8Tmp = 0;u8Tmp < 5;u8Tmp ++)														
	{
		//�ж�R/B�ܽ��Ƿ���ʾ�������
		if(GPIO_ReadInputDataBit(GPIOD,GPIOD_NF_RB))
		{
			break;
		}
		
		Delay_1ms(1);
	}

	//ȥƬѡ	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);	
		
	if(u8Tmp > 4)
	{
		return _WRITE_ERR_;
	}
		
	if((NandFlashStatus() & _NF_STATUS_ERASE_FAILURE_) == _NF_STATUS_ERASE_FAILURE_)
	{
		return _WRITE_ERR_;
	}


	//Ƭѡ
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);
///	EIC_IRQCommand(DISABLE);
		
	// ҳ������
	NF_CMD = NF_READ1;
															
	NF_ADDR = u32Addr&0x000000FF;	
	NF_ADDR = (u32Addr>>8)&0x0000000F;
	NF_ADDR = 0;
	NF_ADDR = 0;
	NF_ADDR = (u32Addr>>16)&0x000000FF;
	NF_ADDR = (u32Addr>>24)&0x000000FF;
	
	NF_CMD = NF_READ2;

///	EIC_IRQCommand(ENABLE);
	
	for(u8Tmp = 0; u8Tmp < 25; u8Tmp++)	
	{
		//�ж�R/B�ܽ��Ƿ���ʾ�������
		if(GPIO_ReadInputDataBit(GPIOD,GPIOD_NF_RB))
		{
			break;
		}
		
		Delay_1us(1);
	}

	if(u8Tmp >= 25)
	{
///		EIC_IRQCommand(ENABLE);
		//ȥƬѡ	
		GPIO_SetBits(GPIOD,GPIOD_NF_CS);	
		return _READ_ERR_;
	}
///	EIC_IRQCommand(DISABLE);
	
	for(u8Tmp = 0; u8Tmp < 32; u8Tmp++)
	{
		if(NF_DATA != NF_UNUSED)
		{

///			EIC_IRQCommand(ENABLE);
			//ȥƬѡ	
			GPIO_SetBits(GPIOD,GPIOD_NF_CS);	
			return _READ_ERR_;

		}
	}

///	EIC_IRQCommand(ENABLE);

	//ȥƬѡ	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);	

	return _NO_ERR_;
}

/*************************************************************************************************
* ����������	ҳ������
* ��    �ڣ�	u32Addr,Ҫ��ȡ��������NAND FLASH��ŵĵ�ַ
				pDataArr,��ȡ�����ݱ�����RAM�е���ʼ��ַ
				u32Size,Ҫ��ȡ�������ֽ���Ŀ			
* ��    �ڣ�	_eERRType ���ص�״̬�ֽ� ��0��ʾ�����ɹ���������ʾ����
				pu8Buf,Ҫ���������ݱ���Ļ�������ʼ��ַ
* ˵    ����	ͨ������NAND FLASH�Ľӿڣ�������Ƭ��FLASH����L41��FLASH�ӿھ���һ��
*************************************************************************************************/
_eERRType		NFLASH_ReadArr(u32 u32Addr,u8* pDataArr,u32 u32Size)
{
	_eERRType errtype;
	errtype = NF_RawPageRead(u32Addr, (u16)u32Size, pDataArr);
	return  errtype;
}
 /************************************************************************************************
* ����������	��Ƭ����FLASH,��Ƭ�����ĵ���ʱ��
* ��    �ڣ�	��
* ��    �ڣ�	_eERRType,���ز���������룬ָʾ������ȷ���ߴ���
************************************************************************************************/
_eERRType	NFLASH_EraseChip(void)
{
	  u16 u16Block = 0;
	  for( u16Block = 0; u16Block < _NF_MAX_BLOCK_NUM_; u16Block++)
	  {
	  		//������ǰ�������
	  		NF_BlockErase(u16Block);
	  }
	  return  _NO_ERR_;
	  
}

/**********************************************************************************************
* ����������	ҳд����
* ��    �ڣ�	u32Addr,Ҫд���������NAND FLASH��ŵĵ�ַ
				u16Size,Ҫд��������ֽ���Ŀ	
				pDataArr����д������ݻ������׵�ַ		
* ��    �ڣ�	_eERRType ���ص�״̬�ֽ� ��0��ʾ�����ɹ���������ʾ����
* ˵    ����	����NAND FLASH��ҳд�����ӿں���ʵ�֣���L1�Ľӿں�����������һ��
**********************************************************************************************/
_eERRType		NFLASH_Program(u16 u16Block,u8 u8Page ,u8* pDataArr,u16 u16Size)
{
	_eERRType errtype;
	u16 	u16LastWriteSize = u16Size;
	while(	u16LastWriteSize >= _NF_PAGE_SIZE_)
	{
		errtype = NF_PageWrite(u16Block, u8Page,0,_NF_PAGE_SIZE_, pDataArr);
	 	u16LastWriteSize -= _NF_PAGE_SIZE_;
		u8Page++;
		if(u8Page >= _NF_MAX_PAGE_NUM_)
		{
			u16Block++;
			u8Page = 0;
		}
	}
	if( u16LastWriteSize)
	{
		errtype = NF_PageWrite(u16Block, u8Page,0,_NF_PAGE_SIZE_, pDataArr);
	} 
	return errtype;
}

/***********************************************************************************************
* ����������	NAND FLASH�Ĳ�ͬBLOCK֮�����BLOCK�е�ҳ֮������ݿ�����
* ��    �ڣ�	u32SourceAddress,Ҫ������Դ���ݵ�ַ��	
				u32DestinyAddress��Ҫ������������Ŀ�ĵ�ַ��
				u16Size,Ҫ�����������ֽ���Ŀ��		
* ��    �ڣ�	_eERRType ���ص�״̬�ֽ� ��0��ʾ�����ɹ���������ʾ����
* ˵    ����	���ÿ�������֮ǰ��Ҫȷ��������Ŀ�ĵ�ַ�ǿ����ݣ����򿽱��϶�ʧ��
***********************************************************************************************/
_eERRType	NFLASH_BlockCopy(u32 u32SourceAddress,u32 u32DestinyAddress,u16 u16Size )
{
/*
	_eERRType	err = _NO_ERR_;
	u8	u8Data[_EFLASH_WRITE_SIZE_];
	u8	u8CopyTimes;
	u8	u8LeftCnt;
	u8CopyTimes = u16Size >> 8;
	u8LeftCnt = (u8)u16Size;	
	//��ǰһ�������а�δ�޸ĵ�����д�뵽���������Ӧλ�ô�
	 while(u8CopyTimes > 0)
	 {
	  	
	  	//��Դ��������ȡ����
	  	EFLASH_ReadArr(u32SourceAddress,&u8Data[0],_EFLASH_WRITE_SIZE_);
	  		  		  	
	  	//д��Ŀ������������
	  	EFLASH_Program(u32DestinyAddress,&u8Data[0],_EFLASH_WRITE_SIZE_);
	  	
	  	u32SourceAddress += _EFLASH_WRITE_SIZE_;
	  	u32DestinyAddress += _EFLASH_WRITE_SIZE_;
		u8CopyTimes-- ;
	 }
	if(u8LeftCnt > 0)
	{
		//��Դ��������ȡ����
	  	EFLASH_ReadArr(u32SourceAddress,&u8Data[0],u8LeftCnt);
	  		  		  	
	  	//д��Ŀ������������
	  	EFLASH_Program(u32DestinyAddress,&u8Data[0],u8LeftCnt);	
	}
*/
	return 	_NO_ERR_;
}


/************************************************************************************************
* ����������	���޸Ĳ��ҿ�������
* ��    �ڣ�	u16SourceBlock,��������Դ��������ţ�
				u16DestBlock,����Ŀ����������ţ�
*				u8SourceStartPage,��������Դ��������ʼҳ�ţ�
*				u8DestStartPage,��������Ŀ����������ʼҳ�ţ�
*				u32DestinyAddress��������Ŀ���������׵�ַ��
*				u8PageNum,��������ҳ��Ŀ��
*				pDataArr���޸ĺ�����ݱ�����RAM�е��׵�ַ��	
				u16Offset,���޸ĵ����������ṹ���ƫ�Ƶ�ַ	
*				u16DataSize�����޸ĵ������ֽ���Ŀ��	
* ��    �ڣ�	_eERRType ���ص�״̬�ֽ� ��0��ʾ�����ɹ���������ʾ����
* ˵    ����	����NAND FLASH��д���������ӿں���ʵ�֣���L41�Ľӿں�����������һ��
************************************************************************************************/
_eERRType	NFLASH_BlockCopyChange(u16 u16SourceBlock,u16 u16DestBlock,u8 u8SourceStartPage,
					u8 u8DestStartPage,u8 u8PageNum,u16 u16Offset,u8* pDataArr,u16 u16DataSize)
{

	u16	u16ChangPos;
	u8	u8CurCopyPage = 0;
	u16	u16CurPageStart = 0;
	u8	u8Data[_NF_PAGE_SIZE_];
	
	//�жϲ����Ƿ�Ϸ�
	if(	(u8SourceStartPage + u8PageNum >= _NF_MAX_PAGE_NUM_) ||
		(u8DestStartPage + u8PageNum >= _NF_MAX_PAGE_NUM_) ||
		pDataArr == 0 )
	{
		return _INVALID_PARAM_;
	}
	u16ChangPos = 0;
	u8CurCopyPage = 0;
	u16CurPageStart = 0;
	while( u8PageNum--)
	{
		//�ȶ�ȡ������������
		NF_PageRead(u16SourceBlock,u8SourceStartPage + u8CurCopyPage,0,_NF_PAGE_SIZE_,(u8*)&u8Data);
		
	
		//�ж϶�ȡ�������Ƿ���Ҫ�޸�
		if(((u16CurPageStart + _NF_PAGE_SIZE_) < u16Offset) || (u16CurPageStart > u16Offset ))
		{
			//����Ҫ�޸����ݣ�ֱ��д��Ŀ�Ŀ�ҳ��
			NF_PageWrite(u16DestBlock,u8DestStartPage + u8CurCopyPage,0,_NF_PAGE_SIZE_,(u8*)&u8Data);
		}
		else
		{
			//��Ҫ�޸�����
			//���޸���Ҫ�ı����������
			//ҳ�ڴ��޸ĵ���ʼλ��
			u16ChangPos = u16Offset & 0x3FF;
	  		for(; (u16ChangPos < _NF_PAGE_SIZE_) && (u16DataSize > 0) ;)
	  		{
	  			u8Data[u16ChangPos++] = *pDataArr++;
				//�޸��ֽں��޸�ƫ�Ƶ�ַҲ���Ÿı�
				u16Offset++;
	  			u16DataSize--;  			
	  		}
			//���޸ĺ������д��Ŀ�Ŀ�ҳ��
			NF_PageWrite(u16DestBlock,u8DestStartPage + u8CurCopyPage,0,_NF_PAGE_SIZE_,(u8*)&u8Data);
		}
		u8CurCopyPage++;
				
	}

	return  _NO_ERR_;
} 




