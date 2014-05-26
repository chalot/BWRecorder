#include "audio.h"
#include <stm32f2xx_conf.h>
#include <board.h>
#include <qp_port.h>

//VS10XXĬ�����ò���
_vs10xx_obj vsset = { 220,	//����:220
		6,		//�������� 60Hz
		15,		//�������� 15dB	
		10,		//�������� 10Khz	
		15,		//�������� 10.5dB
		0,		//�ռ�Ч��	
		};

////////////////////////////////////////////////////////////////////////////////
//��ֲʱ��Ľӿ�
//data:Ҫд�������
//����ֵ:����������
u8 VS_SPI_ReadWriteByte(u8 data) {
	return SPI1_ReadWriteByte(data);
}

//SD����ʼ����ʱ��,��Ҫ����
void VS_SPI_SpeedLow(void) {
	SPI1_SetSpeed(SPI_BaudRatePrescaler_32);		//���õ�����ģʽ 
}

//SD������������ʱ��,���Ը�����
void VS_SPI_SpeedHigh(void) {
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);		//���õ�����ģʽ		 
}

//��ʼ��VS10XX��IO��	 
void VS_Init(void) {
// 	GPIO_InitTypeDef  GPIO_InitStructure;
//
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PB�˿�ʱ��
//
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //PC13
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //����
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
//
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	 //PE6
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//
//														  //PF6,PF7
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	GPIO_Init(GPIOF, &GPIO_InitStructure);
//
//	SPI1_Init();
}
////////////////////////////////////////////////////////////////////////////////	 	  
//��λVS10XX
void VS_Soft_Reset(void) {
	u8 retry = 0;
	while (VS_DREQ_STATE == 0)
		; //�ȴ������λ����
	VS_SPI_ReadWriteByte(0Xff); //��������
	retry = 0;
	while (VS_RD_Reg(SPI_MODE) != 0x0800) // �����λ,��ģʽ  
	{
		VS_WR_Cmd(SPI_MODE, 0x0804); // �����λ,��ģʽ	    
		Delay_1ms(2); //�ȴ�����1.35ms
		if (retry++ > 100)
			break;
	}
	while (VS_DREQ_STATE == 0)
		; //�ȴ������λ����
	retry = 0;
	while (VS_RD_Reg(SPI_CLOCKF) != 0X9800) //����VS10XX��ʱ��,3��Ƶ ,1.5xADD 
	{
		VS_WR_Cmd(SPI_CLOCKF, 0X9800); //����VS10XX��ʱ��,3��Ƶ ,1.5xADD
		if (retry++ > 100)
			break;
	}
	Delay_1ms(20);
}
//Ӳ��λMP3
//����1:��λʧ��;0:��λ�ɹ�	   
u8 VS_HD_Reset(void) {
	u8 retry = 0;
	VS_RST_LOW;
	Delay_1ms(20);
	VS_XDCS_HIGH; //ȡ�����ݴ���
	VS_XCS_HIGH; //ȡ�����ݴ���
	VS_RST_HIGH;
	while (VS_DREQ_STATE == 0 && retry < 200) //�ȴ�DREQΪ��
	{
		retry++;
		Delay_1us(50);
	};
	Delay_1ms(20);
	if (retry >= 200)
		return 1;
	else
		return 0;
}
//���Ҳ��� 
void VS_Sine_Test(void) {
	VS_HD_Reset();
	VS_WR_Cmd(0x0b, 0X2020);	  //��������	 
	VS_WR_Cmd(SPI_MODE, 0x0820);	  //����VS10XX�Ĳ���ģʽ     
	while (VS_DREQ_STATE == 0)
		;     //�ȴ�DREQΪ��
	//printf("mode sin:%x\n",VS_RD_Reg(SPI_MODE));
	//��VS10XX�������Ҳ������0x53 0xef 0x6e n 0x00 0x00 0x00 0x00
	//����n = 0x24, �趨VS10XX�����������Ҳ���Ƶ��ֵ��������㷽����VS10XX��datasheet
	VS_SPI_SpeedLow(); 	//���� 
	VS_XDCS_LOW; 	//ѡ�����ݴ���
	VS_SPI_ReadWriteByte(0x53);
	VS_SPI_ReadWriteByte(0xef);
	VS_SPI_ReadWriteByte(0x6e);
	VS_SPI_ReadWriteByte(0x24);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	Delay_1ms(100);
	VS_XDCS_HIGH;
	//�˳����Ҳ���
	VS_XDCS_LOW; 	//ѡ�����ݴ���
	VS_SPI_ReadWriteByte(0x45);
	VS_SPI_ReadWriteByte(0x78);
	VS_SPI_ReadWriteByte(0x69);
	VS_SPI_ReadWriteByte(0x74);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	Delay_1ms(100);
	VS_XDCS_HIGH;

	//�ٴν������Ҳ��Բ�����nֵΪ0x44���������Ҳ���Ƶ������Ϊ�����ֵ
	VS_XDCS_LOW; 	//ѡ�����ݴ���
	VS_SPI_ReadWriteByte(0x53);
	VS_SPI_ReadWriteByte(0xef);
	VS_SPI_ReadWriteByte(0x6e);
	VS_SPI_ReadWriteByte(0x44);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	Delay_1ms(100);
	VS_XDCS_HIGH;
	//�˳����Ҳ���
	VS_XDCS_LOW; 	//ѡ�����ݴ���
	VS_SPI_ReadWriteByte(0x45);
	VS_SPI_ReadWriteByte(0x78);
	VS_SPI_ReadWriteByte(0x69);
	VS_SPI_ReadWriteByte(0x74);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	Delay_1ms(100);
	VS_XDCS_HIGH;
}
//ram ���� 
//����ֵ:RAM���Խ��
// VS1003����õ���ֵΪ0x807F����������;VS1053Ϊ0X83FF.																				 
u16 VS_Ram_Test(void) {
	VS_HD_Reset();
	VS_WR_Cmd(SPI_MODE, 0x0820); 	// ����VS10XX�Ĳ���ģʽ
	while (VS_DREQ_STATE == 0)
		; // �ȴ�DREQΪ��
	VS_SPI_SpeedLow(); //���� 
	VS_XDCS_LOW;	       		    // xDCS = 1��ѡ��VS10XX�����ݽӿ�
	VS_SPI_ReadWriteByte(0x4d);
	VS_SPI_ReadWriteByte(0xea);
	VS_SPI_ReadWriteByte(0x6d);
	VS_SPI_ReadWriteByte(0x54);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	Delay_1ms(150);
	VS_XDCS_HIGH;
	return VS_RD_Reg(SPI_HDAT0);	       		    // VS1003����õ���ֵΪ0x807F����������;VS1053Ϊ0X83FF.;       
}
//��VS10XXд����
//address:�����ַ
//data:��������
void VS_WR_Cmd(u8 address, u16 data) {
	while (VS_DREQ_STATE == 0)
		;	       		    //�ȴ�����
	VS_SPI_SpeedLow();	       		    //���� 	   
	VS_XDCS_HIGH;
	VS_XCS_LOW;
	VS_SPI_ReadWriteByte(VS_WRITE_COMMAND);	       		    //����VS10XX��д����
	VS_SPI_ReadWriteByte(address); //��ַ
	VS_SPI_ReadWriteByte(data >> 8); //���͸߰�λ
	VS_SPI_ReadWriteByte(data);	 //�ڰ�λ
	VS_XCS_HIGH;
	VS_SPI_SpeedHigh();	 //����	   
}
//��VS10XXд����
//data:Ҫд�������
void VS_WR_Data(u8 data) {
	VS_SPI_SpeedHigh();	 //����,��VS1003B,���ֵ���ܳ���36.864/4Mhz����������Ϊ9M 
	VS_XDCS_LOW;
	VS_SPI_ReadWriteByte(data);
	VS_XDCS_HIGH;
}
//��VS10XX�ļĴ���           
//address���Ĵ�����ַ
//����ֵ��������ֵ
//ע�ⲻҪ�ñ��ٶ�ȡ,�����
u16 VS_RD_Reg(u8 address) {
	u16 temp = 0;
	while (VS_DREQ_STATE == 0)
		;	 //�ǵȴ�����״̬
	VS_SPI_SpeedLow();	 //���� 
	VS_XDCS_HIGH;
	VS_XCS_LOW;
	VS_SPI_ReadWriteByte(VS_READ_COMMAND);	//����VS10XX�Ķ�����
	VS_SPI_ReadWriteByte(address);       	//��ַ
	temp = VS_SPI_ReadWriteByte(0xff); 		//��ȡ���ֽ�
	temp = temp << 8;
	temp += VS_SPI_ReadWriteByte(0xff); 		//��ȡ���ֽ�
	VS_XCS_HIGH;
	VS_SPI_SpeedHigh(); 		//����	  
	return temp;
}
//��ȡVS10xx��RAM
//addr��RAM��ַ
//����ֵ��������ֵ
u16 VS_WRAM_Read(u16 addr) {
	u16 res;
	VS_WR_Cmd(SPI_WRAMADDR, addr);
	res = VS_RD_Reg(SPI_WRAM);
	return res;
}
//���ò����ٶȣ���VS1053��Ч�� 
//t:0,1,�����ٶ�;2,2���ٶ�;3,3���ٶ�;4,4����;�Դ�����
void VS_Set_Speed(u8 t) {
	VS_WR_Cmd(SPI_WRAMADDR, 0X1E04);	//�ٶȿ��Ƶ�ַ  
	while (VS_DREQ_STATE == 0)
		; 				//�ȴ�����
	VS_WR_Cmd(SPI_WRAM, t); 			//д�벥���ٶ�
}
//FOR WAV HEAD0 :0X7761 HEAD1:0X7665    
//FOR MIDI HEAD0 :other info HEAD1:0X4D54
//FOR WMA HEAD0 :data speed HEAD1:0X574D
//FOR MP3 HEAD0 :data speed HEAD1:ID
//������Ԥ��ֵ,�ײ�III
const u16 bitrate[2][16] = { { 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0 }, { 0, 32, 40, 48, 56,
		64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0 } };
