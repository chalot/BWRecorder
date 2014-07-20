#include "myiic.h"
#include <stm32f2xx_conf.h>
#include <utility.h>


/*
** 注意：如果硬件中使用写保护引脚，请在头文件中宏定义“USE_WP_PIN”；
**       否则，请注释掉该语句！！
*/

/*------I2C operate--------------------------------------------------*/
/*
*-------------------------------------------------------------------------------
* 函数功能： I2C总线延时
* 输入数据： 无
* 返 回 值： 无
* 注    意： I2C总线延时可以根据自己的MCU运行时钟频率进行适当的调整
*-------------------------------------------------------------------------------
*/
#define _nop_()      Delay_us(2)        /*在此定义你I2C总线的延时*/
void Delay_us(unsigned int t)
{
 //  t *=4;
   while(--t);
}

/*
*-------------------------------------------------------------------------------
* 函数功能： IO初始化
* 输入数据： 无
* 返 回 值： 无
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
* 函数功能： I2C总线初始化
* 输入数据： 无
* 返 回 值： 无
*-------------------------------------------------------------------------------
*/
void InitI2C(void)
{
   I2C_SCL_DAT(0);
	I2CStop();
}

/*
*-------------------------------------------------------------------------------
* 函数功能： 发送总线起始信号
* 输入数据： 无
* 返 回 值： 无
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
* 函数功能： 发送总线结束信号
* 输入数据： 无
* 返 回 值： 无
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
* 函数功能： 应答，发送总线时钟信号,并返回时钟电平为高期间SDA上的
             状态,可用于向I2C总线发送数据.
* 输入数据： 无
* 返 回 值： ACKCondition，返回SCL为高电平期间SDA的状态
*-------------------------------------------------------------------------------
*/
unsigned char ACK_From_I2C(void)
{
   unsigned char ACKCondition;

   _nop_();
   I2C_SCL_DAT (1);
   I2C_SDA_DIR_IN();                   /*数据引脚配置为输入，准备从总线上读取应答信号*/
	_nop_();
	I2C_SCL_DAT (1);
	_nop_();
	if(I2C_SDA_READ()){
		ACKCondition = 1;                /*未读到应答信号*/
	}
	else{
		ACKCondition = 0;                /*接收到应答信号*/
	}
	I2C_SCL_DAT (0);
   I2C_SDA_DIR_OUT();                  /*配置SDA为输出*/

   return(ACKCondition);
}

/*
*-------------------------------------------------------------------------------
* 函数功能： 向I2C总线发送应答信号，用于主控接收
* 输入数据： 无
* 返 回 值： 无
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

/*发送非应答信号*/
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
* 函数功能： 向I2C总线发送一个字节的数据，并读取总线设备的应答信号
* 输入数据： I2CData，将要发送到I2C总线的数据
* 返 回 值： ACK_From_I2C()，总线上的设备的应答信号，如果发送成功返回0，反之返回1
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
    I2C_SCL_DAT (1);        /*拉高时钟信号线，锁存总线数据*/
    _nop_();
    I2C_SCL_DAT (0);
    _nop_();
  }
  //I2C_SDA_DAT (1);
  return(ACK_From_I2C());
}

/*
*-------------------------------------------------------------------------------
* 函数功能： 从I2C总线接收1个字节,并返回该字节,不回送应答信号
* 输入数据： 无
* 返 回 值： I2CData，从总线读取的数据
*-------------------------------------------------------------------------------
*/
unsigned char I2CReceiveByte(void)
{
   unsigned char I2CData = 0;
	unsigned char idx;

   I2C_SDA_DIR_IN();                      /*配置数据引脚为输入引脚*/
   _nop_();
	for(idx = 0; idx < 8; idx++){
      I2C_SCL_DAT (0);
      _nop_();
      I2C_SCL_DAT (1);                    /*拉高时钟信号，锁存总线状态*/
      if(I2C_SDA_READ()){
         I2CData |= (0x80 >> idx);
      }
      _nop_();
      I2C_SCL_DAT (0);
      _nop_();
	}
   I2C_SCL_DAT (0);
   I2C_SDA_DIR_OUT();                     /*配置数据引脚为输出引脚*/
	return(I2CData);
}

