#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <io.h>
#define MAX_NAME 255							//������������ ��� ����� ��� ����������
#define MAX_NUM_OF_DIRS 5						//������������ ���������� ����������
#define MAX_NUM_OF_FILES 10						//������������ ���������� ������
#define SIZE_OF_SEGMENT 100						//������ 1-�� �����
#define SIZE_OF_THE_DISK 200000					//������ ������������ �����
#define BEGIN_OF_FILES (SIZE_OF_THE_DISK/2)
#define MASK_SIZE (SIZE_OF_THE_DISK - BEGIN_OF_FILES)/SIZE_OF_SEGMENT
#define BUFFER_SIZE 512
#pragma warning(disable: 4996)

struct Info										//���������� � ���, ��� ���������� ��������� ����� �� �����
{
	fpos_t begin_of_free_space;					//������� � �����, ����������� �� ������ ���������� �����
	char MASK[MASK_SIZE];
};

struct FileInfo									//���������� � �����
{
	char name[MAX_NAME];						//��� �����
	fpos_t position;							//������� ������ ����� � �������� �������
	int size;									//������ �����
	int num_of_segments;
	int begin_segment;
};

struct Dir										//����������
{
	char name[MAX_NAME];						//��� ����������
	struct Dir* previous;						//������ �� ���������, � ������� ��������� ������� ����������
	struct Dir* dirs;							//����������, �� ������� ��������� ������� ����������
	struct FileInfo filesInfo[MAX_NUM_OF_FILES];//������ � ����������� � ������
	int num_of_dirs;							//���������� ���������� � ������ ����������
	int num_of_files;							//���������� ������ � ������ ����������
};

struct Buffer
{
	char fileName[MAX_NAME];
	char* fileInfo;
	int bufferSize;
};