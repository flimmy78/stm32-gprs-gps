/* GSM模块M26驱动 */

#ifndef __GSM_M26_H_
#define __GSM_M26_H_
#include "type.h"
#include "circularQueue.h"



/* AT命令响应 */
#define at_answer_ok  			1
#define at_answer_err 			2
#define at_answer_time_out 		3
#define at_answer_unknow		255

/* 语音通话状态 */
#define voice_state_no 			0
#define	voice_state_spking 		1
#define voice_state_held 		2
#define voice_state_dial 		3
#define voice_state_alert 		4
#define voice_state_incoming 	5
#define voice_state_waiting		6
#define voice_state_calling		7
#define voice_state_unkonw 		255



/* AT任务定义 */
#define at_mission_msg_lenth	100	//任务列表长度
#define at_no_mission			0	//无任务
#define at_mission_poweron 		1 	//开机
#define at_mission_poweroff 	2	//关机
#define at_mission_reset		3	//重启
#define at_mission_hpoweroff	4 	//硬关机
/* 初始化 */
#define at_mission_e1			10	//开启回显
#define at_mission_ipr			11	//设置波特率
#define at_mission_c_dcd		12	//设置串口DCD模式
#define at_mission_d_dtr		13	//设置串口DTR模式
#define at_mission_icf			14	//设置串口帧格式
#define at_mission_ifc			15	//设置串口TE-TA流控
#define at_mission_ilrr			16	//设置串口本地速度报告
#define at_mission_f			17 	//初始化所有参数
#define at_mission_cfun			18	//设置电源模式
#define at_mission_creg1		19	//设置网络状态
#define at_mission_cregq		20	//等待登录网络
#define at_mission_cgdcon		21	//设置PDP上下文
#define at_mission_qnitz		22	//设置同步网络时间
/* 查询 */
#define at_mission_cgmm			30 	//查询模块型号
#define at_mission_cgmr			31 	//查询模块软件版本号
#define at_mission_cgsn			32 	//查询模块设备号
#define at_mission_cimi			33	//查询SIM卡IMSI
#define at_mission_qccid		34 	//查询SIM卡CCID
#define at_mission_cops			35	//查询运营商
#define at_mission_csq 			36	//查询信号质量
#define at_mission_cbc 			37	//查询电池电量
#define at_mission_cclk			38	//查询时间
#define at_mission_qcellloc		39	//获取基站定位信息
/* 短信 */
#define at_mission_cmgf			50	//设置短信模式
#define at_mission_cpms			51	//选择短信存储器
#define at_mission_cmgd			52	//删除短信
#define at_mission_cmgl 		53	//列出短信
#define at_mission_cmgr			54	//读取短信
#define at_mission_cmgs 		55	//发送短信
#define at_mission_cnmi			56	//设置短消息报告
/* 通话 */
#define at_mission_snfs			60
#define at_mission_asic			61
#define at_mission_clvl			62
#define at_mission_snfa			63
#define at_mission_snfi			64 	//设置mic增益
#define at_mission_snfo			65 	//设置输出增益
#define at_mission_set_snfi		66 	//调整mic参数
/* TCPIP */
#define at_mission_qimux		70	//设置多路连接
#define at_mission_qipompt		71	//配置是否显示">"
#define at_mission_qisde		72	//配置发送数据是否回显
#define at_mission_qiopen		73	//建立socket连接
#define at_mission_qiclose		74	//关闭socket连接
#define at_mission_qisend		75	//发送TCPIP数据
#define at_mission_qifgcnt		76	//配置前置场景
#define at_mission_qicsgp		77	//设置CSD和GPRS连接模式
#define at_mission_qiregapp		78	//启动任务
#define at_mission_qiact		79	//激活移动场景
#define at_mission_qntp			80	//网络同步本地时间



