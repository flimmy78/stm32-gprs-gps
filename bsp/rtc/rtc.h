#ifndef __RTC_H 
#define __RTC_H 
#include "stm32f10x.h"
#include "type.h"




#define	BKP_RTC_INIT		BKP_DR1


 
extern ST_TIMER CompileTime;
extern ST_TIMER SysTime;
extern DWORD SysTime_dec;
extern void  RTC_Config(void);
extern BYTE  setSysTime(ST_TIMER *sTime);
extern BYTE  getSysTime(ST_TIMER *sTime);
extern void  time_sec2struct(DWORD secDec, ST_TIMER* time);
extern DWORD time_struct2sec(ST_TIMER* time);
#endif

