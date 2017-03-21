#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>  
#define BUFFER_SIZE 512

int main() {
	HANDLE  hNamedPipe; 
	char pipeName[] = "\\\\.\\pipe\\demo_pipe"; 
	hNamedPipe = CreateFile( 
	pipeName,												//Имя канала
	GENERIC_WRITE | GENERIC_READ, 							//Режим чтения-записи
	0, 														//С файлом не может работать несколько процессов
	NULL,													//Дескриптор не может наследоваться
	OPEN_EXISTING,											//Открыть существующий
	0,														//Атрибуты файла
	NULL );  												//Дескриптор шаблона
	//Проверка на то создался ли файл
 if (hNamedPipe == INVALID_HANDLE_VALUE)
	 {  
		printf("Connection with the named pipe failed.");
		_getch();
		return 0;   
 } 
 
 	HANDLE hSemServer = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("SemServer"));
	HANDLE hSemClient = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("SemClient"));

    printf("Connected to server.\n");

	ReleaseSemaphore(hSemServer, 1, NULL);					//Разрешаем работать серверу
	
 char str[BUFFER_SIZE];
while(strcmp(str, "quit"))
 { 
	//Чтения сообщения о том что сервер готов
	WaitForSingleObject(hSemClient, INFINITE);				//Ждем пока сервер разрешит работу
	ReleaseSemaphore(hSemClient, -1, NULL);
	DWORD dwRead;
	//Читает о том, что сервер готов
	if (ReadFile(hNamedPipe,								//Имя файла
	str,													//Строка в которую читаем	    
	BUFFER_SIZE,											//Сколько байт читаем
	&dwRead,												//Сколько байт было прочитано
	NULL ))  												//Для ассинхронной работы
	{
		printf("%s\n", str);								//Вывод прочитанной строки
	} 
	ReleaseSemaphore(hSemServer, 1, NULL);					//Разрешаем работать серверу
	//Чтение строки строки, которая была введена в сервере
	if(WaitForSingleObject(hSemClient, 30000)==WAIT_TIMEOUT)	//Ждем пока сервер разрешит работу
	{
			printf("Connection with server failed. ");
			printf("Time out.\n");
			_getch();
			exit(0);
	}
	ReleaseSemaphore(hSemClient, -1, NULL);
	if (ReadFile(hNamedPipe, str,    						//Чтение строки
	BUFFER_SIZE, &dwRead,NULL ))  
	{
		printf("%s\n", str);								//Вывод прочитанной строки
		DWORD dwWritten;
		if (!WriteFile( hNamedPipe, "the client has received a message", BUFFER_SIZE, 
			&dwWritten, NULL))  							//Пишем сообщение о то что клиент прчоитал сообщение
		{
			break;
		} 
	} 
	printf("%d bytes was read.\n", strlen(str));
	ReleaseSemaphore(hSemServer,1,NULL);						//Разрешаем работу серверу
 } 
 
 CloseHandle(hNamedPipe);
 CloseHandle(hSemClient); 

 	printf("Press key to finish the client:  ");
		_getch();
 return 0;
 } 