/******************************************
文件名称: IO_set.c
作	  者: byy
版    本:
说    明: 按键初始化（IO口输入）
修改记录: 2011-08-18,程序建立。
******************************************/
#include <stm32f10x.h> 
#include "type.h" 
#include "gpio.h" 




/*******************************************
函数名称: GPIO_Configuration(void)
函数功能: 开关量输入端口初始化
入口参数:
出口参数: 
备    注:
*******************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* LED工作指示灯 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED_FLASH_GPIO;
	GPIO_Init(LED_FLASH_PORT, &GPIO_InitStructure);
	
	/* GSM模块供电 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GSM_Power_CTL_GPIO;
	GPIO_Init(GSM_Power_CTL_PORT, &GPIO_InitStructure);
	/* GSM模块ON/OFF */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GSM_Power_ON_GPIO;
	GPIO_Init(GSM_Power_ON_PORT, &GPIO_InitStructure);
	
	/* GPS休眠引脚 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPS_STANDBY_GPIO;
	GPIO_Init(GPS_STANDBY_PORT, &GPIO_InitStructure);
	/* GPS复位引脚 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPS_RESET_GPIO;
	GPIO_Init(GPS_RESET_PORT, &GPIO_InitStructure);
	
	/* 锂电池输出控制 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BATTERY_CTRL_GPIO;
	GPIO_Init(BATTERY_CTRL_PORT, &GPIO_InitStructure);
	
	/* 外部电源检测 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Power_BATT_GPIO;
	GPIO_Init(Power_BATT_PORT, &GPIO_InitStructure);
	
	/* 剪线报警检测 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Shear_Alarm_GPIO;
	GPIO_Init(Shear_Alarm_PORT, &GPIO_InitStructure);
	
	/* 拔出报警检测 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Pull_Out_GPIO;
	GPIO_Init(Pull_Out_PORT, &GPIO_InitStructure);
		
	/* 串口1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 串口2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 串口3 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}




/***************************************************
功    能：GPIO输出控制
入口参数：GPIO_NUM GPIOx,   GPIO端口编号
		  DWORD	pinNum,		管脚编号
		  BYTE DP,		   	电平
返 回 值：无
备    注：
***************************************************/
void GPIO_out(GPIO_TypeDef* pinPort, DWORD pinNum, BYTE DP)
{
	if(DP == 0)
	{
		GPIO_ResetBits(pinPort, pinNum); 
	}
	else
	{
		GPIO_SetBits(pinPort, pinNum); 
	}
}




