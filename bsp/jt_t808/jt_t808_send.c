/******************************************
�ļ�����: jt_t808_send.c
��	  ��: byy
��    ��:
˵    ��: JT/T808Э������,��֯
�޸ļ�¼: 2015-06-26,������
******************************************/
#include "all_use.h"





/*******************************************
��������: ����ע����Ϣ
��ڲ���:
���ڲ���: 
��    ע: 
*******************************************/
void send_regist_data(void)
{
	BYTE* data_body = &ProtocolMakeBuf[13];
	WORD s_lenth;
	BYTE err;

	OSSemPend(sem_make_data_ok, 1000, &err);
	/* ʡ */
	data_body[0] = 0;
	data_body[1] = 0x7E;
	/* �� */
	data_body[2] = 0;
	data_body[3] = 0x7D;
	/* ������ID */
	memcpy(&data_body[4], "75533", 5);
	/* �ն��ͺ� */
	memset(&data_body[9], 0, 20);
	memcpy(&data_body[9], "ZL001_A", 7);
	/* �豸ID */
	memcpy(&data_body[29], "0000001", 7);
	/* ������ɫ */
	data_body[36] = 1;
	/* ���ƺ� */
	memcpy(&data_body[37], "³AM0001", 8);
	
	s_lenth = t808_make_protocol(terminal_regist, 45);
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}



/*******************************************
��������: ���ͼ�Ȩ��Ϣ
��ڲ���:
���ڲ���: 
��    ע: 
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
��������: ��������
��ڲ���: 
���ڲ���: 
��    ע: 
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
��������: ����λ����Ϣ
��ڲ���:
���ڲ���: 
��    ע: 
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
��������: ����ͨ��Ӧ��
��ڲ���: 
���ڲ���: 
��    ע: 
*******************************************/
void send_normal_answer(WORD cycle_num, WORD id, BYTE result)
{
	BYTE* data_body = &ProtocolMakeBuf[13];
	WORD s_lenth;
	BYTE err;

	OSSemPend(sem_make_data_ok, 1000, &err);
	printf_time_msg("-->>����ͨ��Ӧ��,ID:%0.4X", id);
		
	/* Ӧ����ˮ�� */
	data_body[0] = (BYTE)(cycle_num >> 8);
	data_body[1] = (BYTE)(cycle_num >> 0);
	/* Ӧ��ID */
	data_body[2] = (BYTE)(id >> 8);
	data_body[3] = (BYTE)(id >> 0);
	/* ����� */
	data_body[4] = result;
	s_lenth = 5;
	
	s_lenth = t808_make_protocol(terminal_normal_answer, s_lenth);
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}




/*******************************************
��������: ��ѯλ����ϢӦ��
��ڲ���:
���ڲ���: 
��    ע: 
*******************************************/
void send_request_location_answer(WORD cycle_num)
{
	BYTE* data_body = &ProtocolMakeBuf[13];
	WORD s_lenth = 0;
	BYTE err;

	OSSemPend(sem_make_data_ok, 1000, &err);
	printf_time_msg("-->>��ѯλ����ϢӦ��");
	
	/* Ӧ����ˮ�� */	
	short_to_two_char(cycle_num, data_body);
	s_lenth += 2;
	/* λ����Ϣ */
	s_lenth += t808_make_location_info(data_body + 2);	
	s_lenth = t808_make_protocol(terminal_anwser_gps_data, s_lenth);	
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}