//����Kbps�Ĵ�С
//����ֵ���õ�������
u16 VS_Get_HeadInfo(void) {
	unsigned int HEAD0;
	unsigned int HEAD1;
	HEAD0 = VS_RD_Reg(SPI_HDAT0);
	HEAD1 = VS_RD_Reg(SPI_HDAT1);
	//printf("(H0,H1):%x,%x\n",HEAD0,HEAD1);
	switch (HEAD1) {
	case 0x7665: 			//WAV��ʽ
	case 0X4D54: 			//MIDI��ʽ 
	case 0X4154: 			//AAC_ADTS
	case 0X4144: 			//AAC_ADIF
	case 0X4D34: 			//AAC_MP4/M4A
	case 0X4F67: 			//OGG
	case 0X574D: 			//WMA��ʽ
	case 0X664C: 			//FLAC��ʽ
	{
		////printf("HEAD0:%d\n",HEAD0);
		HEAD1 = HEAD0 * 2 / 25; 			//�൱��*8/100
		if ((HEAD1 % 10) > 5)
			return HEAD1 / 10 + 1; 			//��С�����һλ��������
		else
			return HEAD1 / 10;
	}
	default: 			//MP3��ʽ,�����˽ײ�III�ı�
	{
		HEAD1 >>= 3;
		HEAD1 = HEAD1 & 0x03;
		if (HEAD1 == 3)
			HEAD1 = 1;
		else
			HEAD1 = 0;
		return bitrate[HEAD1][HEAD0 >> 12];
	}
	}
}
//�õ�ƽ���ֽ���
//����ֵ��ƽ���ֽ����ٶ�
u32 VS_Get_ByteRate(void) {
	return VS_WRAM_Read(0X1E05); 			//ƽ��λ��
}
//�õ���Ҫ��������
//����ֵ:��Ҫ��������
u16 VS_Get_EndFillByte(void) {
	return VS_WRAM_Read(0X1E06); 			//����ֽ�
}
//����һ����Ƶ����
//�̶�Ϊ32�ֽ�
//����ֵ:0,���ͳɹ�
//		 1,VS10xx��ȱ����,��������δ�ɹ�����    
u8 VS_Send_MusicData(u8* buf) {
	u8 n;
	if (VS_DREQ_STATE != 0)  //�����ݸ�VS10XX
			{
		VS_XDCS_LOW;
		for (n = 0; n < 32; n++) {
			VS_SPI_ReadWriteByte(buf[n]);
		}
		VS_XDCS_HIGH;
	}
	else
		return 1;
	return 0;  //�ɹ�������
}
//�и�
//ͨ���˺����и裬��������л���������				
void VS_Restart_Play(void) {
	u16 temp;
	u16 i;
	u8 n;
	u8 vsbuf[32];
	for (n = 0; n < 32; n++)
		vsbuf[n] = 0;  //����
	temp = VS_RD_Reg(SPI_MODE);	//��ȡSPI_MODE������
	temp |= 1 << 3;					//����SM_CANCELλ
	temp |= 1 << 2;					//����SM_LAYER12λ,������MP1,MP2
	VS_WR_Cmd(SPI_MODE, temp);	//����ȡ����ǰ����ָ��
	for (i = 0; i < 2048;)			//����2048��0,�ڼ��ȡSM_CANCELλ.���Ϊ0,���ʾ�Ѿ�ȡ���˵�ǰ����
			{
		if (VS_Send_MusicData(vsbuf) == 0)			//ÿ����32���ֽں���һ��
				{
			i += 32;						//������32���ֽ�
			temp = VS_RD_Reg(SPI_MODE);	//��ȡSPI_MODE������
			if ((temp & (1 << 3)) == 0)
				break;	//�ɹ�ȡ����
		}
	}
	if (i < 2048)	//SM_CANCEL����
			{
		temp = VS_Get_EndFillByte() & 0xff;	//��ȡ����ֽ�
		for (n = 0; n < 32; n++)
			vsbuf[n] = temp;	//����ֽڷ�������
		for (i = 0; i < 2052;) {
			if (VS_Send_MusicData(vsbuf) == 0)
				i += 32;	//���	  
		}
	}
	else
		VS_Soft_Reset();  	//SM_CANCEL���ɹ�,�����,��Ҫ��λ 	  
	temp = VS_RD_Reg(SPI_HDAT0);
	temp += VS_RD_Reg(SPI_HDAT1);
	if (temp)					//��λ,����û�гɹ�ȡ��,��ɱ���,Ӳ��λ
	{
		VS_HD_Reset();		   	//Ӳ��λ
		VS_Soft_Reset();  		//��λ 
	}
}
//�������ʱ��                          
void VS_Reset_DecodeTime(void) {
	VS_WR_Cmd(SPI_DECODE_TIME, 0x0000);
	VS_WR_Cmd(SPI_DECODE_TIME, 0x0000);  		//��������
}
//�õ�mp3�Ĳ���ʱ��n sec
//����ֵ������ʱ��
u16 VS_Get_DecodeTime(void) {
	u16 dt = 0;
	dt = VS_RD_Reg(SPI_DECODE_TIME);
	return dt;
}
//vs10xxװ��patch.
//patch��patch�׵�ַ
//len��patch����
void VS_Load_Patch(u16 *patch, u16 len) {
	u16 i;
	u16 addr, n, val;
	for (i = 0; i < len;) {
		addr = patch[i++];
		n = patch[i++];
		if (n & 0x8000U) //RLE run, replicate n samples 
				{
			n &= 0x7FFF;
			val = patch[i++];
			while (n--)
				VS_WR_Cmd(addr, val);
		}
		else //copy run, copy n sample 
		{
			while (n--) {
				val = patch[i++];
				VS_WR_Cmd(addr, val);
			}
		}
	}
}
//�趨VS10XX���ŵ������͸ߵ���
//volx:������С(0~254)
void VS_Set_Vol(u8 volx) {
	u16 volt = 0; 			//�ݴ�����ֵ
	volt = 254 - volx;			//ȡ��һ��,�õ����ֵ,��ʾ���ı�ʾ 
	volt <<= 8;
	volt += 254 - volx;			//�õ��������ú��С
	VS_WR_Cmd(SPI_VOL, volt);			//������ 
}
//�趨�ߵ�������
//bfreq:��Ƶ����Ƶ��	2~15(��λ:10Hz)
//bass:��Ƶ����			0~15(��λ:1dB)
//tfreq:��Ƶ����Ƶ�� 	1~15(��λ:Khz)
//treble:��Ƶ����  	 	0~15(��λ:1.5dB,С��9��ʱ��Ϊ����)
void VS_Set_Bass(u8 bfreq, u8 bass, u8 tfreq, u8 treble) {
	u16 bass_set = 0; //�ݴ������Ĵ���ֵ
	signed char temp = 0;
	if (treble == 0)
		temp = 0;	   		//�任
	else if (treble > 8)
		temp = treble - 8;
	else
		temp = treble - 9;
	bass_set = temp & 0X0F;				//�����趨
	bass_set <<= 4;
	bass_set += tfreq & 0xf;			//��������Ƶ��
	bass_set <<= 4;
	bass_set += bass & 0xf;				//�����趨
	bass_set <<= 4;
	bass_set += bfreq & 0xf;			//��������    
	VS_WR_Cmd(SPI_BASS, bass_set);	//BASS 
}
//�趨��Ч
//eft:0,�ر�;1,��С;2,�е�;3,���.
void VS_Set_Effect(u8 eft) {
	u16 temp;
	temp = VS_RD_Reg(SPI_MODE);	//��ȡSPI_MODE������
	if (eft & 0X01)
		temp |= 1 << 4;		//�趨LO
	else
		temp &= ~(1 << 5);			//ȡ��LO
	if (eft & 0X02)
		temp |= 1 << 7;		//�趨HO
	else
		temp &= ~(1 << 7);			//ȡ��HO						   
	VS_WR_Cmd(SPI_MODE, temp);	//�趨ģʽ    
}
///////////////////////////////////////////////////////////////////////////////
//��������,��Ч��.
void VS_Set_All(void) {
	VS_Set_Vol(vsset.mvol);
	VS_Set_Bass(vsset.bflimit, vsset.bass, vsset.tflimit, vsset.treble);
	VS_Set_Effect(vsset.effect);
}

