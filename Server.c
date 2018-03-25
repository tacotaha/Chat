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

int main(int argc, char* argv[]){
  int server_socket, client_socket, clients[MAX_CLIENTS], client_count = 0, port = PORT;
  struct sockaddr_in server;
  char out_buffer[BUF], in_buffer[BUF];
  pthread_t t;
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  char* ip = IP;

  printf("%s", COLORS[GREEN]);
  print_banner();
  printf("%s\n\n", COLORS[RESET]);
  
  for(int i = 1; i < argc; i += 2){
    if(!strcmp(argv[i],"-i") && argc >= i + 1)
      ip = argv[i + 1];
    else if(!strcmp(argv[i],"-p") && argc >= i + 1)
      port = atoi(argv[i + 1]);
    else{
      printf("Usage: %s [-i] ip_addr [-p] port\n", argv[0]);
      exit(0);
    }
  }
  
  memset(&server_socket,0x0,sizeof(server_socket));
  memset(in_buffer,0x0,sizeof(in_buffer));
  memset(out_buffer,0x0, sizeof(out_buffer));
  
  server_socket = create_socket();
  
  server = create_socket_address(port, ip);
  
  bind_connection(server_socket,(struct sockaddr*)&server);
  
  listen_for_connection(server_socket, BACKLOG);

  socklen_t sckln = sizeof(struct sockaddr_in);
  
  /*=======================================Main Loop========================================*/
  while( (client_socket = accept(server_socket,(struct sockaddr *)&server,&sckln))){
    Client c;
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
  
  memset(out_buffer,0x0, sizeof(out_buffer));
  memset(in_buffer,0x0,sizeof(in_buffer));
  fflush(stdout);
  
  
  printf("[-] Closing the connection\n");
  close(server_socket);
  return 0;
}

