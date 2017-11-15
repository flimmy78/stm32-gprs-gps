/******************************************
文件名称: jt_t808_dsp.c
作	  者: byy
版    本:
说    明: JT/T808协议层解析
修改记录: 2015-06-26,程序建立
******************************************/
#include "all_use.h"




static void server_normal_answer_dsp(BYTE* data, WORD lenth);
static void server_regist_answer_dsp(BYTE* data, WORD lenth);
static void gprs_msg_dsp(BYTE* data, WORD lenth);




/*******************************************
函数功能: 服务器数据解析
入口参数:
出口参数: 
备    注: 
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
		/* 查找包头标识 */
		head_loc = 0;
		while(head_loc < lenth)
		{
			if(data[head_loc] == 0x7E && data[head_loc + 1] != 0x7E)
			{
				break;
			}
			head_loc++;
		}
		/* 删除包头前的数据 */
		if(head_loc > 0)
		{
			memmove(data, data + head_loc, lenth - head_loc);
			lenth -= head_loc;
		}
		
		/* 查找包尾标识 */
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
		
		/* 反转移 */
		pack_lenth = t808_reverse_escape(data, tail_loc + 1);
		
		/* 检查长度是否合法 */
		body_lenth = ((WORD)data[3] << 8) + data[4];
		if(body_lenth + 15 != pack_lenth)
		{
			memmove(data, data + tail_loc, lenth - tail_loc);
			lenth -= tail_loc;
			continue;
		}
		
		/* 检验位校验 */
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
		
		/* 解析数据 */
		msg_id = ((WORD)data[1] << 8) + data[2];
		cycle_num = ((WORD)data[11] << 8) + data[12];
		body_data = &data[13];
		switch(msg_id)
		{
			case server_normal_answer:		//0x8001
				//printf_time_msg("<<--服务器通用应答");
				server_normal_answer_dsp(body_data, body_lenth);
				break;
			
			case server_regist_answer:		//0x8100
				//printf_time_msg("<<--终端注册应答");
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
				printf_time_msg("<<--位置信息查询");
				send_request_location_answer(cycle_num);
				break;
			
			case server_send_text:			//0x8300
				//printf_time_msg("<<--下发文本消息");
				gprs_msg_dsp(body_data, body_lenth);
				send_normal_answer(cycle_num, msg_id, 0);
				break;
			
			default:
				printf_time_msg("<<--收到的消息ID不支持,%0.4X", msg_id);
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
函数功能: 服务器注册应答
入口参数: data,  消息体指针
          lenth, 消息体长度
出口参数: 
备    注: 测试通过
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
			printf_time_msg("<<--心跳应答:%d", result);
			break;
		case terminal_regist:
			printf_time_msg("<<--终端注册应答:%d", result);
			break;
		case terminal_deregist:
			printf_time_msg("<<--终端注消应答:%d", result);
			break;
		case terminal_authentication: 		
			if(result == 0)
			{	
				printf_time_msg("<<--设备鉴权应答成功:%d", result);
				if(ServerPara.ctrl_step == ctrl_step_login_server_wait)
				{
					ServerPara.state = SERVER_CONNECT_AUTHENOK;
				}
			}
			else
			{	
				printf_time_msg("<<--设备鉴权应答失败:%d", result);
			}
			break;
		case terminal_up_gps_data:
			printf_time_msg("<<--位置信息上报应答:%d", result);
			break;
		default:
			printf_time_msg("<<--平台通用应答:ID=%0.4X,%d", ret_id, result);
			break;
	}
}




/*******************************************
函数功能: 服务器注册应答
入口参数: data, 消息体指针
          lenth, 消息体长度
出口参数: 
备    注: 测试通过
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
				printf_time_msg("<<--终端注册成功:%s", Device.regist_code);
				ServerPara.state = SERVER_CONNECT_REGOK;
			}
			else
			{
				printf_time_msg("<<--鉴权码太长:%d", lenth - 3);
			}
			break;
		case 1:	
			printf_time_msg("<<--车辆已被注册");
			break;
		case 2:
			printf_time_msg("<<--数据库无此车辆");
			break;
		case 3: 
			printf_time_msg("<<--终端已被注册");
			break;
		case 4:	
			printf_time_msg("<<--数据库无此终端");
			break;
		default:
			printf_time_msg("<<--未知注册状态:%d", data[2]);
			break;
	}
}




/*******************************************
函数功能: 文本信息下发
入口参数: data, 消息体指针
          lenth, 消息体长度
出口参数: 
备    注: 测试通过
*******************************************/
static void gprs_msg_dsp(BYTE* data, WORD lenth)
{	
	BYTE* msg = data + 1;
	WORD msg_lenth = lenth - 1;
	WORD i;
	
	data[lenth] = 0;
	printf_time_msg("<<--文本信息下发:%0.2X,%s", data[0], &data[1]);
	
	/* 检验头和尾 */
	if(!stringIsSame((char*)msg, "*zl,") && msg[msg_lenth - 1] != '#')
	{
		return;
	}
	
	/* 向指定手机号发送终端信息 |*zl,info,13335197169# */
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
						"ID:%s\r\n版本:%s@%02d%02d%02d%02d%02d%02d\r\n服务器:%d.%d.%d.%d:%d",
						Device.phone_num, Version,
						CompileTime.year % 100, CompileTime.month, CompileTime.day,
						CompileTime.hour, CompileTime.min, CompileTime.sec,
						(BYTE)(Device.server_ip_int >> 24),(BYTE)(Device.server_ip_int >> 16),
						(BYTE)(Device.server_ip_int >> 8), (BYTE)(Device.server_ip_int >> 0),
						Device.server_port);
	}
	
	/* 向指定手机号发送报警手机号 |*zl,anum,13335197169# */
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
		send_msg_to_num((char*)&msg[msg_lenth - 12], "ID:%s\r\n报警号1:%s\r\n报警号2:%s",
						Device.phone_num, Device.alarm_sms_phone[0], Device.alarm_sms_phone[1]);
	}
}









