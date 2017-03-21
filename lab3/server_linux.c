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
	struct sembuf semBuffServer;							//Структуры для работы с семафорами
	struct sembuf semBuffClient; 

	int key = ftok("/home/softweremaker/Desktop/ForComp/server_linux.c", 's');	//Гененрируем ключ для семафоров
	semID = semget(key, 2, 0666 | IPC_CREAT);				//Генерируем 2 семафора
	semctl(semID, 0, SETVAL, (int)0);						//Зануление семафоров
	semctl(semID, 1, SETVAL, (int)0);
	semBuffServer.sem_num = 0;								//Номер семафора в наборе
	semBuffServer.sem_op = -1;								//Операция над семафором
	semBuffClient.sem_num = 1;
	semBuffClient.sem_op = 1;
	
	key = ftok("/home/softweremaker/Desktop/ForComp/server_linux.c", 'c');		//Генерируем ключ для разделяемого сегмента памяти
	segmentID = shmget(key, BUFFER_SIZE, 0666 | IPC_CREAT);	//Создаем сегмент разделяемой памяти и получаем его дескриптор
		
	if (segmentID == -1)
	{
		semctl(semID, 0, IPC_RMID, 0);						//Удаление семафоров
		perror("shmget");
		exit(0);
	}
	
	segmentPtr = shmat(segmentID, NULL, 0);					//Получаем адрес сегмента разделяемой памяти. Доступ: чтение-запись.
	 
	if (segmentPtr == (char*)-1)
	{
		perror("shmat");
		exit(0);
	}

	printf("The server is waiting for connection with a client.\n");
	while (semctl(semID, 0, GETVAL, 0) != 1);				//Ждем пока клиент разрешит работу серверу
	printf("Client connected\n");


	char str[BUFFER_SIZE];
	
	while (1)
	{
		if (!sprintf(segmentPtr, "%s", "server ready"))
		{
			break;
		}
				
		semop(semID, &(semBuffServer), 1);					//Отнимаем от семафора сервера 1.
		semop(semID, &(semBuffClient), 1);					//Разрешаем работать клиенту
		
		while (semctl(semID, 0, GETVAL, 0) != 1);			//Ждем пока клиент разрешит работу серверу
		fflush(stdin);
		fgets(str, BUFFER_SIZE, stdin);						//Ввод строки
		int i = strlen(str) - 1;
		if (str[i] == '\n')
		{
			str[i] = '\0';
		}
		if (!sprintf(segmentPtr, "%s", str))				//Записываем стркоу в разделяемую память
		{	
			break;
		}
		semop(semID, &(semBuffServer), 1);					//Отнимаем от семафора сервера 1
		semop(semID, &(semBuffClient), 1);					//Разрешаем работать клиенту

		if (strcmp(str, "quit") == 0)
		{
			break;
		}	
		
		while (semctl(semID, 0, GETVAL, 0) != 1);			//Ждем пока клиент разрешить работу серверу
		if (sprintf(str, "%s", segmentPtr))					//Получаем сообщение о том что клиент принял информацию
		{
			printf("%s\n", str);
		}
	}
	semctl(semID, 0, IPC_RMID, 0);							//Удаление семафоров
	shmctl(segmentID, IPC_RMID, NULL);
	shmdt(segmentPtr);										//Отсоединение разделяемой памяти
}
