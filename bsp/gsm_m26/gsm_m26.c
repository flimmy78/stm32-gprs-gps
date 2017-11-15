/******************************************
文件名称: gsm_m26.c
作	  者: byy
版    本:
说    明: GSM模块驱动
修改记录: 2011-08-18,程序建立。
******************************************/
#include "all_use.h"  


/* GSM 模块信息 */
struct at_info_struct at;

/* GSM模块响应与处理函数结构体 */
struct answer_pro
{
	char* key_char;
    at_program the_program;
};

/* GSM模块响应列表 */
struct answer_pro const comm_answer[]=
{
	{"AT",				at_sended_dsp},			//命令回显
	{"OK",				at_ok_dsp},				//成功
	{"ERROR",			at_err_dsp},			//失败
	{"+CMS ERROR: ",	at_cms_err_dsp},		//CMS错误提示
	{"+CME ERROR: ",	at_cme_err_dsp},		//CES错误提示
	{"+CMGL: ",			at_cmgl_dsp},			//短消息列表
	{"+CMTI: ",			at_cmti_dsp},			//新短消息提示
	{"+CMT: ",			at_cmt_dsp},			//新短消息
	{"+CMGR: ",			at_cmgr_dsp},			//读取短消息
	{"+CMGS: ",			at_cmgs_dsp},			//发送短消息
	{"+CLCC: ",			at_clcc_dsp},			//当前呼叫状态
	{"+CREG: ",			at_creg_dsp},			//网络登录状态
	{"+COPS: ",			at_cops_dsp},			//运营商查询
	{"+CSQ: ", 			at_csq_dsp},			//信号强度
	{"+CBC: ", 			at_cbc_dsp},			//电池电量
	{"+QNITZ: ", 		at_qnitz_dsp},			//网络时间
	{"+QCELLLOC:",		at_qcellloc_dsp},		//基站定位
	{"RING\r\n",		at_ring_dsp},			//来电
	{"BUSY\r\n",		at_noc_dsp},			//线路忙	
	{"NO CARRIER\r\n",	at_noc_dsp},			//建立通话失败
	{"NO DIALTONE\r\n",	at_ndt_dsp},			//被动挂机
	{"0, ", 			at_tcpipMux_dsp},		//多连接下SOCKET信息解析
	{"1, ", 			at_tcpipMux_dsp},		//多连接下SOCKET信息解析
	{"2, ", 			at_tcpipMux_dsp},		//多连接下SOCKET信息解析
	{"3, ", 			at_tcpipMux_dsp},		//多连接下SOCKET信息解析
	{"4, ", 			at_tcpipMux_dsp},		//多连接下SOCKET信息解析
	{"5, ", 			at_tcpipMux_dsp},		//多连接下SOCKET信息解析
	{"+RECEIVE: ", 		at_tcpipRcv_dsp},		//TCPIP数据接收
	{">",				at_sendBegin_dsp},		//TCPIP等待发送标识
	{"",				NULL},
};




/*******************************************
函数功能: 初始化AT模块结构体
入口参数:
出口参数: 
备    注: 
*******************************************/
void init_at(void)
{	
	BYTE i;
	
	at.err_times = 0;
	at.sim_state = 0;
	at.state_reg = 0;
	at.state_reg_wait = 0;
	at.state_csq = 0;
	at.state_csq_wait = 0;
	at.state_batte = 4200;
	at.state_net = 0;
	at.state_net_wait = 0;
	at.state_power = 0;
	at.msg_rlenth = 0;
	at.msg_tlenth = 0;
	at.msg_tRetry = 0;
	at.msg_tstatus = SEND_IDLE;
	at.msg_rdata_dsp = recv_msg_rsv;
	strcpy((char*)at.cops, "UNKNOWN");
	strcpy((char*)at.ccid, "UNKNOWN");
	strcpy((char*)at.cimi, "UNKNOWN");
	strcpy((char*)at.imei, "UNKNOWN");	//设备序号
	strcpy((char*)at.cgmm, "UNKNOWN");	//设备型号
	strcpy((char*)at.cgmr, "UNKNOWN");	//软件版本
	for(i = 0; i < SOCKET_AMOUNT; i++)
	{	
		at.socket[i].on = 0;
		at.socket[i].state = CONNECT_IDLE;
		at.socket[i].tlenth = 0;
		at.socket[i].tdata = 0;
		at.socket[i].rdata = 0;
		at.socket[i].send_status = SEND_IDLE;		
		at.socket[i].send_err_times = 0;
		at.socket[i].connect_err_times = 0;
		at.socket[i].socket_rdata_program = NULL;
		at.socket[i].socket_state_program = NULL;
		at.socket[i].socket_send_program  = NULL;
	}
	strcpy((char*)at.apn, "cmnet");
	at.voice_state = 0;
	at.redi_sended = 0;
	at.buffer_empty = 0;
	at.dog = 0;
	at.flag_loc = 0;
	at.flag_time = 0;
}




