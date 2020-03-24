CC = gcc
CFLAGS = -Wall
CLIENT = client
SERVER = server

all: client server

client:
	$(CC) $(CFLAGS) client.c -o $(CLIENT)
server:
	$(CC) $(CFLAGS) server.c -o $(SERVER)
clean:
	rm *.o $(CLIENT) $(SERVER)