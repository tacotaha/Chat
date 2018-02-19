CC=gcc
CFLAGS= -Wall -pedantic
EXEC=server_di_test_client server_di_test_server client_di_test_client client_di_test_server

all: server_di_test client_di_test

server_di_test: server_di_test_client server_di_test_server

client_di_test: client_di_test_client client_di_test_server

server_di_test_client: connect.o Server_DI_Test_Client.c
	$(CC) $(CFLAGS) Server_DI_Test_Client.c connect.o -o server_di_test_client

server_di_test_server: connect.o Server_DI_Test_Server.c
	$(CC) $(CFLAGS) Server_DI_Test_Server.c connect.o -o server_di_test_server

client_di_test_client: connect.o Client_DI_Test_Client.c
	$(CC) $(CFLAGS) Client_DI_Test_Client.c connect.o -o client_di_test_client

client_di_test_server: connect.o Client_DI_Test_Server.c
	$(CC) $(CFLAGS) Client_DI_Test_Server.c connect.o -o client_di_test_server

connect.o: ../Connect.c ../Connect.h
	$(CC) -c ../Connect.c -o connect.o

clean:
	rm -f $(EXEC) *.o *~ *#*