/*******************************************
函数功能: 根据at关键字选择解析函数
入口参数: BYTE* aline ,接收到的数据
出口参数: 
备    注: 
*******************************************/
at_program search_key_ward(BYTE* aline)
{	
	BYTE list;
	BYTE lenth;
	
	/* 收到的数据只有回车 */
	if(aline[0] == '\r')
	{
		return(0);
	}
	
	/* 检索列表 */
	for(list = 0; comm_answer[list].key_char[0] != 0; list++)
	{	
		lenth = strlen(comm_answer[list].key_char);
		if(memcmp(aline, comm_answer[list].key_char, lenth) == 0)
		{
			return(comm_answer[list].the_program);
		}
	}
	
	switch(at_mission_now % 100)
	{	
		case at_mission_cimi:
			return(at_cimi_dsp);
		case at_mission_cgsn:
			return(at_cgsn_dsp);
		case at_mission_cgmm:
			return(at_cgmm_dsp);
		case at_mission_cgmr:
			return(at_cgmr_dsp);
		case at_mission_qccid:
			return(at_qccid_dsp);
		default:
			break; 
	}
	return(NULL);
}




/* "AT"字符串解析,(回显内容) */
BYTE at_sended_dsp(BYTE *data_addr)
{	
	OSMboxAccept(mbox_at_answer);    
	OSMboxPost(mbox_at_sended, (void*)1);	
	return(0);
}



//OK
BYTE at_ok_dsp(BYTE *data_addr)
{
	
	if(at_mission_now != at_no_mission
	&& (at_mission_now % 100 != at_mission_qisend)
	&& (at_mission_now % 100 != at_mission_cmgs))
  	if(at_mission_now)
	{
		OSMboxPost(mbox_at_answer, (void*)at_answer_ok);
    }
	return(0);
}



//ERROR
BYTE at_err_dsp(BYTE *data_addr)
{ 
	if(at_mission_now != at_no_mission)
	{
		OSMboxPost(mbox_at_answer, (void*)at_answer_err);
	}
	return(0);
}


// CMS ERROR:
BYTE at_cms_err_dsp(BYTE *data_addr)
{
	if(at_mission_now != at_no_mission)
	{
		OSMboxPost(mbox_at_answer, (void*)at_answer_err);
	}
	return(0);
}


// CME ERROR:
BYTE at_cme_err_dsp(BYTE *data_addr)
{
	WORD at_dsp_temp;
	at_dsp_temp = (WORD)search_data_para(1, data_addr);
	
	if(at_mission_now != at_no_mission)
	{
		OSMboxPost(mbox_at_answer, (void*)at_answer_err);
	}
	if(at_dsp_temp == 10)
	{
		print_debug_msg("SIM卡错误(CME ERROR)");
		at.sim_state = 2;
	}
	return(0);
}




/* +CSQ: 信号质量 */
BYTE at_csq_dsp(BYTE *data_addr)
{
	WORD at_dsp_temp;
	
	at_dsp_temp = (WORD)search_data_para(1, data_addr);
	if(at_dsp_temp == 99)
	{
		at_dsp_temp = 0;
	}
	at.state_csq = (BYTE)at_dsp_temp;
	//print_debug_msg("信号质量:%d", at.state_csq);
	return(0);
}




