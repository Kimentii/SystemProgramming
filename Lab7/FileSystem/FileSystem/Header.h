#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <io.h>
#define MAX_NAME 255							//Максимальное имя файла или директории
#define MAX_NUM_OF_DIRS 5						//Максимальное количество директорий
#define MAX_NUM_OF_FILES 10						//Максимальное количество файлов
#define SIZE_OF_SEGMENT 100						//Размер 1-го файла
#define SIZE_OF_THE_DISK 200000					//Размер эмулируемого диска
#define BEGIN_OF_FILES (SIZE_OF_THE_DISK/2)
#define MASK_SIZE (SIZE_OF_THE_DISK - BEGIN_OF_FILES)/SIZE_OF_SEGMENT
#define BUFFER_SIZE 512
#pragma warning(disable: 4996)

struct Info										//Информация о том, где начинается свободное место на диске
{
	fpos_t begin_of_free_space;					//Позиция в файле, указывающая на начало свободного места
	char MASK[MASK_SIZE];
};

struct FileInfo									//Информация о файле
{
	char name[MAX_NAME];						//Имя файла
	fpos_t position;							//Позиция начала файла в файловой системе
	int size;									//Размер файла
	int num_of_segments;
	int begin_segment;
};

struct Dir										//Директория
{
	char name[MAX_NAME];						//Имя директории
	struct Dir* previous;						//Ссылка на директорю, в которой находится текущая директория
	struct Dir* dirs;							//Директории, на которые ссылается текущая директория
	struct FileInfo filesInfo[MAX_NUM_OF_FILES];//Массив с информацией о файлах
	int num_of_dirs;							//Количество директорий в данной директории
	int num_of_files;							//Количество файлов в данной директории
};

struct Buffer
{
	char fileName[MAX_NAME];
	char* fileInfo;
	int bufferSize;
};