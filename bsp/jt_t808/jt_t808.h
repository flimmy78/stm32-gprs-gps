#ifndef __JT_T808_H_
#define __JT_T808_H_
#include "type.h"





/*-------------------------T808协议消息ID--------------------------*/
#define terminal_normal_answer 			0x0001 //通用应答
#define terminal_heartbeat     			0x0002 //心跳
#define terminal_regist					0x0100 //终端注册
#define terminal_update_ask_ver 		0x0107 //查询版本信息
#define terminal_update_get_pkg 		0x0108 //获取升级包
#define terminal_deregist				0x0003 //终端注消
#define terminal_authentication 		0x0102 //终端鉴权
#define terminal_answer_sys_cfg			0x0104 //参数查询应答
#define terminal_up_gps_data			0x0200 //位置信息上报
#define terminal_anwser_gps_data		0x0201 //位置信息应答
#define terminal_event_report			0x0301 //事件报告
#define terminal_return_answer			0x0302 //回答提问
#define terminal_return_oder			0x0303 //选择或取消点播
#define terminal_vehicle_ctrl			0x0500 //车辆控制
#define terminal_up_recode				0x0700 //行驶记录仪数据上传
#define terminal_up_freight_note		0x0701 //电子运单上报
#define terminal_up_driver_info			0x0702 //驾驶员信息
#define terminal_up_multi_gps_data 		0x0704 //批量上传
#define terminal_media_event			0x0800 //多媒体事件上传
#define terminal_up_media				0x0801 //多媒体上传
#define terminal_media_check_return 	0x0802 //媒体检索返回
#define terminal_transmission			0x0900 //上行透传
#define terminal_rsa					0x0A00 //rsa
//=======
#define server_normal_answer 			0x8001 //通用应答
#define server_regist_answer			0x8100 //终端注册应答
#define server_update_ver_answer 		0x8107 //查询版本信息
#define server_update_pkg_answer 		0x8108 //获取升级包
#define server_set_terminal_cfg			0x8103 //设置终端参数
#define server_read_terminal_cfg 		0x8104 //读取终端参数
#define server_ctrl_terminal			0x8105 //终端控制
#define server_read_some_cfg			0x8106 //读取终端指定参数
#define server_read_gps_data			0x8201 //查询终端
#define server_temporary_superv 		0x8202 //服务器临时监控
#define server_confirm_alarm			0x8203 //人工确认报警消息
#define server_send_text				0x8300 //文本信息下发
#define server_event_set				0x8301 //事件下发
#define server_send_ask					0x8302 //提问
#define server_send_oder				0x8303 //点播
#define server_send_msg					0x8304 //信息服务
#define server_call_back				0x8400 //回拨
#define server_set_phone_book			0x8401 //设置电话本
#define server_vehicle_ctrl				0x8500 //车辆控制
#define server_set_area_round			0x8600 //设置原型区域
#define server_del_area_round			0x8601 //删除圆形区域
#define server_set_area_rect			0x8602 //矩形
#define server_del_area_rect			0x8603
#define server_set_area_polygon			0x8604 //多边形
#define server_del_area_polygon			0x8605
#define server_set_area_route			0x8606 //路线
#define server_del_area_route			0x8607
#define server_read_recode				0x8700 //读取行驶记录
#define server_set_recode				0x8701 //记录仪参数设置
#define server_request_idcard			0x8702 //上报驾驶员身份信息请求
#define server_media_return				0x8800 //多媒体上传应答
#define server_take_picture				0x8801 //拍照
#define server_media_check				0x8802 //媒体检索
#define server_read_media				0x8803 //媒体数据上传
#define server_recode_sound				0x8804 //开始录音
#define server_transmission 			0x8900 //下行透传
#define server_rsa						0x8A00 //