/* +CBC: 电池电量 */
BYTE at_cbc_dsp(BYTE *data_addr)
{
	at.state_batte = (WORD)search_data_para(3, data_addr);
	//print_debug_msg("GSM电压:%d", at.state_batte);
	return(0);
}



/* RING 语音呼入 */
BYTE at_ring_dsp(BYTE *data_addr)
{
	print_debug_msg("电话呼入");
	return(0);
}



/* CLCC 通话详情 */
BYTE at_clcc_dsp(BYTE *data_addr)
{	
	BYTE i = 0; 
	
	at.voice_dir = (WORD)search_data_para(2, data_addr);
	at.voice_state = (WORD)search_data_para(3, data_addr);
	data_addr += search_text_para(6, data_addr) + 1;
	i = 0;
	while(isdigit(data_addr[i]))
	{	
		at.call_num[i] = data_addr[i];
		i++;
	}
	at.call_num[i] = '\0';
	switch(at.voice_state)
	{	
		case 0:
			at.voice_state = voice_state_spking;
			at.dog = 0;//清空at看门狗
			break;
		case 4:
			at.voice_state = voice_state_incoming;
			at.dog = 0;//清空at看门狗
			break;
	}
	at.voice_state_check = 3;
	print_debug_msg("来电号码:%s", at.call_num);
	return(0);
}




/* CGMM,模块型号 */
BYTE at_cgmm_dsp(BYTE *data_addr)
{	
	BYTE i = 0;

	while(i < cgmm_lenth)
	{	
		if(*data_addr == ' ' || *data_addr == '\r' || *data_addr == '\0')
		{
			break;
		}
		at.cgmm[i] = *data_addr;
		data_addr++;
		i++;
	}
	at.cgmm[i] = 0;
	return(0);
}




/* CGMR,软件版本 */
BYTE at_cgmr_dsp(BYTE *data_addr)
{	
	BYTE i = 0;
	
	while(*data_addr != '\0')
	{
		if(*data_addr == ' ')
		{
			data_addr++;
			for(i = 0; i < cgmr_lenth; i++)
			{	
				if(*data_addr == ' ' || *data_addr == '\r' || *data_addr == '\0')
				{
					break;
				}
				at.cgmr[i] = *data_addr;
				data_addr++;
			}
			at.cgmr[i] = 0;
			return(0);
		}
		else
		{
			data_addr++;
		}
	}
	return(0);
}




/* BUSY 通话线路忙 */
BYTE at_noc_dsp(BYTE *data_addr)
{
	OSMboxPost(mbox_at_answer, (void *)at_answer_err);
	at.voice_state = voice_state_no;
	print_debug_msg("挂机");
	return(0);
}



/* 被动挂机 */
BYTE at_ndt_dsp(BYTE* data_addr)
{
	OSMboxPost(mbox_at_answer, (void *)at_answer_err);
	at.voice_state = voice_state_no;
	print_debug_msg("被动挂机");	
	return(0);
}




/* 网络注册状态 */
BYTE at_creg_dsp(BYTE *data_addr)
{
	BYTE i;
	BYTE stat;
	BYTE para_num;

	para_num = check_para_num(data_addr);
	if(para_num >= 2 && para_num <= 4)
	{
		stat = (WORD)search_data_para(2, data_addr);
	}
	else if(para_num == 1)
	{
		stat = (WORD)search_data_para(1, data_addr);
	}
	else
	{
		return(0);
	}
	switch(stat)
	{	
		case 1:
			at.state_reg = 1;
			break;
		case 5:
			at.state_reg = 5;
			break;
		case 0:
		default:
			ServerPara.state = SERVER_NO_CONNECT;
			ServerPara.err_times = 0;
			ServerPara.ctrl_step = ctrl_step_wait_reg;
			for(i = 0; i < SOCKET_AMOUNT; i++)
			{
				at.socket[i].state = CONNECT_IDLE;
			}
			at.state_reg = 0;
			break;
	}
	return(0);
}



