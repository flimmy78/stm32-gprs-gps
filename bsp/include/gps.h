#ifndef __GPS_H_ 
#define __GPS_H_ 
#include "stm32f10x.h"
#include "type.h"





typedef struct Gps_info_struct	
{
	BYTE status;
	ST_TIMER time;
	DWORD time_dec;
	BYTE ant_status;
	BYTE satellites;
	BYTE ew;
	BYTE ns;
	DWORD lo_data;
	DWORD la_data;
	WORD speed;			// 1/100km/h
	WORD dir;
	BYTE hdop;
	WORD hight;			//¸ß¶È
	BYTE bds_sate;
	BYTE gps_sate;
}Gps_info_ST;
extern Gps_info_ST Gps_info;

#endif

