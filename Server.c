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
  int server_socket,client_socket, bytes_received;
  struct sockaddr_in server, client;
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
  
  /*Accept a connection from the client*/
  client_socket = accept_connection_from_client(server_socket,(struct sockaddr*)&client,&addr_size);
  
  /*=======================================Main Loop========================================*/
  while(1){
    /*Receive Data From Client*/
    bytes_received = recv(client_socket, in_buffer, sizeof(in_buffer), 0);
    if(bytes_received < 0){
      perror("Recv");
      exit(1);
    }else if(bytes_received == 0){
      printf("[-] Client Disconnected\n");
      exit(1);
    }
    
    /*Print Client Response To stdout*/
    in_buffer[BUF - 1] = '\0';
    printf("Client: %s\n", in_buffer);
    fflush(stdout);
    memset(in_buffer, '\0',sizeof(in_buffer));

    /*Read Response From Server (stdin)*/
    printf(">> ");
    fgets(out_buffer,BUF,stdin);
    fflush(stdout);
    out_buffer[BUF - 1] = '\0';
    
    /*Send Response To Client
    bytes_sent = send(client_socket, out_buffer, sizeof(out_buffer), 0);
    if(bytes_sent < 0){
      perror("Send");
      exit(1);
    }
    */

    Message m;
    m.to_socket = client_socket;
    m.from_socket = server_socket;
    m.message = out_buffer;
    
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid, &attr, p_server_to_clients, &m);
    pthread_join(tid,NULL);

    /*Reset Buffers*/
    memset(out_buffer, '\0', sizeof(out_buffer));
    memset(in_buffer, '\0',sizeof(in_buffer));
    fflush(stdout);
  }
  
  printf("[-] Closing the connection\n");
  close(server_socket);
  close(client_socket);
  return 0;
}