/* CMGL 短信列表 */
BYTE at_cmgl_dsp(BYTE* data_addr)
{	
	//BYTE num;
	
	//num =  search_data_para(1, data_addr);
	//OSQPost(mbox_at_mission, (void*)(at_mission_cmgr + num * 100));	 
	return(0);
}



/* CMTI 新短消息提示 */
BYTE at_cmti_dsp(BYTE* data_addr)
{	
	BYTE num;
	
	num = search_data_para(2, data_addr);
	OSQPost(mbox_at_mission, (void*)(at_mission_cmgr + num * 100));
	return(0);
}


/* CMT 新短消息 */
BYTE at_cmt_dsp(BYTE* data_addr)
{	
	WORD num = 0;
	WORD i = 0;
	WORD time_add = 0;
	BYTE data;
	
	num = search_text_para(1, data_addr) + 1;
	i = 0;
	while(isdigit(data_addr[num + i]) || data_addr[num + i] == '+')
	{
		at.msg_rnum[i] = data_addr[num + i];
		i++;
	}
	at.msg_rnum[i] = 0;
	
	at.msg_rlenth = 0;
	memset(at.msg_rdata, 0, msg_data_lenth);
	while(at.msg_rlenth < msg_data_lenth && time_add < 1000)
	{
		if(GSMRcv_lenth() > 0)
		{
			time_add = 0;
			data = GSMRcv_pop();
			if(data == '\r' || data == '\n')
			{
				break;
			}
			at.msg_rdata[at.msg_rlenth] = data;
			at.msg_rlenth++;
		}
		else
		{
			time_add += 10;
			OSTimeDlyHMSM(0, 0 ,0, 10);
		}
	}
	
	if(at.msg_rdata_dsp != NULL)
	{
		at.msg_rdata_dsp(at.msg_rnum, at.msg_rdata);
	}
	
	return(0);
}



/* +CMGR: 读取短消息 */
BYTE at_cmgr_dsp(BYTE* data_addr)
{
	return(0);
}



/* +CMGS: 发送短信回应 */
BYTE at_cmgs_dsp(BYTE *data_addr)
{
	print_debug_msg("收到+cmgs");
	if(at_mission_now % at_mission_msg_lenth == at_mission_cmgs)
	{
		print_debug_msg("短信发送确认");
		at.msg_tlenth = 0;
		at.msg_tRetry = 0;
	}
	return(0);
}




/* 运营商 */
BYTE at_cops_dsp(BYTE *data_addr)
{	
	BYTE i;
	
	data_addr += search_text_para(3, data_addr) + 1;
	for(i = 0; i < cops_lenth; i++)
	{
		at.cops[i] = *data_addr;
		if(*data_addr == '"') 
		{
			break;
		}
		data_addr++;
	}
	at.cops[i] = 0;
	return(0);
}



/* IMSI码 */
BYTE at_cimi_dsp(BYTE *data_addr)
{
	BYTE i;
	
	for(i = 0; i < cimi_lenth; i++)
	{
		if(isdigit(data_addr[i]))
		{
			at.cimi[i] = data_addr[i];
		}
		else
		{
			at.cimi[i] = 0;
			break;
		}
	}
	return(0);
}




/* 模块SN编码 */
BYTE at_cgsn_dsp(BYTE *data_addr)
{	
	BYTE i;
	
	for(i = 0; i < imei_lenth; i++)
	{	
		if(isdigit(data_addr[i]))
		{
			at.imei[i] = data_addr[i];
		}
		else
		{
			at.imei[i] = 0;
			break;
		}
	}
	return(0);
}



/* +QNITZ: 网络时间同步 */
BYTE at_qnitz_dsp(BYTE *data_addr)
{
	BYTE* timestr;
	BYTE i;
	
	/* +QNITZ: "15/06/20,03:00:14+32,0" */
	if(strlen((char*)data_addr) > 30 && data_addr[8] == '"')
	{
		timestr = &data_addr[9];
		for(i = 0; i < 6; i++)
		{
			if(!isdigit(timestr[0]) && !isdigit(timestr[1]))
			{
				return(1);
			}
			*((BYTE*)&at.web_time.year+i+1) = ((timestr[0]-'0')*10)+timestr[1]-'0';
			timestr += 3;
		}
		at.web_time.year >>= 8;
		at.web_time.year += 2000;
		time_adjust_zone(&at.web_time, 8);
		if(at.flag_time == 0)
		{
			at.flag_time = 1;
			if(Gps_info.status != 'A')
			{
				setSysTime(&at.web_time);
				printf_time_msg("GSM网络校时");
			}
		}
	}
	
	return(0);
}



