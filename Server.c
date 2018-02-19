#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 4444
#define IP "127.0.0.1"
#define BUF 1024
#define BACKLOG 5

int main(){
  int server_socket,client_socket, bytes_received, bytes_sent;
  struct sockaddr_in server, client;
  char out_buffer[BUF], in_buffer[BUF];
  socklen_t addr_size;
  
  /*Clear Repsective Buffers*/
  memset(&server_socket, '\0', sizeof(server_socket));
  memset(in_buffer, '\0', sizeof(in_buffer));
  memset(out_buffer, '\0', sizeof(out_buffer));
  
  /*Create Server Endpoint For Communcation*/
  if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) >= 0)
    printf("[+] Server Socket Created Successfully.\n");
  else{
    perror("Server Socket");
    exit(1);
  }
  
  /*Set up Server Connectoin Attributes*/
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = inet_addr(IP);
  
  /*Assign Name To Socket*/
  if(bind(server_socket, (struct sockaddr*) &server, sizeof(server)) >= 0)
    printf("[+] Address successfully bound to socket.\n");
  else{
    perror("Server Bind");
    exit(1);
  }

  /*Listen For Connections*/
  if(listen(server_socket, BACKLOG) == 0)
    printf("[+] Listening...\n");
  else{
    perror("Listen");
    exit(1);
  }
  
  /*Accept a connection from the client*/
  if((client_socket = accept(server_socket, (struct sockaddr*) &client, &addr_size)) >= 0)
    printf("[+] Accepted Connection.\n");
  else{
    perror("Accept");
    exit(1);
  } 

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
    
    /*Send Response To Client*/
    bytes_sent = send(client_socket, out_buffer, sizeof(out_buffer), 0);
    if(bytes_sent < 0){
      perror("Send");
      exit(1);
    }

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
