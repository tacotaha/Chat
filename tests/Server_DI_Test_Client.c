#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Connect.h"
#include "../Colors.h"

#define PORT 4444
#define IP "127.0.0.1"
#define BUF 1024

int main(){
  int client_socket;
  struct sockaddr_in server_addr;
  char in_buffer[BUF], out_buffer[BUF];
  
  /*Clear Repsective Buffers*/
  memset(&client_socket, '\0', sizeof(client_socket));
  memset(in_buffer, '\0', sizeof(in_buffer));
  memset(out_buffer, '\0', sizeof(out_buffer));
  
  /*Connect To Server*/
  client_socket = create_socket();
  server_addr = create_socket_address(PORT, IP);
  connect_to_server(client_socket,(struct sockaddr*)&server_addr);
  
  FILE* fp = fopen("test_strings.txt","r");
  if(fp == NULL){
    printf("FILE DNE!\n");
    exit(1);
  }
  
  while((fscanf(fp,"%s",out_buffer)) != EOF){    
    send(client_socket, out_buffer, sizeof(out_buffer),0);
    /*Reset Buffers*/
    memset(out_buffer, '\0', sizeof(out_buffer));
    memset(in_buffer, '\0',sizeof(in_buffer));
  }
  
  printf("[-] Closing the connection.\n");
  close(client_socket);
  return 0;
}
