/******************************************
文件名称: jt_t808_dsp.c
作	  者: byy
版    本:
说    明: JT/T808协议参数设置/读取
修改记录: 2015-06-26,程序建立
******************************************/
#include "all_use.h"




/*******************************************
函数功能: 终端参数设置
入口参数: data, 消息体指针
          lenth, 消息体长度
出口参数: 
备    注: 
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
	printf_time_msg("<<--参数设置命令");
	for(i = 0; i < cfg_num; i++)
	{
		cfg_id = four_char_to_long(cfg_ptr);
		cfg_lenth = cfg_ptr[4];
		switch(cfg_id)
		{
			case set_heartbeat_time:				//0x0001
				Device.heartbeat_time_inteval = four_char_to_long(&cfg_ptr[5]);
				print_debug_msg("心跳时间间隔%d秒 ", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_tcp_wait_time:					//0x0002
				print_debug_msg("tcp超时时间%d秒", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_tcp_retry_times:				//0x0003
				print_debug_msg("tcp重传次数%d", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_udp_wait_time:					//0x0004
				print_debug_msg("udp超时时间%d秒", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_udp_retry_times:				//0x0005
				print_debug_msg("udp重传次数%d", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_sms_wait_time:					//0x0006
				print_debug_msg("短消息应答超时%d秒", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_sms_retry_times:				//0x0007
				print_debug_msg("短消息重传次数%d", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_main_server_dial_apn:			//0x0010
				flag_relink = TRUE;
				print_debug_msg("主服务器APN:%s", &cfg_ptr[5]);
				break;			
			case set_main_server_dial_user:			//0x0011
				flag_relink = TRUE;
				print_debug_msg("主服务器拨号用户名:%s", &cfg_ptr[5]);
				break;			
			case set_main_server_dial_pwd:			//0x0012
				flag_relink = TRUE;
				print_debug_msg("主服务器拨号密码:%s", &cfg_ptr[5]);
				break;			
			case set_main_server_addr:				//0x0013
				flag_relink = TRUE;
				print_debug_msg("主服务器地址:%s", &cfg_ptr[5]);
				break;
			case set_back_server_dial_apn:			//0x0014 //备份服务器apn
				flag_relink = TRUE;
				print_debug_msg("备份服务器APN:%s", &cfg_ptr[5]);
				break;
			case set_back_server_dial_user:			//0x0015//用户名
				flag_relink = TRUE;
				print_debug_msg("备份服务器用户名:%s", &cfg_ptr[5]);
				break;
			case set_back_server_dial_pwd:			//0x0016 //密码
				flag_relink = TRUE;
				print_debug_msg("备份服务器密码:%s", &cfg_ptr[5]);
				break;
			case set_back_server_addr:				//0x0017
				print_debug_msg("备份服务器地址:%s", &cfg_ptr[5]);
				flag_relink = TRUE;
				break;
			case set_main_server_tcp_port:			//0x0018
				flag_relink = TRUE;
				print_debug_msg("TCP端口号:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_main_server_udp_port:			//0x0019
				flag_relink = TRUE;
				print_debug_msg("UDP端口号:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_ic_server_addr:				//0x001a
				flag_relink = TRUE;
				print_debug_msg("ID主服务器地址:%s", &cfg_ptr[5]);
				break;
			case set_ic_server_tcp_port:			//0x001b
				flag_relink = TRUE;
				print_debug_msg("ID服务TCP端口号:%d", four_char_to_long(&cfg_ptr[5]));
				break;	
			case set_ic_server_udp_port:			//0x001c
				flag_relink = TRUE;
				print_debug_msg("ID服务UDP端口号:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_ic_server_addr_back:			//0x001d
				flag_relink = TRUE;
				print_debug_msg("ID备份服务器地址:%s", &cfg_ptr[5]);
				break;
			case set_up_gpsdata_tactics:			//0x0020
				print_debug_msg("数据上报策略:%d", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_up_gpsdata_plan:				//0x0021
				print_debug_msg("数据上报计划:%d", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_up_gpsdata_time_nodriv:		//0x0022
				print_debug_msg("无驾驶员上报时间间隔%d秒 ", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_time_sleep:			//0x0027
				Device.send_time_inteval_sleep = four_char_to_long(&cfg_ptr[5]);
				print_debug_msg("休眠时上报时间间隔%d秒", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_time_alarm:			//0x0028
				Device.send_time_inteval_alarm = four_char_to_long(&cfg_ptr[5]);
				print_debug_msg("报警时的上报间隔%d秒", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_time_default:		//0x0029
				Device.send_time_inteval_default = four_char_to_long(&cfg_ptr[5]);
				print_debug_msg("正常上报时间间隔%d秒", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_lenth_default:		//0x002c
				print_debug_msg("正常上报距离间隔%d米", four_char_to_long(&cfg_ptr[5]));
				break;	
			case set_up_gpsdata_lenth_nodriv:		//0x002d
				print_debug_msg("无驾驶员上报距离间隔%d米", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_lenth_sleep:		//0x002e
				print_debug_msg("休眠时的上报距离间隔%d米", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_lenth_alarm:		//0x002f
				print_debug_msg("紧急报警时的上报间隔%d米", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_up_gpsdata_angle:				//0x0030
				print_debug_msg("拐点补传角度:%d度", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_up_illegal_move:				//0x0031
				print_debug_msg("非法位移阀值%d米", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_service_tel_num:				//0x0040
				print_debug_msg("监控平台号码:%s", &cfg_ptr[5]);
				break;			
			case set_reset_tel_num:					//0x0041
				print_debug_msg("复位电话号码:%s", &cfg_ptr[5]);
				break;			
			case set_set_default_tel_num:			//0x0042
				print_debug_msg("恢复出厂号码:%s", &cfg_ptr[5]);
				break;			
			case set_service_sms_num:				//0x0043
				print_debug_msg("服务平台SMS号码:%s", &cfg_ptr[5]);
				break;			
			case set_service_alarm_num:				//0x0044
				print_debug_msg("服务平台报警号SMS号码:%s", &cfg_ptr[5]);
				break;			
			case set_tel_answer_tactics:			//0x0045
				print_debug_msg("电话接听策略:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_max_tel_time_once:				//0x0046
				print_debug_msg("一次通话的最长时间%d秒", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_max_tel_time_month:			//0x0047
				print_debug_msg("一月最长通话时间%d秒", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_listen_tel_num:				//0x0048
				print_debug_msg("服务平台监听号码:%s", &cfg_ptr[5]);
				break;			
			case set_prerog_sms_num:				//0x0049	
				print_debug_msg("监管特权短信号码:%s", &cfg_ptr[5]);
				break;
			case set_alarm_mask:					//0x0050
				print_debug_msg("报警屏蔽字%0.8X", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_alarm_sms_mask:				//0x0051
				print_debug_msg("短消息报警屏蔽字%0.8X", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_cam_mask:						//0x0052
				print_debug_msg("拍照屏蔽字%0.8X", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_cam_save_mask:					//0x0053
				print_debug_msg("拍照存储屏蔽字%0.8X", four_char_to_long(&cfg_ptr[5]));
				break;			
			case set_crucial_alarm_mask:			//0X0054
				print_debug_msg("关键报警屏蔽字%0.8X", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_over_speed:					//0X0055
				print_debug_msg("超速值%dKm/h", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_over_speed_time:				//0X0056
				print_debug_msg("超速持续时间%d秒", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_over_time_once:				//0X0057
				print_debug_msg("单次连续驾驶超时%d秒", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_over_time_day:					//0X0058
				print_debug_msg("一天连续驾驶超时%d秒", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_min_rest_time:					//0X0059
				print_debug_msg("最小休息时间%d秒", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_max_stop_time:					//0X005a
				print_debug_msg("最长停车时间%d秒", four_char_to_long(&cfg_ptr[5]));	
				break;	
			case set_over_speed_notice:				//0x005b
				print_debug_msg("超速预警差值%d(0.1km/h)", two_char_to_short(&cfg_ptr[5]));	
				break;
			case set_over_time_notice:				//0x005c
				print_debug_msg("疲劳驾驶预警差值%d秒", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_collision_alarm:				//0x005d
				print_debug_msg("碰撞参数设置%0.4X", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_rollover_alarm:				//0x005e
				print_debug_msg("侧翻报警角度:%d度", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_fixtime_photograph_control:	//0x0064
				print_debug_msg("定时拍照控制%X", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_fixdis_photograph_control:		//0x0065
				print_debug_msg("定距拍照控制%X", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_media_ality:					//0x0070
				print_debug_msg("媒体质量%d级", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_brightness:					//0x0071
				print_debug_msg("亮度%d级", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_contrast:						//0x0072
				print_debug_msg("对比度%d级", four_char_to_long(&cfg_ptr[5]));	
				break;			
			case set_saturation:					//0x0073
				print_debug_msg("饱和度%d级", four_char_to_long(&cfg_ptr[5]));	
				break;	
			case set_chroma:						//0x0074
				print_debug_msg("色度%d级", four_char_to_long(&cfg_ptr[5]));	
				break;	
			case set_odometer:						//0x0080
				print_debug_msg("设置里程%d(100m)", four_char_to_long(&cfg_ptr[5]));		
				break;
			case set_province_id:					//0x0081
				print_debug_msg("车辆所在省ID:%d", two_char_to_short(&cfg_ptr[5]));
				break;			
			case set_city_id:						//0x0082
				print_debug_msg("车辆所在市ID:%d", two_char_to_short(&cfg_ptr[5]));
				break;			
			case set_vehicle_id:					//0x0083
				print_debug_msg("车牌号:%s", &cfg_ptr[5]);
				break;			
			case set_vehicle_color:					//0x0084
				print_debug_msg("车辆颜色%d", cfg_ptr[5]);
				break;
			case set_gnss_mode_type:				//0x0090//GNSS定位模式
				print_debug_msg("GNSS定位模式:%0.2X", cfg_ptr[5]);
				break;
			case set_gnss_bitrate:					//0x0091//GNSS波特率设置
				print_debug_msg("GNSS波特率设置:%0.2X", cfg_ptr[5]);
				break;
			case set_gnss_message_frequency:		//0x0092//GNSS定位数据输出频率
				print_debug_msg("GNSS输出频率:%d", cfg_ptr[5]);
				break;
			case set_gnss_message_collection:		//0x0093//GNSS定位数据采集频率
				print_debug_msg("GNSS采集频率:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_gnss_message_send_type:		//0x0094//GNSS定位数据上传模式
				print_debug_msg("GNSS上传模式:%0.2X", cfg_ptr[5]);
				break;
			case set_gnss_message_send_set:			//0x0095//GNSS定位数据上传设置
				print_debug_msg("GNSS上传设置:%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_can1_collection_time:			//0x0100
				print_debug_msg("can1 采集间隔设置%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_can1_send_time:				//0x0101
				print_debug_msg("can1 发送间隔设置%d", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_can2_collection_time:			//0x0102
				print_debug_msg("can2 采集间隔设置%d", four_char_to_long(&cfg_ptr[5]));
				break;
			case set_can2_send_time:				//0x0103
				print_debug_msg("can2 发送间隔设置%d", two_char_to_short(&cfg_ptr[5]));
				break;
			case set_can_id_collection:				//0x0110
				print_debug_msg("can 总线ID单独采集设置");
				break;
			default: 
				print_debug_msg("不识别ID:%04X", cfg_id);
				break;
		}
		cfg_ptr += (cfg_lenth + 5);
	}
	if(flag_relink == TRUE)
	{
		print_debug_msg("--->重新连接服务器");
	}
	write_device_info_to_flash();
}






/* 添加DWORD参数 */
static WORD add_long_cfg(BYTE* ptr, BYTE* num, DWORD id, DWORD cfg)
{
	long_to_four_char(id, ptr);
	ptr[4] = 4;
	long_to_four_char(cfg, ptr + 5);
	(*num)++;
	return 9;
}
/* 添加STRING参数 */
static WORD add_string_cfg(BYTE* ptr, BYTE* num, DWORD id, BYTE* cfg)
{
	long_to_four_char(id, ptr);
	ptr[4] = 4;
	strcpy((char*)ptr + 5, (char*)cfg);
	(*num)++;
	return(strlen((char*)cfg) + 5);
}


