/******************************************
�ļ�����: server_ctr.c
��	  ��: byy
��    ��:
˵    ��: ���������ӿ���
�޸ļ�¼: 2015-06-10,��������
******************************************/
#include "all_use.h"



const WORD Server_rcv_cap = 2000;
BYTE Server_rcv_buf[Server_rcv_cap + 1];
const WORD Server_send_cap = 1500;
BYTE Server_send_buf[Server_send_cap + 1];
SERVER_STRUCT ServerPara;		//���������в���


static void location_info_send_pro(WORD step_time);
static void heartbeat_send_pro(WORD step_time);




/*******************************************
��������: Server_Ctr_task
��������: ���������ӿ���
��ڲ���: 
���ڲ���: 
��    ע: 
*******************************************/
void Server_Ctrl_task(void *p_arg)
{
	const WORD step_time = 100;
	const DWORD wait_one_sec = 1000 / step_time;
	const DWORD server_relink_time = 30 * wait_one_sec;
	DWORD ctrl_time_add = 0;	//200MS
	BYTE  login_times = 0;
	
	p_arg = p_arg;
	print_debug_msg("--->ϵͳ����");
	at.power = 1;
	ServerPara.ctrl_step = ctrl_step_wait_reg;
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, step_time);
		
		switch(ServerPara.ctrl_step)
		{
			/* �ȴ��������ע�� */
			case ctrl_step_wait_reg:
				ServerPara.state = SERVER_NO_CONNECT;
				if(at.state_reg && at.power == 1)
				{
					ServerPara.ctrl_step = ctrl_step_connect_server;
					ctrl_time_add = 0;
				}
				break;
				
			/* ��������������� */
			case ctrl_step_connect_server:
				if(at.socket[0].state != CONNECT_IDLE)
				{
					close_socket(0);
				}
				at.socket[0].tdata = Server_send_buf;
				at.socket[0].tlenth = 0;
				at.socket[0].tdata_limit = Server_send_cap;
				at.socket[0].rdata = Server_rcv_buf;
				at.socket[0].rlenth = 0;
				at.socket[0].rdata_limit = Server_rcv_cap;
				at.socket[0].socket_rdata_program = server_rdata_program;
				build_socket(0, IPint2string(Device.server_ip_int, Ip_string_buf),
								Device.server_port, TCP);
				ServerPara.ctrl_step = ctrl_step_connect_server_wait;
				ctrl_time_add = 0;
				break;
			
			case ctrl_step_connect_server_wait:
				if(at.socket[0].state != CONNECT_CONNECT_OK)
				{
					if(++ctrl_time_add > server_relink_time)
					{
						print_debug_msg("���������ӳ�ʱ");
						ServerPara.ctrl_step = ctrl_step_connect_server;	//���·�������
						ctrl_time_add = 0;
						close_socket(0);
					}
				}
				else
				{
					OSTimeDlyHMSM(0, 0, 0, 100);
					login_times = 0;
					ServerPara.ctrl_step = ctrl_step_before_regist_device;
				}
				break;
				
			case ctrl_step_before_regist_device:
				/* �鿴��Ȩ���Ƿ�Ϊ�� */
				if(Device.regist_code[0])
				{	
					ServerPara.ctrl_step = ctrl_step_login_server;
					ServerPara.err_times = 0;
				}
				else
				{
					ServerPara.ctrl_step = ctrl_step_regist_device;
				}
				break;
			
			case ctrl_step_regist_device:
				printf_time_msg("-->>�����豸ע��");
				send_regist_data();
				ServerPara.ctrl_step = ctrl_step_regist_device_wait;
				ctrl_time_add = 0;
				break;
				
			case ctrl_step_regist_device_wait:
				if(ServerPara.state == SERVER_CONNECT_REGOK)
				{
					ServerPara.ctrl_step = ctrl_step_login_server;
				}
				else if(++ctrl_time_add >= 30 * wait_one_sec)
				{
					ctrl_time_add = 0;
					ServerPara.ctrl_step = ctrl_step_regist_device;
				}
				break;
				
			case ctrl_step_login_server:
				printf_time_msg("-->>�����豸��Ȩ:%s", Device.regist_code);
				send_login_data();
				ServerPara.ctrl_step = ctrl_step_login_server_wait;
				ctrl_time_add = 0;
				break;
				
			/* �ȴ���Ȩ��Ӧ */
			case ctrl_step_login_server_wait:
				if(ServerPara.state == SERVER_CONNECT_AUTHENOK)
				{
					ctrl_time_add = 0;
					at.socket[0].tlenth = 0;
					at.socket[0].tcount = 0;
					ServerPara.ctrl_step = ctrl_step_stand_by;
				}
				else if(++ctrl_time_add >= 30 * wait_one_sec)
				{
					if(++login_times >= 3)
					{
						Device.regist_code[0] = '\0';
						write_device_info_to_flash();
						login_times = 0;
					}
					ctrl_time_add = 0;
					ServerPara.ctrl_step = ctrl_step_before_regist_device;
				}
				break;
				
			case ctrl_step_stand_by:
				location_info_send_pro(step_time);
				heartbeat_send_pro(step_time);
				break;
				
			default:
				ServerPara.ctrl_step = ctrl_step_wait_reg;
				break;
		}
	}
}





/*******************************************
��������: location_info_send_pro
��������: λ����Ϣ����Pro
��ڲ���: step_time,����ʱ����
���ڲ���: 
��    ע: 
*******************************************/
static void location_info_send_pro(WORD step_time)
{
	static DWORD wait_time_add = 600;
	DWORD send_interva = 0;
	
	
	send_interva = (Status_stu.reg.power)?
				   Device.send_time_inteval_sleep :
				   Device.send_time_inteval_default;

	if(send_interva < 1 || send_interva > 900)
	{
		send_interva = 30;
	}
	
	if(++wait_time_add >= send_interva * 1000 / step_time)
	{
		printf_time_msg("-->>����λ����Ϣ,%d", send_interva);
		send_location_info();
		wait_time_add = 0;
	}
}




/*******************************************
��������: heartbeat_send_pro
��������: ��������Pro
��ڲ���: step_time,����ʱ����
���ڲ���: 
��    ע: 
*******************************************/
static void heartbeat_send_pro(WORD step_time)
{
	static DWORD wait_time_add = 20;
	DWORD send_interva = 60;
	
	if(++wait_time_add >= send_interva * 1000 / step_time)
	{
		printf_time_msg("-->>��������");
		send_heartbeat();
		wait_time_add = 0;
	}
}













