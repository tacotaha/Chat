#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "Connect.h"
#include "Stream.h"
#include "Queue/Queue.h"

#define PORT 4444
#define IP "127.0.0.1"
#define BUF 1024
#define BACKLOG 5

void* handle_client(void* arg);

int main(){
  QUEUE queue = queue_init();
  
  int server_socket, client_socket, clients[MAX_CLIENTS], client_count = 0;
  struct sockaddr_in server;
  char out_buffer[BUF], in_buffer[BUF];
  pthread_t t;
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  
  /*Clear Repsective Buffers*/
  memset(&server_socket,0x0,sizeof(server_socket));
  memset(in_buffer,0x0,sizeof(in_buffer));
  memset(out_buffer,0x0, sizeof(out_buffer));
  
  /*Create Server Endpoint For Communcation*/
  server_socket = create_socket();
  
  /*Set up Server Connection Attributes*/
  server = create_socket_address(PORT, IP);
  
  /*Assign Name To Socket*/
  bind_connection(server_socket,(struct sockaddr*)&server);
  
  /*Listen For Connections*/
  listen_for_connection(server_socket, BACKLOG);

  socklen_t sckln = sizeof(struct sockaddr_in);
  
  /*=======================================Main Loop========================================*/
  while( (client_socket = accept(server_socket,(struct sockaddr *)&server,&sckln))){
    Client c;
    c.messages = &queue;
    c.mutex = &lock;
    c.sender = client_socket;
    c.clients = clients;
    
    if(pthread_create(&t, NULL, handle_client, &c) < 0){
      perror("pthread_create()");
      return 1;
    }
    
    if(client_socket < 0){
      perror("accept()");
      return 1;
    }else {
      printf("[+] Client %d connected! \n", client_count+1);
      clients[client_count++] = client_socket;
    }
  }
  
  close(server_socket);
  
  /*Reset Buffers*/
  memset(out_buffer, '\0', sizeof(out_buffer));
  memset(in_buffer, '\0',sizeof(in_buffer));
  fflush(stdout);
  
  
  printf("[-] Closing the connection\n");
  close(server_socket);
  return 0;
}

void* handle_client(void* args){
  Client* c = (Client*) args;
  int client_socket = c->sender, read_size;
  char client_message[BUF];
  
  while( (read_size = recv(client_socket,client_message,BUF,0)) > 0 ){
    client_message[read_size] = 0x0;

    pthread_mutex_lock(c->mutex);
    for(int i = 0; i < MAX_CLIENTS; ++i)
      if(c->clients[i] != client_socket)
	send(c->clients[i],client_message,strlen(client_message),0);
    pthread_mutex_unlock(c->mutex);
    
    memset(client_message, 0, BUF);
  }
  
  if(read_size == 0){
    printf("Client %d disconnected\n", client_socket);
    fflush(stdout);
  } else if(read_size == -1)
    perror("recv()");
  
  pthread_exit(NULL);
}
