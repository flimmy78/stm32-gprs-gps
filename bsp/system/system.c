/******************************************
文件名称: system.c
作	  者: byy
版    本:
说    明: 设置向量表偏移地址
修改记录: 2011-08-18,程序建立。
******************************************/
#include <stm32f10x.h>		 
#include "system.h"
#include "type.h"




#define SYS_CLK_72M




//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
static __asm void WFI_SET(void)
{
	WFI;    
}



/* 进入待机模式 */
void Sys_Standby(void)
{
	SCB->SCR|=1<<2;//使能SLEEPDEEP位 (SYS->CTRL)	   
  	RCC->APB1ENR|=1<<28;     //使能电源时钟	    
 	PWR->CSR|=1<<8;          //设置WKUP用于唤醒
	PWR->CR|=1<<2;           //清除Wake-up 标志
	PWR->CR|=1<<1;           //PDDS置位		  
	WFI_SET();				 //执行WFI指令		 
}	  



/*******************************************
函数功能: 软件重启
入口参数:
出口参数: 
备    注: 
*******************************************/
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR = 0X05FA0000 | (DWORD) 0x04;	  
}





/*******************************************
函数功能: 时钟配置
入口参数:
出口参数: 
备    注: 
*******************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;   
  
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
#ifdef	SYS_CLK_72M
		/* PCLK1 = HCLK */
		RCC_PCLK1Config(RCC_HCLK_Div2);
#else 
		RCC_PCLK1Config(RCC_HCLK_Div1);
#endif

#ifdef	SYS_CLK_72M
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		/* Flash 0 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
#endif

#ifdef SYS_CLK_36M
		/* PLLCLK = 8MHz * 9/2 = 36 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);
		/* Flash 0 wait state */
		FLASH_SetLatency(FLASH_Latency_1);
#endif
#ifdef SYS_CLK_24M
		/* PLLCLK = 8MHz * 3/1 = 24 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_3);
		/* Flash 0 wait state */
		FLASH_SetLatency(FLASH_Latency_0);
#endif
#ifdef SYS_CLK_16M
		/* PLLCLK = 8MHz * 2/1 = 16 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_2);
		/* Flash 0 wait state */
		FLASH_SetLatency(FLASH_Latency_0);
#endif
#ifdef SYS_CLK_8M
		/* PLLCLK = 8MHz * 2/2 = 8 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_2);
		/* Flash 0 wait state */
		FLASH_SetLatency(FLASH_Latency_0);
#endif
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}

	/* 时钟开关 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
}




/*******************************************
函数功能: 中断配置
入口参数:
出口参数: 
备    注: 
*******************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
    
	/* 中断向量表位置 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);

	/* 抢占优先级分组 */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
 
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 14;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 13;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART3 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 12;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 16;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}

