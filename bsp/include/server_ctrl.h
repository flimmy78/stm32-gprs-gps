#ifndef __SERVER_CTRL_H_ 
#define __SERVER_CTRL_H_ 
#include "type.h"

/* ServerPara.ctrl_step�Ķ��� */
#define ctrl_step_wait_reg					0
#define ctrl_step_connect_server 			1
#define ctrl_step_connect_server_wait 		2
#define ctrl_step_before_regist_device 		10
#define ctrl_step_regist_device				11
#define ctrl_step_regist_device_wait 		12
#define ctrl_step_regist_device_wait_10min	13
#define ctrl_step_login_server 				20
#define ctrl_step_login_server_wait 		21
#define ctrl_step_deregist 					25
#define ctrl_step_deregist_wait				26
#define ctrl_step_stand_by 					30
#define ctrl_step_server_answer_wait 		31
#define ctrl_step_sleep_close_socket 		40
#define ctrl_step_sleep_wait 				41
#define ctrl_step_connect_updateserver 		50
#define ctrl_step_connect_updateserver_wait 51
#define ctrl_step_login_updateserver 		52
#define ctrl_step_login_updateserver_wait	53
#define ctrl_step_get_updateinfo 			54
#define ctrl_step_get_updateinfo_wait 		55
#define ctrl_step_get_package				56
#define ctrl_step_get_package_wait 			57
#define ctrl_step_sleep	 					60





/* ����������״̬ */
typedef enum
{
	SERVER_NO_CONNECT,			//δ����
	SERVER_CONNECT_REGOK,		//��ע��
	SERVER_CONNECT_AUTHENOK,	//�Ѽ�Ȩ
	SERVER_CONNECT_UPDATE,		//����
}SERVER_STATUS;




//���������в���
typedef struct
{
	SERVER_STATUS  state;		//����������״̬
	BYTE  ctrl_step;			//���Ʋ���
	BYTE  err_times;			//�������
	BYTE  main_or_back;	   		//���������Ǳ��ݷ���
	WORD  answer_wait_time;
	DWORD answer_wait_time_new;
	WORD  answer_try_times_new;
	WORD  sleep_wait_time;
	BYTE  need_server_answer;
	BYTE  need_server_ack;
	WORD  send_serial;
	WORD  heartbeat_time ;
	WORD  ex_buffer_delay_time;
	WORD  received_data_count;
}SERVER_STRUCT;
extern SERVER_STRUCT ServerPara;





#endif
