#include "headers.h"

void CreateSignal(struct Data *data)
{
	HANDLE Event = CreateEvent(NULL, FALSE, FALSE, "MyEvent");		//Создание объекта события(не активно)
	
	if (Event)
		SetEvent(Event);
	else															//Если не удалось создать, то выход
		exit(0);
}

void CreateNewProcess(char *path, struct Data *data, struct Stack **stack)
{

	char *numberCh;
	numberCh = (char*)malloc(10);
	sprintf(numberCh, "%d", size(*stack));							//Записть размера стэка в строку
	
	char *str = NULL;												//str - строка выводимая в командную строку
	str = (char*)malloc(sizeof(char)* 100);
	strcpy(str, path);												//Записываем в str путь к нашей программе

	strcat(str, " ");												//Пробел как разделитель
	strcat(str, "Number_of_process:");								//Перезоваемая строка(argv[1])
	strcat(str, numberCh);

	STARTUPINFO stInfo;
	PROCESS_INFORMATION prInfo;

	ZeroMemory(&stInfo, sizeof(STARTUPINFO));						//Зануление памяти
	stInfo.cb = sizeof(STARTUPINFO);								//Передача размера структруы

	if (!CreateProcess(NULL, str, NULL, NULL, FALSE, 0, NULL, NULL, &stInfo, &prInfo))
	{																//Создание процесса через командную строку
		exit(0);	
	}	

	struct Data data_pr;
	data_pr.prInfo = prInfo;

	push(*&stack, data_pr);											//Добавление в стек нового процесса
}

void CloseProcess(struct Stack **stack, struct Data *data, int code)
{
	HANDLE Event = OpenEvent(EVENT_ALL_ACCESS, FALSE, "MyEvent");	//Открытие события

	if (Event == NULL)												//Если событие не найдено, то выход
		exit(0);

	WaitForSingleObject(Event, INFINITE);							//Ожидание сигнализации события
	if (code == 0){													//Если code = 0, то закрытие одного процесса
	ResetEvent(Event);
	TerminateProcess((*stack)->hand, 0);
	pop(*&stack);
	printf ("Process closed\n");
	SetEvent(Event);
	}
	else if (code == 1){											//Иначе завершение всех процессов
		while (size(*stack) != 0)
	{
		TerminateProcess((*stack)->hand, 0);
		pop(*&stack);
	}
	system("cls");
	}
}

void PrintProcesses(char **argv, struct Data *data)
{
	HANDLE Event = OpenEvent(EVENT_ALL_ACCESS, FALSE, "MyEvent");
	while (1)
	{
		WaitForSingleObject(Event, INFINITE);						//Ожидание сигнала
		ResetEvent(Event);
		printf("%s\n\n", argv[1]);									//Выовд особой надписи

		SetEvent(Event);											//Сигнализация события
		//Sleep(2000);
	}
}
