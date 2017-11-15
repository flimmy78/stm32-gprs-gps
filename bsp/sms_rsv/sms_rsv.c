/******************************************
文件名称: msg_rsv.c
作	  者: byy
版    本:
说    明: 短消息协议解析
修改记录: 2011-08-18,程序建立。
******************************************/
#include "all_use.h"  



static BYTE rsv_buf[20];
static WORD responses_msg(const char *format,...);
static WORD transfor_7bit_to_Btye(BYTE* dis, BYTE* src, WORD lenth);



/*******************************************
函数功能: 短消息解析
入口参数: num: 来自的手机号
		  addr
出口参数: 
备    注: *zl,______#
*******************************************/
BYTE recv_msg_rsv(BYTE* number, BYTE* msg)
{
	WORD msg_lenth;
	DWORD rcvVal = 0;
	WORD i;
	WORD temp = 0;
	BYTE encoding;
	BYTE* gbk_msg = at.msg_rdata;
	BYTE alarm_num = 0;
	
	temp = texthex_to_data(msg);	//长度08即SCA区去除08外后面的字节数单位是字节
	msg += temp * 2 + 2;			//跳过短消息中信号
	msg += 2; 						//跳过消息信息
	temp = texthex_to_data(msg);	//获取源号码长度,Length为地址的数字个数
	msg += 2;						//跳过长度
	msg += 2;						//跳过地址类型
	msg += 2;						//跳过68
	for(i = 0; i < temp - 2;)		//获取短信号码
	{	
		at.msg_rnum[i] = *(msg + 1);
		i++;
   		if(*msg == 'F') 
		{
			msg += 2;
			break;
		}
		else 
		{
			at.msg_rnum[i] = *msg;
		}
		i++;
		msg += 2;
	}
	at.msg_rnum[i] = '\0';
	if(!isCellPhoneNumber((char*)at.msg_rnum))
	{
		print_debug_msg("接收短信手机号有误!");
		memset(at.msg_rnum, 0, 12);
		return 0;
	}
	msg += 2; 							//跳过PID
	encoding = texthex_to_data(msg);	//获取消息编码方式
	encoding = (encoding >> 2) & 0x03;	//只关注两位
	msg += 2;
	at.msg_rtime[0]  = (msg[1]  - '0') * 10 + msg[0]  - '0';
	at.msg_rtime[1]  = (msg[3]  - '0') * 10 + msg[2]  - '0';
	at.msg_rtime[2]  = (msg[5]  - '0') * 10 + msg[4]  - '0';
	at.msg_rtime[3]  = (msg[7]  - '0') * 10 + msg[6]  - '0';
	at.msg_rtime[4]  = (msg[9]  - '0') * 10 + msg[8]  - '0';
	at.msg_rtime[5]  = (msg[11] - '0') * 10 + msg[10] - '0';
	msg += 14;							//跳过时间
	msg_lenth = texthex_to_data(msg);	//数据长度
	msg  += 2;
	print_debug_msg("手机号:%s,时间:%0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d,长度:%d;", at.msg_rnum, 
					at.msg_rtime[0], at.msg_rtime[1], at.msg_rtime[2], 
					at.msg_rtime[3], at.msg_rtime[4], at.msg_rtime[5], msg_lenth);
	switch(encoding)
	{
		case 0:
			transfor_7bit_to_Btye(gbk_msg, msg, msg_lenth);
			print_debug_msg("类型:7BIT;内容:%s", gbk_msg);
			break;
		case 1:
			print_debug_msg("类型:8BIT;");
			msg_lenth = 0;
			break;
		case 2:
			print_debug_msg("类型:UNICODE;");
			msg_lenth = 0;
			break;
		default:
			print_debug_msg("类型:未知;");
			msg_lenth = 0;
			break;
	}
	
	/* 信息长度 */
	if(msg_lenth == 0)
	{
		return(0);
	}
	
	/* 检验头和尾 */
	if(!stringIsSame((char*)gbk_msg, "*zl,") && gbk_msg[msg_lenth - 1] != '#')
	{
		return(0);
	}
	
	/* 设置服务器IP地址和端口| *zl,ip,124.133.2.36,3999# */
	if(stringIsSame((char*)gbk_msg + 4, "ip,"))
	{
		if(check_para_num(gbk_msg) != 3)
		{
			responses_msg("设置服务器IP.\r\n参数错误!");
			return(0);
		}		
		memset(rsv_buf, 0, 20);
		if(search_text_para(3, gbk_msg) - search_text_para(2, gbk_msg) < 20)
		{
			memcpy(rsv_buf, gbk_msg + search_text_para(2, gbk_msg), 
				   search_text_para(3, gbk_msg) - search_text_para(2, gbk_msg) - 1);
		}
		rcvVal = search_data_para(3, gbk_msg);
		if(!isIP((char*)rsv_buf) || rcvVal == 0 || rcvVal > 65535)
		{
			responses_msg("设置服务器IP.\r\n参数错误!");
			return(0);
		}
		Device.server_ip_int = IPstring2int((char*)rsv_buf);
		Device.server_port = rcvVal;
		Device.regist_code[0] = 0;
		ServerPara.ctrl_step = ctrl_step_wait_reg;
		write_device_info_to_flash();
		responses_msg("设置服务器IP.成功!\r\nIP:%s,\r\nport:%d", rsv_buf, rcvVal);
	}
	
	/* 设置终端手机号| *zl,id,13335197169# */
	else if(stringIsSame((char*)gbk_msg + 4, "id,"))
	{
		if(check_para_num(gbk_msg) != 2)
		{
			responses_msg("修改ID号.\r\n参数错误!");
			return(0);
		}
		
		if((msg_lenth - strlen("*zl,id,") - 1) != 11)
		{
			responses_msg("修改ID号.\r\n错误:长度必须为11位!");
			return(0);
		}
		
		for(i = 0; i < 11; i++)
		{
			if(!isdigit(gbk_msg[msg_lenth - 12 + i]))
			{
				responses_msg("修改ID号.\r\n错误:ID只能包含数字!");
				return(0);
			}
		}
		
		memset(rsv_buf, 0, 20);
		rsv_buf[0] = '0';
		memcpy(rsv_buf + 1, gbk_msg + strlen("*zl,id,"), 11);
		responses_msg("修改ID号.\r\n成功!\r\n修改后ID:%s", rsv_buf);
		memcpy((char*)Device.phone_num, rsv_buf, 13);
		Device.regist_code[0] = 0;
		ServerPara.ctrl_step = ctrl_step_wait_reg;
		write_device_info_to_flash();
	}
	
	/* 设置报警手机号| *zl,anum,1,13335197169# */
	else if(stringIsSame((char*)gbk_msg + 4, "anum"))
	{
		if(check_para_num(gbk_msg) != 3)
		{
			responses_msg("设置报警手机号.\r\n参数错误-1!");
			return(0);
		}
		
		if((gbk_msg[strlen("*zl,anum,")] != '1' && gbk_msg[strlen("*zl,anum,")] != '2')
		|| gbk_msg[strlen("*zl,anum,") + 1] != ',')
		{
			responses_msg("设置报警手机号.\r\n参数错误-2!");
			return(0);
		}
		
		alarm_num = gbk_msg[strlen("*zl,anum,")] - '1';
		
		if((msg_lenth - strlen("*zl,anum,1,") - 1) != 11)
		{
			responses_msg("设置报警手机号.\r\n错误:手机号必须11位!");
			return(0);
		}
		
		for(i = 0; i < 11; i++)
		{
			if(!isdigit(gbk_msg[msg_lenth - 12 + i]))
			{
				responses_msg("设置报警手机号.\r\n错误:只能包含数字!");
				return(0);
			}
		}
		
		memset(rsv_buf, 0, 20);
		memcpy(rsv_buf, gbk_msg + strlen("*zl,anum,1,"), 11);
		responses_msg("设置报警手机号%d.\r\n成功!\r\n%s", alarm_num + 1, rsv_buf);
		memcpy((char*)Device.alarm_sms_phone[alarm_num], rsv_buf, 12);
		write_device_info_to_flash();
	}
	
	/* 获取终端信息| *zl,info# */
	else if(stringIsSame((char*)gbk_msg + 4, "info"))
	{
		responses_msg("版本:%s@%02d%02d%02d%02d%02d%02d\r\n服务器:%s:%d", Version,
					  CompileTime.year % 100, CompileTime.month, CompileTime.day,
					  CompileTime.hour, CompileTime.min, CompileTime.sec,
					  IPint2string(Device.server_ip_int, (char*)rsv_buf), Device.server_port);
	}
	
	else
	{
		responses_msg("错误:命令不识别!");
	}
	return(0);
}




