/******************************************
文件名称: feed_wdg.c
作	  者: byy
版    本:
说    明: 系统控制任务
修改记录: 2011-08-18,程序建立。
******************************************/
#include "all_use.h"




/* 创建用于串口接收的环形队列 */
const WORD Cap_UartMainRcv_CQ = 200;
BYTE UartMainRcv_buffer[Cap_UartMainRcv_CQ];
CIRCU_QUEUE UartMainRcv_CQ = {UartMainRcv_buffer,Cap_UartMainRcv_CQ,0,0,0,0};

/*创建用于串口接收的环形队列*/
static const WORD OUTRcv_cap = 200;
static BYTE OUTRcv_buffer[OUTRcv_cap + 1];
static WORD OUTRcv_tail = 0;
static WORD OUTRcv_head = 0;
static WORD OUTRcv_lenth;
static void OUTRcv_push(BYTE data);
static BYTE OUTRcv_pop(void);
static void OUTRcv_rest(void);

/* 串口0解析 */
static const WORD TelnetRcvDataCap = 200;
static BYTE TelnetRecDataBuf[TelnetRcvDataCap];
static void telnet_char(BYTE data);


/* 短信报警队列 */
static const BYTE SMS_alarm_q_cap = 20;
static BYTE SMS_alarm_q[SMS_alarm_q_cap + 1];
static WORD SMS_alarm_count = 0;
#define		SMS_JianXian		1
#define		SMS_BaChu			2
#define		SMS_TEST			3
static void sms_alarm_push(BYTE type);
static void led_flash_ctr(void);
static WORD get_battery_vol(void);




