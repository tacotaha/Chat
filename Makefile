CC=gcc
CFLAGS= -Wall -pedantic
EXEC=server client

all: client server

client: connect.o Client.c
	$(CC) $(CFLAGS) Client.c connect.o -o client

server: connect.o Server.c
	$(CC) $(CFLAGS) Server.c connect.o -o server

connect.o: Connect.c Connect.h
	$(CC) -c Connect.c -o connect.o
clean:
	\rm $(EXEC) *.o *~
