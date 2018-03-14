#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Stream.h"
#include "Connect.h"

void* p_server_to_clients(void* arg){
  Message* m = (Message*) arg;
  int  bytes_sent = 0;
  int client_socket = *(int*)arg;
  char in_buffer[BUF];
  
  int bytes_received = recv(client_socket, in_buffer, sizeof(in_buffer), 0);
  if(bytes_received < 0){
    perror("Recv");
    exit(1);
  }else if(bytes_received == 0){
    printf("[-] Client Disconnected\n");
    exit(1);
  }

  
  bytes_sent = send(client_socket,m->message, sizeof(m->message), 0);
  if(bytes_sent < 0){
    perror("Send()");
    pthread_exit(0);
  }
  
  pthread_detach(pthread_self());
  printf("Socket %d connected!\n", client_socket);
 
  free(m);
  pthread_exit(0);
}

/*Spawn thread to send a message 
  from a client to the server.*/
void* p_client_to_server(void* arg);
