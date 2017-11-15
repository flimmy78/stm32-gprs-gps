#ifndef __JT_T808_H_
#define __JT_T808_H_
#include "type.h"





/*-------------------------T808Э����ϢID--------------------------*/
#define terminal_normal_answer 			0x0001 //ͨ��Ӧ��
#define terminal_heartbeat     			0x0002 //����
#define terminal_regist					0x0100 //�ն�ע��
#define terminal_update_ask_ver 		0x0107 //��ѯ�汾��Ϣ
#define terminal_update_get_pkg 		0x0108 //��ȡ������
#define terminal_deregist				0x0003 //�ն�ע��
#define terminal_authentication 		0x0102 //�ն˼�Ȩ
#define terminal_answer_sys_cfg			0x0104 //������ѯӦ��
#define terminal_up_gps_data			0x0200 //λ����Ϣ�ϱ�
#define terminal_anwser_gps_data		0x0201 //λ����ϢӦ��
#define terminal_event_report			0x0301 //�¼�����
#define terminal_return_answer			0x0302 //�ش�����
#define terminal_return_oder			0x0303 //ѡ���ȡ���㲥
#define terminal_vehicle_ctrl			0x0500 //��������
#define terminal_up_recode				0x0700 //��ʻ��¼�������ϴ�
#define terminal_up_freight_note		0x0701 //�����˵��ϱ�
#define terminal_up_driver_info			0x0702 //��ʻԱ��Ϣ
#define terminal_up_multi_gps_data 		0x0704 //�����ϴ�
#define terminal_media_event			0x0800 //��ý���¼��ϴ�
#define terminal_up_media				0x0801 //��ý���ϴ�
#define terminal_media_check_return 	0x0802 //ý���������
#define terminal_transmission			0x0900 //����͸��
#define terminal_rsa					0x0A00 //rsa
//=======
#define server_normal_answer 			0x8001 //ͨ��Ӧ��
#define server_regist_answer			0x8100 //�ն�ע��Ӧ��
#define server_update_ver_answer 		0x8107 //��ѯ�汾��Ϣ
#define server_update_pkg_answer 		0x8108 //��ȡ������
#define server_set_terminal_cfg			0x8103 //�����ն˲���
#define server_read_terminal_cfg 		0x8104 //��ȡ�ն˲���
#define server_ctrl_terminal			0x8105 //�ն˿���
#define server_read_some_cfg			0x8106 //��ȡ�ն�ָ������
#define server_read_gps_data			0x8201 //��ѯ�ն�
#define server_temporary_superv 		0x8202 //��������ʱ���
#define server_confirm_alarm			0x8203 //�˹�ȷ�ϱ�����Ϣ
#define server_send_text				0x8300 //�ı���Ϣ�·�
#define server_event_set				0x8301 //�¼��·�
#define server_send_ask					0x8302 //����
#define server_send_oder				0x8303 //�㲥
#define server_send_msg					0x8304 //��Ϣ����
#define server_call_back				0x8400 //�ز�
#define server_set_phone_book			0x8401 //���õ绰��
#define server_vehicle_ctrl				0x8500 //��������
#define server_set_area_round			0x8600 //����ԭ������
#define server_del_area_round			0x8601 //ɾ��Բ������
#define server_set_area_rect			0x8602 //����
#define server_del_area_rect			0x8603
#define server_set_area_polygon			0x8604 //�����
#define server_del_area_polygon			0x8605
#define server_set_area_route			0x8606 //·��
#define server_del_area_route			0x8607
#define server_read_recode				0x8700 //��ȡ��ʻ��¼
#define server_set_recode				0x8701 //��¼�ǲ�������
#define server_request_idcard			0x8702 //�ϱ���ʻԱ�����Ϣ����
#define server_media_return				0x8800 //��ý���ϴ�Ӧ��
#define server_take_picture				0x8801 //����
#define server_media_check				0x8802 //ý�����
#define server_read_media				0x8803 //ý�������ϴ�
#define server_recode_sound				0x8804 //��ʼ¼��
#define server_transmission 			0x8900 //����͸��
#define server_rsa						0x8A00 //




