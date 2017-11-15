/******************************************
�ļ�����: gsm_opreat.c
��	  ��: byy
��    ��:
˵    ��: ͨ��ģ������
�޸ļ�¼: 2015-06-10,��������
******************************************/
#include "all_use.h"


/* AT���ݽ��մ�ӡ���� */
BOOL Flag_Printf_at_rcv = FALSE;



/*�������ڴ��ڽ��յĻ��ζ���*/
static const WORD GSMRcv_cap = 200;
static BYTE GSMRcv_buffer[GSMRcv_cap + 1];
static WORD GSMRcv_tail = 0;
static WORD GSMRcv_head = 0;
static void GSMRcv_push(BYTE data);
static void GSMRcv_rest(void);
BYTE GSMRcv_pop(void);
WORD GSMRcv_lenth(void);

static DWORD at_wait(WORD wait_time, DWORD if_ok, DWORD if_err,DWORD if_time_out, BYTE max_err_times);

/* ��ǰat���� */
DWORD at_mission_now;

/* PDU������ϻ��� */
static char PDU_msg_buf[400];



/*******************************************
��������: GSM_Operat_task
��������: GSMģ�����
��ڲ���: 
���ڲ���: 
��    ע: 
*******************************************/
void GSM_Operat_task(void *p_arg)
{
	BYTE  err;
	WORD  ats_i;
	DWORD no_at_missiontime_add = 0;
	DWORD at_mission_para;
	DWORD wait_ret;
	WORD  wait_time = 0;
	BYTE  no_at_step = 0;
	BYTE  flag_print = 1;
	WORD  pdu_lenth = 0;
	
	OSTimeDlyHMSM(0, 0, 1, 0);
	/* ��ʼ�� */
	init_at();
	at_mission_now = 0;
	OSQPost(mbox_at_mission, (void *)at_mission_poweron);
	at_mission_para = at_mission_para;
	
	while(1)
	{
		/* �Ӷ����ж�ȡAT���� */
		if(at_mission_now == 0)		
		{	
			at_mission_now = (DWORD)OSQAccept(mbox_at_mission, &err);
		}
		at_mission_para = at_mission_now / 100;
		
		switch(at_mission_now % 100)
		{
			case at_no_mission:	
				OSTimeDlyHMSM(0, 0, 0, 100);
			
				/* ģ���ϵ����� */
				if(at.state_power == 0) 
				{
					OSQPostFront(mbox_at_mission, (void *)at_mission_poweron);
					break;
				}
				
				/* ��ѯ��¼״̬ */
				if(at.state_reg == 0)
				{
					OSQPostFront(mbox_at_mission, (void *)at_mission_cregq);
					flag_print = 1;
					at.state_reg_wait = 0;
					break;
				}
				
				if(++no_at_missiontime_add < 5)
				{
					break;
				}
				no_at_missiontime_add = 0;
				
				/* Socket���� */
				for(ats_i = 0; ats_i < SOCKET_AMOUNT; ats_i++)
				{
					if(at.socket[ats_i].state == CONNECT_IDLE)
					{
						if(at.socket[ats_i].on && at.voice_state == voice_state_no)
						{
							/* �������� */
							OSQPost(mbox_at_mission, (void*)(at_mission_qiopen + (ats_i * 100)));
						}
					}
					else if(at.socket[ats_i].state == CONNECT_CONNECT_OK)
					{
						if(at.socket[ats_i].tlenth > 0)
						{
							/* �������� */
							OSQPost(mbox_at_mission, (void*)(at_mission_qisend + (ats_i * 100)));
						}
					}
				}
				
				/* ���ŷ��� */
				if(at.msg_tlenth != 0)
				{
					if(at.msg_tRetry == 0)
					{
						at.msg_tRetry = 0;
						at.msg_tlenth = 0;
					}
					else
					{
						at.msg_tRetry--;
						OSQPost(mbox_at_mission, (void*)at_mission_cmgs);
					}
					at.msg_tstatus = SEND_IDLE;
				}
				
				switch(no_at_step)
				{
					case 0:	// ��ѯ�ź�����
						OSQPost(mbox_at_mission, (void *)at_mission_csq);
						no_at_step++;
						break;
					
					case 1:	// ��ѯ��γ��
						OSQPost(mbox_at_mission, (void *)at_mission_qcellloc);
						no_at_step++;
						break;
					
					case 2:	// ��ѯ��ص���
						OSQPost(mbox_at_mission, (void *)at_mission_cbc);
						no_at_step++;
						break;
					
					default:
						no_at_step = 0;
						break;
				}
				break;
			
			case at_mission_poweron:	//��λ,�����ϵ�
				GSM_OFF();
				GSM_POWER_OFF();
				OSTimeDlyHMSM(0, 0, 2, 0);
				print_debug_msg("GSMģ�������ϵ�");
				GSM_POWER_ON();
				OSTimeDlyHMSM(0, 0, 2, 0);
				print_debug_msg("GSMģ������");
				GSM_ON();
				OSTimeDlyHMSM(0, 0, 2, 0);
				GSM_OFF();
				OSTimeDlyHMSM(0, 0, 1, 0);
				GSMRcv_rest();
				USART_Config(Usart_GSM,   19200);
				send_at_comand("      \r\n");
				at.state_power = 1;
				at_mission_now = at_mission_ipr;
				break;
			
			case at_mission_ipr:
				OSQFlush(mbox_at_mission);
				print_debug_msg("GSMģ�����ò�����");
				send_at_comand("AT+IPR=19200\r\n");
				at_wait(300, at_mission_f, at_mission_f, at_mission_reset, 3);
				break;
			
			case at_mission_f:
				print_debug_msg("GSMģ�鸴λ����");
				send_at_comand("AT&F\r\n");
				at_wait(300, at_mission_c_dcd, at_mission_c_dcd, at_mission_c_dcd, 1);
				break;
			
			case at_mission_c_dcd:
				print_debug_msg("GSMģ��ر�DCDģʽ");
				send_at_comand("AT&C1\r\n");
				at_wait(300, at_mission_d_dtr, at_mission_d_dtr, at_mission_d_dtr, 1);
				break;
			
			case at_mission_d_dtr:
				print_debug_msg("GSMģ�����DTR״̬");
				send_at_comand("AT&D0\r\n");
				at_wait(300, at_mission_icf, at_mission_icf, at_mission_icf, 1);
				break;
			
			case at_mission_icf:
				print_debug_msg("GSMģ�鴮��֡��ʽ");
				send_at_comand("AT+ICF=3\r\n");
				at_wait(300, at_mission_ifc, at_mission_ifc, at_mission_ifc, 1);
				break;
			
			case at_mission_ifc:
				print_debug_msg("GSMģ�鴮������");
				send_at_comand("AT+IFC=0,0\r\n");
				at_wait(300, at_mission_ilrr, at_mission_ilrr, at_mission_ilrr, 1);
				break;
			
			case at_mission_ilrr:
				print_debug_msg("GSMģ�鱾���ٶȱ���");
				send_at_comand("AT+ILRR=0\r\n");
				at_wait(300, at_mission_e1, at_mission_e1, at_mission_e1, 1);
				break;

			case at_mission_e1:
				print_debug_msg("GSMģ�鿪������");
				send_at_comand("ATE1\r\n");
				at_wait(300, at_mission_cfun, at_mission_cfun, at_mission_reset, 3);
				flag_print = 1;
				break;
			
			case at_mission_cfun:
				print_debug_msg("GSM���õ�Դģʽ:ȫ����");
				send_at_comand("AT+CFUN=1\r\n");	
				at_wait(300, at_mission_cpms, at_mission_cpms, at_mission_reset, 3);
				break;
			
			case at_mission_cpms:
				if(flag_print)
				{
					print_debug_msg("GSM�ȴ�SIM����ʼ��...");
					flag_print = 0;
				}
				send_at_comand("AT+CPMS=\"ME\",\"ME\",\"MT\"\r\n");
				at.state_csq = 0;
				wait_ret = at_wait(1000, at_mission_cmgf, at_no_mission, at_mission_reset, 100);
				if(wait_ret == at_answer_err)
				{
					OSTimeDlyHMSM(0, 0, 1, 0);
					if(at.err_times > 60)
					{
						print_debug_msg("SIM������");
						at.err_times = 0;
						at.state_csq = 0;
						OSQPostFront(mbox_at_mission, (void *)at_mission_reset);
						at_mission_now = 0;
					}
					else
					{
						if(at.sim_state == 2)
						{
							at_mission_now = at_mission_reset;
						}
						else
						{
							at_mission_now = at_mission_cpms;
						}
					}
				}
				break;
			
			case at_mission_cmgf:
				at.sim_state = 1;
				print_debug_msg("SIM������,���ö���ģʽ:PDU");
				send_at_comand("AT+CMGF=0\r\n");
				at_wait(300, at_mission_cmgd, at_mission_reset, at_mission_reset, 3);
				break;
						
			case at_mission_cmgd:
				print_debug_msg("GSMɾ�����ж���Ϣ");
				send_at_comand("AT+CMGD=1,4\r\n");
				at_wait(300, at_mission_cnmi, at_mission_cnmi, at_mission_reset, 3);
				break;
			
			case at_mission_cnmi:
				print_debug_msg("GSM���ö���Ϣ����");
				send_at_comand("AT+CNMI=2,2\r\n");
				at_wait(300, at_mission_qimux, at_mission_reset, at_mission_reset, 3);
				break;
			
			case at_mission_qimux:
				print_debug_msg("GSM�������ö�·����");
				send_at_comand("AT+QIMUX=1\r\n");
				at_wait(300, at_mission_qipompt, at_mission_reset, at_mission_reset, 3);
				break;
			
			case at_mission_qipompt:
				print_debug_msg("GSM���÷�����ʾ>");
				send_at_comand("AT+QIPROMPT=3\r\n");
				at_wait(1000, at_mission_qisde, at_mission_reset, at_mission_reset, 3);
				break;
			
			case at_mission_qisde:
				print_debug_msg("GSM���ùرշ��ͻ���");
				send_at_comand("AT+QISDE=0\r\n");
				at_wait(300, at_mission_creg1, at_mission_reset, at_mission_reset, 3);	
				break;
			
			case at_mission_creg1: 
				print_debug_msg("GSM����ע��״̬�����ϱ�");
				send_at_comand("AT+CREG=1\r\n");
				at_wait(300, at_mission_qnitz, at_mission_reset, at_mission_reset, 3);	
				break;
			
			case at_mission_qnitz:
				print_debug_msg("GSM����ͬ������ʱ��");
				send_at_comand("AT+QNITZ=1\r\n");
				at_wait(300, at_mission_cgdcon, at_mission_cgdcon, at_mission_reset, 3);
				break;
			
			case at_mission_cgdcon:
				print_debug_msg("GSM����PDP������");
				send_at_comand("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
				at_wait(300, at_mission_cgmm, at_mission_cgmm, at_mission_reset, 3);
				break;
				
			case at_mission_cgmm:
				send_at_comand("AT+CGMM\r\n");
				at_wait(300, at_mission_cgmr, at_mission_cgmr, at_mission_cgmr, 1);
				print_debug_msg("GSM��ȡģ���ͺ�:%s", at.cgmm);
				break;
			
			case at_mission_cgmr:
				send_at_comand("AT+CGMR\r\n");	
				at_wait(300, at_mission_cgsn, at_mission_cgsn, at_mission_cgsn, 1);
				print_debug_msg("GSM��ȡ����汾:%s", at.cgmr);
				break;
			
			case at_mission_cgsn:
				send_at_comand("AT+CGSN\r\n");
				at_wait(300, at_mission_cimi, at_mission_cimi, at_mission_cimi, 1);
				print_debug_msg("GSM��ȡģ��IMEI:%s", at.imei);
				break;
			
			case at_mission_cimi:
				send_at_comand("AT+CIMI\r\n");
				at_wait(300, at_mission_qccid, at_mission_qccid, at_mission_qccid, 1);
				print_debug_msg("GSM��ȡSM��IMSI:%s", at.cimi);
				break;
			
			case at_mission_qccid:
				send_at_comand("AT+QCCID\r\n");
				at_wait(300, at_mission_cregq, at_mission_cregq, at_mission_cregq, 1);
				print_debug_msg("GSM��ȡSM��CCID:%s", at.ccid);
				flag_print = 1;
				break;	

			case at_mission_cregq:
				if(flag_print)
				{
					print_debug_msg("GSM�ȴ���¼�绰��...");
					flag_print = 0;
				}
				send_at_comand("AT+CSQ\r\n");	 
				at_wait(500, at_no_mission, at_no_mission, at_no_mission, 3);
				send_at_comand("AT+CREG?\r");	
				wait_ret = at_wait(300, at_no_mission, at_no_mission, at_mission_reset, 3);
				if(at.state_reg == 0) 
				{
					if(++at.state_reg_wait > 60)
					{	
						at.state_reg_wait = 0;
						at_mission_now = at_mission_reset;
						print_debug_msg("�绰����¼ʧ�ܣ����µ�¼");
					}
					else
					{
						at_mission_now = at_mission_cregq;
					}
					OSTimeDlyHMSM(0, 0, 1, 0);
				}
				else
				{
					print_debug_msg("GSM�����¼�ɹ�");
					at.state_reg_wait = 0;
					at.redi_sended = 0;
					OSQPost(mbox_at_mission, (void*)(at_mission_cops));	
				}
				break;
			
			case at_mission_cops:
				send_at_comand("AT+COPS?\r\n");	
				at_wait(500, at_no_mission, at_no_mission, at_no_mission, 2);
				print_debug_msg("GSM��Ӫ����Ϣ:%s", at.cops);
				break;
			
			case at_mission_qiopen:
				print_debug_msg("����SOCKET[%d],%s,%s:%d",
								at_mission_para,
								(at.socket[at_mission_para].protocol == TCP)? "TCP" : "UDP",
								at.socket[at_mission_para].addr,
								at.socket[at_mission_para].port);
				send_at_comand("AT+QIOPEN=%d,\"%s\",\"%s\",%d\r\n", 
								at_mission_para,
								(at.socket[at_mission_para].protocol == TCP)? "TCP" : "UDP",
								at.socket[at_mission_para].addr,
								at.socket[at_mission_para].port);
				at_wait(500, at_no_mission, at_no_mission, at_no_mission, 1);
				at.socket[at_mission_para].state = CONNECT_CONNECTING;
				break;
				
			case at_mission_qiclose:
				print_debug_msg("�ر�SOCKET[%d]", at_mission_para);
				send_at_comand("AT+QICLOSE=%d\r\n", at_mission_para);
				wait_time = 0;
				while(at.socket[at_mission_para].state != CONNECT_IDLE
				   && wait_time < 50 && at.state_reg)
				{
					++wait_time;
					OSTimeDlyHMSM(0, 0, 0, 100);
				}
				wait_time = 0;
				at.socket[at_mission_para].state = CONNECT_IDLE;
				at_mission_now = at_no_mission;
				break;
				
			case at_mission_qisend:	//��������
				if(at.socket[at_mission_para].tlenth == 0)
				{
					at_mission_now = at_no_mission;
					break;
				}
				if(at.socket[at_mission_para].send_status == SEND_IDLE)
				{
					//print_debug_msg("��������");
					at.socket[at_mission_para].send_status = SEND_BEGIN;
					at.socket[at_mission_para].tcount = at.socket[at_mission_para].tlenth;
					send_at_comand("AT+QISEND=%d,%d\r\n", at_mission_para, 
									at.socket[at_mission_para].tcount);
					wait_ret = at_wait(500, at_no_mission, at_no_mission, at_mission_reset, 5);
					at.socket[at_mission_para].send_status = SEND_TO_MODULE;
					at_mission_now = at_mission_qisend;
				}
				else if(at.socket[at_mission_para].send_status == SEND_TO_MODULE)
				{
					//print_debug_msg("���ݴ���д��");
					Usart_Send_Datas(Usart_GSM, at.socket[at_mission_para].tdata,
												at.socket[at_mission_para].tcount);
					at.socket[at_mission_para].send_status = SEND_WAIT_TCPACK;
					wait_time = 0;
				}
				else if(at.socket[at_mission_para].send_status == SEND_WAIT_TCPACK)
				{
					//print_debug_msg("�ȴ����ͻ�Ӧ");
					if(wait_time < 100 && at.state_reg
					&& at.socket[at_mission_para].state == CONNECT_CONNECT_OK)
					{
						++wait_time;
						OSTimeDlyHMSM(0, 0 ,0, 100);
					}
					else
					{
						wait_time = 0;
						at.socket[at_mission_para].send_status = SEND_IDLE;
						at_mission_now = at_no_mission;
					}
				}
				break;
				
			case at_mission_cmgs:	//���Ͷ���
				if(at.msg_tlenth == 0)
				{
					at_mission_now = at_no_mission;
					break;
				}
				if(at.msg_tstatus == SEND_IDLE)
				{
					pdu_lenth = make_pdu_sms(PDU_msg_buf, (char*)at.msg_tnum, (BYTE*)at.msg_tdata);
					print_debug_msg(">>>���Ͷ�������:%s", at.msg_tnum);
					send_at_comand("AT+CMGS=%d\r\n", pdu_lenth / 2 - 1);
					at.msg_tstatus = SEND_BEGIN;
					wait_time = 0;
				}
				else if(at.msg_tstatus == SEND_BEGIN)
				{
					if(at.state_reg == 0)
					{
						wait_time = 0;
						at.msg_tstatus = SEND_IDLE;
						ServerPara.ctrl_step = ctrl_step_wait_reg;
						at_mission_now = at_no_mission;
					}
					else if(wait_time < 50)
					{
						++wait_time;
						OSTimeDlyHMSM(0, 0 ,0, 100);
					}
					else
					{
						wait_time = 0;
						at.msg_tstatus = SEND_IDLE;
						at_mission_now = at_no_mission;
					}
				}
				else if(at.msg_tstatus == SEND_TO_MODULE)
				{
					Usart_Send_Datas(Usart_GSM, (BYTE*)PDU_msg_buf, pdu_lenth);
					Usart_Send_char(Usart_GSM, 0x1A);
					at.msg_tstatus = SEND_WAIT_TCPACK;
					wait_time = 0;
				}
				else if(at.msg_tstatus == SEND_WAIT_TCPACK)
				{
					if(wait_time < 600 && at.state_reg && at.msg_tlenth > 0)
					{
						++wait_time;
						OSTimeDlyHMSM(0, 0 ,0, 100);
					}
					else
					{
						wait_time = 0;
						at.msg_tstatus = SEND_IDLE;
						at_mission_now = at_no_mission;
					}
				}	
				break;
				
			case at_mission_cmgr:	//��ȡ����
				send_at_comand("AT+CMGR=%d\r\n", at_mission_para);
				at_wait(500, at_no_mission, at_no_mission, at_mission_reset, 1);
				break;
			
			case at_mission_reset:
				print_debug_msg("����GSMģ��");
				init_at();
				ServerPara.ctrl_step = ctrl_step_wait_reg;
				OSQFlush(mbox_at_mission);
				at_mission_now = at_mission_poweron;
				break;
			
			case at_mission_csq:		//��ȡ�ź�
				send_at_comand("AT+CSQ\r\n");	 
				at_wait(500, at_no_mission, at_no_mission, at_mission_reset, 3);
				break;
			
			case at_mission_cbc:		//��ȡ��ص���
				send_at_comand("AT+CBC\r\n");
				at_wait(500, at_no_mission, at_no_mission, at_mission_reset, 3);
				break;
			
			case at_mission_cclk:		//��ȡʱ��
				send_at_comand("AT+CCLK?\r\n");	
				at_wait(500, at_no_mission, at_no_mission, at_mission_reset, 3);
				break;
			
			case at_mission_qcellloc:	//��ȡ��վ��λ��Ϣ
				send_at_comand("AT+QCELLLOC=1\r\n");
				at_wait(500, at_no_mission, at_no_mission, at_no_mission, 1);
				break;
			
			default:
				at_mission_now = at_no_mission;
				break;
		}
	}
}




/*******************************************
��������: atָ��ȴ�����
��ڲ���: wait_time,  	at�ȴ�ʱ��
          if_ok,		at���سɹ�����ת��������
          if_err,		at���ش�����ת��������
          if_time_out,	at���ʱ����ת��������
        �maxerrtimes,	�Ǵ����������̶�
���ڲ���: 
��    ע: ����������յ���GSMģ������
*******************************************/
static DWORD at_wait(WORD wait_time, DWORD if_ok, DWORD if_err,DWORD if_time_out, BYTE max_err_times)
{	
	INT8U err;
	DWORD ret = 0;
	
	OSMboxPend(mbox_at_sended, wait_time, &err);
	switch((DWORD)OSMboxPend(mbox_at_answer, wait_time, &err))
	{
		default:
		case at_answer_err:
			++at.err_times;
			ret = at_answer_err;
			//print_debug_msg("ATָ�����%d\r\n",at.err_times);
			if(at.err_times >=	max_err_times)
			{	
				//print_debug_msg("AT���󳬹�%u��",max_err_times);
				at.err_times = 0;
				at_mission_now = 0;
				if(if_err)
				{
					OSQPostFront(mbox_at_mission, (void *)if_err);
				}
			}
			break;

		case 0:
			if(err == OS_ERR_TIMEOUT)
			{
				at.err_times++;		
				ret = at_answer_time_out;
				send_at_comand("%c", 0x1b);
				//print_debug_msg("ATָ�ʱ%d", at.err_times);
				if(at.err_times >=	max_err_times)
				{	
					if(at_mission_now != at_mission_qcellloc)
					{
						print_debug_msg("AT��ʱ����%d��,at_now=%d", max_err_times, at_mission_now);
					}
					GSMRcv_rest();
					at.err_times = 0;
					at_mission_now = 0;
					if(if_time_out)
					{
						OSQPostFront(mbox_at_mission, (void*)if_time_out);
					}
				}
			}
			else
			{
				print_debug_msg("ϵͳ�������:%d", err);
				ret = at_answer_unknow;
				at_mission_now = 0;		
				if(if_ok)
				{
					OSQPostFront(mbox_at_mission, (void*)if_ok);
				}
			}
			OSTimeDlyHMSM(0, 0, 1, 0);
			break;
					
		case at_answer_ok:
			if(if_ok)
			{				
				OSQPostFront(mbox_at_mission, (void *)if_ok);
			}
			at.err_times = 0;
			at_mission_now = 0;
			ret = at_answer_ok;
			break;
	}
	return(ret);
}




/* �������� */
const static WORD rdata_lenth = 120;
struct
{
	BYTE rdata[rdata_lenth + 1];	//���յ���GSM����
	WORD rdata_i;					//�������ݳ���
	DWORD rstop_time; 				//����2�������ݵ�ֹͣʱ��
	at_program process_program;		//�ӿ�(�ص�)����
}static at_para;
/*******************************************
��������: GSM_Resolve_task
��������: GSMģ�����ݽ��մ���
��ڲ���: 
���ڲ���: 
��    ע: 
*******************************************/
void GSM_Resolve_task(void *p_arg)
{
	const WORD interval = 10;
	BYTE flag_search = 0;
	BYTE data;
	
	p_arg = p_arg;
	at_para.rdata_i = 0;
	at_para.rstop_time = 0;
	while(1)
	{
		OSTimeDlyHMSM(0, 0 ,0, interval);
		
		/* ������ */
		if(GSMRcv_lenth() == 0)
		{
			at_para.rstop_time += interval;
			if(at_para.rdata_i > 0 && at_para.rstop_time > 100)
			{
				at_para.rstop_time += interval;
				if(at_para.rdata_i > 0 && at_para.rstop_time > 100)
				{
					flag_search = 1;
				}
			}
		}
		
		/* ������ */
		while(GSMRcv_lenth() > 0)
		{
			data = GSMRcv_pop();
			if(Flag_Printf_at_rcv)
			{
				printf("%c", data);
			}
			at.dog = 0;
			at_para.rstop_time = 0;
			at_para.rdata[at_para.rdata_i] = data;
			if(at_para.rdata[at_para.rdata_i] == '\n' || at_para.rdata_i > rdata_lenth - 10)
			{
				++at_para.rdata_i;
				flag_search = 1;
				break;
			}
			else
			{
				++at_para.rdata_i;
				if(at_para.rdata_i >= rdata_lenth)
				{
					at_para.rdata_i = 0;
				}
			}
		}
		
		/* �����ؼ��� */
		if(flag_search)
		{
			at_para.rdata[at_para.rdata_i] = '\0';
			at_para.process_program = search_key_ward(at_para.rdata);
			if(at_para.process_program != NULL)
			{
				at_para.process_program(at_para.rdata);
			}
			at_para.rdata_i = 0;
			at_para.rstop_time = 0;
		}
	}
}




/*******************************************
��������:USART3_IRQHandler(void)
��������:����3�жϷ������̣�GSM��Ϣ����
��ڲ���:
���ڲ���: 
��    ע:
*******************************************/
void USART3_IRQHandler(void)
{
	BYTE data;
	OS_CPU_SR  cpu_sr; 

	OS_ENTER_CRITICAL();
	OSIntEnter();
	OS_EXIT_CRITICAL();
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		data = USART_ReceiveData(USART3);
		if(GSMRcv_lenth() == 0)
		{
			GSMRcv_rest();
		}
		GSMRcv_push(data);
	}
	OSIntExit(); 
}




/* GSMģ�������� */
static void GSMRcv_push(BYTE data)
{
	GSMRcv_buffer[GSMRcv_tail] = data;
	if(++GSMRcv_tail >= GSMRcv_cap)
	{
		GSMRcv_tail = 0;
	}
	if(GSMRcv_tail == GSMRcv_head)
	{
		if(++GSMRcv_head >= GSMRcv_cap)
		{
			GSMRcv_head = 0;
		}
	}
}

/* GSMģ����ճ��� */
BYTE GSMRcv_pop(void)
{
	BYTE data;
	
	data = GSMRcv_buffer[GSMRcv_head];
	if(++GSMRcv_head >= GSMRcv_cap)
	{
		GSMRcv_head = 0;
	}
	return data;
}

/* ���и�λ */
static void GSMRcv_rest(void)
{
	GSMRcv_head = 0;
	GSMRcv_tail = 0;
}

/* ���������ݳ��� */
WORD GSMRcv_lenth(void)
{
	if(GSMRcv_tail >= GSMRcv_head)
	{
		return GSMRcv_tail - GSMRcv_head;
	}
	else
	{
		return GSMRcv_cap - (GSMRcv_head - GSMRcv_tail);
	}
}

