/******************************************
文件名称: GPIO.h
作	  者: byy
版    本:
说    明: 开关量输入引脚定义
修改记录: 2011-08-18,程序建立。
******************************************/
#ifndef __GPIO_H_
#define __GPIO_H_
#include "stm32f10x.h" 
#include "type.h"   



/* GSM 相关引脚定义 */
#define	GSM_Power_CTL_PORT		GPIOB
#define	GSM_Power_CTL_GPIO		GPIO_Pin_12
#define	GSM_Power_ON_PORT		GPIOA
#define	GSM_Power_ON_GPIO		GPIO_Pin_6

/* GPS 相关引脚 */
#define	GPS_STANDBY_PORT		GPIOA
#define	GPS_STANDBY_GPIO		GPIO_Pin_4
#define	GPS_RESET_PORT			GPIOA
#define	GPS_RESET_GPIO			GPIO_Pin_5

/* LED指示灯 */
#define	LED_FLASH_PORT			GPIOA
#define	LED_FLASH_GPIO			GPIO_Pin_0

/* 锂电池控制 */
#define	BATTERY_CTRL_PORT		GPIOB
#define	BATTERY_CTRL_GPIO		GPIO_Pin_14

/* 外部电源检测引脚 */
#define	Power_BATT_PORT			GPIOB
#define	Power_BATT_GPIO			GPIO_Pin_13

/* 剪线报警线 */
#define Shear_Alarm_PORT		GPIOA
#define Shear_Alarm_GPIO		GPIO_Pin_7

/* 拔出报警线 */
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
/* 锂电池控制 */
#define Battery_control(dp)		GPIO_out(BATTERY_CTRL_PORT, BATTERY_CTRL_GPIO, dp)
/* 外部电源检测 */
#define POWER_OK				(GPIO_ReadInputDataBit(Power_BATT_PORT, Power_BATT_GPIO))	//外部电源
#define Shear_Alarm				(GPIO_ReadInputDataBit(Shear_Alarm_PORT, Shear_Alarm_GPIO))	//剪线报警
#define PULL_OUT				(GPIO_ReadInputDataBit(Pull_Out_PORT, Pull_Out_GPIO))		//拔出报警




extern void GPIO_Configuration(void);
extern void GPIO_out(GPIO_TypeDef* pinPort, DWORD pinNum, BYTE DP);



#endif 
