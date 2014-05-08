/***********************************************************************************************************
* 文    件：	NANDFLASH.C
* 描    述：	片外FLASH操作接口及应用处理
* 修改记录：	1)	日  期：2009-01-19
*					版  本：V1.0
*					创  建：
*               2)  日  期：
*					版  本：
*					修改者：
*					问  题：
*					修  改：
***********************************************************************************************************/

#include "NandFlash.h"
#include "parameter.h"
#include "utility.h"
#include "board.h"
//#include	"periph.h"

//extern	BARRIER			tBarrier;									//电子栅栏
//extern	MSGTEMPLATE		tMsgTmpl;									//短信模板
//
////程序文件读写控制
//_tEFLASH_DBAK	tUpdProgram;

/***********************************************************************************************************
 ┌───────┬────────────────────────────────────┐
 │ 管脚名称     │管脚功能												                   │
 ├───────┼────────────────────────────────────┤
 │IO0~IO7       │IO管脚是用来输入命令、地址和数据的，也可以在读时候输出数据              │
 ├───────┼────────────────────────────────────┤
 │ CLE          │CLE是命令锁存使能，高有效，在WE腿上升沿把数据通过IO口写入到命令寄存器中 │
 ├───────┼────────────────────────────────────┤
 │ ALE          │ALE是地址锁存使能，当ALE为高时，在WE腿上升沿锁存                        │ 
 ├───────┼────────────────────────────────────┤
 │ CE           │片选使能，低有效                                                        │
 ├───────┼────────────────────────────────────┤
 │ RE           │读使能，低有效	                                                       │
 ├───────┼────────────────────────────────────┤
 │ WE           │写使能，低有效                                                          │
 ├───────┼────────────────────────────────────┤
 │ WP           │写保护，低有效                                                          │
 ├───────┼────────────────────────────────────┤
 │ R/B          │就绪/忙输出，低时，表示在进行编程、擦除或者随机读取操作，高时表示操作完 │
 ├───────┼────────────────────────────────────┤
 │ Vcc          │供电												                       │
 ├───────┼────────────────────────────────────┤
 │ Vss          │低 												                       │ 
 ├───────┼────────────────────────────────────┤
 │ NC           │悬空												                       │
 └───────┴────────────────────────────────────┘


 ┌───────┬───────────┬─────────┐
 │ 地址/偏移    │       大小           │        内容      │
 ├───────┼───────────┼─────────┤
 │  0x000000    │       256K           │    升级程序      │
 ├───────┼───────────┼─────────┤
 │  0x040000    │      4K              │   配置参数       │
 │  0x040000    │      256字节         │   设备参数       │
 │  0x040100    │      256字节         │   工作参数       │
 │  0x040200    │      256字节         │   通讯参数       │
 │  0x040300    │      256字节         │   统计参数       │
 │  0x040400    │      256字节         │   运行参数       │
 ├───────┼───────────┼─────────┤
 │  0x04100     │      4K              │   电子栅栏       │
 └───────┴───────────┴─────────┘
*************************************************写参数、写栅栏参数的设计考虑*****************************************
1.NAND FLASH的操作特点考虑：
1)芯片采用的是K9F1G08U0B，芯片大小为128MB，分为1024个BLOCK(块)，每块大小为128KB。每块分成64页，每页大小为2KB。
2)芯片的擦除操作是以块为单位的。
3)芯片的写入操作以页为单位。
4)NAND FLASH擦除也有次数限制，尽量少擦除。

2.存储数据结构特点：
1)存储的参数主要有：配置参数(包括设备参数、工作参数、通讯参数、统计参数、运行参数等)、点位信息等。
2)配置参数需要在程序运行过程中作修改。

所以我们根据参数大小的考虑，为参数信息分配两个BLOCK，共256K字节大小(其实，大部分的空间都是闲置的)。
BLOCK1为数据缓冲区，BLOCK2为备份缓冲区。
把一个BLOCK(128K)分成8个子块，每块大小为16K(实际中也可以考虑分为16个子块，每个子块大小8K)。
1)	每个子块前0x100个字节保留，在其中前8个字节位置存储块是否写入标志，如果写入配置参数了，则前8个字节信息为0x12345678和0x55AAAA55。
第一次的配置参数首先写入子块位置0，下一次要修改某写数据的话，则把子块0中未修改的数据读出来，写入到子块1中，要修改的部分数据
修改后，写入到子块1中，写入完成，设置子块1占用标志，依次类推。
2)	当所有的子块都被占用后，则在BLOCK2中查找一个空闲的子块，然后把BLOCK1中最后一个子块的数据拷贝到BLOCK2中，修改的数据也拷贝到BLOCK2
中的空闲子块处，然后擦除BLOCK1，再把BLOCK2中写入的数据写回到BLOCK1的第一个子块位置，然后设置写完成标志。
3)	当BLOCK2中的空闲子块也都被占用后，则首先擦除BLOCK2，然后再进行2)操作。

**************************************************写参数、写栅栏函数的设计考虑**********************************/


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
	
	//DO-D7 RD WR ALE CLE等IO口已经在LED初始化中配置
	//NFLASH_LowLevel_Init只需要配置FLASH单独使用的IO口
	
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
* 功能描述：	读取NAND FLASH 的ID
* 入    口：	无			
* 出    口：	pu8Buf,返回的ID信息的起始地址，返回的ID信息包含四个字节，含义为
				第一字节		制造商代码
				第二字节		设备代码
				第三字节		无关紧要
				第四字节		包含页大小，块大小，额外空间大小以及获取时间等信息
				其未定义为  
				Bit 1与Bit 0   控制页大小
				Bit  1    0  	0    0  表示页大小为1K
				                0    1  表示页大小为2K
				                1    0  保留
				                1    1  保留
				Bit 5与Bit 4控制块大小
				Bit  5    4     0    0  表示块大小为64K
								0    1  表示块大小为128K
								1    0  表示块大小为256K
								1	 1  保留
			    Bit 2控制冗余区域大小(每512字节)
			    Bit 2           0  	     冗余区域大小为8字节
			    				1		 冗余区域大小为16字节
			    
			    Bit 6控制总线宽度
			    Bit 6			0        8位
			    				1		 16位
			    Bit 7 与Bit 3表示串行获取时间
