/******************************************
�ļ�����: jt_t808_dsp.c
��	  ��: byy
��    ��:
˵    ��: JT/T808Э������
�޸ļ�¼: 2015-06-26,������
******************************************/
#include "all_use.h"




static void server_normal_answer_dsp(BYTE* data, WORD lenth);
static void server_regist_answer_dsp(BYTE* data, WORD lenth);
static void gprs_msg_dsp(BYTE* data, WORD lenth);




/*******************************************
��������: ���������ݽ���
��ڲ���:
���ڲ���: 
��    ע: 
*******************************************/
BYTE server_rdata_program(WORD lenth)
{
	WORD i;
	BYTE* data = at.socket[0].rdata;
	WORD head_loc = 0;
	WORD tail_loc = 0;
	BYTE* body_data = 0;
	WORD body_lenth = 0;
	WORD pack_lenth = 0;
	WORD msg_id = 0;
	WORD cycle_num = 0;
	BYTE check = 0;
	
	lenth = at.socket[0].rlenth;
	while(lenth > 0)
	{
		/* ���Ұ�ͷ��ʶ */
		head_loc = 0;
		while(head_loc < lenth)
		{
			if(data[head_loc] == 0x7E && data[head_loc + 1] != 0x7E)
			{
				break;
			}
			head_loc++;
		}
		/* ɾ����ͷǰ������ */
		if(head_loc > 0)
		{
			memmove(data, data + head_loc, lenth - head_loc);
			lenth -= head_loc;
		}
		
		/* ���Ұ�β��ʶ */
		tail_loc = 1;
		while(tail_loc < lenth)
		{
			if(data[tail_loc] == 0x7E)
			{
				break;
			}
			tail_loc++;
		}
		if(tail_loc >= lenth)
		{
			break;
		}
		
		/* ��ת�� */
		pack_lenth = t808_reverse_escape(data, tail_loc + 1);
		
		/* ��鳤���Ƿ�Ϸ� */
		body_lenth = ((WORD)data[3] << 8) + data[4];
		if(body_lenth + 15 != pack_lenth)
		{
			memmove(data, data + tail_loc, lenth - tail_loc);
			lenth -= tail_loc;
			continue;
		}
		
		/* ����λУ�� */
		check = 0;
		for(i = 1; i < pack_lenth - 2; i++)
		{
			check ^= data[i];
		}
		
		if(check != data[pack_lenth - 2])
		{
			memmove(data, data + tail_loc, lenth - tail_loc);
			lenth -= tail_loc;
			continue;
		}
		
		/* �������� */
		msg_id = ((WORD)data[1] << 8) + data[2];
		cycle_num = ((WORD)data[11] << 8) + data[12];
		body_data = &data[13];
		switch(msg_id)
		{
			case server_normal_answer:		//0x8001
				//printf_time_msg("<<--������ͨ��Ӧ��");
				server_normal_answer_dsp(body_data, body_lenth);
				break;
			
			case server_regist_answer:		//0x8100
				//printf_time_msg("<<--�ն�ע��Ӧ��");
				server_regist_answer_dsp(body_data, body_lenth);
				break;
			
			case server_set_terminal_cfg:	//0x8103
				sys_cfg_set_dsp(body_data, body_lenth);
				send_normal_answer(cycle_num, msg_id, 0);
				break;
			
			case server_read_terminal_cfg:	//0x8104
				send_sys_cfg_read_anwser(cycle_num);
				break;
			
			case server_read_gps_data:		//0x8201
				printf_time_msg("<<--λ����Ϣ��ѯ");
				send_request_location_answer(cycle_num);
				break;
			
			case server_send_text:			//0x8300
				//printf_time_msg("<<--�·��ı���Ϣ");
				gprs_msg_dsp(body_data, body_lenth);
				send_normal_answer(cycle_num, msg_id, 0);
				break;
			
			default:
				printf_time_msg("<<--�յ�����ϢID��֧��,%0.4X", msg_id);
				if(msg_id & 0x8000)
				{
					send_normal_answer(cycle_num, msg_id, 0);
				}
				break;
		}
		lenth -= (tail_loc + 1);
		memmove(data, data + tail_loc + 1, lenth);
	}
	at.socket[0].rlenth = lenth;
	return 0;
}










