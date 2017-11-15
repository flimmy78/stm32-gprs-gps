/******************************************
文件名称: rtc.c
作	  者: byy
版    本:
说    明: 实时时钟配置
修改记录: 2011-08-18,程序建立。
******************************************/
#include "ucos_ii.h"
#include "rtc.h" 




//平年的月份日期表 
const static BYTE mon_table_leep[12]  = {31,28,31,30,31,30,31,31,30,31,30,31};
//润年的月份日期表
const static BYTE mon_table_nomal[12] = {31,29,31,30,31,30,31,31,30,31,30,31};




/**********************************************************************
* 名    称：RTC_Configuration()
* 功    能：
* 入口参数：
* 出口参数：
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
* 名    称：RTC_Config()
* 功    能：
* 入口参数： 
* 出口参数：
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
		RTC_WaitForSynchro();				/* 等待RTC寄存器同步 */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	/* 使能RTC秒中断 	 */  
		RTC_WaitForLastTask();				/* 等待写入完成		 */
	}
	RCC_ClearFlag();						/* Clear reset flags */
}




/**********************************************************************
* 名    称：SetTime()
* 功    能：
* 入口参数： 
* 出口参数：
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
* 名    称：SetTime()
* 功    能：
* 入口参数： 
* 出口参数：
***********************************************************************/
BYTE getSysTime(ST_TIMER *sTime)
{ 
	DWORD TimeVar;
	
	TimeVar = RTC_GetCounter();
	time_sec2struct(TimeVar, sTime);
	return TRUE;
}
	
	
	
	
/*******************************************
函数名称: BYTE Is_Leap_Year(WORD year)
函数功能: 判断是否是闰年函数
入口参数: 年份
出口参数: 该年份是不是闰年.1，是.0，不是
备    注:
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
函数名称: void time_sec2struct(DWORD secDec, ST_TIMER, time) 
函数功能: 整数秒转化为时间 
入口参数:
出口参数:
备    注:
*******************************************/ 
void time_sec2struct(DWORD secDec, ST_TIMER* time) 
{ 
	static WORD daycnt = 0; 
	DWORD temp_day = 0; 
	WORD  temp_year = 0;
	BYTE  temp_mount = 0;

	temp_day = secDec / 86400;
	if(daycnt != temp_day)//超过一天了 
	{    
		daycnt = temp_day; 
		temp_year = 2000; 			//从2000年开始
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
	time->hour = (secDec / 3600);     	//小时 
	time->min  = (secDec / 60) % 60; 	//分钟  
	time->sec  = (secDec % 60); 		//秒钟
}




/*******************************************
函数名称: void time_sec2struct(DWORD secDec, ST_TIMER, time) 
函数功能: 整数秒转化为时间 
入口参数:
出口参数:
备    注:
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
函数名称: void RTC_IRQHandler(void) 
函数功能: RTC 中断服务函数
入口参数: 
出口参数: 
备    注:
*******************************************/       
void RTC_IRQHandler(void) 
{            
    OS_CPU_SR  cpu_sr; 

    OS_ENTER_CRITICAL();
    OSIntNesting++;
    OS_EXIT_CRITICAL();
	
	/* 秒中断 */
	if(RTC_GetITStatus(RTC_IT_SEC) == SET)
	{
		getSysTime(&SysTime);
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
	OSIntExit();        
}
