CC = gcc
CFLAGS = -c -Wall -pedantic

ifeq ($(OS), Windows_NT)

SERVER = server
CLIENT = client
STAND:=  -std=c99

else

SERVER = server_linux
CLIENT = client_linux
STAND:=  -std=gnu99

endif

all: Server Client

Server:$(SERVER).o
	$(CC) $(SERVER).o -o Server

$(SERVER).o:$(SERVER).c
	$(CC)  $(SERVER).c $(STAND) $(CFLAGS)

Client:$(CLIENT).o
	$(CC) $(CLIENT).o -o Client
	
$(CLIENT).o:$(CLIENT).c
	$(CC)  $(CLIENT).c $(STAND) $(CFLAGS)

clean:
	del $(SERVER).o $(CLIENT).o