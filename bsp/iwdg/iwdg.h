//看门狗  驱动代码     
#ifndef __IWDG_H_
#define __IWDG_H_
#include "type.h"

 
void IWDG_Init(BYTE prer, WORD rlr); 
void IWDG_Feed(void); 


#endif
