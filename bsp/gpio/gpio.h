/******************************************
�ļ�����: GPIO.h
��	  ��: byy
��    ��:
˵    ��: �������������Ŷ���
�޸ļ�¼: 2011-08-18,��������
******************************************/
#ifndef __GPIO_H_
#define __GPIO_H_
#include "stm32f10x.h" 
#include "type.h"   



/* GSM ������Ŷ��� */
#define	GSM_Power_CTL_PORT		GPIOB
#define	GSM_Power_CTL_GPIO		GPIO_Pin_12
#define	GSM_Power_ON_PORT		GPIOA
#define	GSM_Power_ON_GPIO		GPIO_Pin_6

/* GPS ������� */
#define	GPS_STANDBY_PORT		GPIOA
#define	GPS_STANDBY_GPIO		GPIO_Pin_4
#define	GPS_RESET_PORT			GPIOA
#define	GPS_RESET_GPIO			GPIO_Pin_5

/* LEDָʾ�� */
#define	LED_FLASH_PORT			GPIOA
#define	LED_FLASH_GPIO			GPIO_Pin_0

/* ﮵�ؿ��� */
#define	BATTERY_CTRL_PORT		GPIOB
#define	BATTERY_CTRL_GPIO		GPIO_Pin_14

/* �ⲿ��Դ������� */
#define	Power_BATT_PORT			GPIOB
#define	Power_BATT_GPIO			GPIO_Pin_13

/* ���߱����� */
#define Shear_Alarm_PORT		GPIOA
#define Shear_Alarm_GPIO		GPIO_Pin_7

/* �γ������� */
#define Pull_Out_PORT			GPIOB
#define Pull_Out_GPIO			GPIO_Pin_0



/* GSM */
#define GSM_POWER_OFF()			GPIO_ResetBits(GSM_Power_CTL_PORT, GSM_Power_CTL_GPIO)
#define GSM_POWER_ON() 			GPIO_SetBits(GSM_Power_CTL_PORT, GSM_Power_CTL_GPIO)
#define GSM_OFF()				GPIO_ResetBits(GSM_Power_ON_PORT, GSM_Power_ON_GPIO)
#define GSM_ON() 				GPIO_SetBits(GSM_Power_ON_PORT, GSM_Power_ON_GPIO)
/* GPS */
#define GPS_standby(dp)			GPIO_out(GPS_STANDBY_PORT, GPS_STANDBY_GPIO, dp)
#define GPS_reset(dp)			GPIO_out(GPS_RESET_PORT, GPS_RESET_GPIO, dp)
/* LED */
#define WORK_LED(dp)			GPIO_out(LED_FLASH_PORT, LED_FLASH_GPIO, (dp+1)%2)
/* ﮵�ؿ��� */
#define Battery_control(dp)		GPIO_out(BATTERY_CTRL_PORT, BATTERY_CTRL_GPIO, dp)
/* �ⲿ��Դ��� */
#define POWER_OK				(GPIO_ReadInputDataBit(Power_BATT_PORT, Power_BATT_GPIO))	//�ⲿ��Դ
#define Shear_Alarm				(GPIO_ReadInputDataBit(Shear_Alarm_PORT, Shear_Alarm_GPIO))	//���߱���
#define PULL_OUT				(GPIO_ReadInputDataBit(Pull_Out_PORT, Pull_Out_GPIO))		//�γ�����




extern void GPIO_Configuration(void);
extern void GPIO_out(GPIO_TypeDef* pinPort, DWORD pinNum, BYTE DP);



#endif 
