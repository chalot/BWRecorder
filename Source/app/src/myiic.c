#include "myiic.h"
#include <stm32f2xx_conf.h>
#include <utility.h>


/*
** ע�⣺���Ӳ����ʹ��д�������ţ�����ͷ�ļ��к궨�塰USE_WP_PIN����
**       ������ע�͵�����䣡��
*/

/*------I2C operate--------------------------------------------------*/
/*
*-------------------------------------------------------------------------------
* �������ܣ� I2C������ʱ
* �������ݣ� ��
* �� �� ֵ�� ��
* ע    �⣺ I2C������ʱ���Ը����Լ���MCU����ʱ��Ƶ�ʽ����ʵ��ĵ���
*-------------------------------------------------------------------------------
*/
#define _nop_()      Delay_us(2)        /*�ڴ˶�����I2C���ߵ���ʱ*/
void Delay_us(unsigned int t)
{
 //  t *=4;
   while(--t);
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� IO��ʼ��
* �������ݣ� ��
* �� �� ֵ�� ��
*-------------------------------------------------------------------------------
*/
void I2C_CtrlPin_Init(void)
{
//    IO_ConfigPort(I2C_SDA_GPIO_PORT,I2C_SDA_PIN ,IO_GPIO_MODE);     /*config port */
//    IO_GPIOWriteData(I2C_SDA_GPIO_PORT,I2C_SDA_PIN,0x0001);        /* Write data   */
//    IO_GPIOConfigPullup(I2C_SDA_GPIO_PORT,I2C_SDA_PIN,IO_PULLUP_DISCONN); /*pull-up setting */
//
//    IO_ConfigPort(I2C_SCL_GPIO_PORT,I2C_SCL_PIN,IO_GPIO_MODE);     /*config port */
//    IO_GPIOWriteData(I2C_SCL_GPIO_PORT,I2C_SCL_PIN,0x0002);        /* Write data   */
//    IO_GPIOConfigPullup(I2C_SCL_GPIO_PORT,I2C_SCL_PIN,IO_PULLUP_DISCONN); /*pull-up setting */

    I2C_SDA_DIR_OUT();
    I2C_SCL_DIR_OUT();

#ifdef USE_WP_PIN
    IO_ConfigPort(I2C_WP_PORT,I2C_WP_PIN ,IO_GPIO_MODE);     /*config port */
    IO_GPIOWriteData(I2C_WP_PORT,I2C_WP_PIN,0x0001);        /* Write data   */
    IO_GPIOConfigPullup(I2C_WP_PORT,I2C_WP_PIN,IO_PULLUP_DISCONN); /*pull-up setting */

    I2C_WP_DIR_OUT();
#endif
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� I2C���߳�ʼ��
* �������ݣ� ��
* �� �� ֵ�� ��
*-------------------------------------------------------------------------------
*/
void InitI2C(void)
{
   I2C_SCL_DAT(0);
	I2CStop();
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� ����������ʼ�ź�
* �������ݣ� ��
* �� �� ֵ�� ��
*-------------------------------------------------------------------------------
*/
void I2CStart(void)
{
   _nop_();
   I2C_SDA_DAT(1);
   _nop_();
   I2C_SCL_DAT (1);
	_nop_();
	I2C_SDA_DAT (0);
	_nop_();
	I2C_SCL_DAT (0);
	_nop_();
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� �������߽����ź�
* �������ݣ� ��
* �� �� ֵ�� ��
*-------------------------------------------------------------------------------
*/
void I2CStop(void)
{
	_nop_();
   I2C_SDA_DAT (0);
	_nop_();
	I2C_SCL_DAT (1);
	_nop_();
	I2C_SDA_DAT (1);
	_nop_();
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� Ӧ�𣬷�������ʱ���ź�,������ʱ�ӵ�ƽΪ���ڼ�SDA�ϵ�
             ״̬,��������I2C���߷�������.
* �������ݣ� ��
* �� �� ֵ�� ACKCondition������SCLΪ�ߵ�ƽ�ڼ�SDA��״̬
*-------------------------------------------------------------------------------
*/
unsigned char ACK_From_I2C(void)
{
   unsigned char ACKCondition;

   _nop_();
   I2C_SCL_DAT (1);
   I2C_SDA_DIR_IN();                   /*������������Ϊ���룬׼���������϶�ȡӦ���ź�*/
	_nop_();
	I2C_SCL_DAT (1);
	_nop_();
	if(I2C_SDA_READ()){
		ACKCondition = 1;                /*δ����Ӧ���ź�*/
	}
	else{
		ACKCondition = 0;                /*���յ�Ӧ���ź�*/
	}
	I2C_SCL_DAT (0);
   I2C_SDA_DIR_OUT();                  /*����SDAΪ���*/

   return(ACKCondition);
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� ��I2C���߷���Ӧ���źţ��������ؽ���
* �������ݣ� ��
* �� �� ֵ�� ��
*-------------------------------------------------------------------------------
*/
void Ack_To_I2C(void)
{
	_nop_();
   I2C_SDA_DAT (0);
   _nop_();
	I2C_SCL_DAT (1);
	_nop_();
	I2C_SCL_DAT (0);
   _nop_();
	I2C_SDA_DAT (1);
   _nop_();

}

/*���ͷ�Ӧ���ź�*/
void NotAck_To_I2C(void)
{
	_nop_();
   I2C_SDA_DAT (1);
   _nop_();
	I2C_SCL_DAT (1);
	_nop_();
	I2C_SCL_DAT (0);
   _nop_();
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� ��I2C���߷���һ���ֽڵ����ݣ�����ȡ�����豸��Ӧ���ź�
* �������ݣ� I2CData����Ҫ���͵�I2C���ߵ�����
* �� �� ֵ�� ACK_From_I2C()�������ϵ��豸��Ӧ���źţ�������ͳɹ�����0����֮����1
*-------------------------------------------------------------------------------
*/
unsigned char I2CSendByte(unsigned char I2CData)
{
  unsigned char idx;

  _nop_();
  for(idx = 0; idx < 8; idx++){
    if(I2CData & 0x80){
            I2C_SDA_DAT (1);
    }
    else{
            I2C_SDA_DAT (0);
    }
    I2CData <<= 1;
    I2C_SCL_DAT (1);        /*����ʱ���ź��ߣ�������������*/
    _nop_();
    I2C_SCL_DAT (0);
    _nop_();
  }
  //I2C_SDA_DAT (1);
  return(ACK_From_I2C());
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� ��I2C���߽���1���ֽ�,�����ظ��ֽ�,������Ӧ���ź�
* �������ݣ� ��
* �� �� ֵ�� I2CData�������߶�ȡ������
*-------------------------------------------------------------------------------
*/
unsigned char I2CReceiveByte(void)
{
   unsigned char I2CData = 0;
	unsigned char idx;

   I2C_SDA_DIR_IN();                      /*������������Ϊ��������*/
   _nop_();
	for(idx = 0; idx < 8; idx++){
      I2C_SCL_DAT (0);
      _nop_();
      I2C_SCL_DAT (1);                    /*����ʱ���źţ���������״̬*/
      if(I2C_SDA_READ()){
         I2CData |= (0x80 >> idx);
      }
      _nop_();
      I2C_SCL_DAT (0);
      _nop_();
	}
   I2C_SCL_DAT (0);
   I2C_SDA_DIR_OUT();                     /*������������Ϊ�������*/
	return(I2CData);
}

/*
*-------------------------------------------------------------------------------
* �������ܣ� ��I2C�����ϵ�������������
* �������ݣ� Device_Addr��I2C�������豸�ĵ�ַ
*            I2C_Adr���豸�Ĵ洢�����ַ
*            *DataPtr����Ҫ���͸��豸������
*            Data_Size����Ҫ�������ݵĳ���
* �� �� ֵ�� ��
*-------------------------------------------------------------------------------
*/
unsigned char Write_Data_To_I2C(unsigned char Device_Addr, unsigned int I2C_Adr, unsigned char *DataPtr,unsigned int Data_Size)
{
#ifdef USE_WP_PIN
	I2C_WP_DAT (0);
#endif
	I2CStart();
   if(I2CSendByte(E_WRITE | ((Device_Addr&0x07)<<1))) return FALSE; /*����8λ���ݣ�����ò���I2C�����豸��Ӧ���ź���ֱ�ӷ���*/
	if(I2CSendByte(I2C_Adr >> 8)) return FALSE;
	if(I2CSendByte(I2C_Adr & 0x00FF)) return FALSE;

	while(Data_Size --){
		if(I2CSendByte(*DataPtr)) return FALSE;
		DataPtr ++;
	}
	I2CStop();          /*����ֹͣ�ź�*/
#ifdef USE_WP_PIN
	I2C_WP_DAT (1);
#endif
   return TRUE;
}


/*
*-------------------------------------------------------------------------------
* �������ܣ� ��ȡ��I2C�����ϵ��豸���͹���������
* �������ݣ� Device_Addr��I2C�������豸�ĵ�ַ
*            I2C_Adr���豸�Ĵ洢�����ַ
*            *DataPtr���������϶�ȡ�����ݽ������ڸ�ָ��ָ��Ĵ洢����
*            Data_Size����ȡ���ݵĳ���
* �� �� ֵ�� ��
*-------------------------------------------------------------------------------
*/
unsigned char Read_Data_From_I2C(unsigned char Device_Addr,unsigned int I2C_Adr,unsigned char *DataPtr,unsigned int Data_Size)
{
   I2CStart();
   if(I2CSendByte(E_WRITE | ((Device_Addr&0x07)<<1))) return FALSE;
   if(I2CSendByte(I2C_Adr >> 8))return FALSE;
   if(I2CSendByte(I2C_Adr & 0x00FF))return FALSE;
   I2CStart();
   if(I2CSendByte(E_READ | ((Device_Addr&0x07)<<1)))return FALSE;
	while(Data_Size --){
	  *DataPtr = I2CReceiveByte();
		DataPtr ++;
		if(Data_Size != 0){
			Ack_To_I2C();
		}
	}
	NotAck_To_I2C();
	I2CStop();
   return TRUE;
}
/*=====================The End Of The Driver====================================*/






#include "string.h"
#include "stdio.h"

/*==============================================================================*/
/*
* �������ܣ� ���Ժ���
* �������ݣ� ��
* �� �� ֵ�� ��
* ˵    ���� ����ȡ�����ݴ洢��buf�У�����ͨ�����ݹ۲촰�ڲ鿴buf�е�����
*/

unsigned char I2C_test(void)
{
 unsigned char buf[20],buf1[20];
   unsigned long num=0;

   InitI2C();

   Read_Data_From_I2C(DEVICE_ADDR0,0x0010,buf,20);
   Read_Data_From_I2C(DEVICE_ADDR1,0x0010,buf,20);
   Read_Data_From_I2C(DEVICE_ADDR2,0x0010,buf,20);
   Read_Data_From_I2C(DEVICE_ADDR0,0x0010,buf,20);
   Read_Data_From_I2C(DEVICE_ADDR1,0x0010,buf,20);
   Read_Data_From_I2C(DEVICE_ADDR2,0x0010,buf,20);
//   Write_Data_To_I2C( DEVICE_ADDR0,0x0010,"xxxxxxxxxxxxxxxxxxxx",20);
//   Read_Data_From_I2C(DEVICE_ADDR0,0x0010,buf,20);
//   Write_Data_To_I2C( DEVICE_ADDR1,0x0010,"xxxxxxxxxxxxxxxxxxxx",20);
//   Read_Data_From_I2C(DEVICE_ADDR1,0x0010,buf,20);

   while(1){
     /*I2C�����ϵ�Device0*/
      sprintf((char*)buf1,"FM24C64A %d",num);
      Write_Data_To_I2C(DEVICE_ADDR0,0x0010,buf1,20);
      Read_Data_From_I2C(DEVICE_ADDR0,0x0010,buf,20);

      /*I2C�����ϵ�Device1*/
      sprintf((char*)buf1,"FM24C64B %d",num);
      Write_Data_To_I2C(DEVICE_ADDR1,0x0010,buf1,20);
      Read_Data_From_I2C(DEVICE_ADDR1,0x0010,buf,20);

      /*I2C�����ϵ�Device2*/
      sprintf((char*)buf1,"MB85RC64V %d",num);
      Write_Data_To_I2C(DEVICE_ADDR2,0x0010,buf1,20);
      Read_Data_From_I2C(DEVICE_ADDR2,0x0010,buf,20);
      num++;
   }
}



#if 1

/**
 * ��ʼ��IIC
 */
void IIC_Init(void) {

	///ʹ��IOʱ��
//	RCC_APB2PeriphClockCmd( I2C_SCL_GPIO_PORT | I2C_SDA_GPIO_PORT, ENABLE );
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF , ENABLE);
	///SCL=1��SDA=1
	I2C_SDA_DIR_OUT();
	I2C_SDA_DAT(1);
	I2C_SCL_DIR_OUT();
}

/**
 * ����IIC��ʼ�ź�
 */
void IIC_Start(void) {
	_nop_();
	I2C_SDA_DAT(1);
	_nop_();
	I2C_SCL_DAT(1);
	_nop_();
	I2C_SDA_DAT(0);
	_nop_();
	I2C_SCL_DAT(0);
	_nop_();
//	SDA_OUT();     //sda�����
//	IIC_SDA=1;
//	IIC_SCL=1;
//	delay_us(4);
// 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low
//	delay_us(4);
//	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}

/**
 * ����IICֹͣ�ź�
 */
void IIC_Stop(void) {
	_nop_();
	I2C_SDA_DAT(0);
	_nop_();
	I2C_SCL_DAT(1);
	_nop_();
	I2C_SDA_DAT(1);
	_nop_();
//	SDA_OUT();//sda�����
//	IIC_SCL=0;
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	IIC_SCL=1;
//	IIC_SDA=1;//����I2C���߽����ź�
//	delay_us(4);
}

/**
 * �ȴ�Ӧ���źŵ���
 * return 1������Ӧ��ʧ��      0������Ӧ��ɹ�
 */
u8 IIC_Wait_Ack(void) {
	u8 ucErrTime=0;

	I2C_SDA_DIR_IN();      //SDA����Ϊ����
	I2C_SCL_DAT(1);
	_nop_();
	I2C_SCL_DAT(1);
	_nop_();
	while(I2C_SDA_READ())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	I2C_SCL_DAT(0);//ʱ�����0

//	u8 ucErrTime=0;
//	SDA_IN();      //SDA����Ϊ����
//	IIC_SDA=1;delay_us(1);
//	IIC_SCL=1;delay_us(1);
//	while(READ_SDA)
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			IIC_Stop();
//			return 1;
//		}
//	}
//	IIC_SCL=0;//ʱ�����0
	return 0;
}
//����ACKӦ��
void IIC_Ack(void) {
	I2C_SCL_DAT(0);
	_nop_();
	I2C_SDA_DAT(0);
	_nop_();
	I2C_SCL_DAT(1);
	_nop_();
	I2C_SCL_DAT(0);
	_nop_();


//	IIC_SCL=0;
//	SDA_OUT();
//	IIC_SDA=0;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void) {
	I2C_SCL_DAT(0);
	_nop_();
	I2C_SDA_DAT(1);
	_nop_();
	I2C_SCL_DAT(1);
	_nop_();
	I2C_SCL_DAT(0);
	_nop_();

//	IIC_SCL=0;
//	SDA_OUT();
//	IIC_SDA=1;
//	delay_us(2);
//	IIC_SCL=1;
//	delay_us(2);
//	IIC_SCL=0;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd) {
	u8 idx;

	I2C_SCL_DAT(0);
	_nop_();
	for (idx = 0; idx < 8; idx++) {
		if (txd & 0x80) {
			I2C_SDA_DAT(1);
		}
		else {
			I2C_SDA_DAT(0);
		}
		txd <<= 1;
		I2C_SCL_DAT(1); /*����ʱ���ź��ߣ�������������,SCL��0��Ϊ1ʱ����������*/
		_nop_();
		I2C_SCL_DAT(0);
		_nop_();
	}
//	return (ACK_From_I2C());

//    u8 t;
//	SDA_OUT();
//    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
//    for(t=0;t<8;t++)
//    {
//        IIC_SDA=(txd&0x80)>>7;
//        txd<<=1;
//		delay_us(2);
//		IIC_SCL=1;
//		delay_us(2);
//		IIC_SCL=0;
//		delay_us(2);
//    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack) {

	u8 i, receive = 0;
	I2C_SDA_DIR_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
    	I2C_SCL_DAT(0);
    	_nop_();
        I2C_SCL_DAT(1);
        receive<<=1;
        if(I2C_SDA_READ())
        	receive++;
        _nop_();
    }
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
	return receive;
}
#endif



