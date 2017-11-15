/******************************************
文件名称: Start.c
作	  者: byy 
版    本:
说    明: 程序入口;
		  初始化系统，建立任务。
版本信息: 2012-02-27，V2.22-2。
******************************************/		 
#include "all_use.h"




/* 系统事件列表 */
OS_EVENT* mbox_at_answer;		// AT命令回应邮箱
OS_EVENT* mbox_at_sended;		// AT命令发送邮箱
OS_EVENT* mbox_at_mission;		// 模块操作步骤邮箱
OS_EVENT* sem_make_data_ok;		// 组装服务器数据
OS_EVENT* sem_usart1_send;		// 串口0发送信号

/*任务堆栈*/
static OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];
static OS_STK Sys_Ctrl_task_stk[Sys_Ctrl_task_STK_SIZE];
static OS_STK Server_Ctrl_task_stk[Server_Ctrl_task_STK_SIZE];
static OS_STK GSM_Operat_task_stk[GSM_Operat_task_STK_SIZE];
static OS_STK GSM_Resolve_task_stk[GSM_Resolve_task_STK_SIZE];
static OS_STK GPS_Resolve_task_stk[GPS_Resolve_task_STK_SIZE];

/* 函数声明 */
static ST_TIMER* getCompileTime(void);
static void systick_init(void);
static void App_TaskStart(void* p_arg);
extern void Sys_Ctrl_task(void *p_arg);
extern void Server_Ctrl_task(void *p_arg);
extern void GSM_Operat_task(void *p_arg);
extern void GSM_Resolve_task(void *p_arg);
extern void GPS_Resolve_task(void *p_arg);

/* 程序版本 */
char Version[] = "V2.01";
ST_TIMER CompileTime;
ST_TIMER SysTime;
DWORD SysTime_dec;
DEVICE_INFO Device;
char Ip_string_buf[20];
void* at_mission_msg[at_mission_msg_lenth];



/*******************************************
函数名称:main(void)
函数功能:设备初始化,创建起始任务,开启任务调度
入口参数:
出口参数: 
备    注:
*******************************************/
int main(void)
{
	INT8U err;
	
	getCompileTime();
	RCC_Configuration();
	NVIC_Configuration();
	OSInit();					//uC/OS-ii初始化
	systick_init();     		/* 启动 SysTick 定时器 */
	#if (OS_TASK_STAT_EN > 0)
		OSStatInit();      		/* Determine CPU capacity. */
	#endif
	err = OSTaskCreate(App_TaskStart, 	//喂狗任务，同时每分钟保存一次当
					   (void *)0,		//前时间到备用存储区,GPS灯闪烁
					   &App_TaskStartStk[APP_TASK_START_STK_SIZE - 1], 
					   APP_TASK_START_PRIO);
	OSTimeSet(0);
	OSStart(); 					//uC/OS-ii启动
	return err;
}




