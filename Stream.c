#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>

#include "Colors.h"
#include "Connect.h"
#include "Stream.h"

void* handle_client(void* args){
  Client* c = (Client*) args;
  int client_socket = c->sender, read_size;
  char client_message[BUF];
  
  while( (read_size = recv(client_socket,client_message,BUF,0)) > 0 ){
    client_message[read_size] = 0x0;

    pthread_mutex_lock(c->mutex);
    for(int i = 0; i < MAX_CLIENTS; ++i)
      if(c->clients[i] != client_socket)
	send(c->clients[i],client_message,strlen(client_message),0);
    pthread_mutex_unlock(c->mutex);
    
    memset(client_message, 0, BUF);
  }
  
  if(read_size == 0){
    printf("Client %d disconnected\n", client_socket);
    fflush(stdout);
  } else if(read_size == -1)
    perror("recv()");
  
  pthread_exit(NULL);
}

void* client_write_thread(void* arg){
  int bytes_sent = 0, byte_count;
  Message* m = (Message*)arg;
  unsigned char outgoing[BUF*2+COLOR_SIZE*2];
  const char* spacing = ": ";
  
  while(1){
    memset(m->message,0,sizeof(m->message));
    memset(outgoing,0,sizeof(Message));
    
    fgets(m->message, sizeof(m->message),stdin);

    memcpy(outgoing,m->color,strlen(m->color));
    byte_count = strlen(m->color);
    
    memcpy(outgoing + byte_count,m->user_name,strlen(m->user_name));
    byte_count += strlen(m->user_name);

    memcpy(outgoing + byte_count, spacing, strlen(spacing));
    byte_count += strlen(spacing);
    
    memcpy(outgoing + byte_count,COLORS[RESET],strlen(COLORS[RESET]));
    byte_count += strlen(COLORS[RESET]);
    
    memcpy(outgoing + byte_count,m->message,strlen(m->message));
    byte_count += strlen(m->message);
    
    bytes_sent = send(m->socket,outgoing,byte_count,0);
    byte_count = 0;

    if(bytes_sent < 0){
      perror("Send");
      free(m);
      exit(1);
    }
  }
  
  pthread_exit(0);
}

void* client_read_thread(void* arg){
  int bytes_received, sock = *(int*)arg;
  Message* m = malloc(sizeof(Message));
  
  if(m == NULL) exit(1);
  
  unsigned char incoming[COLOR_SIZE*2 + BUF*2];
  
  while(1){
    bytes_received = recv(sock,incoming,sizeof(incoming),0);
    
    if(bytes_received < 0){
      perror("Recv");
      free(m);
      exit(1);
    }else if(bytes_received == 0){
      printf("[-] Server Disconnected\n");
      free(m);
      exit(1);
    }
    
    printf("%s\n", incoming);
    fflush(stdout);
    
    memset(incoming,0,sizeof(incoming));
    memset(m,0,sizeof(Message));
  }
  
  free(m);
  pthread_exit(0);
}

void print_banner(void){
  FILE* fp = fopen("ascii_chat.txt","r");
  char c;
  if(fp)
    while((c = fgetc(fp)) != EOF)
      printf("%c",c);
  fclose(fp);
}
