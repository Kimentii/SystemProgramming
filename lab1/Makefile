CC = gcc
CFLAGS = -c -Wall -pedantic

ifeq ($(OS), Windows_NT)
STAND:=  -std=c99

all:Hello

Hello:main.o lab_win.o
	$(CC) main.o lab_win.o -o Hello
	
main.o:main.c
	$(CC) $(CFLAGS) main.c
		
lab_win.o:lab_win.c
	$(CC) $(CFLAGS) lab_win.c

clean:
	rm -rf *.o Hello

else

STAND:=  -std=gnu99

all:Hello

Hello:main.o lab_linux.o
	$(CC) main.o lab_linux.o -o Hello

main.o:main.c
	$(CC) $(CFLAGS) main.c

lab_linux.o:lab_linux.c
	$(CC) $(CFLAGS) lab_linux.c

clean:
	rm -rf *.o Hello
endif