/*
*-------------------------------------------------------------------------------
* 函数功能： 向I2C总线上的器件发送数据
* 输入数据： Device_Addr，I2C总线上设备的地址
*            I2C_Adr，设备的存储区域地址
*            *DataPtr，将要发送给设备的数据
*            Data_Size，将要发送数据的长度
* 返 回 值： 无
*-------------------------------------------------------------------------------
*/
unsigned char Write_Data_To_I2C(unsigned char Device_Addr, unsigned int I2C_Adr, unsigned char *DataPtr,unsigned int Data_Size)
{
#ifdef USE_WP_PIN
	I2C_WP_DAT (0);
#endif
	I2CStart();
   if(I2CSendByte(E_WRITE | ((Device_Addr&0x07)<<1))) return FALSE; /*发送8位数据，如果得不到I2C总线设备的应答信号则直接返回*/
	if(I2CSendByte(I2C_Adr >> 8)) return FALSE;
	if(I2CSendByte(I2C_Adr & 0x00FF)) return FALSE;

	while(Data_Size --){
		if(I2CSendByte(*DataPtr)) return FALSE;
		DataPtr ++;
	}
	I2CStop();          /*发送停止信号*/
#ifdef USE_WP_PIN
	I2C_WP_DAT (1);
#endif
   return TRUE;
}


/*
*-------------------------------------------------------------------------------
* 函数功能： 读取从I2C总线上的设备发送过来的数据
* 输入数据： Device_Addr，I2C总线上设备的地址
*            I2C_Adr，设备的存储区域地址
*            *DataPtr，从总线上读取的数据将保存在该指针指向的存储区域
*            Data_Size，读取数据的长度
* 返 回 值： 无
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
* 函数功能： 测试函数
* 输入数据： 无
* 返 回 值： 无
* 说    明： 将读取的数据存储在buf中，可以通过数据观察窗口查看buf中的数据
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
     /*I2C总线上的Device0*/
      sprintf((char*)buf1,"FM24C64A %d",num);
      Write_Data_To_I2C(DEVICE_ADDR0,0x0010,buf1,20);
      Read_Data_From_I2C(DEVICE_ADDR0,0x0010,buf,20);

      /*I2C总线上的Device1*/
      sprintf((char*)buf1,"FM24C64B %d",num);
      Write_Data_To_I2C(DEVICE_ADDR1,0x0010,buf1,20);
      Read_Data_From_I2C(DEVICE_ADDR1,0x0010,buf,20);

      /*I2C总线上的Device2*/
      sprintf((char*)buf1,"MB85RC64V %d",num);
      Write_Data_To_I2C(DEVICE_ADDR2,0x0010,buf1,20);
      Read_Data_From_I2C(DEVICE_ADDR2,0x0010,buf,20);
      num++;
   }
}



#if 1

/**
 * 初始化IIC
 */
void IIC_Init(void) {

	///使能IO时钟
//	RCC_APB2PeriphClockCmd( I2C_SCL_GPIO_PORT | I2C_SDA_GPIO_PORT, ENABLE );
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF , ENABLE);
	///SCL=1，SDA=1
	I2C_SDA_DIR_OUT();
	I2C_SDA_DAT(1);
	I2C_SCL_DIR_OUT();
}

/**
 * 产生IIC起始信号
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
//	SDA_OUT();     //sda线输出
//	IIC_SDA=1;
//	IIC_SCL=1;
//	delay_us(4);
// 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low
//	delay_us(4);
//	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}

/**
 * 产生IIC停止信号
 */
void IIC_Stop(void) {
	_nop_();
	I2C_SDA_DAT(0);
	_nop_();
	I2C_SCL_DAT(1);
	_nop_();
	I2C_SDA_DAT(1);
	_nop_();
//	SDA_OUT();//sda线输出
//	IIC_SCL=0;
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	IIC_SCL=1;
//	IIC_SDA=1;//发送I2C总线结束信号
//	delay_us(4);
}

/**
 * 等待应答信号到来
 * return 1，接收应答失败      0，接收应答成功
 */
u8 IIC_Wait_Ack(void) {
	u8 ucErrTime=0;

	I2C_SDA_DIR_IN();      //SDA设置为输入
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
	I2C_SCL_DAT(0);//时钟输出0

//	u8 ucErrTime=0;
//	SDA_IN();      //SDA设置为输入
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
//	IIC_SCL=0;//时钟输出0
	return 0;
}
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
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
		I2C_SCL_DAT(1); /*拉高时钟信号线，锁存总线数据,SCL由0变为1时，锁存数据*/
		_nop_();
		I2C_SCL_DAT(0);
		_nop_();
	}
//	return (ACK_From_I2C());

//    u8 t;
//	SDA_OUT();
//    IIC_SCL=0;//拉低时钟开始数据传输
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
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack) {

	u8 i, receive = 0;
	I2C_SDA_DIR_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
	return receive;
}
#endif



