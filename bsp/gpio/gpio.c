/******************************************
�ļ�����: IO_set.c
��	  ��: byy
��    ��:
˵    ��: ������ʼ����IO�����룩
�޸ļ�¼: 2011-08-18,��������
******************************************/
#include <stm32f10x.h> 
#include "type.h" 
#include "gpio.h" 




/*******************************************
��������: GPIO_Configuration(void)
��������: ����������˿ڳ�ʼ��
��ڲ���:
���ڲ���: 
��    ע:
*******************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* LED����ָʾ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED_FLASH_GPIO;
	GPIO_Init(LED_FLASH_PORT, &GPIO_InitStructure);
	
	/* GSMģ�鹩�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GSM_Power_CTL_GPIO;
	GPIO_Init(GSM_Power_CTL_PORT, &GPIO_InitStructure);
	/* GSMģ��ON/OFF */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GSM_Power_ON_GPIO;
	GPIO_Init(GSM_Power_ON_PORT, &GPIO_InitStructure);
	
	/* GPS�������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPS_STANDBY_GPIO;
	GPIO_Init(GPS_STANDBY_PORT, &GPIO_InitStructure);
	/* GPS��λ���� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPS_RESET_GPIO;
	GPIO_Init(GPS_RESET_PORT, &GPIO_InitStructure);
	
	/* ﮵��������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BATTERY_CTRL_GPIO;
	GPIO_Init(BATTERY_CTRL_PORT, &GPIO_InitStructure);
	
	/* �ⲿ��Դ��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Power_BATT_GPIO;
	GPIO_Init(Power_BATT_PORT, &GPIO_InitStructure);
	
	/* ���߱������ */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Shear_Alarm_GPIO;
	GPIO_Init(Shear_Alarm_PORT, &GPIO_InitStructure);
	
	/* �γ�������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Pull_Out_GPIO;
	GPIO_Init(Pull_Out_PORT, &GPIO_InitStructure);
		
	/* ����1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ����2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ����3 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}




/***************************************************
��    �ܣ�GPIO�������
��ڲ�����GPIO_NUM GPIOx,   GPIO�˿ڱ��
		  DWORD	pinNum,		�ܽű��
		  BYTE DP,		   	��ƽ
�� �� ֵ����
��    ע��
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




