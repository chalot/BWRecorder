/*
 * fram_i2c.c
 *
 * 铁电MB85RC1MT 硬件I2C驱动
 *
 *  Created on: 2014年6月13日
 *      Author: ThinkPad User
 */

#if 0

#include <fram_i2c.h>
#include <stm32f2xx_conf.h>

#if 0
/**
  * @brief  I2C port definitions
  */
#define IOE_I2C                          I2C1
#define IOE_I2C_CLK                      RCC_APB1Periph_I2C1
#define IOE_I2C_SCL_PIN                  GPIO_Pin_6
#define IOE_I2C_SCL_GPIO_PORT            GPIOB
#define IOE_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define IOE_I2C_SCL_SOURCE               GPIO_PinSource6
#define IOE_I2C_SCL_AF                   GPIO_AF_I2C1
#define IOE_I2C_SDA_PIN                  GPIO_Pin_9
#define IOE_I2C_SDA_GPIO_PORT            GPIOB
#define IOE_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define IOE_I2C_SDA_SOURCE               GPIO_PinSource9
#define IOE_I2C_SDA_AF                   GPIO_AF_I2C1
#define IOE_I2C_DR                       ((uint32_t)0x40005410)


/* I2C clock speed configuration (in Hz)
  WARNING:
   Make sure that this define is not already declared in other files (ie.
  stm322xg_eval.h file). It can be used in parallel by other modules. */
#ifndef I2C_SPEED
 #define I2C_SPEED                        100000
#endif /* I2C_SPEED */

/**
  * @brief  IOE DMA definitions
  */
#define IOE_DMA_CLK                      RCC_AHB1Periph_DMA1
#define IOE_DMA_CHANNEL                  DMA_Channel_1
#define IOE_DMA_TX_STREAM                DMA1_Stream6
#define IOE_DMA_RX_STREAM                DMA1_Stream0
#define IOE_DMA_TX_TCFLAG                DMA_FLAG_TCIF6
#define IOE_DMA_RX_TCFLAG                DMA_FLAG_TCIF0


#define TIMEOUT_MAX    0x3000 /*<! The value of the maximal timeout for I2C waiting loops */
/**
  * @}
  */

uint32_t IOE_TimeOut = TIMEOUT_MAX; /*<! Value of Timeout when I2C communication fails */

/**
  * @brief  IOE DMA Direction
  */
typedef enum
{
  IOE_DMA_TX = 0,
  IOE_DMA_RX = 1
}IOE_DMADirection_TypeDef;
#endif

/**
  * @brief  I2C EEPROM Interface pins
  */
#define sEE_I2C                          I2C1
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C1
#define sEE_I2C_SCL_PIN                  GPIO_Pin_6                  /* PB.06 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource6
#define sEE_I2C_SCL_AF                   GPIO_AF_I2C1
#define sEE_I2C_SDA_PIN                  GPIO_Pin_9                  /* PB.09 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource9
#define sEE_I2C_SDA_AF                   GPIO_AF_I2C1

#define sEE_MB85RC1MT
#define sEE_ADDR		0xA0

#define sEE_I2C_DMA                      DMA1
#define sEE_I2C_DMA_CHANNEL              DMA_Channel_1
#define sEE_I2C_DMA_STREAM_TX            DMA1_Stream6
#define sEE_I2C_DMA_STREAM_RX            DMA1_Stream0
#define sEE_I2C_DMA_CLK                  RCC_AHB1Periph_DMA1
#define sEE_I2C_DR_Address               ((uint32_t)0x40005410)
#define sEE_USE_DMA

#define sEE_I2C_DMA_TX_IRQn              DMA1_Stream6_IRQn
#define sEE_I2C_DMA_RX_IRQn              DMA1_Stream0_IRQn
#define sEE_I2C_DMA_TX_IRQHandler        DMA1_Stream6_IRQHandler
#define sEE_I2C_DMA_RX_IRQHandler        DMA1_Stream0_IRQHandler
#define sEE_I2C_DMA_PREPRIO              0
#define sEE_I2C_DMA_SUBPRIO              0

