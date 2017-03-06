#include "headers.h"

int main(int argc, char *argv[])
{
	struct Stack *stack = NULL;
	struct Data data;

	if (argc == 1)
	{
		CreateSignal(&data);						//Создание события

		while (1)
		{
			switch (_getch())
			{
			case '+':								//Добавление процесса
				CreateNewProcess(argv[0], &data, &stack);
				break;
			case '-':								//Закрытие процесса
				if (size(stack) != 0)
					CloseProcess(&stack, &data,0);
				break;
			case 'q':								//Выход из программы
					CloseProcess(&stack, &data,1);
				return 0;
				break;
			}
		}
	}
	else
		printf("New process\n");
		PrintProcesses(argv, &data);

	return 0;
}
