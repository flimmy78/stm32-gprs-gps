#ifndef  _APP_CFG_H_
#define  _APP_CFG_H_

/* task priority */
#define APP_TASK_START_PRIO			2
#define GSM_Operat_task_PRIO 		3
#define GSM_Resolve_task_PRIO 		4
#define Server_Ctrl_task_PRIO 		5
#define GPS_Resolve_task_PRIO 		6
#define Sys_Ctrl_task_PRIO 			7

/* 任务堆栈大小 */
#define APP_TASK_START_STK_SIZE 	64
#define GSM_Operat_task_STK_SIZE  	256
#define GSM_Resolve_task_STK_SIZE  	256
#define Server_Ctrl_task_STK_SIZE   256
#define GPS_Resolve_task_STK_SIZE	256
#define Sys_Ctrl_task_STK_SIZE  	128


#endif