/*******************************************
��������: ������ע��Ӧ��
��ڲ���: data,  ��Ϣ��ָ��
          lenth, ��Ϣ�峤��
���ڲ���: 
��    ע: ����ͨ��
*******************************************/
static void server_normal_answer_dsp(BYTE* data, WORD lenth)
{
	static WORD ret_sel = 0x1234;
	WORD ret_id;
	BYTE result;
	
	
	ret_id  = ((WORD)data[2] << 8) + data[3];
	if(ret_sel == ((WORD)data[0] << 8) + data[1]
	&& ret_id  != terminal_authentication)
	{
		return;
	}
	ret_sel = ((WORD)data[0] << 8) + data[1];
	result  = data[4];
	
	switch(ret_id)
	{
		case terminal_heartbeat:
			printf_time_msg("<<--����Ӧ��:%d", result);
			break;
		case terminal_regist:
			printf_time_msg("<<--�ն�ע��Ӧ��:%d", result);
			break;
		case terminal_deregist:
			printf_time_msg("<<--�ն�ע��Ӧ��:%d", result);
			break;
		case terminal_authentication: 		
			if(result == 0)
			{	
				printf_time_msg("<<--�豸��ȨӦ��ɹ�:%d", result);
				if(ServerPara.ctrl_step == ctrl_step_login_server_wait)
				{
					ServerPara.state = SERVER_CONNECT_AUTHENOK;
				}
			}
			else
			{	
				printf_time_msg("<<--�豸��ȨӦ��ʧ��:%d", result);
			}
			break;
		case terminal_up_gps_data:
			printf_time_msg("<<--λ����Ϣ�ϱ�Ӧ��:%d", result);
			break;
		default:
			printf_time_msg("<<--ƽ̨ͨ��Ӧ��:ID=%0.4X,%d", ret_id, result);
			break;
	}
}




/*******************************************
��������: ������ע��Ӧ��
��ڲ���: data, ��Ϣ��ָ��
          lenth, ��Ϣ�峤��
���ڲ���: 
��    ע: ����ͨ��
*******************************************/
static void server_regist_answer_dsp(BYTE* data, WORD lenth)
{
	switch(data[2])
	{
		case 0:
			if(lenth - 3 < REGIST_CODE_LENTH)
			{
				memcpy(Device.regist_code, &data[3], lenth - 3);
				Device.regist_code[lenth - 3] = '\0';
				write_device_info_to_flash();
				printf_time_msg("<<--�ն�ע��ɹ�:%s", Device.regist_code);
				ServerPara.state = SERVER_CONNECT_REGOK;
			}
			else
			{
				printf_time_msg("<<--��Ȩ��̫��:%d", lenth - 3);
			}
			break;
		case 1:	
			printf_time_msg("<<--�����ѱ�ע��");
			break;
		case 2:
			printf_time_msg("<<--���ݿ��޴˳���");
			break;
		case 3: 
			printf_time_msg("<<--�ն��ѱ�ע��");
			break;
		case 4:	
			printf_time_msg("<<--���ݿ��޴��ն�");
			break;
		default:
			printf_time_msg("<<--δ֪ע��״̬:%d", data[2]);
			break;
	}
}




/*******************************************
��������: �ı���Ϣ�·�
��ڲ���: data, ��Ϣ��ָ��
          lenth, ��Ϣ�峤��
���ڲ���: 
��    ע: ����ͨ��
*******************************************/
static void gprs_msg_dsp(BYTE* data, WORD lenth)
{	
	BYTE* msg = data + 1;
	WORD msg_lenth = lenth - 1;
	WORD i;
	
	data[lenth] = 0;
	printf_time_msg("<<--�ı���Ϣ�·�:%0.2X,%s", data[0], &data[1]);
	
	/* ����ͷ��β */
	if(!stringIsSame((char*)msg, "*zl,") && msg[msg_lenth - 1] != '#')
	{
		return;
	}
	
	/* ��ָ���ֻ��ŷ����ն���Ϣ |*zl,info,13335197169# */
	if(stringIsSame((char*)msg + 4, "info"))
	{
		if(check_para_num(msg) != 2)
		{
			return;
		}
		
		if((msg_lenth - strlen("*zl,info,") - 1) != 11)
		{
			return;
		}
		
		for(i = 0; i < 11; i++)
		{
			if(!isdigit(msg[msg_lenth - 12 + i]))
			{
				return;
			}
		}
		msg[msg_lenth - 1] = '\0';
		send_msg_to_num((char*)&msg[msg_lenth - 12], 
						"ID:%s\r\n�汾:%s@%02d%02d%02d%02d%02d%02d\r\n������:%d.%d.%d.%d:%d",
						Device.phone_num, Version,
						CompileTime.year % 100, CompileTime.month, CompileTime.day,
						CompileTime.hour, CompileTime.min, CompileTime.sec,
						(BYTE)(Device.server_ip_int >> 24),(BYTE)(Device.server_ip_int >> 16),
						(BYTE)(Device.server_ip_int >> 8), (BYTE)(Device.server_ip_int >> 0),
						Device.server_port);
	}
	
	/* ��ָ���ֻ��ŷ��ͱ����ֻ��� |*zl,anum,13335197169# */
	else if(stringIsSame((char*)msg + 4, "anum"))
	{
		if(check_para_num(msg) != 2)
		{
			return;
		}
		
		if((msg_lenth - strlen("*zl,anum,") - 1) != 11)
		{
			return;
		}
		
		for(i = 0; i < 11; i++)
		{
			if(!isdigit(msg[msg_lenth - 12 + i]))
			{
				return;
			}
		}
		msg[msg_lenth - 1] = '\0';
		send_msg_to_num((char*)&msg[msg_lenth - 12], "ID:%s\r\n������1:%s\r\n������2:%s",
						Device.phone_num, Device.alarm_sms_phone[0], Device.alarm_sms_phone[1]);
	}
}









