#ifndef __VS10XX_H__
#define __VS10XX_H__

#include <stm32f2xx_conf.h>

//////////////////////////////////////////////////////////////
//���ⲿ�Ľӿ�
//#define VS_DQ       PCin(13)  	//DREQ
//#define VS_RST      PEout(6) 	//RST
//#define VS_XCS      PFout(7)  	//XCS
//#define VS_XDCS     PFout(6)  	//XDCS

//////////////////////////////////////////////////////////////

typedef struct {
	u8 mvol;		//������,��Χ:0~254
	u8 bflimit;		//����Ƶ���޶�,��Χ:2~15(��λ:10Hz)
	u8 bass;		//����,��Χ:0~15.0��ʾ�ر�.(��λ:1dB)
	u8 tflimit;		//����Ƶ���޶�,��Χ:1~15(��λ:Khz)
	u8 treble;		//����,��Χ:0~15(��λ:1.5dB)(ԭ����Χ��:-8~7,ͨ�������޸���);
	u8 effect;		//�ռ�Ч������.0,�ر�;1,��С;2,�е�;3,���.

	u8 saveflag; 	//�����־,0X0A,�������;����,����δ����	   
}__attribute__((packed)) _vs10xx_obj;

extern _vs10xx_obj vsset;		//VS10XX����

#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03
//VS10XX�Ĵ�������
#define SPI_MODE        	0x00   
#define SPI_STATUS      	0x01   
#define SPI_BASS        	0x02   
#define SPI_CLOCKF      	0x03   
#define SPI_DECODE_TIME 	0x04   
#define SPI_AUDATA      	0x05   
#define SPI_WRAM        	0x06   
#define SPI_WRAMADDR    	0x07   
#define SPI_HDAT0       	0x08   
#define SPI_HDAT1       	0x09 

#define SPI_AIADDR      	0x0a   
#define SPI_VOL         	0x0b   
#define SPI_AICTRL0     	0x0c   
#define SPI_AICTRL1     	0x0d   
#define SPI_AICTRL2     	0x0e   
#define SPI_AICTRL3     	0x0f   
#define SM_DIFF         	0x01   
#define SM_JUMP         	0x02   
#define SM_RESET        	0x04   
#define SM_OUTOFWAV     	0x08   
#define SM_PDOWN        	0x10   
#define SM_TESTS        	0x20   
#define SM_STREAM       	0x40   
#define SM_PLUSV        	0x80   
#define SM_DACT         	0x100   
#define SM_SDIORD       	0x200   
#define SM_SDISHARE     	0x400   
#define SM_SDINEW       	0x800   
#define SM_ADPCM        	0x1000   
#define SM_ADPCM_HP     	0x2000 		 

/* SPI1�ӿڶ���  */
#define AUDIO_SPI				SPI1
#define AUDIO_SPI_CLK  			RCC_APB2Periph_SPI1
#define AUDIO_SPI_CLK_INIT		RCC_APB2PeriphClockCmd

/*��ƵоƬ*/
#define AUDIO_SPI1_SCK_PIN     		GPIO_Pin_5
#define AUDIO_SPI1_SCK_PORT    		GPIOA
#define AUDIO_SPI1_SCK_CLK     		RCC_AHB1Periph_GPIOA
#define AUDIO_SPI1_SCK_SOURCE       GPIO_PinSource5
#define AUDIO_SPI1_SCK_AF           GPIO_AF_SPI1

#define AUDIO_SPI1_MISO_PIN     	GPIO_Pin_6
#define AUDIO_SPI1_MISO_PORT    	GPIOA
#define AUDIO_SPI1_MISO_CLK     	RCC_AHB1Periph_GPIOA
#define AUDIO_SPI1_MISO_SOURCE      GPIO_PinSource6
#define AUDIO_SPI1_MISO_AF          GPIO_AF_SPI1

#define AUDIO_SPI1_MOSI_PIN     	GPIO_Pin_7
#define AUDIO_SPI1_MOSI_PORT    	GPIOA
#define AUDIO_SPI1_MOSI_CLK     	RCC_AHB1Periph_GPIOA
#define AUDIO_SPI1_MOSI_SOURCE      GPIO_PinSource7
#define AUDIO_SPI1_MOSI_AF          GPIO_AF_SPI1

#define AUDIO_XCS_PIN     			GPIO_Pin_4
#define AUDIO_XCS_GPIO_PORT    		GPIOA
#define AUDIO_XCS_GPIO_CLK     		RCC_AHB1Periph_GPIOA