/* SIM卡的CCID */
BYTE at_qccid_dsp(BYTE *data_addr)
{
	BYTE i;
	
	for(i = 0; i < imei_lenth; i++)
	{	
		if(isdigit(data_addr[i])
		|| (data_addr[i] >= 'A' && data_addr[i] <= 'Z')
		|| (data_addr[i] >= 'a' && data_addr[i] <= 'z'))
		{
			at.ccid[i] = data_addr[i];
		}
		else
		{
			at.ccid[i] = 0;
			break;
		}
	}
	return(0);
}


/* 多连接状态下的Socket信息解析 */
BYTE at_tcpipMux_dsp(BYTE *data_addr)
{
	BYTE socket_num;
	
	socket_num = data_addr[0] - '0';
	if(memcmp((char*)data_addr + 3, "CONNECT OK", strlen("CONNECT OK")) == 0)
	{
		print_debug_msg("建立SOCKET[%d],成功", socket_num);
		at.socket[socket_num].state = CONNECT_CONNECT_OK;
	}
	else if(memcmp((char*)data_addr + 3, "ALREADY CONNECT", strlen("ALREADY CONNECT")) == 0)
	{
		print_debug_msg("SOCKET[%d],已建立", socket_num);
		at.socket[socket_num].state = CONNECT_CONNECT_OK;
	}
	else if(memcmp((char*)data_addr + 3, "CONNECT FAIL", strlen("CONNECT FAIL")) == 0)
	{
		print_debug_msg("建立SOCKET[%d],失败", socket_num);
		at.socket[socket_num].state = CONNECT_CONNECT_FAIL;
	}
	else if(memcmp((char*)data_addr + 3, "CLOSE OK", strlen("CLOSE OK")) == 0)
	{
		at.socket[socket_num].state = CONNECT_IDLE;
	}
	else if(memcmp((char*)data_addr + 3, "CLOSE", strlen("CLOSE")) == 0)
	{
		print_debug_msg("SOCKET[%d]远端关闭", socket_num);
		at.socket[socket_num].state = CONNECT_IDLE;
		if(socket_num == 0)
		{
			ServerPara.ctrl_step = ctrl_step_wait_reg;
		}
	}
	else if(memcmp((char*)data_addr + 3, "SEND OK", strlen("SEND OK")) == 0)
	{
		if(at_mission_now / 100 == socket_num
		&& at.socket[socket_num].send_status == SEND_WAIT_TCPACK)
		{
			at.socket[socket_num].send_status = SEND_IDLE;
			if(at.socket[socket_num].tlenth > at.socket[socket_num].tcount)
			{
				at.socket[socket_num].tlenth -= at.socket[socket_num].tcount;
			}
			else
			{
				at.socket[socket_num].tlenth = 0;
			}
			at.socket[socket_num].tcount = 0;
			at_mission_now = at_no_mission;
		}
	}
	else if(memcmp((char*)data_addr + 3, "SEND FAIL", strlen("SEND FAIL")) == 0)
	{
		if(at_mission_now / 100 == socket_num
		&& at.socket[socket_num].send_status == SEND_WAIT_TCPACK)
		{
			at.socket[socket_num].send_status = SEND_IDLE;
			at.socket[socket_num].tcount = 0;
			at_mission_now = at_no_mission;
		}
	}
	
	return (0);
}




/* +QCELLLOC: 基站定位信息 */
BYTE at_qcellloc_dsp(BYTE *data_addr)
{
	Status_stu.reg.gps_fix = 1;
	at.loc_lo = search_data_para2(1, data_addr, 6);
	at.loc_la = search_data_para2(2, data_addr, 6);
	return(0);
}



