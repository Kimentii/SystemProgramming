#include "headers.h"

char _getch()														//магическая реализация getch
{
	struct termios old, new;
	char ch;
	tcgetattr(0, &old);
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &new);
	ch = getchar();

	tcsetattr(0, TCSANOW, &old);
	return ch;
}

void CreateNewProcess(char *path, struct Data *data, struct Stack **stack)
{	
	switch(data->pid = fork())
	{
		case -1:
			printf("fork error...");
			exit(1);
		case 0:	
		{
			int i = 1, k = size(*stack) / 10;

			for(i = 1; k != 0; i++) 
				k = k / 10;
			
			char *params;
			params = (char *)malloc((i) * sizeof(char));
			params[i] = '\0';
			

			for(int num = i, number = size(*stack); num > 0; num--, number /= 10)
				params[num - 1] = number % 10 + '0';
				
			char *cmd[3] = {"/home/softweremaker/Desktop/ForComp/lab2", params, 0};
			execv("/home/softweremaker/Desktop/ForComp/lab2", cmd); 
		}	
		default:
		{
			push(&*stack, *data);
		}
			break;
	}
}

void CloseProcess(struct Stack **stack, struct Data *data, int code)
{
    semop(data->semid, &(data->mybuff), 1);							//Ожидание равенства нуля семофора
	if (code == 0){
		semop(data->semid, &(data->mybuff1), 1);					//Увеличение семафора на 1
		kill((*stack)->pid, SIGKILL);								//Закрывает процесс
		pop(&*stack);
		printf("\nProcess closed\n");
		data->mybuff1.sem_op = -1;
		semop(data->semid, &(data->mybuff1), 1);
		data->mybuff1.sem_op = 1;
	}
	if (code == 1){
		semop(data->semid, &(data->mybuff1), 1);					//Увеличение семафора на 1
		while (size(*stack) != 0)
		{
			kill((*stack)->pid, SIGKILL);
			pop(&*stack);
		}
		data->mybuff1.sem_op = -1;
		semop(data->semid, &(data->mybuff1), 1);					//Уменьшение семафора на 1
		data->mybuff1.sem_op = 1;
		printf("\033[2J\033[0;0f"); 
		return;
	}
}


void CreateSignal(struct Data *data)
{
	data->key = ftok("/home/softweremaker/Desktop/ForComp/lab2", 0);	//Генерация ключа
	data->semid = semget(data->key, 1, IPC_CREAT);					//Создает семафор
	semctl(data->semid, 0, SETVAL, 0);								//Зануляет семофор
	data->mybuff.sem_num = 0;										//Установка функций для управлений семаформами
	data->mybuff.sem_op = 0;
	data->mybuff1.sem_num = 0;
	data->mybuff1.sem_op = 1;
}

void PrintProcesses(char **argv, struct Data *data)
{
	while(1) 
	{
		semop(data->semid, &(data->mybuff), 1);						//Ожидает равенства нуля семафора
		semop(data->semid, &(data->mybuff1), 1);					//Прибовляет 1 к семофору
		//usleep(500000);
		printf("Process number %s\n", argv[1]);						//Выводит надпись
		data->mybuff1.sem_op = -1;									//Изменяет структуру типа sembuf, с именем mybuff1
		semop(data->semid, &(data->mybuff1), 1);					//Отнимает от семафора 1(семофор становится 0)
		data->mybuff1.sem_op = 1;									//Восстанавливает структуру типа sembuf, с именем mybuff1
	}
}
