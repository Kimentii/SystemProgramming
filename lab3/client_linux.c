#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define BUFFER_SIZE 512
 
int main()
{
	int segmentID;
	char* segmentPtr = NULL;
	int semID;
	struct sembuf semBuffServer;
	struct sembuf semBuffClient; 

	int key = ftok("/home/softweremaker/Desktop/ForComp/server_linux.c", 's');	//Гененрируем ключ для семафоров
	semID = semget(key, 0, IPC_EXCL);								//Получеам дескриптор сущестующих семафоров по ключу
	if (semID == -1)
	{
		printf("Run server first.\n");
		exit(0);
	}
	semBuffServer.sem_num = 0;
	semBuffServer.sem_op = 1;
	semBuffClient.sem_num = 1;
	semBuffClient.sem_op = -1;
	
	key = ftok("/home/softweremaker/Desktop/ForComp/server_linux.c", 'c');		//Генерируем ключ для разделяемого сегмента памяти
	segmentID = shmget(key, BUFFER_SIZE, IPC_EXCL);					//Получаем дескриптор существующего сегмента по ключу
		
	if (segmentID == -1)
	{
		perror("shmget");
		exit(0);
	}
	
	segmentPtr = shmat(segmentID, NULL, 0);							//Включение сегмента в адресное пространство процесса
	
	if (segmentPtr == (char*)-1)
	{
		perror("shmat");
		exit(0);
	}
	
	printf("Connected to segment.\n");
	semop(semID, &(semBuffServer), 1);								//Разрешает работу серверу


	char str[BUFFER_SIZE];

	while (1)
	{
		while (semctl(semID, 1, GETVAL, 0) != 1);					//Ждет пока сервер разрешит работу
		if (sprintf(str, "%s", segmentPtr))
		{
			printf("%s\n", str);									//Выводит надпись о том, что сервер готов
		}
		
		semop(semID, &(semBuffClient), 1);							//Умньшает семафор клиента на 1
		semop(semID, &(semBuffServer), 1);							//Разрешает работу серверу

		//Чтение основной строки
		while (semctl(semID, 1, GETVAL, 0) != 1);					//Ждет пока сервер разрешит работу
		if (sprintf(str, "%s", segmentPtr))							//Читает строку из сегмента разделяемой памяти
		{
			printf("%s\n", str);
			if (strcmp(str, "quit") == 0)
			{
				break;
			}
			strcpy(str, "the client has received a message");		
			if (!sprintf(segmentPtr, "%s", str))					//Пишет в сегмент разделяемой памяти, что клиент получил сообщение
			{
				break;
			}
		}
		
		semop(semID, &(semBuffClient), 1);							//Отнимает от семафора клиента 1
		semop(semID, &(semBuffServer), 1);							//Разрешает работу серверу

	}
}
