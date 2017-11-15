/******************************************
�ļ�����: msg_rsv.c
��	  ��: byy
��    ��:
˵    ��: ����ϢЭ�����
�޸ļ�¼: 2011-08-18,��������
******************************************/
#include "all_use.h"  



static BYTE rsv_buf[20];
static WORD responses_msg(const char *format,...);
static WORD transfor_7bit_to_Btye(BYTE* dis, BYTE* src, WORD lenth);



/*******************************************
��������: ����Ϣ����
��ڲ���: num: ���Ե��ֻ���
		  addr
���ڲ���: 
��    ע: *zl,______#
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
	
	temp = texthex_to_data(msg);	//����08��SCA��ȥ��08�������ֽ�����λ���ֽ�
	msg += temp * 2 + 2;			//��������Ϣ���ź�
	msg += 2; 						//������Ϣ��Ϣ
	temp = texthex_to_data(msg);	//��ȡԴ���볤��,LengthΪ��ַ�����ָ���
	msg += 2;						//��������
	msg += 2;						//������ַ����
	msg += 2;						//����68
	for(i = 0; i < temp - 2;)		//��ȡ���ź���
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
		print_debug_msg("���ն����ֻ�������!");
		memset(at.msg_rnum, 0, 12);
		return 0;
	}
	msg += 2; 							//����PID
	encoding = texthex_to_data(msg);	//��ȡ��Ϣ���뷽ʽ
	encoding = (encoding >> 2) & 0x03;	//ֻ��ע��λ
	msg += 2;
	at.msg_rtime[0]  = (msg[1]  - '0') * 10 + msg[0]  - '0';
	at.msg_rtime[1]  = (msg[3]  - '0') * 10 + msg[2]  - '0';
	at.msg_rtime[2]  = (msg[5]  - '0') * 10 + msg[4]  - '0';
	at.msg_rtime[3]  = (msg[7]  - '0') * 10 + msg[6]  - '0';
	at.msg_rtime[4]  = (msg[9]  - '0') * 10 + msg[8]  - '0';
	at.msg_rtime[5]  = (msg[11] - '0') * 10 + msg[10] - '0';
	msg += 14;							//����ʱ��
	msg_lenth = texthex_to_data(msg);	//���ݳ���
	msg  += 2;
	print_debug_msg("�ֻ���:%s,ʱ��:%0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d,����:%d;", at.msg_rnum, 
					at.msg_rtime[0], at.msg_rtime[1], at.msg_rtime[2], 
					at.msg_rtime[3], at.msg_rtime[4], at.msg_rtime[5], msg_lenth);
	switch(encoding)
	{
		case 0:
			transfor_7bit_to_Btye(gbk_msg, msg, msg_lenth);
			print_debug_msg("����:7BIT;����:%s", gbk_msg);
			break;
		case 1:
			print_debug_msg("����:8BIT;");
			msg_lenth = 0;
			break;
		case 2:
			print_debug_msg("����:UNICODE;");
			msg_lenth = 0;
			break;
		default:
			print_debug_msg("����:δ֪;");
			msg_lenth = 0;
			break;
	}
	
	/* ��Ϣ���� */
	if(msg_lenth == 0)
	{
		return(0);
	}
	
	/* ����ͷ��β */
	if(!stringIsSame((char*)gbk_msg, "*zl,") && gbk_msg[msg_lenth - 1] != '#')
	{
		return(0);
	}
	
	/* ���÷�����IP��ַ�Ͷ˿�| *zl,ip,124.133.2.36,3999# */
	if(stringIsSame((char*)gbk_msg + 4, "ip,"))
	{
		if(check_para_num(gbk_msg) != 3)
		{
			responses_msg("���÷�����IP.\r\n��������!");
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
			responses_msg("���÷�����IP.\r\n��������!");
			return(0);
		}
		Device.server_ip_int = IPstring2int((char*)rsv_buf);
		Device.server_port = rcvVal;
		Device.regist_code[0] = 0;
		ServerPara.ctrl_step = ctrl_step_wait_reg;
		write_device_info_to_flash();
		responses_msg("���÷�����IP.�ɹ�!\r\nIP:%s,\r\nport:%d", rsv_buf, rcvVal);
	}
	
	/* �����ն��ֻ���| *zl,id,13335197169# */
	else if(stringIsSame((char*)gbk_msg + 4, "id,"))
	{
		if(check_para_num(gbk_msg) != 2)
		{
			responses_msg("�޸�ID��.\r\n��������!");
			return(0);
		}
		
		if((msg_lenth - strlen("*zl,id,") - 1) != 11)
		{
			responses_msg("�޸�ID��.\r\n����:���ȱ���Ϊ11λ!");
			return(0);
		}
		
		for(i = 0; i < 11; i++)
		{
			if(!isdigit(gbk_msg[msg_lenth - 12 + i]))
			{
				responses_msg("�޸�ID��.\r\n����:IDֻ�ܰ�������!");
				return(0);
			}
		}
		
		memset(rsv_buf, 0, 20);
		rsv_buf[0] = '0';
		memcpy(rsv_buf + 1, gbk_msg + strlen("*zl,id,"), 11);
		responses_msg("�޸�ID��.\r\n�ɹ�!\r\n�޸ĺ�ID:%s", rsv_buf);
		memcpy((char*)Device.phone_num, rsv_buf, 13);
		Device.regist_code[0] = 0;
		ServerPara.ctrl_step = ctrl_step_wait_reg;
		write_device_info_to_flash();
	}
	
	/* ���ñ����ֻ���| *zl,anum,1,13335197169# */
	else if(stringIsSame((char*)gbk_msg + 4, "anum"))
	{
		if(check_para_num(gbk_msg) != 3)
		{
			responses_msg("���ñ����ֻ���.\r\n��������-1!");
			return(0);
		}
		
		if((gbk_msg[strlen("*zl,anum,")] != '1' && gbk_msg[strlen("*zl,anum,")] != '2')
		|| gbk_msg[strlen("*zl,anum,") + 1] != ',')
		{
			responses_msg("���ñ����ֻ���.\r\n��������-2!");
			return(0);
		}
		
		alarm_num = gbk_msg[strlen("*zl,anum,")] - '1';
		
		if((msg_lenth - strlen("*zl,anum,1,") - 1) != 11)
		{
			responses_msg("���ñ����ֻ���.\r\n����:�ֻ��ű���11λ!");
			return(0);
		}
		
		for(i = 0; i < 11; i++)
		{
			if(!isdigit(gbk_msg[msg_lenth - 12 + i]))
			{
				responses_msg("���ñ����ֻ���.\r\n����:ֻ�ܰ�������!");
				return(0);
			}
		}
		
		memset(rsv_buf, 0, 20);
		memcpy(rsv_buf, gbk_msg + strlen("*zl,anum,1,"), 11);
		responses_msg("���ñ����ֻ���%d.\r\n�ɹ�!\r\n%s", alarm_num + 1, rsv_buf);
		memcpy((char*)Device.alarm_sms_phone[alarm_num], rsv_buf, 12);
		write_device_info_to_flash();
	}
	
	/* ��ȡ�ն���Ϣ| *zl,info# */
	else if(stringIsSame((char*)gbk_msg + 4, "info"))
	{
		responses_msg("�汾:%s@%02d%02d%02d%02d%02d%02d\r\n������:%s:%d", Version,
					  CompileTime.year % 100, CompileTime.month, CompileTime.day,
					  CompileTime.hour, CompileTime.min, CompileTime.sec,
					  IPint2string(Device.server_ip_int, (char*)rsv_buf), Device.server_port);
	}
	
	else
	{
		responses_msg("����:���ʶ��!");
	}
	return(0);
}