/* 函数指针类型定义 */
typedef BYTE (*call_program_type)(BYTE *addr);
typedef BYTE (*rmsg_program_type)(BYTE* number, BYTE* msg);
typedef BYTE (*socket_rdata_program_type)(WORD lenth);
typedef BYTE (*socket_state_program_type)(BYTE on_off);
typedef BYTE (*socket_send_program_type)(BYTE ok_err);


/* 函数声明 */
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
#define SOCKET_AMOUNT		6	//socket数量

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

/* socket链接状态 */
typedef struct socket_struct
{
	BYTE on;	 			//开关
	SOCKET_STATUS state;	//状态 0表示失败 1表示正在连接 2表示已经联通 
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
	socket_rdata_program_type	socket_rdata_program;	 //收到数据的处理
	socket_state_program_type	socket_state_program;	 //上线或者下线
	socket_send_program_type	socket_send_program;	 //发送状态，成功还是失败
}SOCKET_STRUCT;

/* AT模块状态 */
struct at_info_struct
{
	BYTE err_times;
	BYTE power;
	BYTE closet_gsm;
	BYTE sim_state;
	BYTE cops[cops_lenth + 1];			//运营商信息
	BYTE ccid[ccid_lenth + 1];			//SIM卡号
	BYTE imei[imei_lenth + 1];			//设备序号
	BYTE cimi[cimi_lenth + 1];			//SIM卡的SIMI
	BYTE cgmm[cgmm_lenth + 1];			//硬件型号
	BYTE cgmr[cgmr_lenth + 1];			//软件版本
	BYTE apn[apn_lenth + 1];			//APN信息
	SOCKET_STRUCT socket[SOCKET_AMOUNT];//Socket信息
	BYTE msg_mode;						//短信模式
	BYTE msg_tnum[gsm_num_lenth + 1];	//短信发送电话号码
	BYTE msg_tdata[msg_data_lenth + 1];	//短信发送数据
	WORD msg_tlenth;					//短信发送长度
	BYTE msg_tRetry;					//短信重发次数
	BYTE msg_rtime[msg_rtime_lenth + 1];//短信接收时间
	BYTE msg_rnum[gsm_num_lenth + 1];	//短信接收电话号码
	BYTE msg_rdata[msg_data_lenth + 1];	//短信接收到的数据
	WORD msg_rlenth;					//短信接收长度
	SEND_STATUS msg_tstatus;			//短信发送状态
	rmsg_program_type msg_rdata_dsp;	//短信接收解析
	BYTE call_num[gsm_num_lenth + 1];	//呼叫号码
	BYTE dtfm[gsm_num_lenth + 1];		//双音多频
	BYTE voice_dir;						
	WORD voice_time_add;
	BYTE voice_state;
	BYTE voice_state_check;
	BYTE voice_mic_state;				//mic状态0为静音   1为正常
	BYTE voice_sp;       				//呼叫特殊号码
	BYTE mic_bbc;						//mic输入增益
	WORD mic_cali;		  				//mic输入衰减
	call_program_type incom_dsp;
	BYTE state_reg;
	BYTE state_reg_wait;
	BYTE state_csq;
	BYTE state_csq_wait;
	BYTE state_power;	  				//开机还是关机
	BYTE state_net;
	BYTE state_net_wait;
	WORD state_batte;
	WORD dog;
	BYTE redi_sended;
	WORD buffer_empty;
	BYTE flag_loc;						//定位标识
	DWORD loc_lo;						//经度
	DWORD loc_la;						//维度
	BYTE flag_time;						//网络同步时间标识
	ST_TIMER web_time;					//网络同步时间
};
extern struct at_info_struct at;

extern BOOL Flag_Printf_at_rcv;
extern WORD GSMRcv_lenth(void);
extern BYTE GSMRcv_pop(void);
extern BYTE build_socket(BYTE socket_num, char* addr, WORD port, PROTOCOL protocol);
extern BYTE close_socket(BYTE socket_num);


#endif