/*******************************************
函数名称: void Sys_Ctrl_task(void *p_arg)
函数功能: 系统控制任务
入口参数:
出口参数: 
备    注:
*******************************************/
void Sys_Ctrl_task(void *p_arg)
{
	const WORD step_time = 100;
	const DWORD wait_one_sec = 1000 / step_time;
	static WORD batt_check_up_count = 0;
	static WORD batt_check_down_count = 0;
	static WORD Shear_Alarm_count = 0;
	static WORD Shear_Alarm_ok_count = 0;
	static WORD pull_out_count = 0;
	static WORD pull_out_ok_count = 0;
	BYTE data;
	BYTE sms_alarm_type = 0;
	BYTE sms_alarm_num_dex = 0;
	WORD adc_battery_vol = 0;
	
	p_arg = p_arg;
	Adc_Init();
	print_debug_msg("ADC初始化");
	SysTime_dec = time_struct2sec(&CompileTime);
	time_sec2struct(SysTime_dec, &SysTime);
	Alarm_stu.value = 0;
	Status_stu.value = 0;
	Status_stu.reg.acc_on = 1;
	Status_stu.reg.gps_fix = 0;
	Status_stu.reg.gps_used = 1;
	memset(SMS_alarm_q, 0, 20);
	SMS_alarm_count = 0;
	while(1)
	{  	
		OSTimeDlyHMSM(0, 0, 0, step_time);
		
		/* LED闪烁控制 */
		led_flash_ctr();
		
		/* 单片机看门狗 */
		IWDG_Feed();	//喂狗一次		
		
		/* GSM模块软件看门狗 */
		if(++at.dog > 120 * wait_one_sec)
		{
			print_debug_msg("GSM模块看门狗超时");
			at.dog = 0;
			OSQPostFront(mbox_at_mission, (void*)at_mission_reset);
		}
		
		/* 电池电压检测 */
		adc_battery_vol = get_battery_vol();
		if(!POWER_OK && adc_battery_vol < 3700)
		{
			print_debug_msg("电池电压:%dmV,关机", get_battery_vol());
			OSTimeDlyHMSM(0, 0, 0, 100);
			Battery_control(0);
			OSTimeDlyHMSM(0, 0, 0, 100);
		}
		
		/* 主电源检测 */
		if(POWER_OK && Status_stu.reg.power == 1)
		{
			batt_check_down_count = 0;
			if(++batt_check_up_count >= 5)
			{
				printf_time_msg("外接电源接通");
				Status_stu.reg.power = 0;
				Alarm_stu.reg.pwr_lose = 0;
				Alarm_stu.reg.theft = 0;
				Alarm_stu.reg.danger = 0;
				send_location_info();
			}
		}
		else if(!POWER_OK && Status_stu.reg.power == 0)
		{
			batt_check_up_count = 0;
			if(++batt_check_down_count >= 5)
			{
				printf_time_msg("外部电源关闭");
				Status_stu.reg.power = 1;
				Alarm_stu.reg.pwr_lose = 1;
				send_location_info();			
			}
		}
		
		/* 剪线报警检测 */
		if(!POWER_OK && Shear_Alarm && Alarm_stu.reg.theft == 0)
		{
			Shear_Alarm_ok_count = 0;
			if(++Shear_Alarm_count >= 5)
			{
				printf_time_msg("剪线报警");
				Alarm_stu.reg.theft = 1;
				send_location_info();
				sms_alarm_push(SMS_JianXian);
			}
		}
		else if(!Shear_Alarm && Alarm_stu.reg.theft == 1)
		{
			Shear_Alarm_count = 0;
			if(++Shear_Alarm_ok_count >= 5)
			{
				printf_time_msg("剪线报警恢复");
				Alarm_stu.reg.theft = 0;
				send_location_info();
			}
		}
		
		/* 剪线报警检测 */
		if(!POWER_OK && !PULL_OUT && Alarm_stu.reg.danger == 0)
		{
			pull_out_ok_count = 0;
			if(++pull_out_count >= 5)
			{
				printf_time_msg("拔出报警");
				Alarm_stu.reg.danger = 1;
				send_location_info();
				sms_alarm_push(SMS_BaChu);
			}
		}
		else if(PULL_OUT && Alarm_stu.reg.danger == 1)
		{
			pull_out_count = 0;
			if(++pull_out_ok_count >= 5)
			{
				printf_time_msg("拔出报警恢复");
				Alarm_stu.reg.danger = 0;
				send_location_info();
			}
		}
		
		/* 检索报警短信队列 */
		if(at.msg_tlenth == 0 && SMS_alarm_count > 0)
		{
			sms_alarm_num_dex = SMS_alarm_q[0] >> 4;
			sms_alarm_type = SMS_alarm_q[0] & 0x0F;
			if(sms_alarm_num_dex >= 1 && sms_alarm_num_dex <= 2)
			{
				if(strlen((char*)Device.alarm_sms_phone[sms_alarm_num_dex - 1]) == 11)
				{
					switch(sms_alarm_type)
					{
						case SMS_JianXian:
							send_msg_to_num((char*)Device.alarm_sms_phone[sms_alarm_num_dex - 1], 
											"ID:%s\r\n剪线报警", Device.phone_num);
							break;
						case SMS_BaChu:
							send_msg_to_num((char*)Device.alarm_sms_phone[sms_alarm_num_dex - 1], 
											"ID:%s\r\n拔出报警", Device.phone_num);
							break;
						case SMS_TEST:
							print_debug_msg("ID:%s--测试:%d", Device.phone_num, sms_alarm_num_dex);
							send_msg_to_num((char*)Device.alarm_sms_phone[sms_alarm_num_dex - 1], 
											"ID:%s\r\n测试:%d", Device.phone_num, sms_alarm_num_dex);
							break;
						default:
							break;
					}
				}
			}
			SMS_alarm_count--;
			if(SMS_alarm_count > 0)
			{
				memmove(SMS_alarm_q, SMS_alarm_q + 1, SMS_alarm_count);
			}
		}
			
		/* 字符控制界面 */
		while(OUTRcv_lenth > 0)
		{
			data = OUTRcv_pop();
			telnet_char(data);
		}
	}
}



/* ADC 电池电压 */
static WORD get_battery_vol(void)
{
	static WORD adc_buf[10];
	DWORD adc_bat = 0;
	BYTE i;
	
	adc_bat = 0;
	for(i = 0; i < 9; i++)
	{
		adc_buf[i] = adc_buf[i + 1];
		adc_bat += adc_buf[i];
	}
	adc_buf[9] = Get_Adc(ADC_Channel_9);
	adc_bat += adc_buf[9];
	return (adc_bat * 3300 * 2 / 4096 / 10);
}