#define sEE_TX_DMA_FLAG_FEIF             DMA_FLAG_FEIF6
#define sEE_TX_DMA_FLAG_DMEIF            DMA_FLAG_DMEIF6
#define sEE_TX_DMA_FLAG_TEIF             DMA_FLAG_TEIF6
#define sEE_TX_DMA_FLAG_HTIF             DMA_FLAG_HTIF6
#define sEE_TX_DMA_FLAG_TCIF             DMA_FLAG_TCIF6
#define sEE_RX_DMA_FLAG_FEIF             DMA_FLAG_FEIF0
#define sEE_RX_DMA_FLAG_DMEIF            DMA_FLAG_DMEIF0
#define sEE_RX_DMA_FLAG_TEIF             DMA_FLAG_TEIF0
#define sEE_RX_DMA_FLAG_HTIF             DMA_FLAG_HTIF0
#define sEE_RX_DMA_FLAG_TCIF             DMA_FLAG_TCIF0

#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1

/* Time constant for the delay caclulation allowing to have a millisecond
   incrementing counter. This value should be equal to (System Clock / 1000).
   ie. if system clock = 120MHz then sEE_TIME_CONST should be 120. */
#define sEE_TIME_CONST                   120

#ifndef I2C_SPEED
 #define I2C_SPEED                        100000
#endif /* I2C_SPEED */

#define sEE_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT         ((uint32_t)(10 * sEE_FLAG_TIMEOUT))

/* Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER     300

/* Defintions for the state of the DMA transfer */
#define sEE_STATE_READY           0
#define sEE_STATE_BUSY            1
#define sEE_STATE_ERROR           2

#define sEE_OK                    0
#define sEE_FAIL                  1

/** @defgroup STM322xG_EVAL_I2C_EE_Private_Variables
  * @{
  */
__IO uint16_t  sEEAddress = 0;
__IO uint32_t  sEETimeout = sEE_LONG_TIMEOUT;
__IO uint16_t* sEEDataReadPointer;
__IO uint8_t*  sEEDataWritePointer;
__IO uint8_t   sEEDataNum;

#if 0
/**
  * @brief  extern function
  */
extern uint8_t IOE_TimeoutUserCallback(void);

static void IOE_TIMEOUT_UserCallback();


/**
  * @brief  Initializes the GPIO pins used by the IO expander.
  * @param  None
  * @retval None
  */
