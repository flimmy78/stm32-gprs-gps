/******************************************
文件名称: timer.c
作	  者: byy
版    本:
说    明: 通用定时器2、3
修改记录: 2011-08-18,程序建立。
******************************************/
#include "timer.h"
#include "wdg.h" 
#include "ucos_ii.h"

u8 Timer3_out_Flag = 0; 
/*******************************************
函数名称:void Delay_byy_us(u8 nus)
函数功能:延时nus
入口参数:
出口参数: 
备    注:*******************************************/
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

//定时器3中断服务程序
//2ms中断1次
void TIM3_IRQHandler(void)
{              
	OSIntEnter();
	if(TIM3->SR & 0X0001)//溢出中断
	{
		Timer3_out_Flag = 1;
	}    
	TIM3-> SR &= ~(1<<0);//清除中断标志位
	OSIntExit();      
}

//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void Timer3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |= 1<<1;//TIM3时钟使能    
 	TIM3->ARR = arr;  //设定计数器自动重装值    
	TIM3->PSC = psc;  //预分频器72,得到1Mhz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM3->DIER |= 1<<0;   //允许更新中断				
	TIM3->DIER |= 1<<6;   //允许触发中断
		  							    
	//TIM3->CR1 |= 0x01;    //使能定时器3
  	MY_NVIC_Init(1,3,TIM3_IRQn,2);//抢占1，子优先级3，组2									 
}

//TIM3 PWM部分
//正点原子@ALIENTEK
//2010/6/2	 

//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<0;       //TIM2时钟使能    

	GPIOA->CRH&=0XFFFFFFF0;//PA8输出
	GPIOA->CRH|=0X00000004;//浮空输入
	  	
	GPIOA->CRL&=0XFFFFF0FF;//PA2输出
	GPIOA->CRL|=0X00000B00;//复用功能输出 	  
	GPIOA->ODR|=1<<2;//PA2上拉	

	TIM2->ARR=arr;//设定计数器自动重装值 
	TIM2->PSC=psc;//预分频器不分频
	
	TIM2->CCMR2|=7<<4;  //CH3 PWM2模式		 
	TIM2->CCMR2|=1<<3;  //CH3预装载使能	   

	TIM2->CCER|=1<<8;   //OC2 输出使能
		   

	TIM2->CR1|=0x80;   //ARPE使能 
	TIM2->CR1|=0x01;    //使能定时器2 										  
}  	 
