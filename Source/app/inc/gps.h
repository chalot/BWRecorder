/**   
 * @Title: gps.h  
 * @Description: TODO(��һ�仰�������ļ���ʲô) 
 * @author Zhengd.Gao zhengdgao@163.com   
 * @date 2013-7-29 ����10:33:01 
 * @version V1.0   
 */

#ifndef GPS_H_
#define GPS_H_

#include <stm32f2xx_conf.h>
#include "stm32_gpio.h"
#include "type.h"

/*=====     GPS��Ϣ����    =====*/
#define		_GPSINFO_GPS_VALID					0x01		//GPS��Ч
#define		_GPSINFO_HEADING_VALID				0x02		//GPS��Ϣ������Ч
#define		_GPSINFO_SPEED_VALID				0x04		//GPS��Ϣ�ٶ���Ч
#define		_GPSINFO_ALTITUDE_VALID				0x08		//GPS��Ϣ�߶���Ч
#define		_GPSINFO_SATNUM_VALID				0x10		//GPS��Ϣ��������Ч
#define		_GPSINFO_SATINFO_VALID				0x20		//GPS��Ϣ�ɼ�������Ч
#define		_GPSINFO_FIRSTTIME					0x80		//GPS-�״�


typedef struct {
	u16 u16GpsInfoValid;		//��Ϣ��Ч��
	u8 Latitude[4];			//γ�ȣ���λ=10^-3��
	u8 Longtitude[4];			//���ȣ���λ=10^-3��
	u8 Altitude[2];			//���θ߶ȣ���λ=��
	u8 Speed[2];				//�ٶȣ���λ=10��/ʱ
	u8 Direction[2];				//���򣬵�λ=��
	s32 s32Latitude;			//γ�ȣ���λ=10^-3��
	s32 s32Longitude;			//���ȣ���λ=10^-3��
	s16 s16Altitude;			//���θ߶ȣ���λ=��
	u16 u16Speed;				//�ٶȣ���λ=10��/ʱ
	u16 u16Heading;				//���򣬵�λ=��
	u8 u8ValidSatellites;		//����ʹ��������
	u8 u8VisibleSatellites;	//�ɼ�������
	u16 u16Year;				//��ǰȡ�õ�GPS��Ϣ-��
	u8 u8Month;				//��ǰȡ�õ�GPS��Ϣ-��
	u8 u8Day;					//��ǰȡ�õ�GPS��Ϣ-��
	u8 u8Hour;					//��ǰȡ�õ�GPS��Ϣ-ʱ
	u8 u8Minute;				//��ǰȡ�õ�GPS��Ϣ-��
	u8 u8Second;				//��ǰȡ�õ�GPS��Ϣ-��
	u8 bAntenaState;			//����״̬��1=�γ���0=����
} _tGPSInfo;


/*********************** GPSģ���Դ���� *****************************************************/
#define GPS_POWER_ON	GPIO_SetBits(GPIOC,GPIOC_GPS_CTRL_)  //ģ�鿪��
#define GPS_POWER_OFF	GPIO_ResetBits(GPIOC,GPIOC_GPS_CTRL_) //ģ��ص�//����ָʾ�ƴ�/�ر�
#define GPS_LED_OFF  	GPIO_SetBits(GPIOC, GPIOC_GPS_LED)
#define GPS_LED_ON  	GPIO_ResetBits(GPIOC, GPIOC_GPS_LED)

//LEDָʾ����˸
#define GPS_FLASH_LED  \
                        GPIO_WriteBit(GPIOC, GPIOC_GPS_LED,   \
                        (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOC, GPIOC_GPS_LED))))

//����GPS_M-RST����
#define GPS_RST_PULLDOWN  	GPIO_ResetBits(GPIOC, GPIOC_GPS_RST_)
#define GPS_RST_PULLUP  	GPIO_SetBits(GPIOC, GPIOC_GPS_RST_)

//GPS���߼�⣬�ߵ�ƽΪ�γ����͵�ƽΪ���룬ʹ��CAN 5V����
#define GPS_GETANTENASTATE() 	GPIO_ReadInputDataBit(GPIOC,GPIOC_GPSANT_DET_)

#define ANTENA_OK			0  //����״̬������
#define ANTENA_ERR			1  //����״̬������/*********************** �������� *****************************************************/

void GPS_Init(); //�ڴ��ʼ��
void GPS_SetAntenaState(u8); //��������״̬
u8 GPS_GetAntenaState(); //��ȡ����״̬
void GPS_ReportGpsInfo(); //�ϱ�GPS��λ��Ϣ
_tGPSInfo* GPS_GetInfo(); //��ȡ��ǰGPS״̬
BOOL GPS_IsLocated();	//�Ƿ�λ�ɹ�
void TRACE_GPSINFO(/*_tGPSInfo *pGpsInfo*/); //��ӡGPS��Ϣ
void GPS_DumpInfo();	//�洢GPS����
void GPS_Lowlevel_Init();

void ISR_UART_GPS(void); //�����ж����

#endif /* GPS_H_ */