*************************************************************************************************/
u8	NandFlashReadId(u8 *pu8Buf)
{
   u8 i = 0;
	//片选							 
//	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
//	while(i++ < 10)
	NF_CMD = NF_READID;
	
	NF_ADDR = 0x00;


	*pu8Buf = NF_DATA;
	*(pu8Buf + 1) = NF_DATA;
	*(pu8Buf + 2) = NF_DATA;
	*(pu8Buf + 3) = NF_DATA;
	*(pu8Buf + 4) = NF_DATA;	
	//去片选	
//	GPIO_SetBits(GPIOD,GPIOD_NF_CS);

	return TRUE;
}


/*************************************************************************************************
* 功能描述：	读状态，返回操作的状态信息
* 入    口：	无			
* 出    口：	返回的状态字节 D[0]=1擦写失败、D[1]=1缓冲区写失败、D[6]=1不忙、D[7]=1没有写保护
*************************************************************************************************/
u8	NandFlashStatus(void)
{
	u8	u8status;

	//片选
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);
		
	NF_CMD = NF_STATUS;
	
	u8status = NF_DATA;

	//去片选	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);
		
	return u8status;
}


u32 NF_GetAddrByBlockPageAndOffset(u16 u16Block,u8 u8Page,u16 u16Offset)
{
	u32 u32Addr = 0;

	//根据块号、页号、页内偏移地址计算出要读取的数据地址
	u32Addr =
		(((u32)u16Block) << _NF_BLOCK_OFFSET_) | (((u32)u8Page) << _NF_PAGE_OFFSET_) | u16Offset;

	return  u32Addr;
}