/*******************************************
函数名称: static void sms_alarm_push(BYTE type)
函数功能: 短信报警信息入队
入口参数:
出口参数: 
备    注:
*******************************************/
static void sms_alarm_push(BYTE type)
{
	if(SMS_alarm_count + 2 >= SMS_alarm_q_cap)
	{
		SMS_alarm_count -= 2;
		memmove(SMS_alarm_q, SMS_alarm_q + 2, SMS_alarm_count);
	}
	SMS_alarm_q[SMS_alarm_count] = (1 << 4) + (type & 0x0F);
	SMS_alarm_count++;
	SMS_alarm_q[SMS_alarm_count] = (2 << 4) + (type & 0x0F);
	SMS_alarm_count++;
}




/*******************************************
函数名称: void telnet_char(BYTE c)
函数功能: 字符交互界面
入口参数:
出口参数: 
备    注:
*******************************************/
static void telnet_char(BYTE data)
{
	char* recData = (char*)TelnetRecDataBuf;
	static WORD len = 0;
	static BOOL flagRcvDataOver = TRUE;
	static DWORD i;
	static DWORD rcvVal = 0;
	
	print_interface("%c", data);
	if(len == TelnetRcvDataCap - 1 || flagRcvDataOver)
	{
		len = 0;
	}
	flagRcvDataOver = FALSE;
	recData[len] = data;
	++len;
	
	if(recData[len -1] == '\b')
	{
		print_interface(" ");
		if(len > 1)
		{
			print_interface("\b");
			len -= 2;
		}
		else 
		{
			len = 0;
		}
	}
	else if(recData[len -1] == '\t')
	{
		for(i = 0; i < (8 - ((len + 3) % 8)); i++)
		{
			print_interface("\b");
		}
		len -= 1;
	}
	else if(recData[len -1] == 0x7F)
	{
		len -= 1;
	}
	else if(recData[len - 1] == '\n')
	{
		if(len <= 2)
		{
			memset(recData, 0, len);
			len = 0;
            print_interface("\r\n~ $ ");
			return;
		}
		if(recData[len - 2] == '\r')
		{
			recData[len] = '\0';
			flagRcvDataOver = TRUE;
			if(!strcmp(recData, "help\r\n") || !strcmp(recData, "?\r\n"))
			{
				print_interface("help,? : Print the command usage list.\r\n");
				print_interface("ver    : Get the version information.\r\n");
				print_interface("reboot : Reboot the device.\r\n");
				print_interface("ip     : Get Server IP information.\r\n");
				print_interface("setip  : Set Server IP.\r\n");
				print_interface("         eg: setip 192.168.1.100\r\n");
				print_interface("setport: Set Server Port.\r\n");
				print_interface("         eg: setport 5555\r\n");
				print_interface("sim    : Get SIM card number.\r\n");
				print_interface("setsim : Set SIM card number.\r\n");
				print_interface("         eg: setsim 13335197169\r\n");
				print_interface("anum   : Get Alarm phone number.\r\n");
				print_interface("setanum: Set Alarm phone number.\r\n");
				print_interface("         eg: setanum 1,13335197169\r\n");
				print_interface("int    : Get GPS Update interval(s).\r\n");
				print_interface("setint : Set GPS Update interval, >=1 & <= 900.\r\n");
				print_interface("         eg: setint 30\r\n");
			}
			else if(!strcmp(recData, "ver\r\n"))
			{
				print_interface("version: %s build %0.4d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d\r\n",
								Version,
								CompileTime.year, CompileTime.month, CompileTime.day,
								CompileTime.hour, CompileTime.min, CompileTime.sec);
			}
			else if(stringIsSame(recData, "debug"))
			{
				if(recData[5] != ' ' || strlen(recData) != 9
				|| (recData[6] != '0' && recData[6] != '1'))
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				Flag_debug_IO = (recData[6] == '1')? TRUE : FALSE;
				print_interface("%s debug\r\n", (Flag_debug_IO)? "Open" : "Close");
				
			}
			else if(!strcmp(recData, "msg\r\n"))
			{
				print_interface("发送短信");
				send_msg_to_num("13335197169", "ID:%s\r\nPDU短信测试", Device.phone_num);
				//sms_alarm_push(SMS_TEST);
			}
			else if(!strcmp(recData, "csq\r\n"))
			{
				print_interface("GSM single:%d\r\n", at.state_csq);
			}
			else if(!strcmp(recData, "exit\r\n"))
			{
				print_interface("Exit this interface.\r\n");
			}
			else if(!strcmp(recData, "reboot\r\n"))
			{
				print_interface("Rebooting...\r\n\r\n");
				Sys_Soft_Reset();
			}
			else if(stringIsSame(recData, "atrcv"))
			{
				if(recData[5] != ' ' || strlen(recData) != 9
				|| (recData[6] != '0' && recData[6] != '1'))
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				Flag_Printf_at_rcv = (recData[6] == '1')? TRUE : FALSE;
				print_interface("%s AT RCV \r\n", (Flag_Printf_at_rcv)? "Open" : "Close");
			}
			else if(!strcmp(recData, "ip\r\n"))
			{
				print_interface("Server = %s:%d\r\n", 
								IPint2string(Device.server_ip_int, Ip_string_buf), 
								Device.server_port);
			}
			else if(stringIsSame(recData, "setip"))
			{
				if(!(recData[5] == ' '
				  && strlen(recData) - 8 >= 7 && strlen(recData) - 8 <= 15))
				{
					print_interface("Error:Illegal character!\r\n", strlen(recData));
					print_interface("\r\n~ $ ");
					return;
				}
				recData[strlen(recData) - 2] = '\0';
				rcvVal = IPstring2int(recData + 6);
				if(rcvVal == 0)
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				else
				{
					Device.server_ip_int = rcvVal;
					write_device_info_to_flash();
					print_interface("Set Server ip successfully.\r\n");
				}
			}
			else if(stringIsSame(recData, "setport"))
			{
				if(!(recData[7] == ' '
				  && strlen(recData) - 10 >= 1 && strlen(recData) - 10 <= 5))
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				rcvVal = 0;
				for(i = 0; i < strlen(recData) - 10; i++)
				{
					if(recData[i + 8] < '0' || recData[i + 8] > '9')
					{
						break;
					}
					rcvVal *= 10;
					rcvVal += recData[i + 8] - '0';
				}
				if(i < strlen(recData) - 10)
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				if(rcvVal == 0 || rcvVal > 65535)
				{
					print_interface("Error:The port is not legitimate!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				Device.server_port = rcvVal;
				write_device_info_to_flash();
				print_interface("Set server port successfully.\r\n");
			}
			else if(!strcmp(recData, "sim\r\n"))
			{
				print_interface("SIM card number = %s\r\n", Device.phone_num);
			}
			else if(stringIsSame(recData, "setsim"))
			{
				if(recData[6] != ' ' || strlen(recData) - 9 != 11)
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				for(i = 0; i < 11; i++)
				{
					if(recData[i + 7] < '0' || recData[i + 7] > '9')
					{
						break;
					}
				}
				if(i < strlen(recData) - 9)
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				Device.phone_num[0] = '0';
				memcpy((char*)Device.phone_num + 1, &recData[7], 11);
				write_device_info_to_flash();
				print_interface("Set SIM card number successfully.\r\n");
			}
			else if(!strcmp(recData, "anum\r\n"))
			{
				print_interface("Alarm phone number 1 = %s\r\n", Device.alarm_sms_phone[0]);
				print_interface("Alarm phone number 2 = %s\r\n", Device.alarm_sms_phone[1]);
			}
			else if(stringIsSame(recData, "setanum"))
			{
				if(recData[7] != ' ' || (recData[8] != '1' && recData[8] != '2')
				|| recData[9] != ',' || strlen(recData) - 12 != 11)
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				for(i = 0; i < 11; i++)
				{
					if(recData[i + 10] < '0' || recData[i + 10] > '9')
					{
						break;
					}
				}
				if(i < 11)
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				memset(Device.alarm_sms_phone[recData[8] - '1'], 0, 13);
				memcpy((char*)Device.alarm_sms_phone[recData[8] - '1'], &recData[10], 11);
				write_device_info_to_flash();
				print_interface("Set Alarm phone number successfully.\r\n");
			}			
			else if(!strcmp(recData, "int\r\n"))
			{
				print_interface("Updata Interval default = %d\r\n", Device.send_time_inteval_default);
				print_interface("Updata Interval sleep   = %d\r\n", Device.send_time_inteval_sleep);
				print_interface("Updata Interval alarm   = %d\r\n", Device.send_time_inteval_alarm);
			}
			else if(stringIsSame(recData, "setint"))
			{
				if(!(recData[6] == ' '
				  && strlen(recData) - 9 >= 1 && strlen(recData) - 9 <= 3))
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				rcvVal = 0;
				for(i = 0; i < strlen(recData) - 9; i++)
				{
					if(recData[i + 7] < '0' || recData[i + 7] > '9')
					{
						break;
					}
					rcvVal *= 10;
					rcvVal += recData[i + 7] - '0';
				}
				if(i < strlen(recData) - 9)
				{
					print_interface("Error:Illegal character!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				if(rcvVal < 1 || rcvVal > 900)
				{
					print_interface("Error:The interval is not legitimate!\r\n");
					print_interface("\r\n~ $ ");
					return;
				}
				Device.send_time_inteval_default = rcvVal;
				write_device_info_to_flash();
				print_interface("Set interval successfully.\r\n");
			}
			else
			{
				print_interface("This command is undefined!\r\n");
			}
            print_interface("\r\n~ $ ");
		}
	}	
}





/***************************************************
功    能：LED闪烁控制
入口参数：无
返 回 值：无
备    注：
***************************************************/
static void led_flash_ctr(void)
{
	static BYTE conter = 0;

	if(ServerPara.state != SERVER_CONNECT_AUTHENOK)
	{
		if(conter < 19)
		{
			WORK_LED(0);
		}
		else
		{
			WORK_LED(1);
			conter = 0;
		}
	}
	else
	{
		if(conter < 15)
		{
			WORK_LED(0);
		}
		else if(conter < 19)
		{
			WORK_LED(conter % 2);
		}
		else
		{
			WORK_LED(1);
			conter = 0;
		}
	}
	++conter;
}





/*******************************************
函数名称:USART1_IRQHandler(void)
函数功能:串口1中断服务例程，GPS信息接收
入口参数:
出口参数: 
备    注:$GPRMC
*******************************************/
void USART1_IRQHandler(void)
{
	BYTE data;
	OS_CPU_SR  cpu_sr; 

	OS_ENTER_CRITICAL();
	OSIntEnter();
	OS_EXIT_CRITICAL();
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		data = USART_ReceiveData(USART1);
		if(OUTRcv_lenth == 0)
		{
			OUTRcv_rest();
		}
		OUTRcv_push(data);
	}
	OSIntExit(); 
}




/* OUT串口接收入队 */
static void OUTRcv_push(BYTE data)
{
	OUTRcv_buffer[OUTRcv_tail] = data;
	if(++OUTRcv_tail >= OUTRcv_cap)
	{
		OUTRcv_tail = 0;
	}
	if(OUTRcv_lenth < OUTRcv_cap)
	{
		OUTRcv_lenth++;
	}				  
	else
	{
		if(++OUTRcv_head >= OUTRcv_cap)
		{
			OUTRcv_head = 0;
		}
	}
}

/* OUT串口接收出队 */
static BYTE OUTRcv_pop(void)
{
	BYTE data;
	
	data = OUTRcv_buffer[OUTRcv_head];
	if(OUTRcv_head + 1 < OUTRcv_cap)
	{
		OUTRcv_head++;
	}
	else
	{
		OUTRcv_head = 0;
	}
	OUTRcv_lenth--;
	
	return data;
}


static void OUTRcv_rest(void)
{
	OUTRcv_head = 0;
	OUTRcv_tail = 0;
	OUTRcv_lenth = 0;
}



