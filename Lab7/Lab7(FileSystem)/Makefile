CC = gcc
CFLAGS = -Wall -pedantic

ifeq ($(OS), Windows_NT)

lab6: main.o
	$(CC) $(CFLAGS) main.o -o lab7
	
main.o: main.c
	$(CC) $(CFLAGS) main.c -c

endif