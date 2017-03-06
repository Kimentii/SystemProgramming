#include "header.h"
struct Info data;

int main()
{
	if (!createProc(&data))
	{
		printf("CreateProcess failed.\n");
		exit(1);
	}
	return 0;
}

