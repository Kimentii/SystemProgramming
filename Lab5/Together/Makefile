CC = gcc
STAND:= -std=c99
APP = lb5
SHAREDFLAG = -shared
THREADFLAG = 
AIOFLAG =
LIBFLAG = 

ifeq ($(OS), Windows_NT)

APP_SOURCES = win_laba
LIB_SOURCES = win_fun
CFLAGS = -Wall -pedantic
LIB = fun.dll

else

APP_SOURCES = laba_linux
LIB_SOURCES = fun_linux
CFLAGS = -Wall -pedantic -fPIC -D_XOPEN_SOURCE=600
THREADFLAG = -pthread
AIOFLAG = -lrt
LIBFLAG = -ldl 
LIB = fun.so

endif

all: $(LIB) $(APP)

$(LIB): $(LIB_SOURCES).o
	$(CC) $(LIB_SOURCES).o $(SHAREDFLAG) $(STAND) $(AIOFLAG) -o $(LIB)
	
$(APP): main.o
	$(CC) $(THREADFLAG) $(STAND) main.o -o $(APP) $(LIBFLAG)
	
main.o: main.c
	$(CC) $(CFLAGS) $(THREADFLAG) $(STAND) main.c -c $(AIOFLAG) $(LIBFLAG)
	
$(LIB_SOURCES).o: $(LIB_SOURCES).c
	$(CC) $(CFLAGS) $(THREADFLAG) $(SHAREDFLAG) $(STAND) $(AIOFLAG) $(LIBFLAG) $(LIB_SOURCES).c -c

clean:
	rm -f $(LIB_SOURCES).o $(APP_SOURCES).o main.o $(APP) $(LIB)
