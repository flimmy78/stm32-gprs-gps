/*
�����ļ�����ȫ�ֱ���
��ҪΪ�豸��ǰ״̬����Ϣ
*/
#ifndef __ALL_USE_H_ 
#define __ALL_USE_H_
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include "stm32f10x.h"
#include "ucos_ii.h"
#include "type.h"
#include "system.h"  
#include "usart.h"
#include "rtc.h"
#include "adc.h"
#include "iwdg.h"
#include "flash.h"
#include "timer.h"
#include "basic.h"
#include "gps.h" 
#include "gsm_m26.h"
#include "sms_rsv.h"
#include "jt_t808.h"
#include "gpio.h"
#include "gbk_utf.h"
#include "server_ctrl.h"






#define __IO			volatile 	/*��д��־λ*/


/* ϵͳ�¼��б� */
extern OS_EVENT* mbox_at_answer;		// AT�����Ӧ����
extern OS_EVENT* mbox_at_sended;		// AT���������
extern OS_EVENT* mbox_at_mission;		// ģ�������������
extern OS_EVENT* sem_make_data_ok;		// ��װ����������
extern OS_EVENT *sem_usart1_send;		// ����0�����ź�


#define REGIST_CODE_LENTH 		30
typedef struct
{
	BYTE  w_falg;
	DWORD server_ip_int;
	WORD  server_port;
	BYTE  regist_code[REGIST_CODE_LENTH];
	BYTE  phone_num[13];
	WORD  send_time_inteval_default;
	WORD  send_time_inteval_sleep;
	WORD  send_time_inteval_alarm;
	WORD  heartbeat_time_inteval;
	BYTE  alarm_sms_phone[2][13];
}DEVICE_INFO;



extern DEVICE_INFO Device;
extern char Ip_string_buf[20];
extern char Version[];






#endif
