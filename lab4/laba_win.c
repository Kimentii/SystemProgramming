#include "headers.h"

void CreateControlThread(struct Data *data)
{
	InitializeCriticalSection( &cs1 );				//Инициализация критической секции
	data->count = 0;
}

void DelControlThread(struct Data *data)
{
	DeleteCriticalSection(&cs1);					//Удаление критической секции(очищение ресурсов)
}

void CreateNewThread(struct Data *data, struct Stack **stack)
{
	sizeh = data->count;							//Запоминаем количество потоков
	HANDLE hThread;
	hThread = (HANDLE) CreateThread(NULL,			//Дескриптор не может наследоваться
	0,												//Размер стека как у исполняемой программы
	(LPTHREAD_START_ROUTINE)&ThreadFun,				//Функция потока
	data,											//Параметр потоку
	0,												//Поток запустится при создании
	NULL);											//Не принимаем индификатор потока
	data->hand=hThread;
	push(*&stack, *data);							//Добавляем в стек(добавляется копия)
}

unsigned int ThreadFun( void* fdata)				//Функци потока
  {
	struct Data data2 = *(struct Data *)fdata;		//Для удобства
	free(fdata);
    while ( TRUE )
    {
		Sleep(100);
		EnterCriticalSection( &cs1 );				//Поток пытается войти в критическую секцию, если не получится, то ожидает
		printf("Thread: %d of %d\n", data2.count, sizeh); //Выводит свой номер и количество потоков
		LeaveCriticalSection( &cs1 );				//Освобождает критическую секцию
    }
	return 0;
  }

void CloseThread(struct Stack **stack, struct Data *data, int code)
{
	if (code == 0)
	{
		EnterCriticalSection( &cs1 );
		data->count--;									//Уменьшаем номер потока, который будет создаваться в будующем
		sizeh = data->count;							//Запоминаем количество потоков
		TerminateThread((*stack)->hand, 0);				//Завершение работы потока
		pop(*&stack);									//Удаление потока из стека
		LeaveCriticalSection( &cs1 );					//Выход из критической секции
	}
	else if (code==1)
	{
	EnterCriticalSection( &cs1 );
		while (*stack)
		{
			TerminateThread((*stack)->hand, 0);
			pop(*&stack);
		}
	DeleteCriticalSection(&cs1);
	system("cls");
	}
}

