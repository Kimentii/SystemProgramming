#include "header.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/wait.h>

int createProc(struct Info* data)
{
	printf("Platform: Unix\n");
	
	sleep(1);
	switch(data->pid = fork())
	{
	case -1:								//Произошла ошибка
		printf("Error\n");
			return 0;
	case 0:									//Код потомока
	{		
			printf("children PID - %d\n", getpid());
            execl("./timeadd",NULL);
            exit(0);
    }
		
	default:								//Код родителя
	{
			printf("parent PID - %d\n", getpid());
            deleteProc(data);
			printf("Daughter process closed");
    }
	}
            
	return 1;
}

void deleteProc(struct Info* data)
{
    wait(&(data->pid));
}
