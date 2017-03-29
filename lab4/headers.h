#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>


#if defined(_WIN32) || defined(_WIN64) 
#include <windows.h>
#include <process.h> 
#include <conio.h> 
unsigned int ThreadFun(void *);
CRITICAL_SECTION cs1;

#else 
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
char _getch();
#endif 

int sizeh;											//Количество потоков
struct Data
{
	#if defined(_WIN32) || defined(_WIN64)
		HANDLE hand;								//Дескриптор потока
	#else
		pthread_mutex_t pMutex;
		pthread_t pThread;
	#endif
		int count;									//Номер потока
};

#include "stack.h"

void CreateNewThread(struct Data *, struct Stack **);
void CloseThread(struct Stack **, struct Data *, int code);
void DelControlThread(struct Data *);
void CreateControlThread(struct Data *);