/************************************************************************************************
* 功能描述：	页读操作
* 入    口：	u16Block,要读取的页块号
				u8Page,要读取的页页号
				u16Offset，页内偏移地址
				u16Size，要读取的数据长度、
				pData[OUT]，读取得数据在RAM中保存的起始地址				
* 出    口：	_eERRType 返回的状态字节 ，0表示操作成功，其他表示错误
* 说    明：	页读操作，首先写入命令字0x00,然后写入32位的地址，再写入命令字0x30,等待操作完成Tr(小于25us)
				后(通过读取R/B管脚状态来确定是否操作完成)，再从总线上读取数据
*************************************************************************************************/
_eERRType		NF_PageRead(u16 u16Block,u8 u8Page,u16 u16Offset,u16 u16Size,u8* pData)
{
	u32 u32Addr = 0;
	//判断参数是否有效
	if(pData == 0 || u16Block >= _NF_MAX_BLOCK_NUM_ || u8Page >= _NF_MAX_PAGE_NUM_ ||
		u16Offset > _NF_PAGE_SIZE_	|| u16Size > _NF_PAGE_SIZE_)
	{
		return _INVALID_PARAM_;
	}	
	//根据块号、页号、页内偏移地址计算出要读取的数据地址
	u32Addr =
		(((u32)u16Block) << _NF_BLOCK_OFFSET_) | (((u32)u8Page) << _NF_PAGE_OFFSET_) | u16Offset;

	return NF_RawPageRead(u32Addr, u16Size, pData)	;

}

 /************************************************************************************************
* 功能描述：	页写操作
* 入    口：	u32Addr,要写入的数据在NAND FLASH存放的地址
				u16Length,要写入的数据字节数目	
				pu8Buf，待写入的数据缓冲区首地址		
* 出    口：	_eERRType 返回的状态字节 ，0表示操作成功，其他表示错误
* 说    明：	页写入操作，首先写入命令字0x80,然后写入32位的地址，再写入待写入的数据
				再写入命令字0x10,然后检查R/B引脚状态，判断操作是否完成，然后读取状态字节
				判断此操作是否成功
*************************************************************************************************/
_eERRType		NF_PageWrite(u16 u16Block,u8 u8Page,u16 u16Offset,u16 u16Size,u8* pData)
{
	u32 u32Addr = 0;
	//判断参数是否有效
	if(pData == 0 || u16Block >= _NF_MAX_BLOCK_NUM_ || u8Page >= _NF_MAX_PAGE_NUM_ ||
		u16Offset > _NF_PAGE_SIZE_	|| u16Size > _NF_PAGE_SIZE_)
	{
		return _INVALID_PARAM_;
	}	
	//根据块号、页号、页内偏移地址计算出要读取的数据地址
	u32Addr =
		(((u32)u16Block) << _NF_BLOCK_OFFSET_) | (((u32)u8Page) << _NF_PAGE_OFFSET_) | u16Offset;

	return NF_RawPageWrite(u32Addr, u16Size, pData);
}

/*************************************************************************************************
* 功能描述：	页读操作
* 入    口：	u32Addr,要读取的数据在NAND FLASH存放的地址
				u16Length,要读取的数据字节数目			
* 出    口：	_eERRType 返回的状态字节 ，0表示操作成功，其他表示错误
				pu8Buf,要读出的数据保存的缓冲区起始地址
* 说    明：	页读操作，首先写入命令字0x00,然后写入32位的地址，再写入命令字0x30,等待操作完成Tr(小于25us)
				后(通过读取R/B管脚状态来确定是否操作完成)，再从总线上读取数据
*************************************************************************************************/
_eERRType	NF_RawPageRead(u32 u32Addr, u16 u16Length, u8 *pu8Buf)
{
	u8	u8Tmp = 0;
	
	//参数错误
	if(u16Length == 0 || pu8Buf == 0 )
	{
		return _INVALID_PARAM_;
	}
	//地址越界
	if(u32Addr >= _NF_MAX_ADDR_)
	{
		return _INVALID_ADDR_;
	}
	
	for(u8Tmp = 0; u8Tmp < 25; u8Tmp++)													
	{
		// 读取状态标志，不忙则开始操作，否则等待
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

	//片选
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);

	//EIC_IRQCommand(DISABLE);		
	// 页读操作
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
		//判断R/B管脚是否提示操作完成
		if(GPIO_ReadInputDataBit(GPIOD,GPIOD_NF_RB))
		{
			break;
		}
		
		Delay_1us(2);
	}

	if(u8Tmp >= 25)
	{
///		EIC_IRQCommand(ENABLE);
		//去片选	
		GPIO_SetBits(GPIOD,GPIOD_NF_CS);
		return _READ_ERR_;
	}
	while(u16Length-- > 0)
		*(pu8Buf++) = NF_DATA;

