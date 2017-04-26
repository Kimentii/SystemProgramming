#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>
#pragma warning (disable:4996)
#define MASK_SIZE 16								//Размер маски
#define SEG_SIZE 16									//Размер одного сегмента
#define MEMORY_SIZE (MASK_SIZE*SEG_SIZE)			//Размер памяти
//MASK - маска секторов, один сектор равен 16 байтам
//MEMORY - сама память(256 байт)
struct pointer										//Структура для хранения указателей
{
	void* p;										//Сам указатель
	int num_of_seg;									//Размер, на который указывает указатель
};

char MASK[MASK_SIZE];								//Маска сегментов, 1 - занят, 0 - свободен
char MEMORY[MEMORY_SIZE];							//Сама память
struct pointer POINTERS[MASK_SIZE];					//Массив указателей на память

//Инициализация памяти
void init()
{
	for (int i = 0; i < MASK_SIZE; i++)				//Зануление маски и зануление указателей
	{
		MASK[i] = 0;
		POINTERS[i].p = nullptr;
		POINTERS[i].num_of_seg = 0;
	}
}

//Выделение памяти
void* Malloc(unsigned int size)
{
	int segNum;										//Количество выделяемых сегментов
	float n = (float)size / SEG_SIZE;
	if (n == (int)size / SEG_SIZE) segNum = (int)n;
	else segNum = n + 1;
	int memBeg, memEnd;								//Номер начального сегмента и сегмента после конечного сегмента
	int i = 0;
	//Поиск по всей памяти(алгоритм первый подходящий, т.е. с самого начала памяти ищется подходящий сегмент)
	while (i < MASK_SIZE)
	{
		while (MASK[i] == 1 && i < MASK_SIZE) i++;	//Пропуск занятых сегментов
		if (i == MASK_SIZE) return nullptr;
		memBeg = i;									//Запоинаем начало подходящего сегмента
		int sn = segNum;
		while ((MASK[i] == 0) && (i < MASK_SIZE) && (sn > 0))	//Проверка, достаточно ли бойшой кусок памяти найден
		{
			sn--;
			i++;
		}
		if (i == MASK_SIZE && sn > 0) return nullptr;
		if (sn > 0) continue;
		else
		{
			memEnd = i;								//Номер сегмента сдедующего за последним сегментом выдел. памяти
			break;
		}
	}
	for (i = memBeg; i < memEnd; i++)				//Обновляем маску
	{
		MASK[i] = 1;
	}
	POINTERS[memBeg].num_of_seg = segNum;			//Заносим указатель в массив указателей
	POINTERS[memBeg].p = (void*)(MEMORY + memBeg * SEG_SIZE);
	return (MEMORY + memBeg * SEG_SIZE);
}

//Очистить память
void Free(void* p)
{
	int i = 0;
	for (; i < MASK_SIZE; i++)						//Ищем указатель в массиве указателей
	{
		if (POINTERS[i].p == p)
		{
			break;
		}
	}
	if (i == SEG_SIZE) return;
	int pIndex = i;
	int ns = POINTERS[i].num_of_seg;				//Запоминаем на сколько сегментов указывает указатель
	for (; i < pIndex + ns; i++)					//Обновляем маску
	{
		MASK[i] = 0;
	}
	for (i = 0; i < ns * SEG_SIZE; i++)				//Зачищаем саму память
	{
		((char*)p)[i] = 0;
	}
	POINTERS[pIndex].p = nullptr;					//Зануляем указатель
	POINTERS[pIndex].num_of_seg = 0;
}

//Функция изменения размера выделенной памяти
void* Realloc(void* old_ptr, unsigned int size)
{
	int i = 0;
	for (; i < MASK_SIZE; i++)						//Ищем старый указатель в массиве указателей
	{
		if (POINTERS[i].p == old_ptr) break;
	}
	int old_size = POINTERS[i].num_of_seg * SEG_SIZE;	//Старый размер выделенного куска памяти
	int old_num_of_seg = POINTERS[i].num_of_seg;	//Старое количество сегментов
	int pIndex = i;
	if (old_size == size) return old_ptr;			//Если потребовали столько же памяти, сколько было, то return
	if (old_size < size)							//Если надо увеличить размер куска памяти
	{
		char* newMem = (char*)Malloc(size);			//Выделяем новый(большый) кусок памяти
		if (newMem == nullptr) return nullptr;
		for (i = 0; i < old_size; i++)				//Переписываем информацию из старого куска памяти
		{
			newMem[i] = ((char*)old_ptr)[i];
		}
		Free(old_ptr);								//Освобождаем старый кусок памяти
		return newMem;
	}
	else {											//Если надо обрезать кусок памяти
		int new_num_of_seg;
		float n = (float)size / SEG_SIZE;
		if (n == (int)size / SEG_SIZE) new_num_of_seg = (int)n;	//Определяем новое количество сегментов
		else new_num_of_seg = n + 1;
		char* delPtr = ((char*)old_ptr + new_num_of_seg*SEG_SIZE);	//Указатель на удаляемый кусок памяти
		for (int j = 0; j < (old_num_of_seg - new_num_of_seg)*SEG_SIZE; j++)	//Удаляем лишние данные
		{
			delPtr[j] = 0;
		}
		for (int j = i + new_num_of_seg; j < i + old_num_of_seg; j++)	//Обновляем маску
		{
			MASK[j] = 0;
		}
		POINTERS[pIndex].num_of_seg = new_num_of_seg;					//Изменения информации об указателе
		return old_ptr;
	}
}

//Функция вывода памяти
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
	puts("Выделение string1(32 байта) с информацией: ");
	char* str1 = (char*)Malloc(32);
	strcpy(str1, "FIRST STRING1111111111111111111");
	puts(str1);
	puts("Выделение string2(16 байт) с информацией: ");
	char* str2 = (char*)Malloc(16);
	strcpy(str2, "2SECOND STRING2");
	puts(str2);
	puts("Вывод памяти:");
	show_mem();
	puts("Обрезание памяти string1 до 16 байт");
	str1 = (char*)Realloc(str1, 16);
	puts("Увеличеник памяти string2 до 78 байт");
	str2 = (char*)Realloc(str2, 78);
	strcpy(str2, "SECOND STRING 222222222222222222222222222222222222222222222222222222222222222");
	puts("Новая строка string2:");
	puts(str2);
	puts("Вывод памяти:");
	show_mem();
	puts("Удаление строк string1 и string2");
	Free(str1);
	Free(str2);
	puts("Вывод памяти:");
	show_mem();
	system("pause");
	return 0;
}