#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>


#include "Colors.h"
#include "Connect.h"
#include "Stream.h"

int main(){
  srand(time(0));
  
  int client_socket;
  struct sockaddr_in server_addr;
  pthread_t read, write;
  Message m;
  char buffer[BUF];
  const char* color = COLORS[rand() % num_colors];
  
  memset(&client_socket,0,sizeof(client_socket));
  
  printf("Username: ");
  fgets(buffer,sizeof(buffer),stdin);
  
  for(size_t i = 0; i < sizeof(buffer); ++i)
    if(isspace(buffer[i]))
      m.user_name[i] = 0;
    else
      m.user_name[i] = buffer[i];

  /*Create Client Endpoint For Communcation*/
  client_socket = create_socket();
  
  /*Set up Client Connection Attributes*/
  server_addr = create_socket_address(PORT, IP);
  
  /*Connect To Server*/
  connect_to_server(client_socket,(struct sockaddr*)&server_addr);
  
  m.socket = client_socket;
  
  for(size_t i = 0; i < COLOR_SIZE; ++i)
    m.color[i] = color[i];
  
  if(pthread_create(&read, NULL, client_read_thread, &client_socket) < 0){
    perror("pthread_create");
    return 1;
  }
  
  if(pthread_create(&write, NULL, client_write_thread, &m) < 0){
    perror("pthread_create");
    return 1;
  }
    
  pthread_join(read,NULL);
  pthread_join(write,NULL);
  
  return 0;
}
