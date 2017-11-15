/******************************************
文件名称: gps_rsv.c
作	  者: byy
版    本:
说    明: 系统控制任务
修改记录: 2011-08-18,程序建立。
******************************************/
#include "all_use.h"




/*创建用于串口接收的环形队列*/
const WORD GPSRcv_cap = 200;
BYTE GPSRcv_buffer[GPSRcv_cap + 1];
WORD GPSRcv_tail = 0;
WORD GPSRcv_head = 0;
static void GPSRcv_push(BYTE data);
static BYTE GPSRcv_pop(void);
static void GPSRcv_rest(void);
static WORD GPSRcv_lenth(void);

/* GPS解析 */
Gps_info_ST Gps_info;
static const WORD GpsRsvDataCap = 100;
static BYTE GpsRsvDataBuf[GpsRsvDataCap + 1];
static BYTE gps_data_program(BYTE *gps_data);





/*******************************************
函数名称: void Sys_Ctrl_task(void *p_arg)
函数功能: 系统控制任务
入口参数:
出口参数: 
备    注:
*******************************************/
void GPS_Resolve_task(void *p_arg)
{
	const WORD interval = 10;
	BYTE data;
	WORD lenth = 0;
	WORD rstop_time = 0;
	
	p_arg = p_arg;
	Gps_info.status = 'V';
	Gps_info.speed = 0;
	Gps_info.dir = 0;
	Gps_info.hight = 0;
	Gps_info.satellites = 0;
	Gps_info.hdop = 0; 
	Gps_info.gps_sate = 0;
	Gps_info.bds_sate = 0;
	GPS_standby(1);
	GPS_reset(1);
	while(1)
	{  	
		OSTimeDlyHMSM(0, 0, 0, 10);
		
		/* 无数据 */
		if(GPSRcv_lenth() == 0)
		{
			rstop_time += interval;
			if(lenth > 0 && rstop_time > 100)
			{
				GpsRsvDataBuf[lenth] = '\0';
				gps_data_program(GpsRsvDataBuf);
				lenth = 0;
				rstop_time = 0;
			}
		}

		while(GPSRcv_lenth() > 0)
		{
			rstop_time = 0;
			data = GPSRcv_pop();
			//printf("%c", data);
			GpsRsvDataBuf[lenth] = data;
			if(GpsRsvDataBuf[lenth] == '\n')
			{
				lenth++;
				GpsRsvDataBuf[lenth] = '\0';
				gps_data_program(GpsRsvDataBuf);
				lenth = 0;
			}
			else
			{
				if(++lenth >= GpsRsvDataCap)
				{
					lenth = 0;
				}
			}
		}
	}
}





/*******************************************
函数功能: 计算校验码
入口参数:
出口参数: 
备    注: 
*******************************************/
static BYTE get_gps_checksum(char* buf, WORD len)
{
	BYTE check_sum = 0;
	WORD i = 0;

	for(i = 0;i < len; i++)
	{
		if(buf[i] == '*')
		{
			break;
		}
		check_sum ^= buf[i];
	}
	return check_sum;
}





