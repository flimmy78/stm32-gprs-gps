/*******************************************
Filename: circularQueue.h
Author  : BYY
Data    : 2012-09-03
VERsion : V1.1
Note    ：循环队列 
*******************************************/

#ifndef __CIRCULAR_QUEUE_H_ 
	#define __CIRCULAR_QUEUE_H_
#include <string.h>


#define __USED_UCOSII

#ifdef __USED_UCOSII
#include "ucos_ii.h"
#endif



//环形队列结构体
typedef struct
{
	unsigned char* vbuf;
	int bufCap;
	int tail;
	int head;
	int length;
	char flagBusy;
}CIRCU_QUEUE;

extern void CQ_reset(CIRCU_QUEUE* queue);
extern void CQ_push(CIRCU_QUEUE* queue, unsigned char data);
extern unsigned char CQ_pop(CIRCU_QUEUE* queue);
extern void CQ_pushs(CIRCU_QUEUE* queue, unsigned char* inbuf, int nLen);
extern int  CQ_pops(CIRCU_QUEUE* queue, unsigned char* outbuf, int nMaxLen);

extern unsigned char CQ_firstData(CIRCU_QUEUE* queue);

#endif
