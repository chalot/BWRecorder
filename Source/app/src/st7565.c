/*
 * st7565.c
 *
 *  Created on: 2014��4��25��
 *      Author: ThinkPad User
 */

#if 0
/*ST7565 ������������*/
/*   ����������ʽ��ΨһҪע���һ�����WR��RD������������Ȼ�ò���������Ҫ�ӵأ������޷�������ʾ;

/**************************Ԥ����*************************/
#include <reg52.h>
#include <intrins.h>

//=========================LCD�ӿ����Ŷ���======================
sbit LCDA0=P2^5;  //Ƭѡ�ź�
sbit LCDSK=P2^6;  //ʱ���ź�
sbit LCDSI=P2^7;  //�����ź�
sbit LCDRS=P2^4;  //��λ�ź�
//=========================��������=============================
void LCDInit();        //��ʼ������
void LCDWriteByte(char dat,char command);  //һΪ��д����,��Ϊ����/����ѡ��(0����,1����)
void LCDShiftWrite(char dat);  //��λдһ���ֽ�
void LCDSetXY(char x, char y); //������������
void LCDcls(char);  //��������,ȫ��д��ĳ���ֽ�value
void menushow(char line,char num,char state);  //
void gbshow(char x,char y,unsigned char *p);  //
void asc816show(char x,char y,unsigned char *p);  //8x16����ASCII�ַ���ʾ
void asc1632show(char x,char y,unsigned char *p); //16x32����ASCII�ַ���ʾ
void asc1632bottom(char x,char state);     //???
void delayms(unsigned char ms);   //��ʱ����

//***************************�����б�*******************************
extern unsigned char code hzcode[][128]; //
extern unsigned char code gbcode[][32];  //
extern unsigned char code asc816code[][16]; //
extern unsigned char code asc1832code[][64];  //
extern unsigned char code arrow[]; //
//===========================������=================================
void main(void)
{

//��ʾʾ����ʾ����,���������ʾ�ض����ַ�������Ҫ���������
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


//========================Һ����ʾģ���ʼ��=================================
void LCDInit()
{
         LCDRS=0;       //LCD ��λ��Ч(L)
         delayms(100); // ��ʱ100ms , Datasheet Ҫ�����ٴ���1us
         LCDRS = 1;    //LCD ��λ��Ч(H)

delayms(100); //Ӳ����λ

         LCDWriteByte(0xe2,0);                  //�Ĵ�����λ

delayms(100);

//  ��Դ�����ʼ

LCDWriteByte(0xaf,0);                  //0xaf��ʾ����,0xae��ʾ����

LCDWriteByte(0x2f,0);                  //0x2f��ѹ��·,��ѹ�����·,
//��ѹ�����·����(0x28-0x2fΪ�����ϵ����ģʽ)


LCDWriteByte(0x26,0);                  //0x20-0x27ΪV5��ѹ�ڲ������������
         LCDWriteByte(0x81,0);                  //��������ģʽ
         LCDWriteByte(0x0f,0);                  //0x01-0x3f�����Ĵ�������ģʽ



         // �������ķ����ʼ����װ����(��λ��Ĭ����������)

         LCDWriteByte(0xa0,0);                  //0xa0ΪSegment����,0xa1 ΪSegment����

//(0xa0-0xa1ΪADC����ѡ��)
         LCDWriteByte(0xc0,0);                  //0xc0����ɨ��,0xc8����ɨ��

// ��ʾ�����ʼ(��λ��Ĭ����������)
         LCDWriteByte(0xa6,0);                  //0xa6������ʾ,0xa7������ʾ
         LCDWriteByte(0xa4,0);                  //0xa4������ʾ,0xa5ȫ������



// ָʾ������

LCDWriteByte(0xac,0);                  //

LCDWriteByte(0x00,0);                  //

//��ѹ������

LCDWriteByte(0xf8,0);                  //
LCDWriteByte(0x00,0);                  //


         LCDcls(0);                             //���ϵĵ㶼������,ȫ��д��0


}

//========================дһ���ֽ�(�������ʾ����)=============================
void LCDWriteByte(char dat,char command)
{
//                 LCDCS = 0;                         //CS=0.ע:��ֻʹ��һƬCS,�ѽ���ֱ�ӽӵ�.
         if(command == 0)
         {
                 LCDA0 = 0;                           //A0=0,Ϊд����
                 LCDShiftWrite(dat);
         }
         else
         {
                 LCDA0 = 1;                          //A0=1,Ϊд����
                 LCDShiftWrite(dat);
         }
//        LCDCS = 1;                                 //CS=1.ע:��ֻʹ��һƬCS,�ѽ���ֱ�ӽӵ�.
}

//===========================��λ(����)д��һ���ֽ�================================
void LCDShiftWrite(char dat)
{
     unsigned char i;                                      //����ֲ�����
     unsigned char Series,Temp;                           //����ֲ�����

     LCDSK = 0;                                           //SCL=0
     Series = dat;
     for(i=0; i<8; i++)
     {
                 LCDSK = 0;                              //SCL=0
                 Temp=Series & 0x80;                    //���ε�7λ
                 if(Temp)                               //���Temp=1,��ʹSI=1.���Temp+0,��ʹSI=0
                 {
                         LCDSI = 1;                    //SI=1
                 }
                 else
                 {
                         LCDSI = 0;                   //SI=0
                 }
                 LCDSK = 1;                          //SCL=1������д
                 Series = Series << 1;               //����1λ
     }
}
#endif
