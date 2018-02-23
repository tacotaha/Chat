CC=gcc
CFLAGS= -Wall -pedantic
PFLAGS=-lpthread
EXEC=server client

all: client server

client: connect.o stream.o Client.c
	$(CC) $(CFLAGS) $(PFLAGS) Client.c connect.o stream.o -o client

server: connect.o stream.o Server.c
	$(CC) $(CFLAGS) $(PFLAGS) Server.c connect.o stream.o -o server

connect.o: Connect.c Connect.h
	$(CC) -c Connect.c -o connect.o

stream.o: Stream.c Stream.h
	$(CC) -c Stream.c -o stream.o

clean:
	\rm $(EXEC) *.o *~
