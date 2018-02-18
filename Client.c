#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
#define IP "127.0.0.1"
#define BUF 1024

int main(){	
  int client_socket;
  struct sockaddr_in server_addr;
  char buffer[BUF];
  
  client_socket = socket(PF_INET, SOCK_STREAM, 0);
  printf("[+]Client Socket Created Sucessfully.\n");
  
  memset(&server_addr, '\0', sizeof(server_addr));
  memset(&buffer, '\0',sizeof(buffer));
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr(IP);
  
  connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
  printf("[+]Connected to Server.\n");
  
  recv(client_socket, buffer, sizeof(buffer), 0);
  printf("[+]Data Recv: %s\n", buffer);
  printf("[+]Closing the connection.\n");
  
  return 0;
}
