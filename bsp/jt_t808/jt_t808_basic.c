/******************************************
文件名称: jt_t808_basic.c
作	  者: byy
版    本:
说    明: JT/T808协议层解析,组织
修改记录: 2015-06-26,程序建立
******************************************/
#include "all_use.h"



ALARM_STATUS Alarm_stu;
UNIT_STATUS  Status_stu;
const WORD ProtocolMakeCap = 1500;
BYTE ProtocolMakeBuf[ProtocolMakeCap + 1];





/*******************************************
函数功能: 数据反转义
入口参数: data, 转义前的数据
          lenth,转以后的数据
出口参数: 转义后的字节数
备    注: 测试通过
*******************************************/
WORD t808_reverse_escape(BYTE* data, WORD lenth)
{
	WORD i, j;
	
	for(i = 0; i < lenth; i++)
	{
		if(data[i] == 0x7D && (data[i+1] == 0x01 || data[i+1] == 0x02))
		{
			data[i] += (data[i + 1] - 1);
			for(j = i + 1; j < lenth - 1; j++)
			{
				data[j] = data[j + 1];
			}
			lenth--;
		}
	}
	return(lenth);
}





/*******************************************
函数功能: T808协议组装
入口参数: id, 	命令ID
		  lenth,数据体长度
出口参数: 组装后的数据长度
备    注: 测试通过,没有分包机制
*******************************************/
WORD t808_make_protocol(WORD id, WORD lenth)
{
	static WORD serial_num = 0;
	WORD ret_lenth = 0;
	BYTE check = 0;
	WORD i, j;
	
	/* 包标识 */
	ProtocolMakeBuf[0]  = 0x7E;
	/* 消息ID */
	ProtocolMakeBuf[1]  = (BYTE)(id >> 8);
	ProtocolMakeBuf[2]  = (BYTE)(id >> 0);
	/* 消息属性 */
	ProtocolMakeBuf[3]  = (BYTE)(lenth >> 8);
	ProtocolMakeBuf[4]  = (BYTE)(lenth >> 0);
	/* 手机号 */
	ProtocolMakeBuf[5]  = ((Device.phone_num[0]  - '0') << 4) + (Device.phone_num[1]  - '0');
	ProtocolMakeBuf[6]  = ((Device.phone_num[2]  - '0') << 4) + (Device.phone_num[3]  - '0');
	ProtocolMakeBuf[7]  = ((Device.phone_num[4]  - '0') << 4) + (Device.phone_num[5]  - '0');
	ProtocolMakeBuf[8]  = ((Device.phone_num[6]  - '0') << 4) + (Device.phone_num[7]  - '0');
	ProtocolMakeBuf[9]  = ((Device.phone_num[8]  - '0') << 4) + (Device.phone_num[9]  - '0');
	ProtocolMakeBuf[10] = ((Device.phone_num[10] - '0') << 4) + (Device.phone_num[11] - '0');
	/* 流水号 */
	ProtocolMakeBuf[11] = (BYTE)(serial_num >> 8);
	ProtocolMakeBuf[12] = (BYTE)(serial_num >> 0);
	serial_num++;
	/* 校验码 */
	check = 0;
	for(i = 0; i < lenth + 12; i++)
	{
		check ^= ProtocolMakeBuf[i + 1];
	}
	ProtocolMakeBuf[13 + lenth] = check;
	/* 包标识 */
	ProtocolMakeBuf[14 + lenth] = 0x7E;
	ret_lenth = 15 + lenth;
	
	/* 转义 */
	for(i = 1; i < ret_lenth - 1; i++)
	{
		if(ProtocolMakeBuf[i] == 0x7E
		|| ProtocolMakeBuf[i] == 0x7D)
		{
			for(j = 0; j < ret_lenth - i; j++)
			{
				ProtocolMakeBuf[ret_lenth-j] = ProtocolMakeBuf[ret_lenth-j-1];
			}
			ProtocolMakeBuf[i + 1] = (ProtocolMakeBuf[i] == 0x7E)? 0x02 : 0x01;
			ProtocolMakeBuf[i] = 0x7D;
			i++;
			ret_lenth++;
		}
	}
	
	return ret_lenth;
}




/*******************************************
函数功能: 组织位置信息
入口参数:
出口参数: 
备    注: 
*******************************************/
WORD t808_make_location_info(BYTE* data)
{
	WORD s_lenth = 0;
	
	/* 报警标识 */
	long_to_four_char(Alarm_stu.value, &data[0]);
	/* 状态 */
	long_to_four_char(Status_stu.value, &data[4]);
	/* 经维度 */
	if(Gps_info.status == 'A')
	{
		long_to_four_char(Gps_info.la_data, &data[8]);
		long_to_four_char(Gps_info.lo_data, &data[12]);
	}
	else
	{
		long_to_four_char(at.loc_la, &data[8]);
		long_to_four_char(at.loc_lo, &data[12]);
	}
	/* 高度 */
	short_to_two_char(Gps_info.hight, &data[16]);
	/* 速度 */
	short_to_two_char(Gps_info.speed, &data[18]);
	/* 方向 */
	short_to_two_char(Gps_info.dir, &data[20]);
	/* 时间 */
	time_hex2bcd(SysTime, &data[22]);
	s_lenth += 28;
	data += 28;
	
	/* 附加信息 */
	/* 模拟量 */
	data[0] = 0x2B;
	data[1] = 4;
	short_to_two_char(0, &data[2]);
	short_to_two_char(at.state_batte, &data[4]);
	data += 6;
	s_lenth += 6;
	/* GSM信号质量 */
	data[0] = 0x30;
	data[1] = 1;
	data[2] = at.state_csq;
	data += 3;
	s_lenth += 3;
	/* GNSS卫星数 */
	data[0] = 0x31;
	data[1] = 1;
	data[2] = Gps_info.gps_sate;
	data += 3;
	s_lenth += 3;
	
	return(s_lenth);
}




/*******************************************
函数功能: 将待发送的数据压入发送缓存
入口参数: 
出口参数: 
备    注: 
*******************************************/
WORD t808_push_data_to_buf(BYTE* data, WORD lenth)
{
	WORD move_lenth = 0;
	
	/* 发送缓存中空间不足 */
	if(at.socket[0].tdata_limit - at.socket[0].tlenth < lenth)
	{
		move_lenth = lenth - (at.socket[0].tdata_limit - at.socket[0].tlenth);
		memmove(at.socket[0].tdata, 
				at.socket[0].tdata + move_lenth, 
				at.socket[0].tlenth - move_lenth);
		at.socket[0].tlenth -= move_lenth;
	}
	
	memcpy(at.socket[0].tdata + at.socket[0].tlenth, data, lenth);
	at.socket[0].tlenth += lenth;
	
	return(lenth);
}







