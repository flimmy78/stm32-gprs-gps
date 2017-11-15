/******************************************
�ļ�����: timer.c
��	  ��: byy
��    ��:
˵    ��: ͨ�ö�ʱ��2��3
�޸ļ�¼: 2011-08-18,��������
******************************************/
#include "timer.h"
#include "wdg.h" 
#include "ucos_ii.h"

u8 Timer3_out_Flag = 0; 
/*******************************************
��������:void Delay_byy_us(u8 nus)
��������:��ʱnus
��ڲ���:
���ڲ���: 
��    ע:*******************************************/
void Delay_byy_us(u32 nus)
{
	//Timer3_Init(nus,72);
	TIM3->CR1 |= 0x01;
	TIM3 -> ARR = nus;  	
	while(!Timer3_out_Flag); 	 
	TIM3 -> ARR = 0;
	Timer3_out_Flag = 0;
	TIM3->CR1 &= ~(0x01);
}

//��ʱ��3�жϷ������
//2ms�ж�1��
void TIM3_IRQHandler(void)
{              
	OSIntEnter();
	if(TIM3->SR & 0X0001)//����ж�
	{
		Timer3_out_Flag = 1;
	}    
	TIM3-> SR &= ~(1<<0);//����жϱ�־λ
	OSIntExit();      
}

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void Timer3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |= 1<<1;//TIM3ʱ��ʹ��    
 	TIM3->ARR = arr;  //�趨�������Զ���װֵ    
	TIM3->PSC = psc;  //Ԥ��Ƶ��72,�õ�1Mhz�ļ���ʱ��
	//����������Ҫͬʱ���òſ���ʹ���ж�
	TIM3->DIER |= 1<<0;   //��������ж�				
	TIM3->DIER |= 1<<6;   //�������ж�
		  							    
	//TIM3->CR1 |= 0x01;    //ʹ�ܶ�ʱ��3
  	MY_NVIC_Init(1,3,TIM3_IRQn,2);//��ռ1�������ȼ�3����2									 
}

//TIM3 PWM����
//����ԭ��@ALIENTEK
//2010/6/2	 

//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void PWM_Init(u16 arr,u16 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1<<0;       //TIM2ʱ��ʹ��    

	GPIOA->CRH&=0XFFFFFFF0;//PA8���
	GPIOA->CRH|=0X00000004;//��������
	  	
	GPIOA->CRL&=0XFFFFF0FF;//PA2���
	GPIOA->CRL|=0X00000B00;//���ù������ 	  
	GPIOA->ODR|=1<<2;//PA2����	

	TIM2->ARR=arr;//�趨�������Զ���װֵ 
	TIM2->PSC=psc;//Ԥ��Ƶ������Ƶ
	
	TIM2->CCMR2|=7<<4;  //CH3 PWM2ģʽ		 
	TIM2->CCMR2|=1<<3;  //CH3Ԥװ��ʹ��	   

	TIM2->CCER|=1<<8;   //OC2 ���ʹ��
		   

	TIM2->CR1|=0x80;   //ARPEʹ�� 
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2 										  
}  	 