/*******************************************
函数功能: 定位数据解析
入口参数:
出口参数: 
备    注: 	
*******************************************/
static BYTE gps_data_program(BYTE *gps_data)
{
	DWORD gps_temp;
	BYTE  check_sum = 0;
	
	if(gps_data[0] != '$')
	{
		return 0;
	}
	check_sum = get_gps_checksum((char*)&gps_data[1], strlen((char*)gps_data) - 5);
	if(check_sum != texthex_to_data(gps_data + strlen((char*)gps_data) - 4)) 
	{
		//print_debug_msg("GPS数据校验错误");
		//printf("%s", gps_data);
		return 0;
	}

/*
GPRMC,025327.00,A,3639.97654,N,11707.58554,E,0.014,,140912,,,A*74
$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
<1> UTC时间，hhmmss（时分秒）格式
<2> 定位状态，A=有效定位，V=无效定位
<3> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输）
<4> 纬度半球N（北半球）或S（南半球）
<5> 经度dddmm.mmmm（度分）格式（前面的0也将被传输）
<6> 经度半球E（东经）或W（西经）
<7> 地面速率（000.0~999.9节，前面的0也将被传输）
<8> 地面航向（000.0~359.9度，以真北为参考基准，前面的0也将被传输）
<9> UTC日期，ddmmyy（日月年）格式
<10> 磁偏角（000.0~180.0度，前面的0也将被传输）
<11> 磁偏角方向，E（东）或W（西）
<12> 模式指示（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）
*/
	if((!memcmp("$GPRMC", gps_data, 6)) 
	|| (!memcmp("$GNRMC", gps_data, 6)) 
	|| (!memcmp("$BDRMC", gps_data, 6)))
	{		
		/* 定位标识 */
		gps_temp = search_text_para(2, gps_data);
		
		if(gps_data[gps_temp] == 'A')
		{
			/* 首次定位校时，整点校时 */
			if(Gps_info.status != 'A')
			{
				/* 解析GPS时间 */
				gps_temp = search_text_para(9, gps_data);
				if(isdigit(gps_data[gps_temp]))
				{
					Gps_info.time.day   = (gps_data[gps_temp + 0] - '0') * 10 + gps_data[gps_temp + 1] - '0';//日
					Gps_info.time.month = (gps_data[gps_temp + 2] - '0') * 10 + gps_data[gps_temp + 3] - '0';//月
					Gps_info.time.year  = (gps_data[gps_temp + 4] - '0') * 10 + gps_data[gps_temp + 5] - '0';//年
				}
				gps_temp = search_text_para(1, gps_data);
				if(isdigit(gps_data[gps_temp]))
				{
					Gps_info.time.hour = (gps_data[gps_temp + 0] - '0') * 10 + gps_data[gps_temp + 1] - '0';//时
					Gps_info.time.min  = (gps_data[gps_temp + 2] - '0') * 10 + gps_data[gps_temp + 3] - '0';//分
					Gps_info.time.sec  = (gps_data[gps_temp + 4] - '0') * 10 + gps_data[gps_temp + 5] - '0';//秒
				}
				time_adjust_zone(&Gps_info.time, 8);
				Gps_info.time_dec = time_struct2sec(&Gps_info.time);
				setSysTime(&Gps_info.time);
				/* 校对RTC时间 */
				printf_time_msg("GNSS校时");
			}
			
			Status_stu.reg.gps_fix = 1;
			
			/* GPS速度，单位0.1Km/h */
			Gps_info.speed = search_data_para2(7, gps_data, 3) * 1852 / 1000;
			
			if(Gps_info.speed < 20)
			{
				Gps_info.speed = 0;
			}

			/* 方向 */
			gps_temp = search_text_para(8, gps_data);
			if(gps_data[gps_temp] != ',')
			{
				Gps_info.dir = search_data_para2(8, gps_data, 0) % 360;
			}
				
			/* 东/西经，南/北纬 */
			Gps_info.ew = gps_data[search_text_para(6, gps_data)];
			Gps_info.ns = gps_data[search_text_para(4, gps_data)];
			
			/* 经纬度提取 */
			Gps_info.lo_data = search_data_para2(5, gps_data, 4);//小数点后面4位
			gps_temp = Gps_info.lo_data % 1000000 * 100 / 60;	//转换为度
			Gps_info.lo_data /= 1000000; 
			Gps_info.lo_data *= 1000000;
			Gps_info.lo_data += gps_temp;
			Gps_info.la_data = search_data_para2(3, gps_data, 4);//小数点后面4位
			gps_temp = Gps_info.la_data % 1000000 * 100 / 60;	//转换为度
			Gps_info.la_data /= 1000000;
			Gps_info.la_data *= 1000000;
			Gps_info.la_data += gps_temp;
			
			/* 定位状态 */
			Gps_info.status = 'A';
		}
		else
		{
			Gps_info.status = 'V';
			Gps_info.speed = 0;
			Gps_info.dir = 0;
			Gps_info.hight = 0;
			Gps_info.satellites = 0;
			Gps_info.hdop = 0; 
			Gps_info.gps_sate = 0;
			Gps_info.bds_sate = 0;
		}
	}
	
/*	
$GPGGA,092204.999,4250.5589,S,14718.5084,E,1,04,24.4,19.7,M,,,,0000*1F
$GNGGA,070254.000,3639.9087,N,11707.5713,E,1,14,0.8,179.9,M,0.0,M,,*74	
字段0：$GPGGA，语句ID，表明该语句为Global Positioning System Fix Data（GGA）GPS定位信息	 		
字段1：UTC 时间，hhmmss.sss，时分秒格式 														
字段2：纬度ddmm.mmmm，度分格式（前导位数不足则补0） 								   		
字段3：纬度N（北纬）或S（南纬） 													  		
字段4：经度dddmm.mmmm，度分格式（前导位数不足则补0）								  		
字段5：经度E（东经）或W（西经） 													 		
字段6：GPS状态，0=未定位，1=非差分定位，2=差分定位，3=无效PPS，6=正在估算 					
字段7：正在使用的卫星数量（00 - 12）（前导位数不足则补0） 						   		
字段8：HDOP水平精度因子（0.5 - 99.9） 											   		
字段9：海拔高度（-9999.9 - 99999.9） 	  																						  
字段10：地球椭球面相对大地水准面的高度 		 		
字段11：差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空）		
字段12：差分站ID号0000 - 1023（前导位数不足则补0，如果不是差分定位将为空）
*/
	else if((!memcmp("$GPGGA", gps_data, 6))
		 || (!memcmp("$GNGGA", gps_data, 6))
		 || (!memcmp("$BDGGA", gps_data, 6)))
	{
		if(Gps_info.status == 'A')
		{
			Gps_info.satellites = search_data_para(7, gps_data);
			Gps_info.hdop = search_data_para(8, gps_data); 
			Gps_info.hight =  (WORD)search_data_para(9, gps_data);		
		}
	}
		
	/* 可视GPS卫星数 */
	else if(!memcmp("$GPGSV", gps_data, 6))
	{	
		Gps_info.gps_sate = search_data_para(3, gps_data);
	}
	
	/* 可视北斗卫星数 */
	else if(!memcmp("$BDGSV", gps_data, 6))
	{
		Gps_info.bds_sate = search_data_para(3, gps_data);
	}
	
	return(1);	
}




