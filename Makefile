CC=gcc
CFLAGS=-Wall -Werror -std=c99 -pedantic
PFLAGS=-lpthread
EXEC=server client

all: client server

client: client.o stream.o connect.o queue.o
	$(CC) client.o stream.o connect.o queue.o $(PFLAGS) -o client

server: server.o stream.o connect.o queue.o
	$(CC) server.o stream.o connect.o queue.o $(PFLAGS) -o server

client.o: Client.c
	$(CC) $(CFLAGS) -c Client.c -o client.o

server.o: Server.c
	$(CC) $(CFLAGS) -c Server.c -o server.o

connect.o: Connect.c Connect.h
	$(CC) $(CFLAGS) -c Connect.c -o connect.o

stream.o: Stream.c Stream.h
	$(CC) $(CFLAGS) -c Stream.c -o stream.o

queue.o: Queue/Queue.c Queue/Queue.h Queue/Status.h
	$(CC) $(CFLAGS) -o queue.o -c Queue/Queue.c

clean:
	\rm $(EXEC) *.o *~
