/******************************************
文件名称: jt_t808_send.c
作	  者: byy
版    本:
说    明: JT/T808协议层解析,组织
修改记录: 2015-06-26,程序建立
******************************************/
#include "all_use.h"





/*******************************************
函数功能: 发送注册信息
入口参数:
出口参数: 
备    注: 
*******************************************/
void send_regist_data(void)
{
	BYTE* data_body = &ProtocolMakeBuf[13];
	WORD s_lenth;
	BYTE err;

	OSSemPend(sem_make_data_ok, 1000, &err);
	/* 省 */
	data_body[0] = 0;
	data_body[1] = 0x7E;
	/* 市 */
	data_body[2] = 0;
	data_body[3] = 0x7D;
	/* 制造商ID */
	memcpy(&data_body[4], "75533", 5);
	/* 终端型号 */
	memset(&data_body[9], 0, 20);
	memcpy(&data_body[9], "ZL001_A", 7);
	/* 设备ID */
	memcpy(&data_body[29], "0000001", 7);
	/* 车牌颜色 */
	data_body[36] = 1;
	/* 车牌号 */
	memcpy(&data_body[37], "鲁AM0001", 8);
	
	s_lenth = t808_make_protocol(terminal_regist, 45);
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}



/*******************************************
函数功能: 发送鉴权信息
入口参数:
出口参数: 
备    注: 
*******************************************/
void send_login_data(void)
{
	BYTE* data_body = &ProtocolMakeBuf[13];
	WORD s_lenth;
	BYTE err;

	OSSemPend(sem_make_data_ok, 1000, &err);
	
	strcpy((char*)data_body, (char*)Device.regist_code);
	s_lenth = strlen((char*)Device.regist_code);
	
	s_lenth = t808_make_protocol(terminal_authentication, s_lenth);
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}




/*******************************************
函数功能: 发送心跳
入口参数: 
出口参数: 
备    注: 
*******************************************/
void send_heartbeat(void)
{
	WORD s_lenth = 0;
	BYTE err;

	OSSemPend(sem_make_data_ok, 1000, &err);
	
	s_lenth = t808_make_protocol(terminal_heartbeat, s_lenth);	
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}





/*******************************************
函数功能: 发送位置信息
入口参数:
出口参数: 
备    注: 
*******************************************/
void send_location_info(void)
{
	BYTE* data_body = &ProtocolMakeBuf[13];
	WORD s_lenth = 0;
	BYTE err;
	
	if(ServerPara.state != SERVER_CONNECT_AUTHENOK)
	{
		return;
	}

	OSSemPend(sem_make_data_ok, 1000, &err);
	s_lenth += t808_make_location_info(data_body);
	s_lenth = t808_make_protocol(terminal_up_gps_data, s_lenth);
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}




/*******************************************
函数功能: 发送通用应答
入口参数: 
出口参数: 
备    注: 
*******************************************/
void send_normal_answer(WORD cycle_num, WORD id, BYTE result)
{
	BYTE* data_body = &ProtocolMakeBuf[13];
	WORD s_lenth;
	BYTE err;

	OSSemPend(sem_make_data_ok, 1000, &err);
	printf_time_msg("-->>发送通用应答,ID:%0.4X", id);
		
	/* 应答流水号 */
	data_body[0] = (BYTE)(cycle_num >> 8);
	data_body[1] = (BYTE)(cycle_num >> 0);
	/* 应答ID */
	data_body[2] = (BYTE)(id >> 8);
	data_body[3] = (BYTE)(id >> 0);
	/* 结果码 */
	data_body[4] = result;
	s_lenth = 5;
	
	s_lenth = t808_make_protocol(terminal_normal_answer, s_lenth);
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}




/*******************************************
函数功能: 查询位置信息应答
入口参数:
出口参数: 
备    注: 
*******************************************/
void send_request_location_answer(WORD cycle_num)
{
	BYTE* data_body = &ProtocolMakeBuf[13];
	WORD s_lenth = 0;
	BYTE err;

	OSSemPend(sem_make_data_ok, 1000, &err);
	printf_time_msg("-->>查询位置信息应答");
	
	/* 应答流水号 */	
	short_to_two_char(cycle_num, data_body);
	s_lenth += 2;
	/* 位置信息 */
	s_lenth += t808_make_location_info(data_body + 2);	
	s_lenth = t808_make_protocol(terminal_anwser_gps_data, s_lenth);	
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}

