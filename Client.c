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

int main(){
  int client_socket, bytes_sent, bytes_received;
  struct sockaddr_in server_addr;
  char in_buffer[BUF], out_buffer[BUF];

  /*Clear Repsective Buffers*/
  memset(&client_socket, '\0', sizeof(client_socket));
  memset(in_buffer, '\0', sizeof(in_buffer));
  memset(out_buffer, '\0', sizeof(out_buffer));

  /*Create Client Endpoint For Communcation*/
  if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) >= 0)
    printf("[+] Client Socket Created Successfully.\n");
  else{
    perror("Client Socket");
    exit(1);
  }

  /*Set up Client Connection Attributes*/
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr(IP);

  /* Connect To Server*/
  if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0)
    printf("[+] Address successfully bound to socket.\n");
  else{
    perror("Server Bind");
    exit(1);
  }
  
  /*=======================================Main Loop========================================*/
  while(1){
    printf(">> ");
    fgets(out_buffer, sizeof(out_buffer), stdin);
    fflush(stdout);
    in_buffer[BUF - 1] = '\0';
    
    /* Initialize Conversation With Server*/
    bytes_sent = send(client_socket, out_buffer, sizeof(out_buffer),0);
    if(bytes_sent < 0){
      perror("Send");
      exit(1);
    }
    
    /*Read Response From Server*/
    bytes_received = recv(client_socket, in_buffer, sizeof(in_buffer), 0);
    if(bytes_received < 0){
      perror("Recv");
      exit(1);
    }else if(bytes_received == 0){
      printf("[-] Server Disconnected\n");
      exit(1);
    }
    
    /*Print Server Response To stdout*/
    in_buffer[BUF - 1] = '\0';
    printf("Server: %s\n", in_buffer);
    fflush(stdout);
    
    /*Reset Buffers*/
    memset(out_buffer, '\0', sizeof(out_buffer));
    memset(in_buffer, '\0',sizeof(in_buffer));
  }
  
  printf("[-] Closing the connection.\n");
  close(client_socket);
  return 0;
}
