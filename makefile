CC = gcc
CFLAGS = -Wall
CLIENT = client
SERVER = server

all: client server

client: client.o
	$(CC) $(CFLAGS) client.c -o $(CLIENT)
server: server.o
	$(CC) $(CFLAGS) server.c -o $(SERVER)
clean:
	rm *.o $(CLIENT) $(SERVER)