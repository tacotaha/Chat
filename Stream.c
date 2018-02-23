#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Stream.h"
#include "Connect.h"


void* p_server_to_clients(void* arg){
  int  bytes_sent = 0;
  char out_buffer[] = "Hello there! This is a concurrency test!";

  int client_socket = *(int*)arg;

  /*Send Response To Client*/
  bytes_sent = send(client_socket, out_buffer, sizeof(out_buffer), 0);
  if(bytes_sent < 0){
    perror("DID NOT SEND!!");
    pthread_exit(0);
  }else{
    printf("IT ACTUAL SENT YEEEE!!\n");
  }

  pthread_detach(pthread_self());
  printf("Socket %d connected!\n", client_socket);
   
  pthread_exit(0);
}