/*******************************************
函数功能: 7bit格式转化为BYTE型
入口参数: 
出口参数: 
备    注: 
*******************************************/
static WORD transfor_7bit_to_Btye(BYTE* dis, BYTE* src, WORD lenth)
{
	WORD relenth = 0;
	WORD bit7_lenth = 0;
	WORD i = 0;
	BYTE temp = 0;
	
	
	bit7_lenth = (lenth * 7 + 7) / 8;
	for(i = 0; i < bit7_lenth; i++)
	{
		dis[i / 7 * 8 + i % 7] = temp >> (8 - (i % 7));
		temp = texthex_to_data(&src[2 * i]);
		dis[i / 7 * 8 + i % 7] += temp << (i % 7);
		dis[i / 7 * 8 + i % 7] &= 0x7F;
		if(i % 7 == 6)
		{
			dis[i / 7 * 8 + 7] = temp >> 1;
		}
	}
	dis[lenth] = '\0';
	return relenth;
}







/*******************************************
函数功能: 短消息回应
入口参数: 
出口参数: 
备    注: 
*******************************************/
static WORD responses_msg(const char *format,...)
{
	WORD lenth = 0;
	va_list args;
	
	strcpy((char*)at.msg_tnum, (char*)at.msg_rnum);
	lenth = snprintf((char*)at.msg_tdata, msg_data_lenth, "ID:%s\r\n", Device.phone_num);
	va_start(args, format);
	lenth += vsprintf((char*)at.msg_tdata + lenth, format, args);
	va_end(args);
	at.msg_tlenth = lenth;
	return(lenth);
}	





