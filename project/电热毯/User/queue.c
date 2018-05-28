#include "queue.h"

Queue Uart_Queue;
Queue Length_Queue;

void Queue_Init(void)
{
	Uart_Queue.front = 0;
	Uart_Queue.rear = 0;
	Length_Queue.front = 0;
	Length_Queue.rear = 0;
}

uint8_t In_Queue(Queue *q,uint8_t d)
{
	if(q->front == (q->rear+1)%QueueSize)
	{
		return ERROR;
	}else{
		q->dat[q->rear] = d;
		q->rear=(q->rear+1) % QueueSize; 
		return OK;
	}
}

uint8_t Out_Queue(Queue *q,uint8_t *d)
{
	if(q->front==q->rear)
	{
		return ERROR;
	}else{
		*d = q->dat[q->front];
		q->front=(q->front+1) % QueueSize; 
		return OK;
	}
}