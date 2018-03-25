#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>


#include "Colors.h"
#include "Connect.h"
#include "Stream.h"

int main(int argc, char* argv[]){
  srand(time(0));
  
  int client_socket, port = PORT;
  struct sockaddr_in server_addr;
  pthread_t read, write;
  Message m;
  char buffer[BUF], *ip = IP;
  char* color = COLORS[rand() % num_colors];

  printf("%s", COLORS[GREEN]);
  print_banner();
  printf("%s\n\n", COLORS[RESET]);
  
  for(int i = 1; i < argc; i += 2){
    if(!strcmp(argv[i],"-i") && argc >= i + 1)
      ip = argv[i + 1];
    else if(!strcmp(argv[i],"-p") && argc >= i + 1)
      port = atoi(argv[i + 1]);
    else if(!strcmp(argv[i],"-c") && argc >= i + 1){
      if(!strcmp(argv[i+1],"red"))
	color = COLORS[RED];
      if(!strcmp(argv[i+1],"green"))
	color = COLORS[GREEN];
      if(!strcmp(argv[i+1],"yellow"))
	color = COLORS[YELLOW];
      if(!strcmp(argv[i+1],"blue"))
	color = COLORS[BLUE];
      if(!strcmp(argv[i+1],"purple"))
	color = COLORS[PURPLE];
      if(!strcmp(argv[i+1],"cyan"))
	color = COLORS[CYAN];
    }else{
      printf("Usage: %s [-i] ip_addr [-p] port [-c] <color>\n", argv[0]);
      printf("Default: ip = 127.0.0.0.1, port = 4444\n");
      printf("\nWhere <color> defines the color of the user's username as it appears in the chat window.\nValid colors include: red, green, yellow, blue, purple, cyan\n");
      exit(0);
    }
  }

  memset(&client_socket,0,sizeof(client_socket));
  
  client_socket = create_socket();
  
  server_addr = create_socket_address(port, ip);
  
  connect_to_server(client_socket,(struct sockaddr*)&server_addr);

  printf("Username: ");
  fgets(buffer,sizeof(buffer),stdin);
  
  for(size_t i = 0; i < sizeof(buffer); ++i)
    if(isspace(buffer[i]))
      m.user_name[i] = 0;
    else
      m.user_name[i] = buffer[i];

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
