/*******************************************
Filename: circularQueue.c
Author  : BYY
Data    : 2012-09-03
VERsion : V1.1
Note    ��ѭ������ 
*******************************************/
#include "circularQueue.h"




/**************************************
Function : ���/��λ����
Input    : 
Output   : 
Note     : 
**************************************/
void CQ_reset(CIRCU_QUEUE* queue)
{
	queue->tail = 0;
	queue->head = 0;
	queue->length = 0;
	queue->flagBusy = 0;		
}




/**************************************
Function : ��Ӻ�����
Input    : ����ַ�
Output   : 
Note     : 
**************************************/
void CQ_push(CIRCU_QUEUE* queue, unsigned char data)
{
   	queue->vbuf[queue->tail] = data;
	//queue->vbuf[0] = data;
	
	if(++(queue->tail) >= queue->bufCap)
	{
		queue->tail = 0;
	}
	if(queue->length < queue->bufCap)
	{
		queue->length++;		
	}				  
	else
	{
	   	queue->head++;	
	}
}





/**************************************
Function : ��Ӻ�����
Input    : �������ָ�룬Ҫ����ӳ���
Output   : ʵ����ӳ���
Note     : 
**************************************/
void CQ_pushs(CIRCU_QUEUE* queue, unsigned char* inbuf, int nLen)
{
	if(nLen <= queue->bufCap - queue->length)//ʣ��ռ��㹻
    {
        if(queue->tail + nLen < queue->bufCap)
        {
            memcpy(queue->vbuf + queue->tail, inbuf, nLen);
            queue->tail += nLen;
        }
        else
        {
            memcpy(queue->vbuf + queue->tail, inbuf, queue->bufCap - queue->tail);
            memcpy(queue->vbuf, inbuf + (queue->bufCap - queue->tail), nLen - (queue->bufCap - queue->tail));
            queue->tail = nLen - (queue->bufCap - queue->tail);
        }
        queue->length += nLen;
    }
    else //ʣ��ռ䲻��
	{
		char full;
        queue->flagBusy = 1;
        full = (nLen >= queue->bufCap - queue->length)? 1 : 0;
        if(queue->tail + nLen < queue->bufCap)
        {
            memcpy(queue->vbuf + queue->tail, inbuf, nLen);
            queue->tail += nLen;
        }
        else
        {
            memcpy(queue->vbuf + queue->tail, inbuf, queue->bufCap - queue->tail);
            memcpy(queue->vbuf, inbuf + (queue->bufCap - queue->tail), nLen - (queue->bufCap - queue->tail));
            queue->tail = nLen - (queue->bufCap - queue->tail);
        }
        if(full)
        {
            queue->head = queue->tail;
            queue->length = queue->bufCap;
        }
        queue->flagBusy = 0;	
	}
}




/**************************************
Function : ���Ӻ�����
Input    : ����
Output   : ��������
Note     : 
**************************************/
unsigned char CQ_pop(CIRCU_QUEUE* queue)
{
   	char data;
    if(queue->length == 0)
    {
        return 0;
    }
	while(queue->flagBusy)
	{
		#ifdef __USED_UCOSII
		OSTimeDly(1);
		#endif
    }
    queue->flagBusy = 1;
	data = queue->vbuf[queue->head];
	if(queue->head + 1 < queue->bufCap)
	{
        queue->head++;
    }
    else
    {
		queue->head = 0;
	}
    queue->length--;
    queue->flagBusy = 0;
    return data;
}




/**************************************
Function : ȡ���е�һ������
Input    : ����
Output   : ��һ������
Note     : 
**************************************/
unsigned char CQ_firstData(CIRCU_QUEUE* queue)
{
	return(queue->vbuf[queue->head]);
}




/**************************************
Function : ���Ӻ�����
Input    : ���ӻ����ַ��Ҫ����ӳ���
Output   : ʵ�ʳ��ӳ���
Note     : 
**************************************/
int CQ_pops(CIRCU_QUEUE* queue, unsigned char* outbuf, int nMaxLen)
{
    int popedLen;
    int tailx = queue->tail;
    if(queue->length == 0)
    {
        return 0;
    }
	while(queue->flagBusy)
	{
		#ifdef __USED_UCOSII
		OSTimeDly(1);
		#endif
	}    
	queue->flagBusy = 1;
    popedLen = (nMaxLen < queue->length)? nMaxLen : queue->length;
    if(queue->head < tailx)
    {
        memcpy(outbuf, queue->vbuf + queue->head, popedLen);
        queue->head += popedLen;
    }
    else
    {
        if(queue->head + popedLen < queue->bufCap)
        {
            memcpy(outbuf, queue->vbuf + queue->head, popedLen);
            queue->head += popedLen;
        }
        else
        {
            memcpy(outbuf, queue->vbuf + queue->head, queue->bufCap - queue->head);
            memcpy(outbuf + (queue->bufCap - queue->head), queue->vbuf, popedLen - (queue->bufCap - queue->head));
            queue->head = popedLen - (queue->bufCap - queue->head);
        }
    }
    queue->length -= popedLen;
    queue->flagBusy = 0;
    return popedLen;
}




