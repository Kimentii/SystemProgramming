#pragma warning (disable:4996)
#include <stdio.h>
#include <time.h>
void main()
{
	time_t t;
	time(&t);
	printf("The time is %s", ctime(&t));
	printf("Press key to go out.\n");
	getchar();
	return ;
}
