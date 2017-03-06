#include "header.h"
#include <conio.h>
//const char WayToProc[] = "C:\Users\Семен\Desktop\ForComp\ProcToStart.exe"
int createProc(struct Info* data)
{
	ZeroMemory(&(data->stInfo), sizeof(data->stInfo)); 						//Зануление структуры, с информацией о окне
	data->stInfo.cb = sizeof(data->stInfo);
	ZeroMemory(&(data->prInfo), sizeof(data->prInfo));						//Зануление структуры, с ниформацией о процессе
        TCHAR CommandLine[] = TEXT("D:/ProcessToStart/ProcToStart.exe");	//Командная строка для выполнения
    if (CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &(data->stInfo), &(data->prInfo)))
	{
		printf("Process is work.\n");
		return 1;
	}
	else
	{
		printf("Error\n");
	}
        getch();
        system("pause");
	return 0;
}
	