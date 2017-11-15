#ifndef __ADC_H_
#define __ADC_H_	
#include "stm32f10x.h"
#include "type.h"



 
extern void Adc_Init(void);
extern WORD Get_Adc(BYTE ch);  




#endif 
