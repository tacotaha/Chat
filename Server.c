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

#define PORT 4444
#define IP "127.0.0.1"
#define BUF 1024
#define BACKLOG 5

int main(){
  int server_socket;
  struct sockaddr_in server;
  char out_buffer[BUF], in_buffer[BUF];
  socklen_t addr_size;
  
  /*Clear Repsective Buffers*/
  memset(&server_socket, '\0', sizeof(server_socket));
  memset(in_buffer, '\0', sizeof(in_buffer));
  memset(out_buffer, '\0', sizeof(out_buffer));
  
  /*Create Server Endpoint For Communcation*/
  server_socket = create_socket();

  /*Set up Server Connection Attributes*/
  server = create_socket_address(PORT, IP);

  /*Assign Name To Socket*/
  bind_connection(server_socket,(struct sockaddr*)&server);

  /*Listen For Connections*/
  listen_for_connection(server_socket, BACKLOG);

  
  /*=======================================Main Loop========================================*/
  while(1){
    struct sockaddr_storage client_addr;
    int client_socket;
    
    /*Try accepting any incoming connections, skip to next iteration if none*/
    if((client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &addr_size)) < 0){
      perror("accept()");
      continue;
    }

    /* Spawn a thread to handle the client that just connected*/
    pthread_t client_thread;
    pthread_attr_t client_thread_attr;
    pthread_attr_init(&client_thread_attr);
    if((pthread_create(&client_thread, &client_thread_attr, p_server_to_clients, &client_socket)) < 0){
      perror("pthread_create()");
      pthread_exit(0);
    }
    
    /*Reset Buffers*/
    memset(out_buffer, '\0', sizeof(out_buffer));
    memset(in_buffer, '\0',sizeof(in_buffer));
    fflush(stdout);
  }
  
  printf("[-] Closing the connection\n");
  close(server_socket);
  return 0;
}
