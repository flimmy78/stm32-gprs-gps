/******************************************
�ļ�����: system.c
��	  ��: byy
��    ��:
˵    ��: ����������ƫ�Ƶ�ַ
�޸ļ�¼: 2011-08-18,��������
******************************************/
#include <stm32f10x.h>		 
#include "system.h"
#include "type.h"




#define SYS_CLK_72M




//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI
static __asm void WFI_SET(void)
{
	WFI;    
}



/* �������ģʽ */
void Sys_Standby(void)
{
	SCB->SCR|=1<<2;//ʹ��SLEEPDEEPλ (SYS->CTRL)	   
  	RCC->APB1ENR|=1<<28;     //ʹ�ܵ�Դʱ��	    
 	PWR->CSR|=1<<8;          //����WKUP���ڻ���
	PWR->CR|=1<<2;           //���Wake-up ��־
	PWR->CR|=1<<1;           //PDDS��λ		  
	WFI_SET();				 //ִ��WFIָ��		 
}	  



/*******************************************
��������: �������
��ڲ���:
���ڲ���: 
��    ע: 
*******************************************/
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR = 0X05FA0000 | (DWORD) 0x04;	  
}





/*******************************************
��������: ʱ������
��ڲ���:
���ڲ���: 
��    ע: 
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

	/* ʱ�ӿ��� */
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
��������: �ж�����
��ڲ���:
���ڲ���: 
��    ע: 
*******************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
    
	/* �ж�������λ�� */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);

	/* ��ռ���ȼ����� */  
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

