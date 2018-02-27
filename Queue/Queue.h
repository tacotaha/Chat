#ifndef QUEUE_H
#define QUEUE_H

#include "Status.h"

typedef void* QUEUE;
typedef struct node Node;

struct node{
  void* data;
  Node* next;
  Node* prev;
};

typedef struct queue{
  Node* head;
  Node* tail;
  int size;
}Queue;


QUEUE queue_init(void);
Status queue_enqueue(QUEUE hQueue, void* data);
Status queue_dequeue(QUEUE hQueue);
Boolean queue_empty(QUEUE hQueue);
int queue_size(QUEUE hQueue);
void* queue_front(QUEUE hQueue);
void queue_destroy(QUEUE* phQueue);

#endif