static void IOE_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable IOE_I2C and IOE_I2C_GPIO_PORT & Alternate Function clocks */
  RCC_APB1PeriphClockCmd(IOE_I2C_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(IOE_I2C_SCL_GPIO_CLK | IOE_I2C_SDA_GPIO_CLK
                        , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Reset IOE_I2C IP */
  RCC_APB1PeriphResetCmd(IOE_I2C_CLK, ENABLE);

  /* Release reset signal of IOE_I2C IP */
  RCC_APB1PeriphResetCmd(IOE_I2C_CLK, DISABLE);

  /* IOE_I2C SCL and SDA pins configuration */
  GPIO_InitStructure.GPIO_Pin = IOE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(IOE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = IOE_I2C_SDA_PIN;
  GPIO_Init(IOE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

  GPIO_PinAFConfig(IOE_I2C_SCL_GPIO_PORT, IOE_I2C_SCL_SOURCE, IOE_I2C_SCL_AF);
  GPIO_PinAFConfig(IOE_I2C_SDA_GPIO_PORT, IOE_I2C_SDA_SOURCE, IOE_I2C_SDA_AF);
}

/**
  * @brief  Configure the I2C Peripheral used to communicate with IO_Expanders.
  * @param  None
  * @retval None
  */
static void IOE_I2C_Config(void)
{
  I2C_InitTypeDef I2C_InitStructure;

  /* IOE_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;

  I2C_Init(IOE_I2C, &I2C_InitStructure);
}

/**
  * @brief  Configure the DMA Peripheral used to handle communication via I2C.
  * @param  None
  * @retval None
  */

static void IOE_DMA_Config(IOE_DMADirection_TypeDef Direction, uint8_t* buffer)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(IOE_DMA_CLK, ENABLE);

  /* Initialize the DMA_Channel member */
  DMA_InitStructure.DMA_Channel = IOE_DMA_CHANNEL;

  /* Initialize the DMA_PeripheralBaseAddr member */
  DMA_InitStructure.DMA_PeripheralBaseAddr = IOE_I2C_DR;

  /* Initialize the DMA_Memory0BaseAddr member */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer;

  /* Initialize the DMA_PeripheralInc member */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

  /* Initialize the DMA_MemoryInc member */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

  /* Initialize the DMA_PeripheralDataSize member */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

  /* Initialize the DMA_MemoryDataSize member */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

  /* Initialize the DMA_Mode member */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

  /* Initialize the DMA_Priority member */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;

  /* Initialize the DMA_FIFOMode member */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;

  /* Initialize the DMA_FIFOThreshold member */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

  /* Initialize the DMA_MemoryBurst member */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

  /* Initialize the DMA_PeripheralBurst member */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  /* If using DMA for Reception */
  if (Direction == IOE_DMA_RX)
  {
    /* Initialize the DMA_DIR member */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;

    /* Initialize the DMA_BufferSize member */
    DMA_InitStructure.DMA_BufferSize = 2;

    DMA_DeInit(IOE_DMA_RX_STREAM);

    DMA_Init(IOE_DMA_RX_STREAM, &DMA_InitStructure);
  }
  /* If using DMA for Transmission */
  else if (Direction == IOE_DMA_TX)
  {
    /* Initialize the DMA_DIR member */
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

    /* Initialize the DMA_BufferSize member */
    DMA_InitStructure.DMA_BufferSize = 1;

    DMA_DeInit(IOE_DMA_TX_STREAM);

    DMA_Init(IOE_DMA_TX_STREAM, &DMA_InitStructure);
  }
}
#endif


#if 0
/**
 * 读取一个字节
 *
 * @param ReadAddr		读取地址
 * @param u8Val			字节值
 * @return
 */
uint8_t fram_ReadByte(uint16_t ReadAddr, uint8_t* pu8Val) {
/*Random Read
The one byte of data from the memory address saved in the memory address buffer can be read out
synchronously to SCL by specifying the address in the same way as for a write, and then issuing another
start condition and sending the Device Address Word (R/W “1” input).
Setting value for the most significant address bit in the first and second Device Address Word shall be the
same. The final NACK (SDA is the “H” level) is issued by the receiver that receives the data. In this case,
this bit is issued by the master side.*/

	/*!< While the bus is busy */
	IOE_TimeOut = TIMEOUT_MAX;
	while(I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_BUSY))
	{
	if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*1,发送开始*/
	/*!< Send START condition */
	I2C_GenerateSTART(IOE_TimeOut, ENABLE);

	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*2,发送主机地址*/
	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(IOE_I2C, FRAM_I2C_ADDR, I2C_Direction_Transmitter);

	/*!< Test on EV6 and clear it */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*3,写地址-高字节*/
	/*!< Send the EEPROM's internal address to read from: MSB of the address first */
	I2C_SendData(IOE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));

	/*!< Test on EV8 and clear it */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*4,写地址-低字节*/
	/*!< Send the EEPROM's internal address to read from: MSB of the address first */
	I2C_SendData(IOE_I2C, (uint8_t)(ReadAddr & 0x00FF));

	/*!< Test on EV8 and clear it */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*以上过程完成待读取字节地址的写操作，以下过程完成读字节操作*/

	/*第二次发送起始条件*/
	/*!< Send STRAT condition a second time */
	I2C_GenerateSTART(IOE_I2C, ENABLE);

	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
	if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*发送读命令*/
	/*!< Send EEPROM address for read */
	I2C_Send7bitAddress(IOE_I2C, FRAM_I2C_ADDR, I2C_Direction_Receiver);

	/* If number of data to be read is 1, then DMA couldn't be used */
	/* One Byte Master Reception procedure (POLLING) ---------------------------*/

	/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	///有什么作用？？？？？
	/*!< Disable Acknowledgment */
	I2C_AcknowledgeConfig(IOE_I2C, DISABLE);

	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
	//	    (void)IOE_I2C->SR2;

	/*!< Send STOP Condition */
	I2C_GenerateSTOP(IOE_I2C, ENABLE);

	/*等待数据到达*/
	/* Wait for the byte to be received */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*!< Read the byte received from the EEPROM */
	*pu8Val = I2C_ReceiveData(IOE_I2C);

	I2C_AcknowledgeConfig(IOE_I2C, ENABLE);

	return 0;
}

uint32_t FRAM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead) {
	  /* Set the pointer to the Number of data to be read. This pointer will be used
	      by the DMA Transfer Completer interrupt Handler in order to reset the
	      variable to 0. User should check on this variable in order to know if the
	      DMA transfer has been complete or not. */
static uint16_t*  IOEDataReadPointer = NumByteToRead;

	/*!< While the bus is busy */
	IOE_TimeOut = TIMEOUT_MAX;
	while(I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_BUSY))
	{
	if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*!< Send START condition */
	I2C_GenerateSTART(IOE_I2C, ENABLE);

	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
	if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(IOE_I2C, FRAM_I2C_ADDR, I2C_Direction_Transmitter);

	/*!< Test on EV6 and clear it */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
	if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*3,写地址-高字节*/
	/*!< Send the EEPROM's internal address to read from: MSB of the address first */
	I2C_SendData(IOE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));

	/*!< Test on EV8 and clear it */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*4,写地址-低字节*/
	/*!< Send the EEPROM's internal address to read from: MSB of the address first */
	I2C_SendData(IOE_I2C, (uint8_t)(ReadAddr & 0x00FF));

	/*!< Test on EV8 and clear it */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*!< Send STRAT condition a second time */
	I2C_GenerateSTART(IOE_I2C, ENABLE);

	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	IOE_TimeOut = TIMEOUT_MAX;
	while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
	if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
	}

	/*!< Send EEPROM address for read */
	I2C_Send7bitAddress(IOE_I2C, FRAM_I2C_ADDR, I2C_Direction_Receiver);

	/* If number of data to be read is 1, then DMA couldn't be used */
	/* One Byte Master Reception procedure (POLLING) ---------------------------*/
	if ((uint16_t)(*NumByteToRead) < 2)
	{
		/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
		IOE_TimeOut = TIMEOUT_MAX;
		while(I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_ADDR) == RESET)
		{
		  if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
		}

		/*!< Disable Acknowledgment */
		I2C_AcknowledgeConfig(IOE_I2C, DISABLE);

		/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
		(void)IOE_I2C->SR2;

		/*!< Send STOP Condition */
		I2C_GenerateSTOP(IOE_I2C, ENABLE);

		/* Wait for the byte to be received */
		IOE_TimeOut = TIMEOUT_MAX;
		while(I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_RXNE) == RESET)
		{
		  if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
		}

		/*!< Read the byte received from the EEPROM */
		*pBuffer = I2C_ReceiveData(IOE_I2C);

		/*!< Decrement the read bytes counter */
		(uint16_t)(*NumByteToRead)--;

		/* Wait to make sure that STOP control bit has been cleared */
		IOE_TimeOut = TIMEOUT_MAX;
		while(IOE_I2C->CR1 & I2C_CR1_STOP)
		{
		  if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
		}

		/*!< Re-Enable Acknowledgment to be ready for another reception */
		I2C_AcknowledgeConfig(IOE_I2C, ENABLE);
	  }
	  else/* More than one Byte Master Reception procedure (DMA) -----------------*/
	  {
		/*!< Test on EV6 and clear it */
		IOE_TimeOut = TIMEOUT_MAX;
		while(!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
		  if((IOE_TimeOut--) == 0) return IOE_TIMEOUT_UserCallback();
		}

		/* Configure the DMA Rx Channel with the buffer address and the buffer size */
		IOE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint16_t)(*NumByteToRead), IOE_DIRECTION_RX);
		IOE_DMA_Config();


		/* Inform the DMA that the next End Of Transfer Signal will be the last one */
		I2C_DMALastTransferCmd(IOE_I2C, ENABLE);

		/* Enable the DMA Rx Stream */
		DMA_Cmd(IOE_I2C_DMA_STREAM_RX, ENABLE);
	  }

	  /* If all operations OK, return IOE_OK (0) */
	  return IOE_OK;
	}



}


