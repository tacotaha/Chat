#ifndef QUEUE_H
#define QUEUE_H

#include "Status.h"

typedef void* QUEUE;

QUEUE queue_init(void);
Status queue_enqueue(QUEUE hQueue, int data);
Status queue_dequeue(QUEUE hQueue);
Boolean queue_empty(QUEUE hQueue);
int queue_size(QUEUE hQueue);
int queue_front(QUEUE hQueue);
void queue_destroy(QUEUE* phQueue);
void queue_print(QUEUE hQueue);



#endif
