/* GSMģ��M26���� */

#ifndef __GSM_M26_H_
#define __GSM_M26_H_
#include "type.h"
#include "circularQueue.h"



/* AT������Ӧ */
#define at_answer_ok  			1
#define at_answer_err 			2
#define at_answer_time_out 		3
#define at_answer_unknow		255

/* ����ͨ��״̬ */
#define voice_state_no 			0
#define	voice_state_spking 		1
#define voice_state_held 		2
#define voice_state_dial 		3
#define voice_state_alert 		4
#define voice_state_incoming 	5
#define voice_state_waiting		6
#define voice_state_calling		7
#define voice_state_unkonw 		255



/* AT������ */
#define at_mission_msg_lenth	100	//�����б���
#define at_no_mission			0	//������
#define at_mission_poweron 		1 	//����
#define at_mission_poweroff 	2	//�ػ�
#define at_mission_reset		3	//����
#define at_mission_hpoweroff	4 	//Ӳ�ػ�
/* ��ʼ�� */
#define at_mission_e1			10	//��������
#define at_mission_ipr			11	//���ò�����
#define at_mission_c_dcd		12	//���ô���DCDģʽ
#define at_mission_d_dtr		13	//���ô���DTRģʽ
#define at_mission_icf			14	//���ô���֡��ʽ
#define at_mission_ifc			15	//���ô���TE-TA����
#define at_mission_ilrr			16	//���ô��ڱ����ٶȱ���
#define at_mission_f			17 	//��ʼ�����в���
#define at_mission_cfun			18	//���õ�Դģʽ
#define at_mission_creg1		19	//��������״̬
#define at_mission_cregq		20	//�ȴ���¼����
#define at_mission_cgdcon		21	//����PDP������
#define at_mission_qnitz		22	//����ͬ������ʱ��
/* ��ѯ */
#define at_mission_cgmm			30 	//��ѯģ���ͺ�
#define at_mission_cgmr			31 	//��ѯģ������汾��
#define at_mission_cgsn			32 	//��ѯģ���豸��
#define at_mission_cimi			33	//��ѯSIM��IMSI
#define at_mission_qccid		34 	//��ѯSIM��CCID
#define at_mission_cops			35	//��ѯ��Ӫ��
#define at_mission_csq 			36	//��ѯ�ź�����
#define at_mission_cbc 			37	//��ѯ��ص���
#define at_mission_cclk			38	//��ѯʱ��
#define at_mission_qcellloc		39	//��ȡ��վ��λ��Ϣ
/* ���� */
#define at_mission_cmgf			50	//���ö���ģʽ
#define at_mission_cpms			51	//ѡ����Ŵ洢��
#define at_mission_cmgd			52	//ɾ������
#define at_mission_cmgl 		53	//�г�����
#define at_mission_cmgr			54	//��ȡ����
#define at_mission_cmgs 		55	//���Ͷ���
#define at_mission_cnmi			56	//���ö���Ϣ����
/* ͨ�� */
#define at_mission_snfs			60
#define at_mission_asic			61
#define at_mission_clvl			62
#define at_mission_snfa			63
#define at_mission_snfi			64 	//����mic����
#define at_mission_snfo			65 	//�����������
#define at_mission_set_snfi		66 	//����mic����
/* TCPIP */
#define at_mission_qimux		70	//���ö�·����
#define at_mission_qipompt		71	//�����Ƿ���ʾ">"
#define at_mission_qisde		72	//���÷��������Ƿ����
#define at_mission_qiopen		73	//����socket����
#define at_mission_qiclose		74	//�ر�socket����
#define at_mission_qisend		75	//����TCPIP����
#define at_mission_qifgcnt		76	//����ǰ�ó���
#define at_mission_qicsgp		77	//����CSD��GPRS����ģʽ
#define at_mission_qiregapp		78	//��������
#define at_mission_qiact		79	//�����ƶ�����
#define at_mission_qntp			80	//����ͬ������ʱ��



