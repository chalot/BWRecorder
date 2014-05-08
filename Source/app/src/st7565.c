/*
 * st7565.c
 *
 *  Created on: 2014年4月25日
 *      Author: ThinkPad User
 */

#if 0
/*ST7565 串行驱动程序*/
/*   串行驱动方式，唯一要注意的一点就是WR和RD这两个引脚虽然用不到，但是要接地，否则无法正常显示;

/**************************预处理*************************/
#include <reg52.h>
#include <intrins.h>

//=========================LCD接口引脚定义======================
sbit LCDA0=P2^5;  //片选信号
sbit LCDSK=P2^6;  //时钟信号
sbit LCDSI=P2^7;  //数据信号
sbit LCDRS=P2^4;  //复位信号
//=========================函数声明=============================
void LCDInit();        //初始化函数
void LCDWriteByte(char dat,char command);  //一为待写内容,二为命令/数据选择(0命令,1数据)
void LCDShiftWrite(char dat);  //逐位写一个字节
void LCDSetXY(char x, char y); //行列坐标设置
void LCDcls(char);  //清屏函数,全屏写入某个字节value
void menushow(char line,char num,char state);  //
void gbshow(char x,char y,unsigned char *p);  //
void asc816show(char x,char y,unsigned char *p);  //8x16点阵ASCII字符显示
void asc1632show(char x,char y,unsigned char *p); //16x32点阵ASCII字符显示
void asc1632bottom(char x,char state);     //???
void delayms(unsigned char ms);   //延时函数

//***************************数组列表*******************************
extern unsigned char code hzcode[][128]; //
extern unsigned char code gbcode[][32];  //
extern unsigned char code asc816code[][16]; //
extern unsigned char code asc1832code[][64];  //
extern unsigned char code arrow[]; //
//===========================主程序=================================
void main(void)
{

//显示示例显示内容,如何让其显示特定的字符串和需要的随机内容
         LCDInit();
         while(1)
         {
         asc816show(1,3,30);
         LCDSetXY(0x00,0x00);
         LCDWriteByte(0x00,1);

         LCDSetXY(0x01,0x00);
         LCDWriteByte(0x3C,1);

         LCDSetXY(0x02,0x00);
         LCDWriteByte(0x7C,1);

         LCDSetXY(0x03,0x00);
         LCDWriteByte(0xC0,1);

         LCDSetXY(0x04,0x00);
         LCDWriteByte(0xC0,1);

         LCDSetXY(0x05,0x00);
         LCDWriteByte(0x7C,1);

         LCDSetXY(0x06,0x00);
         LCDWriteByte(0x3C,1);

         LCDSetXY(0x07,0x00);
         LCDWriteByte(0x00,1);

         LCDSetXY(0x00,0x01);
         LCDWriteByte(0x00,1);

         LCDSetXY(0x01,0x01);
         LCDWriteByte(0x00,1);

         LCDSetXY(0x02,0x01);
         LCDWriteByte(0x08,1);

         LCDSetXY(0x03,0x01);
         LCDWriteByte(0x0f,1);

         LCDSetXY(0x04,0x01);
         LCDWriteByte(0x0f,1);

         LCDSetXY(0x05,0x01);
         LCDWriteByte(0x08,1);

         LCDSetXY(0x06,0x01);
         LCDWriteByte(0x00,1);

         LCDSetXY(0x07,0x01);
         LCDWriteByte(0x00,1);


         }
}


//========================液晶显示模块初始化=================================
void LCDInit()
{
         LCDRS=0;       //LCD 复位有效(L)
         delayms(100); // 延时100ms , Datasheet 要求至少大于1us
         LCDRS = 1;    //LCD 复位无效(H)

delayms(100); //硬件复位

         LCDWriteByte(0xe2,0);                  //寄存器复位

delayms(100);

//  电源管理初始

LCDWriteByte(0xaf,0);                  //0xaf显示器开,0xae显示器关

LCDWriteByte(0x2f,0);                  //0x2f升压电路,电压管理电路,
//电压跟随电路均开(0x28-0x2f为设置上电控制模式)


LCDWriteByte(0x26,0);                  //0x20-0x27为V5电压内部电阻调整设置
         LCDWriteByte(0x81,0);                  //电量设置模式
         LCDWriteByte(0x0f,0);                  //0x01-0x3f电量寄存器设置模式



         // 根据屏的方向初始化安装方向(复位后默认下面配置)

         LCDWriteByte(0xa0,0);                  //0xa0为Segment正向,0xa1 为Segment反向

//(0xa0-0xa1为ADC方向选择)
         LCDWriteByte(0xc0,0);                  //0xc0正向扫描,0xc8反射扫描

// 显示管理初始(复位后默认下面配置)
         LCDWriteByte(0xa6,0);                  //0xa6正向显示,0xa7反向显示
         LCDWriteByte(0xa4,0);                  //0xa4正常显示,0xa5全屏点亮



// 指示器设置

LCDWriteByte(0xac,0);                  //

LCDWriteByte(0x00,0);                  //

//背压比设置

LCDWriteByte(0xf8,0);                  //
LCDWriteByte(0x00,0);                  //


         LCDcls(0);                             //屏上的点都不点亮,全屏写入0


}

//========================写一个字节(命令或显示数据)=============================
void LCDWriteByte(char dat,char command)
{
//                 LCDCS = 0;                         //CS=0.注:因只使用一片CS,已将其直接接地.
         if(command == 0)
         {
                 LCDA0 = 0;                           //A0=0,为写命令
                 LCDShiftWrite(dat);
         }
         else
         {
                 LCDA0 = 1;                          //A0=1,为写数据
                 LCDShiftWrite(dat);
         }
//        LCDCS = 1;                                 //CS=1.注:因只使用一片CS,已将其直接接地.
}

//===========================逐位(串行)写完一个字节================================
void LCDShiftWrite(char dat)
{
     unsigned char i;                                      //定义局部变量
     unsigned char Series,Temp;                           //定义局部变量

     LCDSK = 0;                                           //SCL=0
     Series = dat;
     for(i=0; i<8; i++)
     {
                 LCDSK = 0;                              //SCL=0
                 Temp=Series & 0x80;                    //屏蔽低7位
                 if(Temp)                               //如果Temp=1,则使SI=1.如果Temp+0,则使SI=0
                 {
                         LCDSI = 1;                    //SI=1
                 }
                 else
                 {
                         LCDSI = 0;                   //SI=0
                 }
                 LCDSK = 1;                          //SCL=1上升沿写
                 Series = Series << 1;               //左移1位
     }
}
#endif
