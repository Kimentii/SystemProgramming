#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <io.h>
#define MAX_NAME 255							//������������ ��� ����� ��� ����������
#define MAX_NUM_OF_DIRS 5						//������������ ���������� ����������
#define MAX_NUM_OF_FILES 10						//������������ ���������� ������
#define SIZE_OF_FILE 64							//������ 1-�� �����
#define SIZE_OF_THE_DISK 200000					//������ ������������ �����

struct Info										//���������� � ���, ��� ���������� ��������� ����� �� �����
{
	fpos_t begin_of_free_space;					//������� � �����, ����������� �� ������ ���������� �����
};

struct FileInfo									//���������� � �����
{
	char name[MAX_NAME];						//��� �����
	fpos_t position;							//������� ������ ����� � �������� �������
	int size;									//������ �����
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