#ifndef __TIMER_H_
#define __TIMER_H_
#include "type.h"


#define LCD_PWM_VAL TIM2->CCR3 

extern void Delay_byy_us(DWORD nus);
extern void Timer3_Init(WORD arr, WORD psc);
extern void PWM_Init(WORD arr, WORD psc);
extern void feed_wdg_Init(WORD Nms);

#endif