///   	EIC_IRQCommand(ENABLE);

	//去片选	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);
		
	return _NO_ERR_;
}


/***********************************************************************************************
* 功能描述：	页写操作
* 入    口：	u32Addr,要写入的数据在NAND FLASH存放的地址
				u16Length,要写入的数据字节数目	
				pu8Buf，待写入的数据缓冲区首地址		
* 出    口：	_eERRType 返回的状态字节 ，0表示操作成功，其他表示错误
* 说    明：	页写入操作，首先写入命令字0x80,然后写入32位的地址，再写入待写入的数据
				再写入命令字0x10,然后检查R/B引脚状态，判断操作是否完成，然后读取状态字节
				判断此操作是否成功
*************************************************************************************************/
_eERRType	NF_RawPageWrite(u32 u32Addr, u16 u16Length, u8 *pu8Buf)
{
	u8	u8Tmp = 0;
	u16	u16Index = 0;
	//参数错误
	if(u16Length == 0 || pu8Buf == 0 )
	{
		return _INVALID_PARAM_;
	}
	//地址越界
	if(u32Addr >= _NF_MAX_ADDR_)
	{
		return _INVALID_ADDR_;
	}
	
	for(u8Tmp = 0; u8Tmp < 25; u8Tmp++)													
	{
		// 读取状态标志，允许则开始操作，否则等待
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
	//片选
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);

///	EIC_IRQCommand(DISABLE);

	// 页写操作
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

	// 最多等待5ms
	for(u8Tmp = 0;u8Tmp < 5;u8Tmp ++)														
	{
		//判断R/B管脚是否提示操作完成
		if(GPIO_ReadInputDataBit(GPIOD,GPIOD_NF_RB))
		{
			break;
		}
		
		Delay_1ms(1);
	}

	//去片选	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);	

	if(u8Tmp > 4)
		return _WRITE_ERR_;
		
	if((NandFlashStatus() & _NF_STATUS_ERASE_FAILURE_) == _NF_STATUS_ERASE_FAILURE_)
		return _WRITE_ERR_;

	//片选
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);

///	EIC_IRQCommand(DISABLE);
	// 页读操作
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
		//判断R/B管脚是否提示操作完成
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
			//去片选	
			GPIO_SetBits(GPIOD,GPIOD_NF_CS);	
			return _READ_ERR_;
		}
	}
///	EIC_IRQCommand(ENABLE);
	//去片选	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);	

	return _NO_ERR_;
}