/**
 //SPI1��ʼ��
 void SPI1_Init(void)
 {
 GPIO_InitTypeDef GPIO_InitStructure;
 SPI_InitTypeDef  SPI_InitStructure;

 RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );//PORTB��SPI2ʱ��ʹ��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //PA5.6.7��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //PA5.6.7����

 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
 SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
 SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
 SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
 SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
 SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

 SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����

 SPI2_ReadWriteByte(0xff);//��������
 }
 */

//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ
//SPI_BaudRatePrescaler_8   8��Ƶ
//SPI_BaudRatePrescaler_16  16��Ƶ
//SPI_BaudRatePrescaler_256 256��Ƶ
void SPI1_SetSpeed(u8 SpeedSet) {
	SPI1->CR1 &= 0XFFC7;
	SPI1->CR1 |= SpeedSet;
	SPI_Cmd(SPI1, ENABLE);
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData) {
	u8 retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if (retry > 200)
			return 0;
	}
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry = 0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if (retry > 200)
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����
}

/**
 * SPI1��ʼ��
 */
void AUDIO_Lowlevel_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	/*!< Enable the SPI clock */
	AUDIO_SPI_CLK_INIT(AUDIO_SPI_CLK, ENABLE);

	/*!< Enable GPIO clocks */
	RCC_AHB1PeriphClockCmd(
	AUDIO_SPI1_SCK_PIN | AUDIO_SPI1_MISO_PIN | AUDIO_SPI1_MOSI_PIN, ENABLE);

	/*!< SPI pins configuration *************************************************/

	GPIO_PinAFConfig(AUDIO_SPI1_SCK_PIN, AUDIO_SPI1_SCK_SOURCE,
	AUDIO_SPI1_SCK_AF);
	GPIO_PinAFConfig(AUDIO_SPI1_MISO_PIN, AUDIO_SPI1_MISO_SOURCE,
	AUDIO_SPI1_MISO_AF);
	GPIO_PinAFConfig(AUDIO_SPI1_MOSI_PIN, AUDIO_SPI1_MOSI_PIN,
	AUDIO_SPI1_MOSI_AF);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	/*!< SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = AUDIO_SPI1_SCK_PIN;
	GPIO_Init(AUDIO_SPI1_SCK_PORT, &GPIO_InitStructure);

	/*!< SPI MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin = AUDIO_SPI1_MOSI_PIN;
	GPIO_Init(AUDIO_SPI1_MOSI_PORT, &GPIO_InitStructure);

	/*!< SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = AUDIO_SPI1_MISO_PIN;
	GPIO_Init(AUDIO_SPI1_MISO_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(
	AUDIO_XDCS_PIN | AUDIO_DREQ_PIN | AUDIO_RST_PIN | AUDIO_XCS_PIN, ENABLE);

	GPIO_InitStructure.GPIO_Pin = AUDIO_XCS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(AUDIO_XCS_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	/*!< SPI XDCS pin configuration */
	GPIO_InitStructure.GPIO_Pin = AUDIO_XDCS_PIN;
	GPIO_Init(AUDIO_XDCS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = AUDIO_RST_PIN;
	GPIO_Init(AUDIO_RST_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = AUDIO_DREQ_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(AUDIO_DREQ_PORT, &GPIO_InitStructure);

}

/**
 * ��Ƶ�豸��ʼ��
 *
 */
void AUDIO_Init() {
	SPI_InitTypeDef SPI_InitStructure;

//	AUDIO_Lowlevel_Init();

	/*!< Deselect the FLASH: Chip Select high */
	AUDIO_CS_HIGH();

	/*!< SPI configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(AUDIO_SPI, &SPI_InitStructure);

	/*ʹ��SPII*/
	SPI_Cmd(AUDIO_SPI, ENABLE);
}

