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
  int client_socket, status;
  struct sockaddr_in server_addr;
  char buffer[BUF];
  
  client_socket = socket(PF_INET, SOCK_STREAM, 0);
  if(client_socket == -1){
    fprintf(stderr, "Could not open client socket!\n");
    exit(-1);
  }else printf("[+]Client Socket Created Sucessfully.\n");
  
  memset(&server_addr, '\0', sizeof(server_addr));
  memset(&buffer, '\0',sizeof(buffer));
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr(IP);
  
  status = connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(status == -1){
    fprintf(stderr, "Error connecting to server!\n");
    exit(-1);
  }else printf("[+]Connected to Server.\n");

  char client_buffer[BUF];
  while(1){
    printf("Enter Mesage: ");
    fgets(client_buffer, sizeof(client_buffer), stdin);
    send(client_socket, client_buffer, sizeof(buffer),0);
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("[+]Server: %s\n", buffer);
    memset(client_buffer, '\0', sizeof(client_buffer));
  }
  
  close(client_socket);
  printf("[+] Closing the connection.\n");
  return 0;
}