/*************************************************************************************************
* 功能描述：	块擦除操作
* 入    口：	u32Addr,要擦除的块地址		
* 出    口：	_eERRType 返回的状态字节 ，0表示操作成功，其他表示错误
* 说    明：	块擦除操作只需要写入列地址，行地址不需要
*************************************************************************************************/
_eERRType	NF_BlockErase(u16 u16Block)
{
	u8	u8Tmp = 0;

 	u32  u32Addr = ((u32)u16Block) << _NF_BLOCK_OFFSET_;

  	//地址越界
	if(u16Block >= _NF_MAX_BLOCK_NUM_)
	{
		return _INVALID_ADDR_;
	}

	for(u8Tmp = 0; u8Tmp < 5; u8Tmp++)													
	{
		// 读取状态标志，允许则开始操作，否则等待
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

	//片选
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);

///	EIC_IRQCommand(DISABLE);
	
	// 块擦除操作	
	NF_CMD = NF_BLOCKERASE1;
												
	NF_ADDR = (u32Addr>>16) &0x000000FF;
	NF_ADDR = (u32Addr>>24) &0x000000FF;
	
	NF_CMD = NF_BLOCKERASE2;

///	EIC_IRQCommand(ENABLE);

	// 最多等待5ms
	for(u8Tmp = 0;u8Tmp < 5;u8Tmp ++)														
	{
		//判断R/B管脚是否提示操作完成
		if(GPIO_ReadInputDataBit(GPIOD,GPIOD_NF_RB))
		{
			break;
		}
		
		Delay_1ms(1);
	}

	//去片选	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);	
		
	if(u8Tmp > 4)
	{
		return _WRITE_ERR_;
	}
		
	if((NandFlashStatus() & _NF_STATUS_ERASE_FAILURE_) == _NF_STATUS_ERASE_FAILURE_)
	{
		return _WRITE_ERR_;
	}


	//片选
	GPIO_ResetBits(GPIOD,GPIOD_NF_CS);
	
	Delay_1us(1);
///	EIC_IRQCommand(DISABLE);
		
	// 页读操作
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
		//判断R/B管脚是否提示操作完成
		if(GPIO_ReadInputDataBit(GPIOD,GPIOD_NF_RB))
		{
			break;
		}
		
		Delay_1us(1);
	}

	if(u8Tmp >= 25)
	{
///		EIC_IRQCommand(ENABLE);
		//去片选	
		GPIO_SetBits(GPIOD,GPIOD_NF_CS);	
		return _READ_ERR_;
	}
///	EIC_IRQCommand(DISABLE);
	
	for(u8Tmp = 0; u8Tmp < 32; u8Tmp++)
	{
		if(NF_DATA != NF_UNUSED)
		{

///			EIC_IRQCommand(ENABLE);
			//去片选	
			GPIO_SetBits(GPIOD,GPIOD_NF_CS);	
			return _READ_ERR_;

		}
	}

///	EIC_IRQCommand(ENABLE);

	//去片选	
	GPIO_SetBits(GPIOD,GPIOD_NF_CS);	

	return _NO_ERR_;
}

/*************************************************************************************************
* 功能描述：	页读操作
* 入    口：	u32Addr,要读取的数据在NAND FLASH存放的地址
				pDataArr,读取的数据保持在RAM中的起始地址
				u32Size,要读取的数据字节数目			
* 出    口：	_eERRType 返回的状态字节 ，0表示操作成功，其他表示错误
				pu8Buf,要读出的数据保存的缓冲区起始地址
* 说    明：	通过调用NAND FLASH的接口，来操作片外FLASH，与L41的FLASH接口尽量一致
*************************************************************************************************/
_eERRType		NFLASH_ReadArr(u32 u32Addr,u8* pDataArr,u32 u32Size)
{
	_eERRType errtype;
	errtype = NF_RawPageRead(u32Addr, (u16)u32Size, pDataArr);
	return  errtype;
}
 /************************************************************************************************
* 功能描述：	整片擦除FLASH,整片擦除的典型时间
* 入    口：	无
* 出    口：	_eERRType,返回操作错误代码，指示擦除正确或者错误
************************************************************************************************/
_eERRType	NFLASH_EraseChip(void)
{
	  u16 u16Block = 0;
	  for( u16Block = 0; u16Block < _NF_MAX_BLOCK_NUM_; u16Block++)
	  {
	  		//擦除当前块的内容
	  		NF_BlockErase(u16Block);
	  }
	  return  _NO_ERR_;
	  
}

