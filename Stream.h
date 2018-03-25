#ifndef __STREAM_H__
#define __STREAM_H__

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "Colors.h"

#define BUF 1024
#define MAX_CLIENTS 100
#define PORT 4444
#define IP "127.0.0.1"
#define BACKLOG 10

typedef struct client{
  pthread_mutex_t* mutex;
  int sender;
  int* clients;
}Client;

typedef struct message{
  char message[BUF];
  char user_name[BUF];
  char color[COLOR_SIZE];
  int socket;
}Message;

void* handle_client(void* arg);
void* client_write_thread(void* arg);
void* client_read_thread(void* arg);

#endif