/**
 *
 */
void IOE_TIMEOUT_UserCallback() {


}
#endif


/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_DeInit(void)
{
  sEE_LowLevel_DeInit();
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_Init(void)
{
	I2C_InitTypeDef  I2C_InitStructure;

	sEE_LowLevel_Init();

	/*!< I2C configuration */
	/* sEE_I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = sEE_ADDR;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;

	/* sEE_I2C Peripheral Enable */
	I2C_Cmd(sEE_I2C, ENABLE);
	/* Apply sEE_I2C configuration after enabling it */
	I2C_Init(sEE_I2C, &I2C_InitStructure);

	/* Enable the sEE_I2C peripheral DMA requests */
	I2C_DMACmd(sEE_I2C, ENABLE);

	sEEAddress = 0xA0;
}

/**
 * 读指定字节数据
 *
 * @param pBuffer			接收缓区
 * @param ReadAddr			待读取地址
 * @param NumByteToRead		待读取字节数
 * @return
 */
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{
  /* Set the pointer to the Number of data to be read. This pointer will be used
      by the DMA Transfer Completer interrupt Handler in order to reset the
      variable to 0. User should check on this variable in order to know if the
      DMA transfer has been complete or not. */
  sEEDataReadPointer = NumByteToRead;

  /*!< While the bus is busy */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*!< Send START condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);

  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*!< Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));

  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*!< Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(ReadAddr & 0x00FF));

  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*!< Send STRAT condition a second time */
  I2C_GenerateSTART(sEE_I2C, ENABLE);

  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*!< Send EEPROM address for read */
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Receiver);

  /* If number of data to be read is 1, then DMA couldn't be used */
  /* One Byte Master Reception procedure (POLLING) ---------------------------*/
  if ((uint16_t)(*NumByteToRead) < 2)
  {
    /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_ADDR) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }

    /*!< Disable Acknowledgment */
    I2C_AcknowledgeConfig(sEE_I2C, DISABLE);

    /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
    (void)sEE_I2C->SR2;

    /*!< Send STOP Condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);

    /* Wait for the byte to be received */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_RXNE) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }

    /*!< Read the byte received from the EEPROM */
    *pBuffer = I2C_ReceiveData(sEE_I2C);

    /*!< Decrement the read bytes counter */
    (uint16_t)(*NumByteToRead)--;

    /* Wait to make sure that STOP control bit has been cleared */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(sEE_I2C->CR1 & I2C_CR1_STOP)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }

    /*!< Re-Enable Acknowledgment to be ready for another reception */
    I2C_AcknowledgeConfig(sEE_I2C, ENABLE);
  }
  else/* More than one Byte Master Reception procedure (DMA) -----------------*/
  {
    /*!< Test on EV6 and clear it */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }

    /* Configure the DMA Rx Channel with the buffer address and the buffer size */
    sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint16_t)(*NumByteToRead), sEE_DIRECTION_RX);

    /* Inform the DMA that the next End Of Transfer Signal will be the last one */
    I2C_DMALastTransferCmd(sEE_I2C, ENABLE);

    /* Enable the DMA Rx Stream */
    DMA_Cmd(sEE_I2C_DMA_STREAM_RX, ENABLE);
  }

  /* If all operations OK, return sEE_OK (0) */
  return sEE_OK;
}


