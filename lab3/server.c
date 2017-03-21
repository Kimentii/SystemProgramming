#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h> 
#define BUFFER_SIZE 512

int main() {
	HANDLE  hNamedPipe;   
	// Создаем именованный канал 
	hNamedPipe = CreateNamedPipe("\\\\.\\pipe\\demo_pipe", 			//Имя канала
		PIPE_ACCESS_DUPLEX,											//Для записи и чтения
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | 				//Запись и чтение в канал сообщениями.
		PIPE_WAIT,													//Процесс ждет освобождения канала
		PIPE_UNLIMITED_INSTANCES,									//Количество экземпляров не ограниченно
		BUFFER_SIZE, BUFFER_SIZE,									//Размер буферов для ввода и чтения из канала
		NMPWAIT_USE_DEFAULT_WAIT,									//Время ожидания для реализации канала по умолчанию
		NULL);														//Атрибуты защиты по умолчанию
	//Проверяем на успешное создание 
	if (hNamedPipe == INVALID_HANDLE_VALUE) 
	{  
		printf("error");
		_getch();
		return 0; 
	}   
	//Создание семафоров клиента и сервера
	HANDLE hSemServer = CreateSemaphore(NULL, 0, 1, "SemServer");	//Семафор отвечает за то можно ли работать серверу
	HANDLE hSemClient = CreateSemaphore(NULL, 0, 1, "SemClient");	//Семафор отвечает за то можно ли работать клиенту
	
	printf("The server is waiting for connection with a client.\n");
	//Ждем пока клиент свяжется с каналом 
	if(!ConnectNamedPipe( 
		hNamedPipe,NULL )) 
	{  
		CloseHandle(hNamedPipe); 
		printf("Press key to finish the server: ");
		_getch();
		return 0;
	}

    WaitForSingleObject(hSemServer, INFINITE);						//Ждем когда клиент даст сигнал серверу работать
	ReleaseSemaphore(hSemServer, -1, NULL);
	printf("Client connected\n");	
	
	char str[BUFFER_SIZE];
	while(strcmp(str, "quit")) 
	{
		//Записывает в канал информацию о том что он готов
		DWORD dwWritten; 
		if (!WriteFile( hNamedPipe,									//Дескриптор канала
			"server ready", BUFFER_SIZE,							//Записываемая надпись и размер буфура записи
			&dwWritten, 											//Размер записанного сообщения
			NULL))  												//Ассинхронный буфер(у нас синхоррный)
		{
			break;
		} 
		ReleaseSemaphore(hSemClient, 1, NULL);						//Разрешаем работать клиенту
		//Запись вводимой строки
		WaitForSingleObject(hSemServer, INFINITE);					//Ждем пока клиент разрешит работу
		ReleaseSemaphore(hSemServer, -1, NULL);
		fflush(stdin);
		gets(str);													//Пользователь вводит строку
		DWORD dwWritten2; 
		if (!WriteFile( hNamedPipe, str, BUFFER_SIZE, 				//Записываем введенную стрку
			&dwWritten2, NULL))  
		{
			break;
		}
		printf("%d bytes was set.\n", strlen(str));
		ReleaseSemaphore(hSemClient, 1, NULL);						//Разрешаем работу клиенту
		//Читает информацию о том что клиент получил сообщение
		WaitForSingleObject(hSemServer, INFINITE);					//Ждем пока клиент разрешит работу
		ReleaseSemaphore(hSemServer, -1, NULL);
		DWORD dwRead;
		if (ReadFile( hNamedPipe, str, BUFFER_SIZE, 				//Читаем сообщение о том что клиент получил сообщение
			&dwRead, NULL))  
		{
			printf("%s\n", str);
		} 
		
	}  
	
	CloseHandle(hNamedPipe);
	CloseHandle(hSemServer);	
	printf("Press key to finish the server: ");
    _getch();	
	return 0;
} 