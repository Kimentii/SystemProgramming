#include <windows.h>

#define BUFFER_SIZE 512

//Запись данных в файл
void WriteDataInFile(HANDLE hFile, char* buff)
{
	struct _OVERLAPPED overlapped;
	overlapped.Offset = 0xFFFFFFFF;						//Младшее слово смещения в фийле
	overlapped.OffsetHigh = 0xFFFFFFFF;					//Старшее смлово смещения в файле
	overlapped.hEvent = NULL;
	DWORD cbWritten;	
	WriteFile(hFile,									//Декскриптор файла
	buff,												//Буфер, который будем записывать
	strlen(buff),										//Размер байтов для записи
	&cbWritten,											//Число записаных байт
	&overlapped);										//Структура для ассинхонной работы
	WaitForSingleObject(hFile, INFINITE);				//Ждем пока запишется в файл
}

//Чтение данных из файла
void ReadDataFromFile(HANDLE hFile, char* buff)
{
	memset(buff, 0, BUFFER_SIZE * sizeof(char));		//Зануляет буфер
	struct _OVERLAPPED overlapped;
	overlapped.Offset = 0;								//Младшее слово смещения в фийле
	overlapped.OffsetHigh = 0;							//Старшее смлово смещения в файле
	overlapped.hEvent = NULL;
	DWORD cbRead;
	ReadFile(hFile,										//Дескриптор файла
	buff,												//Буфер для чтения
	BUFFER_SIZE,										//Количество байт для чтения
	&cbRead,											//Количество реально прочитанных байт
	&overlapped);										//Для ассинхонной работы
	WaitForSingleObject(hFile, INFINITE);
}