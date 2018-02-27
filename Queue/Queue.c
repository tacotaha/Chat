#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

#define INT_MIN -2147483648

//Known types...
typedef struct node Node;

struct node{
  int data;
  Node* next;
  Node* prev;
};

typedef struct queue{
  Node* head;
  Node* tail;
  int size;
}Queue;

QUEUE queue_init(void){
  Queue *queue = (Queue*) malloc(sizeof(Queue));

  if(queue != NULL){
    queue->head = NULL; 
    queue->tail = NULL;
    queue->size = 0;
  }

  return (QUEUE) queue;
}

Status queue_enqueue(QUEUE hQueue, int data){

  Queue *queue = (Queue*) hQueue;
  if(queue == NULL) return FAILURE;
  
  Node* new_node = (Node*) malloc(sizeof(Node));
  if(new_node == NULL) return FAILURE;

  new_node->data = data;
  
  if(queue->head == NULL){
    queue->head = new_node;
    queue->head->next = NULL;
    queue->head->prev = NULL;
    queue->tail = new_node;
    queue->size++;
    return SUCCESS;
  }

  queue->head->prev = new_node;
  new_node->next = queue->head;
  new_node->prev = NULL;
  queue->head = new_node;
  queue->size++;
  
  return SUCCESS;
}

Status queue_dequeue(QUEUE hQueue){
  Queue* queue = (Queue*) hQueue;


  //Nonexistent or empty queue...
  if(queue == NULL || queue->tail == NULL)
    return FAILURE;

  //only one node in the list...
  if(queue->head->next == NULL){
    free(queue->head);
    queue->head = NULL;
  }else{
    Node* temp = queue->tail->prev;
    free(queue->tail);
    temp->next = NULL; 
    queue->tail = temp;
  }
  
  return SUCCESS;
}


Boolean queue_empty(QUEUE hQueue){
  Queue* queue = (Queue*) hQueue; 
  return (Boolean) (queue == NULL || queue->head == NULL);
}

int queue_size(QUEUE hQueue){
  Queue* queue = (Queue*) hQueue;

  if(queue == NULL) return INT_MIN;
  
  return queue->size;
}

int queue_front(QUEUE hQueue){
  Queue* queue = (Queue*) hQueue;

  if(queue != NULL && queue->head != NULL)
    return queue->tail->data;

  return INT_MIN; //Our way of reuturning failure
}

void queue_print(QUEUE hQueue){
  Queue* queue = (Queue*) hQueue;

  if(queue == NULL || queue->head == NULL)
    return;
 
  Node *temp = queue->head;
  
  while(temp != NULL){
    printf("%d\n", temp->data);
    temp = temp->next;
  }
}
    
void queue_destroy(QUEUE* phQueue){
  Queue* queue = (Queue*) *phQueue;

  if(queue == NULL) return;
  
  while(queue->head != NULL){
    Node* temp = queue->head;
    queue->head = temp->next;
    free(temp);
  }

  free(*phQueue); *phQueue = NULL;
}
