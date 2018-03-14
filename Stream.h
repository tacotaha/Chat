#ifndef __STREAM_H__
#define __STREAM_H__

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "Queue/Queue.h"

#define BUF 1024
#define MAX_CLIENTS 100

typedef struct client{
  QUEUE* messages;
  pthread_mutex_t* mutex;
  int sender;
  int* clients;
}Client;

typedef struct message{
  char* message;
  int sender;
}Message;

typedef struct chat{
  fd_set listeners;
  int socket;
  int clients[MAX_CLIENTS];
  pthread_mutex_t *clients_m;
}Chat;

/*Spawn thread to output the server's
 buffer to the each client.

 RACE: Lock the server's output buffer 
 untill it's been written out, to prevent
 other theads from overwriting it.*/
void* p_server_to_clients(void* arg);

#endif