#define AUDIO_XDCS_PIN     			GPIO_Pin_0
#define AUDIO_XDCS_PORT    			GPIOB
#define AUDIO_XDCS_CLK     			RCC_AHB1Periph_GPIOB

#define AUDIO_DREQ_PIN     			GPIO_Pin_1
#define AUDIO_DREQ_PORT    			GPIOB
#define AUDIO_DREQ_CLK     			RCC_AHB1Periph_GPIOB

#define AUDIO_RST_PIN     			GPIO_Pin_2
#define AUDIO_RST_PORT    			GPIOB
#define AUDIO_RST_CLK     			RCC_AHB1Periph_GPIOB

/* Select sFLASH: Chip Select pin low */
#define AUDIO_CS_LOW()       GPIO_ResetBits(AUDIO_XCS_GPIO_PORT, AUDIO_XCS_PIN)

/* Deselect sFLASH: Chip Select pin high */
#define AUDIO_CS_HIGH()      GPIO_SetBits(AUDIO_XCS_GPIO_PORT, AUDIO_XCS_PIN)

#define VS_DREQ_STATE     GPIO_ReadInputDataBit(AUDIO_DREQ_PORT, AUDIO_DREQ_PIN)  	//DREQ
#define VS_RST_LOW   	GPIO_ResetBits(AUDIO_RST_PORT, AUDIO_RST_PIN) 	//RST#define VS_RST_HIGH   	GPIO_SetBits(AUDIO_RST_PORT, AUDIO_RST_PIN) 	//RST
#define VS_XCS_LOW   	GPIO_ResetBits(AUDIO_XCS_GPIO_PORT, AUDIO_XCS_PIN) 	//RST#define VS_XCS_HIGH   	GPIO_SetBits(AUDIO_XCS_GPIO_PORT, AUDIO_XCS_PIN) 	//RST
#define VS_XDCS_LOW   	GPIO_ResetBits(AUDIO_XDCS_PORT, AUDIO_XDCS_PIN) 	//RST#define VS_XDCS_HIGH   	GPIO_SetBits(AUDIO_XDCS_PORT, AUDIO_XDCS_PIN) 	//RST
u16 VS_RD_Reg(u8 address);				//���Ĵ���
u16 VS_WRAM_Read(u16 addr);	    	//��RAM
void VS_WR_Data(u8 data);				//д����
void VS_WR_Cmd(u8 address, u16 data);	//д����
u8 VS_HD_Reset(void);			    	//Ӳ��λ
void VS_Soft_Reset(void);           	//��λ
u16 VS_Ram_Test(void);             		//RAM����	    
void VS_Sine_Test(void);            	//���Ҳ���

u8 VS_SPI_ReadWriteByte(u8 data);
void VS_SPI_SpeedLow(void);
void VS_SPI_SpeedHigh(void);
void VS_Init(void);						//��ʼ��VS10XX	 
void VS_Set_Speed(u8 t);				//���ò����ٶ�
u16 VS_Get_HeadInfo(void);     		//�õ�������
u32 VS_Get_ByteRate(void);				//�õ��ֽ�����
u16 VS_Get_EndFillByte(void);			//�õ�����ֽ�
u8 VS_Send_MusicData(u8* buf);		//��VS10XX����32�ֽ�
void VS_Restart_Play(void);				//���¿�ʼ��һ�׸貥��	  
void VS_Reset_DecodeTime(void);			//�������ʱ��
u16 VS_Get_DecodeTime(void);   		//�õ�����ʱ��

void VS_Load_Patch(u16 *patch, u16 len);	//�����û�patch
u8 VS_Get_Spec(u16 *p);       		//�õ���������	   
void VS_Set_Bands(u16 *buf, u8 bands);	//��������Ƶ��
void VS_Set_Vol(u8 volx);				//����������   
void VS_Set_Bass(u8 bfreq, u8 bass, u8 tfreq, u8 treble);				//���øߵ���
void VS_Set_Effect(u8 eft);				//������Ч
void VS_Set_All(void);

void vs10xx_read_para(_vs10xx_obj * vs10xxdev);
void vs10xx_save_para(_vs10xx_obj * vs10xxdev);

//�ڲ��ӿڣ�SPI1��ʼ��
void SPI1_SetSpeed(u8 SpeedSet);
u8 SPI1_ReadWriteByte(u8 TxData);
void AUDIO_Lowlevel_Init();
void AUDIO_Init();

#endif

