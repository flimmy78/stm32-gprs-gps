/******************************************
�ļ�����: jt_t808_dsp.c
��	  ��: byy
��    ��:
˵    ��: JT/T808Э���������/��ȡ
�޸ļ�¼: 2015-06-26,������
******************************************/
#include "all_use.h"




/*******************************************
��������: �ն˲�������
��ڲ���: data, ��Ϣ��ָ��
          lenth, ��Ϣ�峤��
���ڲ���: 
��    ע: 
*******************************************/
void sys_cfg_set_dsp(BYTE* data, WORD lenth)
{
	BOOL  flag_relink = FALSE;
	BYTE  cfg_num;
	DWORD cfg_id;
	BYTE  cfg_lenth;
	BYTE* cfg_ptr;
	BYTE  i;
	
	flag_relink = FALSE;
	cfg_num = data[0];
	cfg_ptr = &data[1];
	printf_time_msg("<<--������������");
	for(i = 0; i < cfg_num; i++)
	{
		cfg_id = four_char_to_long(cfg_ptr);
		cfg_lenth = cfg_ptr[4];
		switch(cfg_id)
		{
			case set_heartbeat_time:				//0x0001
				Device.heartbeat_time_inteval = four_char_to_long(&cfg_ptr[5]);
				print_debug_msg("����ʱ����%d�� ", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_tcp_wait_time:					//0x0002
				print_debug_msg("tcp��ʱʱ��%d��", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_tcp_retry_times:				//0x0003
				print_debug_msg("tcp�ش�����%d", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_udp_wait_time:					//0x0004
				print_debug_msg("udp��ʱʱ��%d��", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_udp_retry_times:				//0x0005
				print_debug_msg("udp�ش�����%d", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_sms_wait_time:					//0x0006
				print_debug_msg("����ϢӦ��ʱ%d��", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_sms_retry_times:				//0x0007
				print_debug_msg("����Ϣ�ش�����%d", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_main_server_dial_apn:			//0x0010
				flag_relink = TRUE;
				print_debug_msg("��������APN:%s", &cfg_ptr[5]);
				break;			
			case set_main_server_dial_user:			//0x0011
				flag_relink = TRUE;
				print_debug_msg("�������������û���:%s", &cfg_ptr[5]);
				break;			
			case set_main_server_dial_pwd:			//0x0012
				flag_relink = TRUE;
				print_debug_msg("����������������:%s", &cfg_ptr[5]);
				break;			
			case set_main_server_addr:				//0x0013
				flag_relink = TRUE;
				print_debug_msg("����������ַ:%s", &cfg_ptr[5]);
				break;
			case set_back_server_dial_apn:			//0x0014 //���ݷ�����apn
				flag_relink = TRUE;
				print_debug_msg("���ݷ�����APN:%s", &cfg_ptr[5]);
				break;
			case set_back_server_dial_user:			//0x0015//�û���
				flag_relink = TRUE;
				print_debug_msg("���ݷ������û���:%s", &cfg_ptr[5]);
				break;
			case set_back_server_dial_pwd:			//0x0016 //����
				flag_relink = TRUE;
				print_debug_msg("���ݷ���������:%s", &cfg_ptr[5]);
				break;
			case set_back_server_addr:				//0x0017
				print_debug_msg("���ݷ�������ַ:%s", &cfg_ptr[5]);
				flag_relink = TRUE;
				break;
			case set_main_server_tcp_port:			//0x0018
				flag_relink = TRUE;
				print_debug_msg("TCP�˿ں�:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_main_server_udp_port:			//0x0019
				flag_relink = TRUE;
				print_debug_msg("UDP�˿ں�:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_ic_server_addr:				//0x001a
				flag_relink = TRUE;
				print_debug_msg("ID����������ַ:%s", &cfg_ptr[5]);
				break;
			case set_ic_server_tcp_port:			//0x001b
				flag_relink = TRUE;
				print_debug_msg("ID����TCP�˿ں�:%d", four_char_to_long(&cfg_ptr[5]));
				break;	
			case set_ic_server_udp_port:			//0x001c
				flag_relink = TRUE;
				print_debug_msg("ID����UDP�˿ں�:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_ic_server_addr_back:			//0x001d
				flag_relink = TRUE;
				print_debug_msg("ID���ݷ�������ַ:%s", &cfg_ptr[5]);
				break;
			case set_up_gpsdata_tactics:			//0x0020
				print_debug_msg("�����ϱ�����:%d", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_up_gpsdata_plan:				//0x0021
				print_debug_msg("�����ϱ��ƻ�:%d", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_up_gpsdata_time_nodriv:		//0x0022
				print_debug_msg("�޼�ʻԱ�ϱ�ʱ����%d�� ", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_time_sleep:			//0x0027
				Device.send_time_inteval_sleep = four_char_to_long(&cfg_ptr[5]);
				print_debug_msg("����ʱ�ϱ�ʱ����%d��", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_time_alarm:			//0x0028
				Device.send_time_inteval_alarm = four_char_to_long(&cfg_ptr[5]);
				print_debug_msg("����ʱ���ϱ����%d��", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_time_default:		//0x0029
				Device.send_time_inteval_default = four_char_to_long(&cfg_ptr[5]);
				print_debug_msg("�����ϱ�ʱ����%d��", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_lenth_default:		//0x002c
				print_debug_msg("�����ϱ�������%d��", four_char_to_long(&cfg_ptr[5]));
				break;	
			case set_up_gpsdata_lenth_nodriv:		//0x002d
				print_debug_msg("�޼�ʻԱ�ϱ�������%d��", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_lenth_sleep:		//0x002e
				print_debug_msg("����ʱ���ϱ�������%d��", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_lenth_alarm:		//0x002f
				print_debug_msg("��������ʱ���ϱ����%d��", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_angle:				//0x0030
				print_debug_msg("�յ㲹���Ƕ�:%d��", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_up_illegal_move:				//0x0031
				print_debug_msg("�Ƿ�λ�Ʒ�ֵ%d��", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_service_tel_num:				//0x0040
				print_debug_msg("���ƽ̨����:%s", &cfg_ptr[5]);
				break;			
			case set_reset_tel_num:					//0x0041
				print_debug_msg("��λ�绰����:%s", &cfg_ptr[5]);
				break;			
			case set_set_default_tel_num:			//0x0042
				print_debug_msg("�ָ���������:%s", &cfg_ptr[5]);
				break;			
			case set_service_sms_num:				//0x0043
				print_debug_msg("����ƽ̨SMS����:%s", &cfg_ptr[5]);
				break;			
			case set_service_alarm_num:				//0x0044
				print_debug_msg("����ƽ̨������SMS����:%s", &cfg_ptr[5]);
				break;			
			case set_tel_answer_tactics:			//0x0045
				print_debug_msg("�绰��������:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_max_tel_time_once:				//0x0046
				print_debug_msg("һ��ͨ�����ʱ��%d��", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_max_tel_time_month:			//0x0047
				print_debug_msg("һ���ͨ��ʱ��%d��", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_listen_tel_num:				//0x0048
				print_debug_msg("����ƽ̨��������:%s", &cfg_ptr[5]);
				break;			
			case set_prerog_sms_num:				//0x0049	
				print_debug_msg("�����Ȩ���ź���:%s", &cfg_ptr[5]);
				break;
			case set_alarm_mask:					//0x0050
				print_debug_msg("����������%0.8X", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_alarm_sms_mask:				//0x0051
				print_debug_msg("����Ϣ����������%0.8X", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_cam_mask:						//0x0052
				print_debug_msg("����������%0.8X", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_cam_save_mask:					//0x0053
				print_debug_msg("���մ洢������%0.8X", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_crucial_alarm_mask:			//0X0054
				print_debug_msg("�ؼ�����������%0.8X", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_over_speed:					//0X0055
				print_debug_msg("����ֵ%dKm/h", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_over_speed_time:				//0X0056
				print_debug_msg("���ٳ���ʱ��%d��", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_over_time_once:				//0X0057
				print_debug_msg("����������ʻ��ʱ%d��", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_over_time_day:					//0X0058
				print_debug_msg("һ��������ʻ��ʱ%d��", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_min_rest_time:					//0X0059
				print_debug_msg("��С��Ϣʱ��%d��", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_max_stop_time:					//0X005a
				print_debug_msg("�ͣ��ʱ��%d��", four_char_to_long(&cfg_ptr[5]));	
				break;	
			case set_over_speed_notice:				//0x005b
				print_debug_msg("����Ԥ����ֵ%d(0.1km/h)", two_char_to_short(&cfg_ptr[5]));	
				break;
			case set_over_time_notice:				//0x005c
				print_debug_msg("ƣ�ͼ�ʻԤ����ֵ%d��", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_collision_alarm:				//0x005d
				print_debug_msg("��ײ��������%0.4X", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_rollover_alarm:				//0x005e
				print_debug_msg("�෭�����Ƕ�:%d��", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_fixtime_photograph_control:	//0x0064
				print_debug_msg("��ʱ���տ���%X", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_fixdis_photograph_control:		//0x0065
				print_debug_msg("�������տ���%X", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_media_ality:					//0x0070
				print_debug_msg("ý������%d��", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_brightness:					//0x0071
				print_debug_msg("����%d��", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_contrast:						//0x0072
				print_debug_msg("�Աȶ�%d��", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_saturation:					//0x0073
				print_debug_msg("���Ͷ�%d��", four_char_to_long(&cfg_ptr[5]));	
				break;	
			case set_chroma:						//0x0074
				print_debug_msg("ɫ��%d��", four_char_to_long(&cfg_ptr[5]));	
				break;	
			case set_odometer:						//0x0080
				print_debug_msg("�������%d(100m)", four_char_to_long(&cfg_ptr[5]));		
				break;
			case set_province_id:					//0x0081
				print_debug_msg("��������ʡID:%d", two_char_to_short(&cfg_ptr[5]));
				break;			
			case set_city_id:						//0x0082
				print_debug_msg("����������ID:%d", two_char_to_short(&cfg_ptr[5]));
				break;			
			case set_vehicle_id:					//0x0083
				print_debug_msg("���ƺ�:%s", &cfg_ptr[5]);
				break;			
			case set_vehicle_color:					//0x0084
				print_debug_msg("������ɫ%d", cfg_ptr[5]);
				break;
			case set_gnss_mode_type:				//0x0090//GNSS��λģʽ
				print_debug_msg("GNSS��λģʽ:%0.2X", cfg_ptr[5]);
				break;
			case set_gnss_bitrate:					//0x0091//GNSS����������
				print_debug_msg("GNSS����������:%0.2X", cfg_ptr[5]);
				break;
			case set_gnss_message_frequency:		//0x0092//GNSS��λ�������Ƶ��
				print_debug_msg("GNSS���Ƶ��:%d", cfg_ptr[5]);
				break;
			case set_gnss_message_collection:		//0x0093//GNSS��λ���ݲɼ�Ƶ��
				print_debug_msg("GNSS�ɼ�Ƶ��:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_gnss_message_send_type:		//0x0094//GNSS��λ�����ϴ�ģʽ
				print_debug_msg("GNSS�ϴ�ģʽ:%0.2X", cfg_ptr[5]);
				break;
			case set_gnss_message_send_set:			//0x0095//GNSS��λ�����ϴ�����
				print_debug_msg("GNSS�ϴ�����:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_can1_collection_time:			//0x0100
				print_debug_msg("can1 �ɼ��������%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_can1_send_time:				//0x0101
				print_debug_msg("can1 ���ͼ������%d", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_can2_collection_time:			//0x0102
				print_debug_msg("can2 �ɼ��������%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_can2_send_time:				//0x0103
				print_debug_msg("can2 ���ͼ������%d", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_can_id_collection:				//0x0110
				print_debug_msg("can ����ID�����ɼ�����");
				break;
			default: 
				print_debug_msg("��ʶ��ID:%04X", cfg_id);
				break;
		}
		cfg_ptr += (cfg_lenth + 5);
	}
	if(flag_relink == TRUE)
	{
		print_debug_msg("--->�������ӷ�����");
	}
	write_device_info_to_flash();
}






/* ���DWORD���� */
static WORD add_long_cfg(BYTE* ptr, BYTE* num, DWORD id, DWORD cfg)
{
	long_to_four_char(id, ptr);
	ptr[4] = 4;
	long_to_four_char(cfg, ptr + 5);
	(*num)++;
	return 9;
}
/* ���STRING���� */
static WORD add_string_cfg(BYTE* ptr, BYTE* num, DWORD id, BYTE* cfg)
{
	long_to_four_char(id, ptr);
	ptr[4] = 4;
	strcpy((char*)ptr + 5, (char*)cfg);
	(*num)++;
	return(strlen((char*)cfg) + 5);
}


/*******************************************
��������: ��ѯ�豸������ϢӦ��
��ڲ���:
���ڲ���: 
��    ע: 
*******************************************/
void send_sys_cfg_read_anwser(WORD cycle_num)
{
	BYTE* data_body = &ProtocolMakeBuf[13];
	WORD s_lenth = 0;
	BYTE cfg_num = 0;
	BYTE* cfg_ptr;
	BYTE err;

	OSSemPend(sem_make_data_ok, 1000, &err);
	/* Ӧ����ˮ�� */	
	short_to_two_char(cycle_num, data_body);
	s_lenth += 2;
	cfg_num = 0;
	/* �������б� */
	cfg_ptr = &data_body[3];
	/* �������ͼ��          0x0001 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_heartbeat_time, Device.heartbeat_time_inteval);
	/* TCPӦ��ʱʱ��       0x0002 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_tcp_wait_time, 5);
	/* TCP�ش�����           0x0003 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_tcp_retry_times, 3);
	/* UDPӦ��ʱʱ��       0x0004 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_udp_wait_time, 5);
	/* UDP�ش�����           0x0005 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_udp_retry_times, 3);
	/* ����Ӧ��ʱʱ��      0x0006 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_sms_wait_time, 5);
	/* �����ش�����          0x0007 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_sms_retry_times, 3);
	/* ����������ַ          0x0013 */
	cfg_ptr += add_string_cfg(cfg_ptr, &cfg_num, set_main_server_addr, (BYTE*)IPint2string(Device.server_ip_int, Ip_string_buf));
	/* ������TCP�˿�         0x0018 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_main_server_tcp_port, Device.server_port);
	/* λ���ϱ�����          0x0020 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_tactics, 0);
	/* λ���ϱ�����          0x0021 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_plan, 0);
	/* �޼�ʻԱʱ���ϱ�ʱ��  0x0022 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_time_nodriv, 60);
	/* ����ʱ���ϱ�ʱ����  0x0027 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_time_sleep, Device.send_time_inteval_sleep);
	/* ����ʱ���ϱ����      0x0028 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_time_alarm, Device.send_time_inteval_alarm);
	/* �����ϱ�ʱ����      0x0029 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_time_default, Device.send_time_inteval_default);
	
	/* �������� */
	data_body[2] = cfg_num;
	s_lenth++;
	s_lenth = t808_make_protocol(terminal_heartbeat, s_lenth);	
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}


