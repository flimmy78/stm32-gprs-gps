/******************************************
�ļ�����: Start.c
��	  ��: byy 
��    ��:
˵    ��: �������;
		  ��ʼ��ϵͳ����������
�汾��Ϣ: 2012-02-27��V2.22-2��
******************************************/		 
#include "all_use.h"




/* ϵͳ�¼��б� */
OS_EVENT* mbox_at_answer;		// AT�����Ӧ����
OS_EVENT* mbox_at_sended;		// AT���������
OS_EVENT* mbox_at_mission;		// ģ�������������
OS_EVENT* sem_make_data_ok;		// ��װ����������
OS_EVENT* sem_usart1_send;		// ����0�����ź�

/*�����ջ*/
static OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];
static OS_STK Sys_Ctrl_task_stk[Sys_Ctrl_task_STK_SIZE];
static OS_STK Server_Ctrl_task_stk[Server_Ctrl_task_STK_SIZE];
static OS_STK GSM_Operat_task_stk[GSM_Operat_task_STK_SIZE];
static OS_STK GSM_Resolve_task_stk[GSM_Resolve_task_STK_SIZE];
static OS_STK GPS_Resolve_task_stk[GPS_Resolve_task_STK_SIZE];

/* �������� */
static ST_TIMER* getCompileTime(void);
static void systick_init(void);
static void App_TaskStart(void* p_arg);
extern void Sys_Ctrl_task(void *p_arg);
extern void Server_Ctrl_task(void *p_arg);
extern void GSM_Operat_task(void *p_arg);
extern void GSM_Resolve_task(void *p_arg);
extern void GPS_Resolve_task(void *p_arg);

/* ����汾 */
char Version[] = "V2.01";
ST_TIMER CompileTime;
ST_TIMER SysTime;
DWORD SysTime_dec;
DEVICE_INFO Device;
char Ip_string_buf[20];
void* at_mission_msg[at_mission_msg_lenth];



/*******************************************
��������:main(void)
��������:�豸��ʼ��,������ʼ����,�����������
��ڲ���:
���ڲ���: 
��    ע:
*******************************************/
int main(void)
{
	INT8U err;
	
	getCompileTime();
	RCC_Configuration();
	NVIC_Configuration();
	OSInit();					//uC/OS-ii��ʼ��
	systick_init();     		/* ���� SysTick ��ʱ�� */
	#if (OS_TASK_STAT_EN > 0)
		OSStatInit();      		/* Determine CPU capacity. */
	#endif
	err = OSTaskCreate(App_TaskStart, 	//ι������ͬʱÿ���ӱ���һ�ε�
					   (void *)0,		//ǰʱ�䵽���ô洢��,GPS����˸
					   &App_TaskStartStk[APP_TASK_START_STK_SIZE - 1], 
					   APP_TASK_START_PRIO);
	OSTimeSet(0);
	OSStart(); 					//uC/OS-ii����
	return err;
}




/*******************************************
��������: ��������
��ڲ���:
���ڲ���: 
��    ע: �����ź��������䡢����
          ͳ������
*******************************************/
static void App_TaskStart(void* p_arg)
{
	p_arg = p_arg;
	
	OSTimeDlyHMSM(0, 0, 0, 100);
#if (OS_TASK_STAT_EN > 0)
	OSStatInit();						//----ͳ�������ʼ������
#endif
	RTC_Config();
	GPIO_Configuration();				//�˿�����
	USART_Config(Usart_Debug, 115200);	//����1��ʼ��,debug
	USART_Config(Usart_GPS,   9600);	//����2��ʼ��,GPS����
	USART_Config(Usart_GSM,   19200); 	//����3��ʼ��,GSMͨ��
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
	
	/* ��ʼ���豸��Ϣ */
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
		print_debug_msg("�豸����д��Flash");
		write_device_info_to_flash();
	}
	
	IWDG_Init(5,1250);    							//��ʼ�����Ź�,���ʱ��ԼΪ4s
	mbox_at_answer  = OSMboxCreate((void *) 1);		//����atָ��Ӧ���ź���
	mbox_at_sended  = OSMboxCreate((void *) 1);		//����atָ����ź���
	mbox_at_mission =  OSQCreate(&at_mission_msg[0], at_mission_msg_lenth);
	sem_make_data_ok = OSSemCreate(1);
	sem_usart1_send = OSSemCreate(1);				//����0�����ź�
	OSQFlush(mbox_at_mission);	
	
	/* ϵͳ�������� */
	OSTaskCreate(Sys_Ctrl_task, 	
				 (void *)0,
				 &Sys_Ctrl_task_stk[Sys_Ctrl_task_STK_SIZE - 1], 
				 Sys_Ctrl_task_PRIO);	
				 
	/* GSM�������� */					   
	OSTaskCreate(GSM_Operat_task,
				 (void *)0,		
				 &GSM_Operat_task_stk[GSM_Operat_task_STK_SIZE - 1], 
				 GSM_Operat_task_PRIO);
					   
	/* GSM�������� */	   
	OSTaskCreate(GSM_Resolve_task,
			     (void *)0,		
			     &GSM_Resolve_task_stk[GSM_Resolve_task_STK_SIZE - 1], 
			     GSM_Resolve_task_PRIO);
				 
	/* GPS�������� */					   
	OSTaskCreate(GPS_Resolve_task,
			     (void *)0,		
			     &GPS_Resolve_task_stk[GPS_Resolve_task_STK_SIZE - 1], 
			     GPS_Resolve_task_PRIO);
				 
	/* �������������� */
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
��������:systick_init(void)
��������:������ʼ�������� SysTick ��ʱ�� 
��ڲ���:
���ڲ���: 
��    ע:
*******************************************/
void systick_init(void)
{
	RCC_ClocksTypeDef rcc_clocks;

	RCC_GetClocksFreq(&rcc_clocks);
	SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);
}




/**************************************
Function : ��ȡ����ʱ��
Input    : ��
Output   : �ṹ��ʱ��ָ��
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
