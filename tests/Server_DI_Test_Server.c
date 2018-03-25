#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Connect.h"
#include "../Colors.h"

#define PORT 4444
#define IP "127.0.0.1"
#define BUF 1024
#define BACKLOG 5

int main(){
  int server_socket,client_socket;
  struct sockaddr_in server, client;
  char out_buffer[BUF], in_buffer[BUF];
  socklen_t addr_size;
  
  /*Clear Repsective Buffers*/
  memset(&server_socket, '\0', sizeof(server_socket));
  memset(in_buffer, '\0', sizeof(in_buffer));
  memset(out_buffer, '\0', sizeof(out_buffer));
  
  /*Connect To Client*/
  server_socket = create_socket();
  server = create_socket_address(PORT, IP);
  bind_connection(server_socket,(struct sockaddr*)&server);
  listen_for_connection(server_socket, BACKLOG);
  client_socket = accept_connection_from_client(server_socket,(struct sockaddr*)&client,&addr_size);

  FILE* fp = fopen("test_strings.txt","r");
  if(fp == NULL){
    printf("FILE DNE!\n");
    exit(1);
  }

  int passed = 0, count = 0;
  while((fscanf(fp,"%s",out_buffer)) != EOF){
    /*Receive Data From Client*/
    recv(client_socket, in_buffer, sizeof(in_buffer), 0);

    /*Compare Data In File To Data Received From Client*/
    if(strcmp(out_buffer, in_buffer) == 0){
      printf("%sSTRING %d PASSED%s\n",COLORS[GREEN],count++,COLORS[RESET]);
      ++passed;
    }else{
      printf("%sSTRING %d FAILED%s\n",COLORS[GREEN],count++,COLORS[RESET]);
      ++passed;
    }
    
    /*Reset Buffers*/
    memset(out_buffer, '\0', sizeof(out_buffer));
    memset(in_buffer, '\0',sizeof(in_buffer));
    fflush(stdout);
  }

  
  printf("PASSED %d/%d TESTS\n", passed, count);
  close(server_socket);
  close(client_socket);
  return 0;
}
