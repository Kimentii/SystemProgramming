CC = gcc
CFLAGS = -c -Wall -pedantic

ifeq ($(OS), Windows_NT)

STAND:=  -std=c99
FILENAME = laba_win

else

STAND:=  -std=gnu99
FILENAME = laba_linux

endif

all: output

output: $(FILENAME).o stack.o main.o 
	$(CC) $(FILENAME).o stack.o main.o -o lab2

$(FILENAME).o: $(FILENAME).c
	$(CC) $(CFLAGS) $(STAND) $(FILENAME).c 	
	
stack.o: stack.c
	$(CC) $(CFLAGS) $(STAND) stack.c 
	
main.o: main.c
	$(CC) $(CFLAGS) $(STAND) main.c 