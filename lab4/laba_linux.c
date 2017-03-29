#include "headers.h"

void CreateControlThread(struct Data *data)
{
	pthread_mutex_init(&(data->pMutex), NULL);						//Инициализация мьютекса
	data->count = 0;
}

void DelControlThread(struct Data *data)
{
	if(!pthread_mutex_destroy(&(data->pMutex)))						//Удаляет мьютекс
	{	
		printf("Mutex is deleted.\n");
	}
	else
	{
		printf("Mutex is bad.\n");
	}
}

void* ThreadFun(void* fdata)
{
	struct Data data = *(struct Data *)fdata;						//Для удобства
	free(fdata);
	while (1)
	{
		pthread_mutex_lock(&(data.pMutex)); 						//Захватывает мьютекс, если не получатеся, то поток переводится в режим ожидания
		printf("Thread: %d of %d\n", data.count, sizeh);			//Выводит свой номер и количество потоков
		pthread_mutex_unlock(&(data.pMutex));						//Освобождает мьютекс
		usleep(10000);
	}
}

void CreateNewThread(struct Data *data, struct Stack **stack)
{
	sizeh = data->count;
	struct Data *tempData = (struct Data *)malloc(sizeof(struct Data));
	*tempData = *data;
	pthread_create(&(tempData->pThread),							//Индификатор потока
 	NULL,															//Атрибуты по умолчанию
	 &ThreadFun,													//Функция потока
 	tempData);														//Аргумент функции потока
	push(stack, *tempData);
}

void CloseThread(struct Stack **stack,struct Data *data, int code)
{
	if (code == 0)
	{
		pthread_mutex_lock(&(data->pMutex)); 						//Захватывает мьютекс, если не получатеся, то поток переводится в режим ожидания
		data->count--;
		sizeh = data->count;
		pthread_cancel((*stack)->pThread);							//Отмена потока
		pop(*&stack);
		pthread_mutex_unlock(&(data->pMutex));						//Освобождает мьютекс
	}
	else if (code == 1)
	{

		pthread_mutex_lock(&(data->pMutex)); 						//Захватывает мьютекс, если не получатеся, то поток переводится в режим ожидания
		while(*stack)
		{
			pthread_cancel((*stack)->pThread);						//Отмена потока
			pop(*&stack);
			data->count--;
			system("clear");
		}
		pthread_mutex_unlock(&(data->pMutex));						//Освобождает мьютекс
	
	}
}

char _getch()
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