/*******************************************
函数名称:USART2_IRQHandler(void)
函数功能:串口2中断服务例程，GPS信息接收
入口参数:
出口参数: 
备    注:$GPRMC
*******************************************/
void USART2_IRQHandler(void)
{
	BYTE data;
	OS_CPU_SR  cpu_sr; 

	OS_ENTER_CRITICAL();
	OSIntEnter();
	OS_EXIT_CRITICAL();
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		data = USART_ReceiveData(USART2);
		if(GPSRcv_lenth() == 0)
		{
			GPSRcv_rest();
		}
		GPSRcv_push(data);
	}
	OSIntExit(); 
}




/* GPS串口接收入队 */
static void GPSRcv_push(BYTE data)
{
	GPSRcv_buffer[GPSRcv_tail] = data;
	if(++GPSRcv_tail >= GPSRcv_cap)
	{
		GPSRcv_tail = 0;
	}
	if(GPSRcv_tail == GPSRcv_head)
	{
		if(++GPSRcv_head >= GPSRcv_cap)
		{
			GPSRcv_head = 0;
		}
	}
}

/* GPS串口接收出队 */
static BYTE GPSRcv_pop(void)
{
	BYTE data;
	
	data = GPSRcv_buffer[GPSRcv_head];
	GPSRcv_head++;
	if(GPSRcv_head >= GPSRcv_cap)
	{
		GPSRcv_head = 0;
	}
	
	return data;
}


static void GPSRcv_rest(void)
{
	GPSRcv_head = 0;
	GPSRcv_tail = 0;
}


static WORD GPSRcv_lenth(void)
{
	if(GPSRcv_tail >= GPSRcv_head)
	{
		return GPSRcv_tail - GPSRcv_head;
	}
	else
	{
		return GPSRcv_cap - (GPSRcv_head - GPSRcv_tail);
	}
}



