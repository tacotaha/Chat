#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Stream.h"
#include "Connect.h"


void* p_server_to_clients(void* arg){
  Message* m  = (Message*) arg;
  int  bytes_sent, client_socket;
  char* out_buffer;
 
  client_socket = m->to_socket;
  out_buffer = m->message;
  
  /*Send Response To Client*/
  bytes_sent = send(client_socket, out_buffer, sizeof(out_buffer), 0);
  if(bytes_sent < 0){
      perror("Send");
      exit(1);
  }
  
  pthread_exit(0);
}
