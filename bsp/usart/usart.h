#ifndef __USART_H_
#define __USART_H_
#include <stm32f10x.h>
#include <stdio.h>	
#include "type.h"	 
	  	

#define EN_USART1_RX //使能串口1接收
#define EN_USART2_RX //使能串口2接收
#define EN_USART3_RX //使能串口3接收



#define Usart_Debug			USART1
#define Usart_GPS			USART2
#define Usart_GSM			USART3




extern void USART_Config(USART_TypeDef* USARTx, DWORD baud);
extern void Usart_Send_char(USART_TypeDef* USARTx, BYTE data);
extern void Usart_Send_Datas(USART_TypeDef* USARTx, BYTE* data, WORD send_len);

extern BOOL Flag_debug_IO;
extern WORD print_interface(const char *format,...);
extern WORD print_debug_msg(const char *format,...);
extern WORD printf_time_msg(const char *format,...);
extern WORD send_at_comand(const char *format,...);

#endif	   