/*-------------------------T808Э�����ID--------------------------*/
#define	set_heartbeat_time				0x0001	//����ʱ���� ��
#define	set_tcp_wait_time				0x0002	//tcp��ʱʱ�� ��
#define	set_tcp_retry_times				0x0003	//tcp�ش�����
#define	set_udp_wait_time				0x0004 	//udp��ʱʱ�� ��
#define	set_udp_retry_times				0x0005	//udp�ش�����
#define	set_sms_wait_time				0x0006 	//����ϢӦ��ʱ ��
#define	set_sms_retry_times				0x0007	//����Ϣ�ش����� 
#define	set_main_server_dial_apn		0x0010 	//��������apn
#define	set_main_server_dial_user		0x0011	//�û���
#define	set_main_server_dial_pwd		0x0012 	//����
#define	set_main_server_addr			0x0013	//����������ַ
#define	set_back_server_dial_apn		0x0014 	//���ݷ�����apn
#define	set_back_server_dial_user		0x0015	//�û���
#define	set_back_server_dial_pwd		0x0016 	//����
#define	set_back_server_addr			0x0017	//���ݷ�������ַ
#define	set_main_server_tcp_port		0x0018	//������TCP�˿�
#define	set_main_server_udp_port		0x0019	//������UDP�˿�
#define set_ic_server_addr				0x001a	//IC����֤����������ַ
#define set_ic_server_tcp_port			0x001b	//IC����֤������TCP�˿�
#define set_ic_server_udp_port			0x001c	//IC����֤������UDP�˿�
#define set_ic_server_addr_back   		0x001d	//IC����֤���������õ�ַ
#define	set_up_gpsdata_tactics			0x0020	//�����ϱ����� 0��ʱ 1���� 2���
#define	set_up_gpsdata_plan				0x0021	//0:acc 1 ��ʻԱ��¼״̬
#define	set_up_gpsdata_time_nodriv		0x0022	//�޼�ʻԱ��Ϣʱ���ϱ�ʱ���� ��
#define	set_up_gpsdata_time_sleep		0x0027 	//����ʱ���ϱ�ʱ����
#define	set_up_gpsdata_time_alarm		0x0028	//����ʱ���ϱ����
#define	set_up_gpsdata_time_default		0x0029	//�����ϱ�ʱ����
#define	set_up_gpsdata_lenth_default	0x002c	//�����ϱ�������
#define	set_up_gpsdata_lenth_nodriv		0x002d	//�޼�ʻԱ��Ϣʱ���ϱ������� ��
#define	set_up_gpsdata_lenth_sleep		0x002e 	//����ʱ���ϱ�ʱ����
#define	set_up_gpsdata_lenth_alarm		0x002f	//����ʱ���ϱ����
#define	set_up_gpsdata_angle			0x0030	//�յ㲹��
#define	set_up_illegal_move				0x0031	//�Ƿ�λ�Ʒ�ֵ(��)
#define	set_service_tel_num				0x0040	//����ƽ̨���� 
#define	set_reset_tel_num				0x0041 	//��λ����
#define	set_set_default_tel_num			0x0042 	//�ָ���������
#define	set_service_sms_num				0x0043 	//����ƽ̨�Ŷ�Ϣ���� 
#define	set_service_alarm_num			0x0044 	//����ƽ̨�����Ŷ�Ϣ����
#define	set_tel_answer_tactics			0x0045	//0�Զ����� 1 acconʱ�Զ�����
#define	set_max_tel_time_once			0x0046	//һ��ͨ�����ʱ�� 0Ϊ������ 0xffffffffΪ������
#define	set_max_tel_time_month			0x0047	//һ���ͨ��ʱ��	  ͬ��
#define	set_listen_tel_num				0x0048 	//��������
#define	set_prerog_sms_num				0x0049	//��Ȩ���ź���
#define	set_alarm_mask					0x0050	//���������֣���λ����Ϣ�еı���λ��Ӧ��Ϊ1�ķ�����Ӧ����
#define	set_alarm_sms_mask				0x0051	//����Ϣ���������֣���λ����Ϣ�еı���λ��Ӧ��Ϊ1�ķ�����Ӧ����
#define	set_cam_mask			    	0x0052	//���������֣�ͬ��
#define	set_cam_save_mask				0x0053	//���մ洢�����֣�Ϊ1�洢ͬ��
#define	set_crucial_alarm_mask			0x0054	//�ؼ�����������Ϊ1
#define	set_over_speed					0x0055	//����ֵ
#define	set_over_speed_time				0x0056	//���ٳ���ʱ��
#define	set_over_time_once				0x0057	//����������ʻ��ʱ ��
#define	set_over_time_day				0x0058	//һ��������ʻ��ʱ
#define	set_min_rest_time				0x0059	//��С��Ϣʱ��
#define	set_max_stop_time				0x005a	//�ͣ��ʱ��
#define	set_over_speed_notice			0x005b	//���ٱ���Ԥ����ֵ
#define	set_over_time_notice			0x005c	//���ٱ���Ԥ����ֵ
#define	set_collision_alarm				0x005d	//��ײ������������
#define	set_rollover_alarm				0x005e	//�෭������������
#define	set_fixtime_photograph_control	0x0064	//��ʱ���տ���
#define	set_fixdis_photograph_control	0x0065	//�������տ���
#define	set_media_ality					0x0070	//ý������ 1-10 1���
#define	set_brightness					0x0071	//����0-255
#define	set_contrast					0x0072	//�Աȶ� ͬ��
#define	set_saturation					0x0073	//���Ͷ� ͬ��
#define set_chroma						0x0074	//ɫ��
#define	set_odometer					0x0080	//��� 0.1km
#define	set_province_id					0x0081	//��������ʡID
#define	set_city_id						0x0082	//����������ID
#define	set_vehicle_id					0x0083	//���ƺ�
#define	set_vehicle_color				0x0084	//������ɫ  
#define	set_gnss_mode_type				0x0090	//GNSS��λģʽ
#define	set_gnss_bitrate				0x0091	//GNSS����������
#define	set_gnss_message_frequency		0x0092	//GNSS��λ�������Ƶ��
#define	set_gnss_message_collection		0x0093	//GNSS��λ���ݲɼ�Ƶ��
#define	set_gnss_message_send_type		0x0094	//GNSS��λ�����ϴ�Ƶ��
#define	set_gnss_message_send_set		0x0095	//GNSS��λ�����ϴ�����
#define	set_can1_collection_time		0x0100	//CANͨ��1�ɼ�ʱ����
#define	set_can1_send_time				0x0101	//CANͨ��1����ʱ����
#define	set_can2_collection_time		0x0102	//CANͨ��2�ɼ�ʱ����
#define	set_can2_send_time				0x0103	//CANͨ��2����ʱ����
#define	set_can_id_collection			0x0110	//CAN����ID0�����ɼ�����
#define	set_can_id_collection1			0x0111	//CAN����ID1�����ɼ�����
#define	set_can_id_collection2			0x0112	//CAN����ID2�����ɼ�����
#define	set_can_id_collection3			0x0113	//CAN����ID3�����ɼ�����




/* ״̬λ */
typedef union
{
	DWORD value;
	struct
	{
		BYTE acc_on				:1;		// 0 acc״̬	  0��     1��
		BYTE gps_fix			:1;		// 1 gps�Ƿ�λ  0����λ 1��λ
		BYTE gps_ns				:1;		// 2 0��γ 1��γ
		BYTE gps_ew				:1;		// 3 0���� 1����
		BYTE op_status			:1;		// 4 ��Ӫ״̬     0��Ӫ   1ͣ��
		BYTE gps_enc			:1;		// 5 ��γ�ȼ���   0������ 1����
		BYTE recv6to7			:2;		// 6--7����
		BYTE car_state			:2;		// 8:9	00:����  01:��� 10:Ref  11: ����
		BYTE oilway				:1;		// 10 ������·    0����   1�Ͽ�
		BYTE power				:1;		// 11 ������·    0����   1�Ͽ�
		BYTE door_lock			:1;		// 12 ����        0����   1����
		BYTE door_stata1		:1;		// 13 ��1״̬
		BYTE door_stata2		:1;		// 14 ��2״̬
		BYTE door_stata3		:1;		// 15 ��3״̬
		BYTE door_stata4		:1;		// 16 ��4״̬
		BYTE door_stata5		:1;		// 17 ��5״̬
		BYTE gps_used			:1;		// 18 ʹ��GPS��λ
		BYTE beidou_used		:1;		// 19 ʹ�ñ�����λ
		BYTE glonass_used		:1;		// 20 ʹ��glonass���Ƕ�λ
		BYTE galileo_used		:1;		// 21 ʹ��٤���Զ�λ
		WORD recv22to31			:10;	// 22--31 ����
	}reg;
}UNIT_STATUS;



/* ����״̬ */
typedef union
{
	DWORD value;
	struct
	{
		BYTE SOS					:1;
		BYTE speed					:1;
		BYTE overtime				:1;
		BYTE danger					:1;
		BYTE gnss					:1;
		BYTE gnss_anten_cut			:1;
		BYTE gnss_anten_short		:1;
		BYTE pwr_low				:1;
		BYTE pwr_lose				:1;
		BYTE lcd_error				:1;
		BYTE tts_error				:1;
		BYTE camera_error			:1;
		BYTE id_error				:1;
		BYTE recv1					:1;
		BYTE recv2					:1;
		BYTE recv3					:1;
		BYTE recv4					:1;
		BYTE recv5					:1;
		BYTE cnt_drive_overtime		:1;
		BYTE stop_overtime			:1;
		BYTE area					:1;
		BYTE line					:1;
		BYTE line_time				:1;
		BYTE line_deviate			:1;
		BYTE vss					:1;
		BYTE fuel					:1;
		BYTE theft					:1;
		BYTE acc_noallow			:1;
		BYTE move_noallow			:1;
		BYTE collision				:1;
		BYTE Rollover				:1;
		BYTE door_noallow			:1;
	}reg;
}ALARM_STATUS;



extern ALARM_STATUS Alarm_stu;
extern UNIT_STATUS  Status_stu;
extern const WORD ProtocolMakeCap;
extern BYTE ProtocolMakeBuf[];

extern BYTE server_rdata_program(WORD lenth);
extern WORD t808_reverse_escape(BYTE* data, WORD lenth);
extern WORD t808_make_protocol(WORD id, WORD lenth);
extern WORD t808_make_location_info(BYTE* data);
extern WORD t808_push_data_to_buf(BYTE* data, WORD lenth);
extern void sys_cfg_set_dsp(BYTE* data, WORD lenth);
extern void send_normal_answer(WORD cycle_num, WORD id, BYTE result);
extern void send_sys_cfg_read_anwser(WORD cycle_num);
extern void send_request_location_answer(WORD cycle_num);
extern void send_regist_data(void);
extern void send_login_data(void);
extern void send_heartbeat(void);
extern void send_location_info(void);




#endif
