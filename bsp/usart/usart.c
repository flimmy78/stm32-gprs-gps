/******************************************
�ļ�����: usart.c
��	  ��: byy
��    ��:
˵    ��: ֧����Ӧ��ͬƵ���µĴ��ڲ���������
�޸ļ�¼: 2011-08-18,��������
******************************************/
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "usart.h"	  
#include "ucos_ii.h" 
#include "type.h" 
#include "rtc.h" 



BOOL Flag_debug_IO = TRUE;



#if 1
/*printf()����ض���*/
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{   	
	USART_SendData(USART1, (uint8_t) ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
	return ch;
}
#endif 




/* �������� */
void USART_Config(USART_TypeDef* USARTx, DWORD baud)
{
	USART_InitTypeDef USART_InitStructure;
	USART_StructInit(&USART_InitStructure);
	USART_DeInit(USARTx);
	USART_InitStructure.USART_BaudRate = baud;						//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USARTx, &USART_InitStructure);						//���ô��ڲ�������   
	USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);					//�رս����ж�
	USART_ITConfig(USARTx, USART_IT_TC, DISABLE);    				//�رշ����ж�
	
	if (USARTx == USART1)
	{	  
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 		//ʹ�ܽ����ж�
		USART_ClearFlag(USART1, USART_FLAG_TC);     		//�巢����ɱ�־
	}
	else if (USARTx == USART2)
	{
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 		//ʹ�ܽ����ж�
		USART_ClearFlag(USART2, USART_FLAG_TC);     		//�巢����ɱ�־  
	}
	else if (USARTx == USART3)
	{
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 		//ʹ�ܽ����ж�
		USART_ClearFlag(USART3, USART_FLAG_TC);     		//�巢����ɱ�־
	}	 
 
	USART_Cmd(USARTx, ENABLE);
}




/*******************************************
��������: void Usart1_Send_Char(BYTE send_char)
��������: ����1���͵����ַ�
��ڲ���: send_char:�������ַ�
���ڲ���: 
��    ע: 
*******************************************/
void Usart_Send_char(USART_TypeDef* USARTx, BYTE data)
{
	WORD time;
	
	USART_SendData(USARTx, data);
	time = 0;
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET && ++time < 10000)
	{
		USART_GetFlagStatus(USARTx, USART_FLAG_TC);
	}
	USART_ClearFlag(USARTx, USART_FLAG_TC);
}




/*******************************************
��������: void Usart1_Send_Char(BYTE send_char)
��������: ����1���͵����ַ�
��ڲ���: send_char:�������ַ�
���ڲ���: 
��    ע: 
*******************************************/
void Usart_Send_Datas(USART_TypeDef* USARTx, BYTE* data, WORD send_len)
{
	WORD i;
	WORD time;
	
	for(i = 0; i < send_len; i++)
	{
		USART_SendData(USARTx, data[i]);
		time = 0;
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET && ++time < 10000)
		{
			USART_GetFlagStatus(USARTx, USART_FLAG_TC);
		}
		USART_ClearFlag(USARTx, USART_FLAG_TC);
	}
}





/*****************************************************************/
extern OS_EVENT *sem_usart1_send;
const WORD com1_tx_buffer_lenth = 500;
BYTE com1_tx_buffer[com1_tx_buffer_lenth + 1];

WORD print_interface(const char *format,...)
{
	WORD lenth = 0;
	static va_list args;
	BYTE err = 0;
	
	OSSemPend(sem_usart1_send, 1000, &err);
	va_start(args, format);
	lenth = 0;
	lenth += vsprintf((char*)com1_tx_buffer, format, args);
	va_end(args);
	Usart_Send_Datas(USART1, com1_tx_buffer, lenth);
	OSSemPost(sem_usart1_send);	
	return(lenth);
}

WORD print_debug_msg(const char *format,...)
{
	WORD lenth = 0;
	BYTE err;
	va_list args;
	
	if(!Flag_debug_IO)
	{
		return(0);
	}
	
	//�ȴ����ͻ�������
	OSMboxPend(sem_usart1_send, 1000, &err);
	va_start(args, format);
	lenth = 0;
	lenth += vsprintf((char*)com1_tx_buffer, format, args);
	va_end(args);
	memcpy(com1_tx_buffer + lenth, "\r\n", 3);
	lenth += 2;
	Usart_Send_Datas(USART1, com1_tx_buffer, lenth);
	OSMboxPost(sem_usart1_send, (void*)1);
    return(lenth);
}

WORD printf_time_msg(const char *format,...)
{
	WORD lenth = 0;
	BYTE err;
	va_list args;
	
	if(!Flag_debug_IO)
	{
		return(0);
	}
	
	//�ȴ����ͻ�������
	OSMboxPend(sem_usart1_send, 1000, &err);
	lenth = sprintf((char*)com1_tx_buffer, "$$%0.2d%0.2d%0.2d %0.2d:%0.2d:%0.2d, ", 
					SysTime.year % 100, SysTime.month,  SysTime.day,
					SysTime.hour, SysTime.min, SysTime.sec);
	va_start(args, format);
	lenth += vsprintf((char*)com1_tx_buffer + lenth, format, args);
	va_end(args);
	memcpy(com1_tx_buffer + lenth, "\r\n", 3);
	lenth += 2;
	Usart_Send_Datas(USART1, com1_tx_buffer, lenth);
	OSMboxPost(sem_usart1_send, (void*)1);
    return(lenth);
}




const WORD com3_tx_buffer_lenth = 500;
BYTE com3_tx_buffer[com3_tx_buffer_lenth + 1];
WORD send_at_comand(const char *format,...)
{
	BYTE err = 0;
	WORD lenth = 0;
	static va_list args;
	static BOOL flagCreateSEM = FALSE;
	static OS_EVENT *com3_Sem;
	
	if(flagCreateSEM == FALSE)
	{
		com3_Sem = OSSemCreate(1);
		flagCreateSEM = TRUE;
	}
	
	OSSemPend(com3_Sem, 1000, &err);
	va_start(args, format);
	lenth = 0;
	lenth += vsprintf((char*)com3_tx_buffer, format, args);
	va_end(args);
	Usart_Send_Datas(USART3, com3_tx_buffer, lenth);
	OSSemPost(com3_Sem);	
	return (lenth);
}


