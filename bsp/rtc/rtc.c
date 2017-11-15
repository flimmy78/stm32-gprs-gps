/******************************************
�ļ�����: rtc.c
��	  ��: byy
��    ��:
˵    ��: ʵʱʱ������
�޸ļ�¼: 2011-08-18,��������
******************************************/
#include "ucos_ii.h"
#include "rtc.h" 




//ƽ����·����ڱ� 
const static BYTE mon_table_leep[12]  = {31,28,31,30,31,30,31,31,30,31,30,31};
//������·����ڱ�
const static BYTE mon_table_nomal[12] = {31,29,31,30,31,30,31,31,30,31,30,31};




/**********************************************************************
* ��    �ƣ�RTC_Configuration()
* ��    �ܣ�
* ��ڲ�����
* ���ڲ�����
***********************************************************************/
static void RTC_Configuration(void)
{
	WORD WaitForOscSource = 0;
	
	/* Reset Backup Domain */
	BKP_DeInit();
	
	/* Enable LSE RCC_DBCR*/
	RCC_LSEConfig(RCC_LSE_ON);
	while(++WaitForOscSource < 10000);

	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  

	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForLastTask();
	
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Enable the RTC Second */  
	RTC_ITConfig(RTC_IT_SEC, ENABLE);

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767);

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}




/**********************************************************************
* ��    �ƣ�RTC_Config()
* ��    �ܣ�
* ��ڲ����� 
* ���ڲ�����
***********************************************************************/
void RTC_Config(void)
{
	PWR_BackupAccessCmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_RTC_INIT) != 0xA5A5)
	{
		RTC_Configuration();
		SysTime = CompileTime;		
		setSysTime(&SysTime);
		BKP_WriteBackupRegister(BKP_RTC_INIT, 0xA5A5);    
	}
	else
	{
		RTC_WaitForSynchro();				/* �ȴ�RTC�Ĵ���ͬ�� */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	/* ʹ��RTC���ж� 	 */  
		RTC_WaitForLastTask();				/* �ȴ�д�����		 */
	}
	RCC_ClearFlag();						/* Clear reset flags */
}




/**********************************************************************
* ��    �ƣ�SetTime()
* ��    �ܣ�
* ��ڲ����� 
* ���ڲ�����
***********************************************************************/
BYTE setSysTime(ST_TIMER *sTime)
{
	DWORD  CounterValue;
	
	CounterValue = time_struct2sec(sTime);
	RTC_WaitForLastTask();
	RTC_SetCounter(CounterValue);
	RTC_WaitForLastTask();
	return TRUE;
}




/**********************************************************************
* ��    �ƣ�SetTime()
* ��    �ܣ�
* ��ڲ����� 
* ���ڲ�����
***********************************************************************/
BYTE getSysTime(ST_TIMER *sTime)
{ 
	DWORD TimeVar;
	
	TimeVar = RTC_GetCounter();
	time_sec2struct(TimeVar, sTime);
	return TRUE;
}
	
	
	
	
/*******************************************
��������: BYTE Is_Leap_Year(WORD year)
��������: �ж��Ƿ������꺯��
��ڲ���: ���
���ڲ���: ������ǲ�������.1����.0������
��    ע:
*******************************************/	 
static BYTE is_leap_year(WORD year) 
{    
	if(year % 400 == 0
	|| (year % 100 != 0 && year % 4 == 0))
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}



	
/*******************************************
��������: void time_sec2struct(DWORD secDec, ST_TIMER, time) 
��������: ������ת��Ϊʱ�� 
��ڲ���:
���ڲ���:
��    ע:
*******************************************/ 
void time_sec2struct(DWORD secDec, ST_TIMER* time) 
{ 
	static WORD daycnt = 0; 
	DWORD temp_day = 0; 
	WORD  temp_year = 0;
	BYTE  temp_mount = 0;

	temp_day = secDec / 86400;
	if(daycnt != temp_day)//����һ���� 
	{    
		daycnt = temp_day; 
		temp_year = 2000; 			//��2000�꿪ʼ
		while((temp_day > 365 && !is_leap_year(temp_year)) || temp_day > 366)
		{
			temp_day -= (is_leap_year(temp_year))? 366 : 365;
			temp_year++;
		}
		time->year = temp_year;
		temp_mount = 0;
		if(is_leap_year(temp_year))
		{
			while(temp_day > mon_table_leep[temp_mount])
			{
				temp_mount++;
				temp_day -= mon_table_leep[temp_mount];
			}
		}
		else
		{
			while(temp_day > mon_table_nomal[temp_mount])
			{
				temp_mount++;
				temp_day -= mon_table_nomal[temp_mount];
			}
		}
		time->month = temp_mount;
		time->day = temp_day + 1; 
	}
	
	secDec %= 86400;      
	time->hour = (secDec / 3600);     	//Сʱ 
	time->min  = (secDec / 60) % 60; 	//����  
	time->sec  = (secDec % 60); 		//����
}




/*******************************************
��������: void time_sec2struct(DWORD secDec, ST_TIMER, time) 
��������: ������ת��Ϊʱ�� 
��ڲ���:
���ڲ���:
��    ע:
*******************************************/ 
DWORD time_struct2sec(ST_TIMER* time)
{
	DWORD secDec = 0;
	WORD temp;
	
	temp = time->year;
	while(temp > 2000)
	{
		secDec += (is_leap_year(temp))? 366 : 365;
		temp--;
	}
	temp = time->month;
	while(temp > 0)
	{
		secDec += (is_leap_year(temp))? mon_table_leep[temp - 1] 
									  : mon_table_nomal[temp - 1];
		temp--;
	}
	secDec += time->day;
	secDec *= 86400;
	secDec += time->hour * 3600;
	secDec += time->min * 60;
	secDec += time->sec;
	
	return secDec;
}




/*******************************************
��������: void RTC_IRQHandler(void) 
��������: RTC �жϷ�����
��ڲ���: 
���ڲ���: 
��    ע:
*******************************************/       
void RTC_IRQHandler(void) 
{            
    OS_CPU_SR  cpu_sr; 

    OS_ENTER_CRITICAL();
    OSIntNesting++;
    OS_EXIT_CRITICAL();
	
	/* ���ж� */
	if(RTC_GetITStatus(RTC_IT_SEC) == SET)
	{
		getSysTime(&SysTime);
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
	OSIntExit();        
}