/**
 * 写缓区数据到铁电
 *
 * @param pBuffer				数据缓区
 * @param WriteAddr				写入位置
 * @param NumByteToWrite		待写字节数
 */
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
/*
	If additional 8 bits are continuously sent after the same command (except stop condition) as Byte Write, a
	page write is performed. The memory address rolls over to first memory address (0 0000H) at the end of the
	address. Therefore, if more than 128 Kbytes are sent, the data is overwritten in order starting from the start
	of the memory address that was written first. Because FRAM performs the high-speed write operations, the
	data will be written to FRAM right after the ACK response finished.
*/

	/*!< While the bus is busy */
	sEETimeout = sEE_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
	{
		if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
	}

	/*!< Send START condition */
	I2C_GenerateSTART(sEE_I2C, ENABLE);

	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	sEETimeout = sEE_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
	}

	/*!< Send EEPROM address for write */
	I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);

	/*!< Test on EV6 and clear it */
	sEETimeout = sEE_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
	}

	/*!< Send the EEPROM's internal address to read from: MSB of the address first */
	I2C_SendData(sEE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));

	/*!< Test on EV8 and clear it */
	sEETimeout = sEE_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
	}

	/*!< Send the EEPROM's internal address to read from: LSB of the address */
	I2C_SendData(sEE_I2C, (uint8_t)(ReadAddr & 0x00FF));

	/*!< Test on EV8 and clear it */
	sEETimeout = sEE_FLAG_TIMEOUT;
	while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF) == RESET)
	{
		if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
	}












  uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % sEE_PAGESIZE;
  count = sEE_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
  NumOfSingle = NumByteToWrite % sEE_PAGESIZE;

  /*!< If WriteAddr is sEE_PAGESIZE aligned  */
  if(Addr == 0)
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage == 0)
    {
      /* Store the number of data to be written */
      sEEDataNum = NumOfSingle;
      /* Start writing data */
      sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
      /* Wait transfer through DMA to be complete */
      sEETimeout = sEE_LONG_TIMEOUT;
      while (sEEDataNum > 0)
      {
        if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
      }
      sEE_WaitEepromStandbyState();
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else
    {
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }
        sEE_WaitEepromStandbyState();
      }
    }
  }
  /*!< If WriteAddr is not sEE_PAGESIZE aligned  */
  else
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage== 0)
    {
      /*!< If the number of data to be written is more than the remaining space
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        sEEDataNum = count;
        /*!< Write the data contained in same page */
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }
        sEE_WaitEepromStandbyState();

        /* Store the number of data to be written */
        sEEDataNum = (NumByteToWrite - count);
        /*!< Write the remaining data in the following page */
        sEE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }
        sEE_WaitEepromStandbyState();
      }
      else
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }
        sEE_WaitEepromStandbyState();
      }
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
      NumOfSingle = NumByteToWrite % sEE_PAGESIZE;

      if(count != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = count;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }
        sEE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      }

      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }
      if(NumOfSingle != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }
        sEE_WaitEepromStandbyState();
      }
    }
  }
}