/*******************************************
函数功能: 启动任务
入口参数:
出口参数: 
备    注: 创建信号量、邮箱、任务
          统计任务
*******************************************/
static void App_TaskStart(void* p_arg)
{
	p_arg = p_arg;
	
	OSTimeDlyHMSM(0, 0, 0, 100);
#if (OS_TASK_STAT_EN > 0)
	OSStatInit();						//----统计任务初始化函数
#endif
	RTC_Config();
	GPIO_Configuration();				//端口配置
	USART_Config(Usart_Debug, 115200);	//串口1初始化,debug
	USART_Config(Usart_GPS,   9600);	//串口2初始化,GPS接收
	USART_Config(Usart_GSM,   19200); 	//串口3初始化,GSM通信
	print_interface("    \r\n");
	print_interface("/***************************\r\n");
	print_interface("STM32_GPS_Black_boad\r\n");
	print_interface("Version:%s\r\n", Version);
	print_interface("Compile:%4d-%02d-%02d %02d:%02d:%02d\r\n", 
					 CompileTime.year, CompileTime.month, CompileTime.day,
					 CompileTime.hour, CompileTime.min, CompileTime.sec);
	print_interface("Author:YanLiang\r\n");
	print_interface("chinayanliang@sohu.com\r\n");
	print_interface("***************************/\r\n");
	Battery_control(1);
	
	/* 初始化设备信息 */
	read_device_info_from_flash();
	if(Device.w_falg != 'W')
	{
		Device.w_falg = 'W';
		Device.server_ip_int = IPstring2int("124.133.2.36");
		Device.server_port = 3999;
		memcpy((char*)Device.phone_num, "015098852216\0", 13);
		Device.regist_code[0] = 0;
		Device.send_time_inteval_default = 60;
		Device.send_time_inteval_sleep = 300;
		Device.send_time_inteval_alarm = 30;
		Device.heartbeat_time_inteval = 60;
		memset(Device.alarm_sms_phone[0], 0, 13);
		memset(Device.alarm_sms_phone[1], 0, 13);
		print_debug_msg("设备参数写入Flash");
		write_device_info_to_flash();
	}
	
	IWDG_Init(5,1250);    							//初始化看门狗,溢出时间约为4s
	mbox_at_answer  = OSMboxCreate((void *) 1);		//建立at指令应答信号量
	mbox_at_sended  = OSMboxCreate((void *) 1);		//建立at指令发送信号量
	mbox_at_mission =  OSQCreate(&at_mission_msg[0], at_mission_msg_lenth);
	sem_make_data_ok = OSSemCreate(1);
	sem_usart1_send = OSSemCreate(1);				//串口0发送信号
	OSQFlush(mbox_at_mission);	
	
	/* 系统控制任务 */
	OSTaskCreate(Sys_Ctrl_task, 	
				 (void *)0,
				 &Sys_Ctrl_task_stk[Sys_Ctrl_task_STK_SIZE - 1], 
				 Sys_Ctrl_task_PRIO);	
				 
	/* GSM操作任务 */					   
	OSTaskCreate(GSM_Operat_task,
				 (void *)0,		
				 &GSM_Operat_task_stk[GSM_Operat_task_STK_SIZE - 1], 
				 GSM_Operat_task_PRIO);
					   
	/* GSM解析任务 */	   
	OSTaskCreate(GSM_Resolve_task,
			     (void *)0,		
			     &GSM_Resolve_task_stk[GSM_Resolve_task_STK_SIZE - 1], 
			     GSM_Resolve_task_PRIO);
				 
	/* GPS解析任务 */					   
	OSTaskCreate(GPS_Resolve_task,
			     (void *)0,		
			     &GPS_Resolve_task_stk[GPS_Resolve_task_STK_SIZE - 1], 
			     GPS_Resolve_task_PRIO);
				 
	/* 服务器控制任务 */
	OSTaskCreate(Server_Ctrl_task,
			     (void *)0,		
			     &Server_Ctrl_task_stk[Server_Ctrl_task_STK_SIZE - 1], 
			     Server_Ctrl_task_PRIO);
	
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}





/*******************************************
函数名称:systick_init(void)
函数功能:用来初始化并启动 SysTick 定时器 
入口参数:
出口参数: 
备    注:
*******************************************/
void systick_init(void)
{
	RCC_ClocksTypeDef rcc_clocks;

	RCC_GetClocksFreq(&rcc_clocks);
	SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);
}




/**************************************
Function : 获取编译时间
Input    : 无
Output   : 结构体时间指针
Note     : 
**************************************/
static ST_TIMER* getCompileTime(void)
{
    const char* szEnMonth[12]=
    {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	int i;
	char* pt;

	pt = __DATE__;
	for(i = 0; i < 12; i++)
    {
		if(!strncmp(pt, szEnMonth[i], 3))
        {
			CompileTime.month *= 10;
            CompileTime.month = i + 1;
            break;
        }
    }
	pt += 4;
	while(!isdigit(*pt))
	{
		++pt;
	}
	CompileTime.day = 0;
	while(isdigit(*pt))
	{
		CompileTime.day *= 10;
		CompileTime.day += *pt - '0';
		++pt;
	}
	while(!isdigit(*pt))
	{
		++pt;
	}
	CompileTime.year = 0;
	while(isdigit(*pt))
	{
		CompileTime.year *= 10;
		CompileTime.year += *pt - '0';
		++pt;
	}
	
	pt = __TIME__;
	CompileTime.hour = 0;
	while(isdigit(*pt))
	{
		CompileTime.hour *= 10;
		CompileTime.hour += *pt - '0';
		++pt;
	}
	++pt;
	CompileTime.min = 0;
	while(isdigit(*pt))
	{
		CompileTime.min *= 10;
		CompileTime.min += *pt - '0';
		++pt;
	}
	++pt;
	CompileTime.sec = 0;
	while(isdigit(*pt))
	{
		CompileTime.sec *= 10;
		CompileTime.sec += *pt - '0';
		++pt;
	}
    return &CompileTime;
}
