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
  int server_socket,client_socket;
  struct sockaddr_in server, client;

  socklen_t addr_size;
  char buffer[BUF];

  /* Clear repsective buffers */
  memset(&server_socket, '\0', sizeof(server_socket));
  memset(buffer, '\0', sizeof(buffer));
  
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket)
    printf("[+] Server Socket Created Successfully!\n");
  else{
    fprintf(stderr, "Error! Failed to Create Server Socket!\n");
    exit(1);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = inet_addr(IP);

  bind(server_socket, (struct sockaddr*) &server, sizeof(server));
  
  listen(server_socket, 5);
  printf("[+] Listening...\n");

  client_socket = accept(server_socket, (struct sockaddr*) &client, &addr_size);

  strcpy(buffer, "Test String!");
  send(client_socket, buffer, strlen(buffer), 0);
  printf("[+] Closing the connection\n");

  close(server_socket);
  close(client_socket);
  
  return 0;
}