/* TCPIP数据接收 */
BYTE at_tcpipRcv_dsp(BYTE *data_addr)
{
	BYTE socket_num;
	WORD lenth;
	BYTE* strp;
	BYTE data;
	WORD i;
	WORD move_lenth = 0;
	WORD time_add = 0;
	
	strp = data_addr + strlen("+RECEIVE: ");
	
	/* 多连接模式 */
	if(strp[0] >= '0' && strp[0] <= '5' && strp[1] == ',' && strp[2] == ' ')
	{
		socket_num = strp[0] - '0';
		lenth = (WORD)search_data_para(2, data_addr);
		
		if(lenth > 1000)
		{
			return(1);
		}
		
		i = 0;
		time_add = 0;
		
		/* 剩余空间不足 */
		if(at.socket[socket_num].rlenth + lenth > at.socket[socket_num].rdata_limit)
		{
			move_lenth = at.socket[socket_num].rlenth + lenth - at.socket[socket_num].rdata_limit;
			memmove(at.socket[socket_num].rdata, 
					at.socket[socket_num].rdata + move_lenth, 
					at.socket[socket_num].rlenth - move_lenth);
			at.socket[socket_num].rlenth -= move_lenth;
		}
		
		while(i < lenth && time_add < 1000)
		{
			if(GSMRcv_lenth() > 0)
			{
				data = GSMRcv_pop();
				i++;
				if(at.socket[socket_num].socket_rdata_program != NULL)
				{
					if(at.socket[socket_num].rlenth < at.socket[socket_num].rdata_limit)
					{
						at.socket[socket_num].rdata[at.socket[socket_num].rlenth] = data;
						at.socket[socket_num].rlenth++;
					}
				}
			}
			else
			{
				time_add += 10;
				OSTimeDlyHMSM(0, 0 ,0, 10);
			}
		}
		if(at.socket[socket_num].socket_rdata_program != NULL)
		{
			at.socket[socket_num].socket_rdata_program(at.socket[socket_num].rlenth);
		}
	}
	return(0);
}



/* > 开始发送数据 */
BYTE at_sendBegin_dsp(BYTE *data_addr)
{
	BYTE socket_num;
	
	if(at_mission_now % 100 == at_mission_qisend)
	{
		socket_num = at_mission_now / 100;
		if(at.socket[socket_num].send_status == SEND_BEGIN)
		{
			OSMboxPost(mbox_at_answer, (void*)at_answer_ok);
		}
	}
	else if(at_mission_now % 100 == at_mission_cmgs)
	{
		if(at.msg_tstatus == SEND_BEGIN)
		{
			at.msg_tstatus = SEND_TO_MODULE;
		}
	}
	return(0);
}




/*******************************************
函数名称: build_socket
函数功能: 设置Socket信息
入口参数: socket_num,	socket编号
          addr,			服务器地址
          port,         服务器端口
          protocol,     协议类型
出口参数: 
备    注: 
*******************************************/
BYTE build_socket(BYTE socket_num, char* addr, WORD port, PROTOCOL protocol)
{	
	if(socket_num < SOCKET_AMOUNT)
	{
		strcpy((char*)at.socket[socket_num].addr, (char*)addr);
		at.socket[socket_num].port = port;
		at.socket[socket_num].protocol = protocol;
		at.socket[socket_num].on = 1;
		at.socket[socket_num].state = CONNECT_IDLE;
		return(0);
	}
	else
	{
		return (1);
	}
}




/*******************************************
函数名称: open_socket
函数功能: 关闭socket
入口参数: socket_num,	socket编号
出口参数: 
备    注: 
*******************************************/
BYTE close_socket(BYTE socket_num)
{	
	if(socket_num <SOCKET_AMOUNT)
	{	
		if(at.socket[socket_num].on)
		{
			OSQPost(mbox_at_mission, (void*)(at_mission_qiclose + (socket_num * 100)));
		}
		at.socket[socket_num].on = 0;	
		ServerPara.err_times = 0;
		return(1);
	}
	return(0);
}