/*-------------------------T808协议参数ID--------------------------*/
#define	set_heartbeat_time				0x0001	//心跳时间间隔 秒
#define	set_tcp_wait_time				0x0002	//tcp超时时间 秒
#define	set_tcp_retry_times				0x0003	//tcp重传次数
#define	set_udp_wait_time				0x0004 	//udp超时时间 秒
#define	set_udp_retry_times				0x0005	//udp重传次数
#define	set_sms_wait_time				0x0006 	//短消息应答超时 秒
#define	set_sms_retry_times				0x0007	//短消息重传次数 
#define	set_main_server_dial_apn		0x0010 	//主服务器apn
#define	set_main_server_dial_user		0x0011	//用户名
#define	set_main_server_dial_pwd		0x0012 	//密码
#define	set_main_server_addr			0x0013	//主服务器地址
#define	set_back_server_dial_apn		0x0014 	//备份服务器apn
#define	set_back_server_dial_user		0x0015	//用户名
#define	set_back_server_dial_pwd		0x0016 	//密码
#define	set_back_server_addr			0x0017	//备份服务器地址
#define	set_main_server_tcp_port		0x0018	//服务器TCP端口
#define	set_main_server_udp_port		0x0019	//服务器UDP端口
#define set_ic_server_addr				0x001a	//IC卡认证服务器主地址
#define set_ic_server_tcp_port			0x001b	//IC卡认证服务器TCP端口
#define set_ic_server_udp_port			0x001c	//IC卡认证服务器UDP端口
#define set_ic_server_addr_back   		0x001d	//IC卡认证服务器备用地址
#define	set_up_gpsdata_tactics			0x0020	//数据上报策略 0定时 1定距 2混合
#define	set_up_gpsdata_plan				0x0021	//0:acc 1 驾驶员登录状态
#define	set_up_gpsdata_time_nodriv		0x0022	//无驾驶员信息时的上报时间间隔 秒
#define	set_up_gpsdata_time_sleep		0x0027 	//休眠时的上报时间间隔
#define	set_up_gpsdata_time_alarm		0x0028	//报警时的上报间隔
#define	set_up_gpsdata_time_default		0x0029	//正常上报时间间隔
#define	set_up_gpsdata_lenth_default	0x002c	//正常上报距离间隔
#define	set_up_gpsdata_lenth_nodriv		0x002d	//无驾驶员信息时的上报距离间隔 米
#define	set_up_gpsdata_lenth_sleep		0x002e 	//休眠时的上报时间间隔
#define	set_up_gpsdata_lenth_alarm		0x002f	//报警时的上报间隔
#define	set_up_gpsdata_angle			0x0030	//拐点补传
#define	set_up_illegal_move				0x0031	//非法位移阀值(米)
#define	set_service_tel_num				0x0040	//服务平台号码 
#define	set_reset_tel_num				0x0041 	//复位号码
#define	set_set_default_tel_num			0x0042 	//恢复出厂号码
#define	set_service_sms_num				0x0043 	//服务平台号短息号码 
#define	set_service_alarm_num			0x0044 	//服务平台报警号短息号码
#define	set_tel_answer_tactics			0x0045	//0自动接听 1 accon时自动接听
#define	set_max_tel_time_once			0x0046	//一次通话的最长时间 0为不允许 0xffffffff为无限制
#define	set_max_tel_time_month			0x0047	//一月最长通话时间	  同上
#define	set_listen_tel_num				0x0048 	//监听号码
#define	set_prerog_sms_num				0x0049	//特权短信号码
#define	set_alarm_mask					0x0050	//报警屏蔽字，和位置信息中的报警位对应，为1的发送相应报警
#define	set_alarm_sms_mask				0x0051	//短消息报警屏蔽字，和位置信息中的报警位对应，为1的发送相应报警
#define	set_cam_mask			    	0x0052	//拍照屏蔽字，同上
#define	set_cam_save_mask				0x0053	//拍照存储屏蔽字，为1存储同上
#define	set_crucial_alarm_mask			0x0054	//关键报警屏蔽字为1
#define	set_over_speed					0x0055	//超速值
#define	set_over_speed_time				0x0056	//超速持续时间
#define	set_over_time_once				0x0057	//单次连续驾驶超时 秒
#define	set_over_time_day				0x0058	//一天连续驾驶超时
#define	set_min_rest_time				0x0059	//最小休息时间
#define	set_max_stop_time				0x005a	//最长停车时间
#define	set_over_speed_notice			0x005b	//超速报警预警差值
#define	set_over_time_notice			0x005c	//超速报警预警差值
#define	set_collision_alarm				0x005d	//碰撞报警参数设置
#define	set_rollover_alarm				0x005e	//侧翻报警参数设置
#define	set_fixtime_photograph_control	0x0064	//定时拍照控制
#define	set_fixdis_photograph_control	0x0065	//定距拍照控制
#define	set_media_ality					0x0070	//媒体质量 1-10 1最好
#define	set_brightness					0x0071	//亮度0-255
#define	set_contrast					0x0072	//对比度 同上
#define	set_saturation					0x0073	//饱和度 同上
#define set_chroma						0x0074	//色度
#define	set_odometer					0x0080	//里程 0.1km
#define	set_province_id					0x0081	//车辆所在省ID
#define	set_city_id						0x0082	//车辆所在市ID
#define	set_vehicle_id					0x0083	//车牌号
#define	set_vehicle_color				0x0084	//车辆颜色  
#define	set_gnss_mode_type				0x0090	//GNSS定位模式
#define	set_gnss_bitrate				0x0091	//GNSS波特率设置
#define	set_gnss_message_frequency		0x0092	//GNSS定位数据输出频率
#define	set_gnss_message_collection		0x0093	//GNSS定位数据采集频率
#define	set_gnss_message_send_type		0x0094	//GNSS定位数据上传频率
#define	set_gnss_message_send_set		0x0095	//GNSS定位数据上传设置
#define	set_can1_collection_time		0x0100	//CAN通道1采集时间间隔
#define	set_can1_send_time				0x0101	//CAN通道1发送时间间隔
#define	set_can2_collection_time		0x0102	//CAN通道2采集时间间隔
#define	set_can2_send_time				0x0103	//CAN通道2发送时间间隔
#define	set_can_id_collection			0x0110	//CAN总线ID0单独采集设置
#define	set_can_id_collection1			0x0111	//CAN总线ID1单独采集设置
#define	set_can_id_collection2			0x0112	//CAN总线ID2单独采集设置
#define	set_can_id_collection3			0x0113	//CAN总线ID3单独采集设置




