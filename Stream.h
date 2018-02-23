#ifndef __STREAM_H__
#define __STREAM_H__

#define BUF 1024

typedef struct message{
  int to_socket;
  int from_socket;
char* message;
}Message;

/*Spawn thread to send a message 
  from a client to the server.*/
void* p_client_to_server(void* arg);

/*Spawn thread to output the server's
 buffer to the each client.

 RACE: Lock the server's output buffer 
 untill it's been written out, to prevent
 other theads from overwriting it.*/
void* p_server_to_clients(void* arg);
#endif
