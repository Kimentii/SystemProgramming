#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>
#pragma warning (disable:4996)
#define MASK_SIZE 16								//������ �����
#define SEG_SIZE 16									//������ ������ ��������
#define MEMORY_SIZE (MASK_SIZE*SEG_SIZE)			//������ ������
//MASK - ����� ��������, ���� ������ ����� 16 ������
//MEMORY - ���� ������(256 ����)
struct pointer										//��������� ��� �������� ����������
{
	void* p;										//��� ���������
	int num_of_seg;									//������, �� ������� ��������� ���������
};

char MASK[MASK_SIZE];								//����� ���������, 1 - �����, 0 - ��������
char MEMORY[MEMORY_SIZE];							//���� ������
struct pointer POINTERS[MASK_SIZE];					//������ ���������� �� ������

//������������� ������
void init()
{
	for (int i = 0; i < MASK_SIZE; i++)				//��������� ����� � ��������� ����������
	{
		MASK[i] = 0;
		POINTERS[i].p = nullptr;
		POINTERS[i].num_of_seg = 0;
	}
}

//��������� ������
void* Malloc(unsigned int size)
{
	int segNum;										//���������� ���������� ���������
	float n = (float)size / SEG_SIZE;
	if (n == (int)size / SEG_SIZE) segNum = (int)n;
	else segNum = n + 1;
	int memBeg, memEnd;								//����� ���������� �������� � �������� ����� ��������� ��������
	int i = 0;
	//����� �� ���� ������(�������� ������ ����������, �.�. � ������ ������ ������ ������ ���������� �������)
	while (i < MASK_SIZE)
	{
		while (MASK[i] == 1 && i < MASK_SIZE) i++;	//������� ������� ���������
		if (i == MASK_SIZE) return nullptr;
		memBeg = i;									//��������� ������ ����������� ��������
		int sn = segNum;
		while ((MASK[i] == 0) && (i < MASK_SIZE) && (sn > 0))	//��������, ���������� �� ������ ����� ������ ������
		{
			sn--;
			i++;
		}
		if (i == MASK_SIZE && sn > 0) return nullptr;
		if (sn > 0) continue;
		else
		{
			memEnd = i;								//����� �������� ���������� �� ��������� ��������� �����. ������
			break;
		}
	}
	for (i = memBeg; i < memEnd; i++)				//��������� �����
	{
		MASK[i] = 1;
	}
	POINTERS[memBeg].num_of_seg = segNum;			//������� ��������� � ������ ����������
	POINTERS[memBeg].p = (void*)(MEMORY + memBeg * SEG_SIZE);
	return (MEMORY + memBeg * SEG_SIZE);
}

//�������� ������
void Free(void* p)
{
	int i = 0;
	for (; i < MASK_SIZE; i++)						//���� ��������� � ������� ����������
	{
		if (POINTERS[i].p == p)
		{
			break;
		}
	}
	if (i == SEG_SIZE) return;
	int pIndex = i;
	int ns = POINTERS[i].num_of_seg;				//���������� �� ������� ��������� ��������� ���������
	for (; i < pIndex + ns; i++)					//��������� �����
	{
		MASK[i] = 0;
	}
	for (i = 0; i < ns * SEG_SIZE; i++)				//�������� ���� ������
	{
		((char*)p)[i] = 0;
	}
	POINTERS[pIndex].p = nullptr;					//�������� ���������
	POINTERS[pIndex].num_of_seg = 0;
}

//������� ��������� ������� ���������� ������
void* Realloc(void* old_ptr, unsigned int size)
{
	int i = 0;
	for (; i < MASK_SIZE; i++)						//���� ������ ��������� � ������� ����������
	{
		if (POINTERS[i].p == old_ptr) break;
	}
	int old_size = POINTERS[i].num_of_seg * SEG_SIZE;	//������ ������ ����������� ����� ������
	int old_num_of_seg = POINTERS[i].num_of_seg;	//������ ���������� ���������
	int pIndex = i;
	if (old_size == size) return old_ptr;			//���� ����������� ������� �� ������, ������� ����, �� return
	if (old_size < size)							//���� ���� ��������� ������ ����� ������
	{
		char* newMem = (char*)Malloc(size);			//�������� �����(�������) ����� ������
		if (newMem == nullptr) return nullptr;
		for (i = 0; i < old_size; i++)				//������������ ���������� �� ������� ����� ������
		{
			newMem[i] = ((char*)old_ptr)[i];
		}
		Free(old_ptr);								//����������� ������ ����� ������
		return newMem;
	}
	else {											//���� ���� �������� ����� ������
		int new_num_of_seg;
		float n = (float)size / SEG_SIZE;
		if (n == (int)size / SEG_SIZE) new_num_of_seg = (int)n;	//���������� ����� ���������� ���������
		else new_num_of_seg = n + 1;
		char* delPtr = ((char*)old_ptr + new_num_of_seg*SEG_SIZE);	//��������� �� ��������� ����� ������
		for (int j = 0; j < (old_num_of_seg - new_num_of_seg)*SEG_SIZE; j++)	//������� ������ ������
		{
			delPtr[j] = 0;
		}
		for (int j = i + new_num_of_seg; j < i + old_num_of_seg; j++)	//��������� �����
		{
			MASK[j] = 0;
		}
		POINTERS[pIndex].num_of_seg = new_num_of_seg;					//��������� ���������� �� ���������
		return old_ptr;
	}
}

//������� ������ ������
void show_mem()
{
	printf("Mask: ");
	for (int i = 0; i < MASK_SIZE; i++)
		printf("%d", MASK[i]);
	printf("\n");

	printf("Memory:\n");
	for (int i = 0; i < MEMORY_SIZE; i++)
	{
		if (i % SEG_SIZE == 0 && i) printf("\n");
		if (MEMORY[i] == 0) printf("%d", MEMORY[i]);
		else printf("%c", MEMORY[i]);
	}
	printf("\n");
}

int main()
{
	init();
	setlocale(LC_ALL, "Rus");
	puts("��������� string1(32 �����) � �����������: ");
	char* str1 = (char*)Malloc(32);
	strcpy(str1, "FIRST STRING1111111111111111111");
	puts(str1);
	puts("��������� string2(16 ����) � �����������: ");
	char* str2 = (char*)Malloc(16);
	strcpy(str2, "2SECOND STRING2");
	puts(str2);
	puts("����� ������:");
	show_mem();
	puts("��������� ������ string1 �� 16 ����");
	str1 = (char*)Realloc(str1, 16);
	puts("���������� ������ string2 �� 78 ����");
	str2 = (char*)Realloc(str2, 78);
	strcpy(str2, "SECOND STRING 222222222222222222222222222222222222222222222222222222222222222");
	puts("����� ������ string2:");
	puts(str2);
	puts("����� ������:");
	show_mem();
	puts("�������� ����� string1 � string2");
	Free(str1);
	Free(str2);
	puts("����� ������:");
	show_mem();
	system("pause");
	return 0;
}