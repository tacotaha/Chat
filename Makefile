CC=gcc
CFLAGS= -Wall -pedantic
EXEC=server client

all: client server

client: Client.c
	$(CC) $(CFLAGS) Client.c -o client

server: Server.c
	$(CC) $(CFLAGS) Server.c -o server

clean:
	\rm $(EXEC) *~