/*******************************************
函数功能: 查询设备参数信息应答
入口参数:
出口参数: 
备    注: 
*******************************************/
void send_sys_cfg_read_anwser(WORD cycle_num)
{
	BYTE* data_body = &ProtocolMakeBuf[13];
	WORD s_lenth = 0;
	BYTE cfg_num = 0;
	BYTE* cfg_ptr;
	BYTE err;

	OSSemPend(sem_make_data_ok, 1000, &err);
	/* 应答流水号 */	
	short_to_two_char(cycle_num, data_body);
	s_lenth += 2;
	cfg_num = 0;
	/* 参数项列表 */
	cfg_ptr = &data_body[3];
	/* 心跳发送间隔          0x0001 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_heartbeat_time, Device.heartbeat_time_inteval);
	/* TCP应答超时时间       0x0002 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_tcp_wait_time, 5);
	/* TCP重传次数           0x0003 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_tcp_retry_times, 3);
	/* UDP应答超时时间       0x0004 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_udp_wait_time, 5);
	/* UDP重传次数           0x0005 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_udp_retry_times, 3);
	/* 短信应答超时时间      0x0006 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_sms_wait_time, 5);
	/* 短信重传次数          0x0007 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_sms_retry_times, 3);
	/* 主服务器地址          0x0013 */
	cfg_ptr += add_string_cfg(cfg_ptr, &cfg_num, set_main_server_addr, (BYTE*)IPint2string(Device.server_ip_int, Ip_string_buf));
	/* 服务器TCP端口         0x0018 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_main_server_tcp_port, Device.server_port);
	/* 位置上报策略          0x0020 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_tactics, 0);
	/* 位置上报方案          0x0021 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_plan, 0);
	/* 无驾驶员时的上报时间  0x0022 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_time_nodriv, 60);
	/* 休眠时的上报时间间隔  0x0027 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_time_sleep, Device.send_time_inteval_sleep);
	/* 报警时的上报间隔      0x0028 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_time_alarm, Device.send_time_inteval_alarm);
	/* 正常上报时间间隔      0x0029 */
	cfg_ptr += add_long_cfg(cfg_ptr, &cfg_num, set_up_gpsdata_time_default, Device.send_time_inteval_default);
	
	/* 参数个数 */
	data_body[2] = cfg_num;
	s_lenth++;
	s_lenth = t808_make_protocol(terminal_heartbeat, s_lenth);	
	t808_push_data_to_buf(ProtocolMakeBuf, s_lenth);
	OSSemPost(sem_make_data_ok);
}


