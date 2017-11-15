/******************************************
�ļ�����: gps_rsv.c
��	  ��: byy
��    ��:
˵    ��: ϵͳ��������
�޸ļ�¼: 2011-08-18,��������
******************************************/
#include "all_use.h"




/*�������ڴ��ڽ��յĻ��ζ���*/
const WORD GPSRcv_cap = 200;
BYTE GPSRcv_buffer[GPSRcv_cap + 1];
WORD GPSRcv_tail = 0;
WORD GPSRcv_head = 0;
static void GPSRcv_push(BYTE data);
static BYTE GPSRcv_pop(void);
static void GPSRcv_rest(void);
static WORD GPSRcv_lenth(void);

/* GPS���� */
Gps_info_ST Gps_info;
static const WORD GpsRsvDataCap = 100;
static BYTE GpsRsvDataBuf[GpsRsvDataCap + 1];
static BYTE gps_data_program(BYTE *gps_data);





/*******************************************
��������: void Sys_Ctrl_task(void *p_arg)
��������: ϵͳ��������
��ڲ���:
���ڲ���: 
��    ע:
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
		
		/* ������ */
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
��������: ����У����
��ڲ���:
���ڲ���: 
��    ע: 
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
��������: ��λ���ݽ���
��ڲ���:
���ڲ���: 
��    ע: 	
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
		//print_debug_msg("GPS����У�����");
		//printf("%s", gps_data);
		return 0;
	}

/*
GPRMC,025327.00,A,3639.97654,N,11707.58554,E,0.014,,140912,,,A*74
$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
<1> UTCʱ�䣬hhmmss��ʱ���룩��ʽ
<2> ��λ״̬��A=��Ч��λ��V=��Ч��λ
<3> γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<4> γ�Ȱ���N�������򣩻�S���ϰ���
<5> ����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<6> ���Ȱ���E����������W��������
<7> �������ʣ�000.0~999.9�ڣ�ǰ���0Ҳ�������䣩
<8> ���溽��000.0~359.9�ȣ����汱Ϊ�ο���׼��ǰ���0Ҳ�������䣩
<9> UTC���ڣ�ddmmyy�������꣩��ʽ
<10> ��ƫ�ǣ�000.0~180.0�ȣ�ǰ���0Ҳ�������䣩
<11> ��ƫ�Ƿ���E��������W������
<12> ģʽָʾ����NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч��
*/
	if((!memcmp("$GPRMC", gps_data, 6)) 
	|| (!memcmp("$GNRMC", gps_data, 6)) 
	|| (!memcmp("$BDRMC", gps_data, 6)))
	{		
		/* ��λ��ʶ */
		gps_temp = search_text_para(2, gps_data);
		
		if(gps_data[gps_temp] == 'A')
		{
			/* �״ζ�λУʱ������Уʱ */
			if(Gps_info.status != 'A')
			{
				/* ����GPSʱ�� */
				gps_temp = search_text_para(9, gps_data);
				if(isdigit(gps_data[gps_temp]))
				{
					Gps_info.time.day   = (gps_data[gps_temp + 0] - '0') * 10 + gps_data[gps_temp + 1] - '0';//��
					Gps_info.time.month = (gps_data[gps_temp + 2] - '0') * 10 + gps_data[gps_temp + 3] - '0';//��
					Gps_info.time.year  = (gps_data[gps_temp + 4] - '0') * 10 + gps_data[gps_temp + 5] - '0';//��
				}
				gps_temp = search_text_para(1, gps_data);
				if(isdigit(gps_data[gps_temp]))
				{
					Gps_info.time.hour = (gps_data[gps_temp + 0] - '0') * 10 + gps_data[gps_temp + 1] - '0';//ʱ
					Gps_info.time.min  = (gps_data[gps_temp + 2] - '0') * 10 + gps_data[gps_temp + 3] - '0';//��
					Gps_info.time.sec  = (gps_data[gps_temp + 4] - '0') * 10 + gps_data[gps_temp + 5] - '0';//��
				}
				time_adjust_zone(&Gps_info.time, 8);
				Gps_info.time_dec = time_struct2sec(&Gps_info.time);
				setSysTime(&Gps_info.time);
				/* У��RTCʱ�� */
				printf_time_msg("GNSSУʱ");
			}
			
			Status_stu.reg.gps_fix = 1;
			
			/* GPS�ٶȣ���λ0.1Km/h */
			Gps_info.speed = search_data_para2(7, gps_data, 3) * 1852 / 1000;
			
			if(Gps_info.speed < 20)
			{
				Gps_info.speed = 0;
			}

			/* ���� */
			gps_temp = search_text_para(8, gps_data);
			if(gps_data[gps_temp] != ',')
			{
				Gps_info.dir = search_data_para2(8, gps_data, 0) % 360;
			}
				
			/* ��/��������/��γ */
			Gps_info.ew = gps_data[search_text_para(6, gps_data)];
			Gps_info.ns = gps_data[search_text_para(4, gps_data)];
			
			/* ��γ����ȡ */
			Gps_info.lo_data = search_data_para2(5, gps_data, 4);//С�������4λ
			gps_temp = Gps_info.lo_data % 1000000 * 100 / 60;	//ת��Ϊ��
			Gps_info.lo_data /= 1000000; 
			Gps_info.lo_data *= 1000000;
			Gps_info.lo_data += gps_temp;
			Gps_info.la_data = search_data_para2(3, gps_data, 4);//С�������4λ
			gps_temp = Gps_info.la_data % 1000000 * 100 / 60;	//ת��Ϊ��
			Gps_info.la_data /= 1000000;
			Gps_info.la_data *= 1000000;
			Gps_info.la_data += gps_temp;
			
			/* ��λ״̬ */
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
�ֶ�0��$GPGGA�����ID�����������ΪGlobal Positioning System Fix Data��GGA��GPS��λ��Ϣ	 		
�ֶ�1��UTC ʱ�䣬hhmmss.sss��ʱ�����ʽ 														
�ֶ�2��γ��ddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0�� 								   		
�ֶ�3��γ��N����γ����S����γ�� 													  		
�ֶ�4������dddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��								  		
�ֶ�5������E����������W�������� 													 		
�ֶ�6��GPS״̬��0=δ��λ��1=�ǲ�ֶ�λ��2=��ֶ�λ��3=��ЧPPS��6=���ڹ��� 					
�ֶ�7������ʹ�õ�����������00 - 12����ǰ��λ��������0�� 						   		
�ֶ�8��HDOPˮƽ�������ӣ�0.5 - 99.9�� 											   		
�ֶ�9�����θ߶ȣ�-9999.9 - 99999.9�� 	  																						  
�ֶ�10��������������Դ��ˮ׼��ĸ߶� 		 		
�ֶ�11�����ʱ�䣨�����һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ��Ϊ�գ�		
�ֶ�12�����վID��0000 - 1023��ǰ��λ��������0��������ǲ�ֶ�λ��Ϊ�գ�
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
		
	/* ����GPS������ */
	else if(!memcmp("$GPGSV", gps_data, 6))
	{	
		Gps_info.gps_sate = search_data_para(3, gps_data);
	}
	
	/* ���ӱ��������� */
	else if(!memcmp("$BDGSV", gps_data, 6))
	{
		Gps_info.bds_sate = search_data_para(3, gps_data);
	}
	
	return(1);	
}




/*******************************************
��������:USART2_IRQHandler(void)
��������:����2�жϷ������̣�GPS��Ϣ����
��ڲ���:
���ڲ���: 
��    ע:$GPRMC
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




/* GPS���ڽ������ */
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

/* GPS���ڽ��ճ��� */
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