/*******************************************
函数功能: 向指定号码发送短信
入口参数: 
出口参数: 
备    注: 
*******************************************/
WORD send_msg_to_num(char* num, const char *format,...)
{
	WORD lenth = 0;
	va_list args;
	
	if(strlen(num) == 0)
	{
		return(0);
	}
	strcpy((char*)at.msg_tnum, num);
	va_start(args, format);
	lenth = vsprintf((char*)at.msg_tdata + lenth, format, args);
	va_end(args);
	at.msg_tlenth = lenth;
	at.msg_tRetry = 3;		// 发送尝试3次
	return(lenth);
}




static BYTE uni_buf[140];
static char* phone_to_pdu(char* pdu, char* phone)
{
	BYTE i;
	BYTE lenth;
	
	lenth = strlen(phone);
	for(i = 0; i < lenth / 2; i++)
	{
		pdu[i * 2 + 0] = phone[i * 2 + 1];
		pdu[i * 2 + 1] = phone[i * 2 + 0];
	}
	if(lenth % 2 == 1)
	{
		pdu[lenth - 1] = 'F';
		pdu[lenth] = phone[lenth - 1];
	}
	return pdu;
}

/*******************************************
函数功能: 组装PDU短信格式
入口参数: oa,	对方号码
出口参数: 
备    注: 
*******************************************/
WORD make_pdu_sms(char* pdu_buf, char* oa, BYTE* msg)
{
	WORD lenth = 0;
	BYTE udl;
	WORD i;
	
	memset(pdu_buf, 0, 200);
	lenth = 0;
	memcpy(pdu_buf + lenth, "00", 2);
	lenth += 2;
	/* PDU Type */
	memcpy(pdu_buf + lenth, "11", 2);
	lenth += 2;
	/* 收件人号码 */
	memcpy(pdu_buf + lenth, "000D9168", 8);
	lenth += 8;
	phone_to_pdu(pdu_buf + lenth, oa);
	lenth = strlen(pdu_buf);
	/* PID:协议标识 */
	memcpy(pdu_buf + lenth, "00", 2);
	lenth += 2;
	/* DCS:数据编码方案，USC2 */
	memcpy(pdu_buf + lenth, "08", 2);
	lenth += 2;
	/* VP:信息有效期 */
	memcpy(pdu_buf + lenth, "00", 2);
	lenth += 2;
	/* UDL:数据长度 */
	udl = GBToUni(uni_buf, msg, 140);
	snprintf(pdu_buf + lenth, 3, "%0.2X", udl);
	lenth += 2;
	/* UD:数据 */
	for(i = 0; i < udl; i++)
	{
		snprintf(pdu_buf + lenth, 3, "%0.2X", uni_buf[i]);
		lenth += 2;
	}
	return lenth;
}



