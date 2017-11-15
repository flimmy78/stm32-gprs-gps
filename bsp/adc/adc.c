/******************************************
文件名称: adc.c
作	  者: byy
版    本:
说    明: 实时时钟配置
修改记录: 2011-08-18,程序建立。
******************************************/
#include "adc.h"
 
		   


/**********************************************************************
* 名    称：Adc_Init()
* 功    能：
* 入口参数：
* 出口参数：
***********************************************************************/
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1 , ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   				// 72M/6=12,ADC最大时间不能超过14M
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			// PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// 模拟输入引脚
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	ADC_DeInit(ADC1);  									//将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;				//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);					//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	ADC_Cmd(ADC1, ENABLE);								//使能指定的ADC1
	ADC_ResetCalibration(ADC1);							//重置指定的ADC1的校准寄存器	 
	while(ADC_GetResetCalibrationStatus(ADC1));			//获取ADC1重置校准寄存器的状态,设置状态则等待
	ADC_StartCalibration(ADC1);							//开始指定ADC1的校准状态
	while(ADC_GetCalibrationStatus(ADC1));				//获取指定ADC1的校准程序,设置状态则等待
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);				//使能指定的ADC1的软件转换启动功能	
}




//获得ADC值
//ch:通道值 0~3
WORD Get_Adc(BYTE ch)
{
  	// 设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );		  			    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);			//使能指定的ADC1的软件转换启动功能		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));	//等待转换结束
	return ADC_GetConversionValue(ADC1);			//返回最近一次ADC1规则组的转换结果
}

