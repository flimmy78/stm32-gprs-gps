/*
各个文件常用全局变量
主要为设备当前状态等信息
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






#define __IO			volatile 	/*读写标志位*/


/* 系统事件列表 */
extern OS_EVENT* mbox_at_answer;		// AT命令回应邮箱
extern OS_EVENT* mbox_at_sended;		// AT命令发送邮箱
extern OS_EVENT* mbox_at_mission;		// 模块操作步骤邮箱
extern OS_EVENT* sem_make_data_ok;		// 组装服务器数据
extern OS_EVENT *sem_usart1_send;		// 串口0发送信号


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