/* ����ָ�����Ͷ��� */
typedef BYTE (*call_program_type)(BYTE *addr);
typedef BYTE (*rmsg_program_type)(BYTE* number, BYTE* msg);
typedef BYTE (*socket_rdata_program_type)(WORD lenth);
typedef BYTE (*socket_state_program_type)(BYTE on_off);
typedef BYTE (*socket_send_program_type)(BYTE ok_err);


/* �������� */
extern DWORD AT_Comand_wait;
extern DWORD at_mission_now;
extern void init_at(void);
typedef BYTE (*at_program)(BYTE *data_addr);
extern BYTE at_sended_dsp(BYTE *data_addr);
extern BYTE at_ok_dsp(BYTE *data_addr);
extern BYTE at_err_dsp(BYTE *data_addr);
extern BYTE at_cms_err_dsp(BYTE *data_addr);
extern BYTE at_cme_err_dsp(BYTE *data_addr);
extern BYTE at_creg_dsp(BYTE *data_addr);
extern BYTE at_csq_dsp(BYTE *data_addr);
extern BYTE at_cbc_dsp(BYTE *data_addr);
extern BYTE at_cmgl_dsp(BYTE *data_addr);
extern BYTE at_cmgr_dsp(BYTE *data_addr);
extern BYTE at_cmgs_dsp(BYTE *data_addr);
extern BYTE at_cmti_dsp(BYTE *data_addr);
extern BYTE at_cmt_dsp(BYTE *data_addr);
extern BYTE at_cimi_dsp(BYTE *data_addr);
extern BYTE at_cgmm_dsp(BYTE *data_addr);
extern BYTE at_cgmr_dsp(BYTE *data_addr);
extern BYTE at_cgsn_dsp(BYTE *data_addr);
extern BYTE at_qnitz_dsp(BYTE *data_addr);
extern BYTE at_qccid_dsp(BYTE *data_addr);
extern BYTE at_ring_dsp(BYTE *data_addr);
extern BYTE at_noc_dsp(BYTE *data_addr);
extern BYTE at_down_dsp(BYTE *data_addr);
extern BYTE at_ndt_dsp(BYTE *data_addr);
extern BYTE at_clcc_dsp(BYTE *data_addr);
extern BYTE at_cops_dsp(BYTE *data_addr);
extern BYTE at_qcellloc_dsp(BYTE *data_addr);
extern BYTE at_tcpipMux_dsp(BYTE *data_addr);
extern BYTE at_tcpipRcv_dsp(BYTE *data_addr);
extern BYTE at_sendBegin_dsp(BYTE *data_addr);
extern at_program search_key_ward(BYTE *aline);






#define apn_lenth 			24
#define gsm_num_lenth 		24
#define msg_data_lenth 		200
#define tel_num_lenth 		gsm_num_lenth
#define tel_num_len1		24
#define dial_user_lenth 	24
#define voice_opera_ath  	1
#define voice_opera_ata  	2
#define ccid_lenth 			21
#define imei_lenth 			19
#define cimi_lenth 			21
#define cgmm_lenth 			15
#define cgmr_lenth 			15
#define msg_rtime_lenth 	6
#define cops_lenth 			15
#define SOCKET_AMOUNT		6	//socket����

typedef	enum
{
	CONNECT_IDLE,
	CONNECT_CONNECTING,
	CONNECT_CONNECT_OK,
	CONNECT_CONNECT_FAIL,
}SOCKET_STATUS;

typedef enum 
{
	SEND_IDLE,
	SEND_BEGIN,
	SEND_TO_MODULE,
	SEND_WAIT_TCPACK,
}SEND_STATUS;

typedef enum
{
	TCP,
	UDP
}PROTOCOL;

