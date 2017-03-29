#include "headers.h"

int main(int argc, char *argv[])
{
	struct Stack *stack = NULL;
	struct Data data;								//Переменная - буффер, которая содержит количество потоков,
													//и информацию о потоке
		CreateControlThread(&data);
		while (1)
		{
			switch (_getch())
			{
			case '+':
				data.count++;
				CreateNewThread(&data, &stack);
				break;
			case '-':
				if (stack){
					CloseThread(&stack, &data,0);
					}
				break;	
			case 'q':
					CloseThread(&stack, &data,1);
					DelControlThread(&data);
					free(stack);
					return 0;
				break;
			}
		}
}
