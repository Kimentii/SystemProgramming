CC = gcc
CFLAGS = -c -Wall -pedantic

ifeq ($(OS), Windows_NT)

SOURCES = laba_win.c
OBJECTS = laba_win.o
UNIXFLAG =
THREADFLAG = 

else

SOURCES = laba_linux.c
OBJECTS = laba_linux.o
UNIXFLAG = -D_XOPEN_SOURCE=600
THREADFLAG = -pthread

endif

lab4: main.o stack.o $(OBJECTS)
	$(CC) $(THREADFLAG) main.o stack.o $(OBJECTS) -o lab4
	
main.o: main.c
	$(CC) $(CFLAGS) main.c -c

stack.o: stack.c
	$(CC) $(CFLAGS) stack.c -c

$(OBJECTS): $(SOURCES)
	$(CC) $(THREADFLAG) $(CFLAGS) $(UNIXFLAG) $(SOURCES) -c

clean:
	del main.o stack.o $(OBJECTS) lab4
