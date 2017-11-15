/******************************************
文件名称: usart.c
作	  者: byy
版    本:
说    明: 支持适应不同频率下的串口波特率设置
修改记录: 2011-08-18,程序建立。
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
/*printf()输出重定向*/
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{   	
	USART_SendData(USART1, (uint8_t) ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
	return ch;
}
#endif 




/* 串口配置 */
void USART_Config(USART_TypeDef* USARTx, DWORD baud)
{
	USART_InitTypeDef USART_InitStructure;
	USART_StructInit(&USART_InitStructure);
	USART_DeInit(USARTx);
	USART_InitStructure.USART_BaudRate = baud;						//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USARTx, &USART_InitStructure);						//配置串口参数函数   
	USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);					//关闭接收中断
	USART_ITConfig(USARTx, USART_IT_TC, DISABLE);    				//关闭发送中断
	
	if (USARTx == USART1)
	{	  
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 		//使能接收中断
		USART_ClearFlag(USART1, USART_FLAG_TC);     		//清发送完成标志
	}
	else if (USARTx == USART2)
	{
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 		//使能接收中断
		USART_ClearFlag(USART2, USART_FLAG_TC);     		//清发送完成标志  
	}
	else if (USARTx == USART3)
	{
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 		//使能接收中断
		USART_ClearFlag(USART3, USART_FLAG_TC);     		//清发送完成标志
	}	 
 
	USART_Cmd(USARTx, ENABLE);
}




/*******************************************
函数名称: void Usart1_Send_Char(BYTE send_char)
函数功能: 串口1发送单个字符
入口参数: send_char:待发送字符
出口参数: 
备    注: 
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
函数名称: void Usart1_Send_Char(BYTE send_char)
函数功能: 串口1发送单个字符
入口参数: send_char:待发送字符
出口参数: 
备    注: 
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
	
	//等待发送缓冲区空
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
	
	//等待发送缓冲区空
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