/**********************************************************************************************
* 功能描述：	页写操作
* 入    口：	u32Addr,要写入的数据在NAND FLASH存放的地址
				u16Size,要写入的数据字节数目	
				pDataArr，待写入的数据缓冲区首地址		
* 出    口：	_eERRType 返回的状态字节 ，0表示操作成功，其他表示错误
* 说    明：	调用NAND FLASH的页写操作接口函数实现，与L1的接口函数尽量保持一致
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
* 功能描述：	NAND FLASH的不同BLOCK之间或者BLOCK中的页之间的数据拷贝，
* 入    口：	u32SourceAddress,要拷贝的源数据地址；	
				u32DestinyAddress，要拷贝到的数据目的地址；
				u16Size,要拷贝的数据字节数目。		
* 出    口：	_eERRType 返回的状态字节 ，0表示操作成功，其他表示错误
* 说    明：	调用拷贝函数之前，要确保拷贝的目的地址是空数据，否则拷贝肯定失败
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
	//从前一个区块中把未修改的数据写入到新区块的相应位置处
	 while(u8CopyTimes > 0)
	 {
	  	
	  	//从源数据区读取数据
	  	EFLASH_ReadArr(u32SourceAddress,&u8Data[0],_EFLASH_WRITE_SIZE_);
	  		  		  	
	  	//写入目的数据区保存
	  	EFLASH_Program(u32DestinyAddress,&u8Data[0],_EFLASH_WRITE_SIZE_);
	  	
	  	u32SourceAddress += _EFLASH_WRITE_SIZE_;
	  	u32DestinyAddress += _EFLASH_WRITE_SIZE_;
		u8CopyTimes-- ;
	 }
	if(u8LeftCnt > 0)
	{
		//从源数据区读取数据
	  	EFLASH_ReadArr(u32SourceAddress,&u8Data[0],u8LeftCnt);
	  		  		  	
	  	//写入目的数据区保存
	  	EFLASH_Program(u32DestinyAddress,&u8Data[0],u8LeftCnt);	
	}
*/
	return 	_NO_ERR_;
}


/************************************************************************************************
* 功能描述：	块修改并且拷贝函数
* 入    口：	u16SourceBlock,待拷贝的源数据区块号；
				u16DestBlock,拷贝目的数据区块号；
*				u8SourceStartPage,待拷贝的源数据区起始页号；
*				u8DestStartPage,待拷贝的目的数据区起始页号；
*				u32DestinyAddress，拷贝的目的数据区首地址；
*				u8PageNum,待拷贝的页数目；
*				pDataArr，修改后的数据保存在RAM中的首地址；	
				u16Offset,待修改的数据相对与结构体的偏移地址	
*				u16DataSize，待修改的数据字节数目。	
* 出    口：	_eERRType 返回的状态字节 ，0表示操作成功，其他表示错误
* 说    明：	调用NAND FLASH的写参数操作接口函数实现，与L41的接口函数尽量保持一致
************************************************************************************************/
_eERRType	NFLASH_BlockCopyChange(u16 u16SourceBlock,u16 u16DestBlock,u8 u8SourceStartPage,
					u8 u8DestStartPage,u8 u8PageNum,u16 u16Offset,u8* pDataArr,u16 u16DataSize)
{

	u16	u16ChangPos;
	u8	u8CurCopyPage = 0;
	u16	u16CurPageStart = 0;
	u8	u8Data[_NF_PAGE_SIZE_];
	
	//判断参数是否合法
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
		//先读取待拷贝的数据
		NF_PageRead(u16SourceBlock,u8SourceStartPage + u8CurCopyPage,0,_NF_PAGE_SIZE_,(u8*)&u8Data);
		
	
		//判断读取的数据是否需要修改
		if(((u16CurPageStart + _NF_PAGE_SIZE_) < u16Offset) || (u16CurPageStart > u16Offset ))
		{
			//不需要修改数据，直接写到目的块页处
			NF_PageWrite(u16DestBlock,u8DestStartPage + u8CurCopyPage,0,_NF_PAGE_SIZE_,(u8*)&u8Data);
		}
		else
		{
			//需要修改数据
			//先修改需要改变的数据内容
			//页内待修改的起始位置
			u16ChangPos = u16Offset & 0x3FF;
	  		for(; (u16ChangPos < _NF_PAGE_SIZE_) && (u16DataSize > 0) ;)
	  		{
	  			u8Data[u16ChangPos++] = *pDataArr++;
				//修改字节后，修改偏移地址也跟着改变
				u16Offset++;
	  			u16DataSize--;  			
	  		}
			//把修改后的数据写到目的块页处
			NF_PageWrite(u16DestBlock,u8DestStartPage + u8CurCopyPage,0,_NF_PAGE_SIZE_,(u8*)&u8Data);
		}
		u8CurCopyPage++;
				
	}

	return  _NO_ERR_;
} 




