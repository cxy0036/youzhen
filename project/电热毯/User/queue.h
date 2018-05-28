#ifndef __QUEUE_H
#define __QUEUE_H

#include "SN8F5702.h"

#define QueueSize 				15

#define ERROR							1
#define OK								0

typedef struct FIFO_Queue
{
    uint8_t front;     
    uint8_t rear;        
    uint8_t count;       
    uint8_t dat[QueueSize];
}Queue;

uint8_t In_Queue(Queue *q,uint8_t d);
uint8_t Out_Queue(Queue *q,uint8_t *d);
void Queue_Init(void);

#endif