/* socket����״̬ */
typedef struct socket_struct
{
	BYTE on;	 			//����
	SOCKET_STATUS state;	//״̬ 0��ʾʧ�� 1��ʾ�������� 2��ʾ�Ѿ���ͨ 
	DWORD rx_count;			//
	DWORD tx_count;
	DWORD last_tx_count;
	DWORD ack_data;
	DWORD uack_data;
	WORD  send_wait_time;
	BYTE  send_err_times;
	BYTE  connect_err_times;
	BYTE  connecting_time; 
	BYTE  addr[32];
	WORD  port;
	PROTOCOL	protocol;
	SEND_STATUS send_status;
	WORD  tdata_limit;
	WORD  tlenth;
	WORD  tcount;
	BYTE* tdata;
	WORD  rdata_limit;
	WORD  rlenth;
	BYTE* rdata;
	socket_rdata_program_type	socket_rdata_program;	 //�յ����ݵĴ���
	socket_state_program_type	socket_state_program;	 //���߻�������
	socket_send_program_type	socket_send_program;	 //����״̬���ɹ�����ʧ��
}SOCKET_STRUCT;

/* ATģ��״̬ */
struct at_info_struct
{
	BYTE err_times;
	BYTE power;
	BYTE closet_gsm;
	BYTE sim_state;
	BYTE cops[cops_lenth + 1];			//��Ӫ����Ϣ
	BYTE ccid[ccid_lenth + 1];			//SIM����
	BYTE imei[imei_lenth + 1];			//�豸���
	BYTE cimi[cimi_lenth + 1];			//SIM����SIMI
	BYTE cgmm[cgmm_lenth + 1];			//Ӳ���ͺ�
	BYTE cgmr[cgmr_lenth + 1];			//����汾
	BYTE apn[apn_lenth + 1];			//APN��Ϣ
	SOCKET_STRUCT socket[SOCKET_AMOUNT];//Socket��Ϣ
	BYTE msg_mode;						//����ģʽ
	BYTE msg_tnum[gsm_num_lenth + 1];	//���ŷ��͵绰����
	BYTE msg_tdata[msg_data_lenth + 1];	//���ŷ�������
	WORD msg_tlenth;					//���ŷ��ͳ���
	BYTE msg_tRetry;					//�����ط�����
	BYTE msg_rtime[msg_rtime_lenth + 1];//���Ž���ʱ��
	BYTE msg_rnum[gsm_num_lenth + 1];	//���Ž��յ绰����
	BYTE msg_rdata[msg_data_lenth + 1];	//���Ž��յ�������
	WORD msg_rlenth;					//���Ž��ճ���
	SEND_STATUS msg_tstatus;			//���ŷ���״̬
	rmsg_program_type msg_rdata_dsp;	//���Ž��ս���
	BYTE call_num[gsm_num_lenth + 1];	//���к���
	BYTE dtfm[gsm_num_lenth + 1];		//˫����Ƶ
	BYTE voice_dir;						
	WORD voice_time_add;
	BYTE voice_state;
	BYTE voice_state_check;
	BYTE voice_mic_state;				//mic״̬0Ϊ����   1Ϊ����
	BYTE voice_sp;       				//�����������
	BYTE mic_bbc;						//mic��������
	WORD mic_cali;		  				//mic����˥��
	call_program_type incom_dsp;
	BYTE state_reg;
	BYTE state_reg_wait;
	BYTE state_csq;
	BYTE state_csq_wait;
	BYTE state_power;	  				//�������ǹػ�
	BYTE state_net;
	BYTE state_net_wait;
	WORD state_batte;
	WORD dog;
	BYTE redi_sended;
	WORD buffer_empty;
	BYTE flag_loc;						//��λ��ʶ
	DWORD loc_lo;						//����
	DWORD loc_la;						//ά��
	BYTE flag_time;						//����ͬ��ʱ���ʶ
	ST_TIMER web_time;					//����ͬ��ʱ��
};
extern struct at_info_struct at;

extern BOOL Flag_Printf_at_rcv;
extern WORD GSMRcv_lenth(void);
extern BYTE GSMRcv_pop(void);
extern BYTE build_socket(BYTE socket_num, char* addr, WORD port, PROTOCOL protocol);
extern BYTE close_socket(BYTE socket_num);


#endif