/*******************************************
��������: 7bit��ʽת��ΪBYTE��
��ڲ���: 
���ڲ���: 
��    ע: 
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
��������: ����Ϣ��Ӧ
��ڲ���: 
���ڲ���: 
��    ע: 
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
��������: ��ָ�����뷢�Ͷ���
��ڲ���: 
���ڲ���: 
��    ע: 
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
	at.msg_tRetry = 3;		// ���ͳ���3��
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
��������: ��װPDU���Ÿ�ʽ
��ڲ���: oa,	�Է�����
���ڲ���: 
��    ע: 
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
	/* �ռ��˺��� */
	memcpy(pdu_buf + lenth, "000D9168", 8);
	lenth += 8;
	phone_to_pdu(pdu_buf + lenth, oa);
	lenth = strlen(pdu_buf);
	/* PID:Э���ʶ */
	memcpy(pdu_buf + lenth, "00", 2);
	lenth += 2;
	/* DCS:���ݱ��뷽����USC2 */
	memcpy(pdu_buf + lenth, "08", 2);
	lenth += 2;
	/* VP:��Ϣ��Ч�� */
	memcpy(pdu_buf + lenth, "00", 2);
	lenth += 2;
	/* UDL:���ݳ��� */
	udl = GBToUni(uni_buf, msg, 140);
	snprintf(pdu_buf + lenth, 3, "%0.2X", udl);
	lenth += 2;
	/* UD:���� */
	for(i = 0; i < udl; i++)
	{
		snprintf(pdu_buf + lenth, 3, "%0.2X", uni_buf[i]);
		lenth += 2;
	}
	return lenth;
}