/* 状态位 */
typedef union
{
	DWORD value;
	struct
	{
		BYTE acc_on				:1;		// 0 acc状态	  0关     1开
		BYTE gps_fix			:1;		// 1 gps是否定位  0不定位 1定位
		BYTE gps_ns				:1;		// 2 0北纬 1南纬
		BYTE gps_ew				:1;		// 3 0东经 1西经
		BYTE op_status			:1;		// 4 运营状态     0运营   1停运
		BYTE gps_enc			:1;		// 5 经纬度加密   0不加密 1加密
		BYTE recv6to7			:2;		// 6--7保留
		BYTE car_state			:2;		// 8:9	00:空载  01:半截 10:Ref  11: 满截
		BYTE oilway				:1;		// 10 车辆油路    0正常   1断开
		BYTE power				:1;		// 11 车辆电路    0正常   1断开
		BYTE door_lock			:1;		// 12 车门        0解锁   1加锁
		BYTE door_stata1		:1;		// 13 门1状态
		BYTE door_stata2		:1;		// 14 门2状态
		BYTE door_stata3		:1;		// 15 门3状态
		BYTE door_stata4		:1;		// 16 门4状态
		BYTE door_stata5		:1;		// 17 门5状态
		BYTE gps_used			:1;		// 18 使用GPS定位
		BYTE beidou_used		:1;		// 19 使用北斗定位
		BYTE glonass_used		:1;		// 20 使用glonass卫星定位
		BYTE galileo_used		:1;		// 21 使用伽利略定位
		WORD recv22to31			:10;	// 22--31 保留
	}reg;
}UNIT_STATUS;



/* 报警状态 */
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