/**
  * @brief  This function handles the DMA Tx Channel interrupt Handler.
  * @param  None
  * @retval None
  */
void sEE_I2C_DMA_TX_IRQHandler(void)
{
  /* Check if the DMA transfer is complete */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_TCIF) != RESET)
  {
    /* Disable the DMA Tx Stream and Clear TC flag */
    DMA_Cmd(sEE_I2C_DMA_STREAM_TX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_TCIF);

    /*!< Wait till all data have been physically transferred on the bus */
    sEETimeout = sEE_LONG_TIMEOUT;
    while(!I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF))
    {
      if((sEETimeout--) == 0) sEE_TIMEOUT_UserCallback();
    }

    /*!< Send STOP condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);

    /* Reset the variable holding the number of data to be written */
    *sEEDataWritePointer = 0;
  }
}

/**
  * @brief  This function handles the DMA Rx Channel interrupt Handler.
  * @param  None
  * @retval None
  */
void sEE_I2C_DMA_RX_IRQHandler(void)
{
  /* Check if the DMA transfer is complete */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_STREAM_RX, sEE_RX_DMA_FLAG_TCIF) != RESET)
  {
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);

    /* Disable the DMA Rx Stream and Clear TC Flag */
    DMA_Cmd(sEE_I2C_DMA_STREAM_RX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_STREAM_RX, sEE_RX_DMA_FLAG_TCIF);

    /* Reset the variable holding the number of data to be read */
    *sEEDataReadPointer = 0;
  }
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
  while (1)
  {
  }
}
#endif
