#include "Header.h"
struct Info info;											//���������� ���������, ������� ������ ������ ���������� �����

//������������� ����������
void initDir(struct Dir* dptr, struct Dir* prev, const char* name)
{
	dptr->previous = prev;									//���������� ��������� �� ���������� ����������
	strcpy(dptr->name, name);								//���������� ��� ����������
	dptr->dirs = NULL;										//��������� ��������� �� ������ ����������
	dptr->num_of_dirs = 0;
	dptr->num_of_files = 0;
	for (int i = 0; i < MAX_NUM_OF_FILES; i++)				//���������� ���������� � ������
	{
		dptr->filesInfo[i].position = -1;
		dptr->filesInfo[i].size = -1;
		strcpy(dptr->filesInfo[i].name, "");
	}
}

//���������� ����������(������������ ��������: 1 - �������, 0 - ������� ���������� �������� max ����� ����������,
//-1 - ����� ���������� ��� ����������)
int addDir(struct Dir* dptr, const char* n)
{
	for (int i = 0; i < dptr->num_of_dirs; i++)				//�������� �� ��, ���������� �� ���������� � ����� ������
	{
		if (!strcmp(dptr->dirs[i].name, n)) return -1;
	}
	if ((dptr->num_of_dirs + 1) <= 5)
	{
		if (dptr->num_of_dirs == 0)							//��������� ������, ���� ���������� ���� 0
		{
			dptr->dirs = (struct Dir*)malloc(sizeof(struct Dir));
		}
		else
		{													//������������� ������
			dptr->dirs = (struct Dir*)realloc(dptr->dirs, (dptr->num_of_dirs + 1) * sizeof(struct Dir));
		}
		initDir(&(dptr->dirs[dptr->num_of_dirs]), dptr, n);	//������������� ����� ����������
		dptr->num_of_dirs++;								//��������� ����� ���������� ������ ����������
	}
	else
	{
		return 0;
	}
}

//������� ������ ���������� � ����������
void showDir(struct Dir* dptr)
{
	for (int i = 0; i < dptr->num_of_dirs; i++)				//����� ��������� �������� ��������
	{
		printf("dir: %s\n", dptr->dirs[i].name);
	}
	for (int i = 0; i < dptr->num_of_files; i++)			//����� ������ �������� ��������
	{
		if (dptr->filesInfo[i].position >= 0)
		{
			printf("file: %s\n", dptr->filesInfo[i].name);
		}
	}
}

//������ ���������� � �������� ������� � ����(�� ������������)
void writeInfoInFile()
{
	FILE* FS = fopen("FS", "r+b");
	fwrite(&info, sizeof(struct Info), 1, FS);
	fclose(FS);
}

//������ ���������� � �������� ������� �� �����(�� ������������)
void readInfoFromFile()
{
	FILE* FS = fopen("FS", "rb");
	fread(&info, sizeof(struct Info), 1, FS);
	fclose(FS);
}

//������ ���������� � ����
void writeInFile(struct Dir* root)
{
	FILE* FS = fopen("FS", "r+b");
	if (!FS)
	{
		puts("ERROR");
		exit(1);
	}
	fwrite(&info, sizeof(struct Info), 1, FS);				//������� ����� ����������� ������ �� sizeof( struct Info)(fseek �� ��������)
	fwrite(root, sizeof(struct Dir), 1, FS);				//������ � ���� ��������� ��������
	fwrite(root->dirs, sizeof(struct Dir), root->num_of_dirs, FS);	//������ ��������� ��������� �������� � ����
	struct Dir* level = root->dirs;							//��������� �� ������ ������� ������ ������ ���������
	struct Dir* next_level = NULL;							//��������� �� ������ ���������� ������ ������ ���������
	int level_size = root->num_of_dirs;						//������ �������� ������ � ������ ���������
	int next_level_size = 0;								//������ ���������� ������ � ������ ���������
	int cur_dir_num = 0;									//������� ��� ������ ��������� �� ��������� �������
	while (1)
	{
		for (int i = 0; i < level_size; i++)				//�������� �� �������� ������ � ������ ���������
		{
			if (level[i].num_of_dirs > 0)					//���� � �������� �� �������� ������ ���� �����������, ��		
			{												//��������� �� � ����. �������
				if (next_level_size == 0)
				{
					next_level = (struct Dir*)malloc(sizeof(struct Dir)*level[i].num_of_dirs);
					next_level_size = level[i].num_of_dirs;
				}
				else {
					cur_dir_num = next_level_size;			//����� �������
					next_level_size += level[i].num_of_dirs;
					next_level = (struct Dir*)realloc(next_level, next_level_size * sizeof(struct Dir));
				}
				for (int j = 0; cur_dir_num < next_level_size; cur_dir_num++, j++)	//���������� ��������� �������� ������
					next_level[cur_dir_num] = level[i].dirs[j];				//� ������ ��������� ����. ������ 
			}
		}
		if (next_level_size == 0)							//���� ���������� ������ ���, �� ����� ������
		{
			break;
		}
		level = next_level;									//������ ��������� ������� ����� ����������
		level_size = next_level_size;
		fwrite(level, sizeof(struct Dir), level_size, FS);	//������� �������� ������ � ����
		next_level_size = 0;								//��������� ��������� ������
		next_level = NULL;
		cur_dir_num = 0;
	}
	fclose(FS);
	FS = fopen("FS", "ab");									//��������� ��������� � ���������, �.� fseek �� ��������
	fseek(FS, 0, SEEK_SET);
	fwrite(&info, sizeof(struct Info), 1, FS);				//������ ���������� � �������� ������� � ����
	fclose(FS);
}

//������ ���������� �� �����
void readFromFile(struct Dir* root)
{
	FILE* FS = fopen("FS", "rb");
	if (FS == NULL)											//���� ����� �� �������, �� �������������� �������� �������
	{
		initDir(root, NULL, "root");
		return;
	}
	long int size = filelength(fileno(FS));					//���� ���� 0-�� �������, �� �������������� �������� �������
	if (size == 0)
	{
		initDir(root, NULL, "root");
		fclose(FS);
		return;
	}
	fread(&info, sizeof(struct Info), 1, FS);				//������ ���������� � �������� ��������
	fread(root, sizeof(struct Dir), 1, FS);					//������ ��������� ��������
	if (root->num_of_dirs > 0)								//���� � ��������� �������� ���� �����������, �� ������ ��
	{
		root->dirs = (struct Dir*)malloc(sizeof(struct Dir)*root->num_of_dirs);	//��������� ������ �� �����������
		fread(root->dirs, sizeof(struct Dir), root->num_of_dirs, FS);			//��������������� ������ ������������
	}
	else
	{
		return;
	}
	for (int i = 0; i < root->num_of_dirs; i++)				//���� ������ ������������ �� �� ���������� �������
	{
		root->dirs[i].previous = root;
	}
	struct Dir** level;										//������� ������� � ������ ���������(������ ���������� �� ��������)
	level = (struct Dir**)malloc(sizeof(struct Dir*)*root->num_of_dirs);
	for (int i = 0; i < root->num_of_dirs; i++)				//�������������� ������� ������� �������������
	{
		level[i] = &(root->dirs[i]);
	}
	struct Dir** next_level = NULL;							//��������� ������� � ������ ���������
	int level_size = root->num_of_dirs;
	int next_level_size = 0;
	int cur_dir_num = 0;									//���������� ����� � ������� ��������� ������
	while (1)
	{
		for (int i = 0; i < level_size; i++)				//������ �� �������� ������
		{
			if ((*level[i]).num_of_dirs > 0)				//���� � �������� �������� ������ ���� �����������, �� ��������� �� �
			{												//������ ���������� ������ ������
				(*level[i]).dirs = (struct Dir*)malloc(sizeof(struct Dir)*(*level[i]).num_of_dirs);
				fread((*level[i]).dirs, sizeof(struct Dir), (*level[i]).num_of_dirs, FS);	//������ ������������
				for (int k = 0; k < (*level[i]).num_of_dirs; k++)	//���������� �� ������������
				{
					(*level[i]).dirs[k].dirs = NULL;		//�������� �� ��������� �� �� �����������
					(*level[i]).dirs[k].previous = level[i];//���� �� ��������� �� ���������� �������
				}
				if (next_level_size == 0)					//���� ���� ������� ��� ����, �� ��������� ������
				{
					next_level = (struct Dir**)malloc(sizeof(struct Dir)*(*level[i]).num_of_dirs);
					next_level_size = (*level[i]).num_of_dirs;
				}
				else {										//���� ����. ������� �� ���� �� ������������� ������
					cur_dir_num = next_level_size;			//����� �������
					next_level_size += (*level[i]).num_of_dirs;
					next_level = (struct Dir**)realloc(next_level, next_level_size * sizeof(struct Dir*));
				}											//���������� ������������ � ������ ���������� ������
				for (int j = 0; cur_dir_num < next_level_size; cur_dir_num++, j++)
					next_level[cur_dir_num] = &((*level[i]).dirs[j]);
			}
		}
		if (next_level_size == 0)							//���� ����. ������� ����, �� ����� ������
		{
			break;
		}
		level = next_level;									//���������� � ��������� ����. ������
		level_size = next_level_size;
		next_level_size = 0;
		next_level = NULL;
		cur_dir_num = 0;
	}
	fclose(FS);
}

//����� ������ ���������(�� ������������)
void showDirTree(struct Dir* root)
{
	puts(root->name);
	for (int i = 0; i < root->num_of_dirs; i++)
	{
		printf("%s\t", root->dirs[i].name);
	}
	printf("\n");
	struct Dir* level = root->dirs;
	struct Dir* next_level = NULL;
	int level_size = root->num_of_dirs;
	int next_level_size = 0;
	int cur_dir_num = 0;
	while (1)
	{
		for (int i = 0; i < level_size; i++)
		{
			if (level[i].num_of_dirs > 0)
			{
				//fwrite(&(level[i]), sizeof( struct Dir), level[i].num_of_dirs, FS);
				if (next_level_size == 0)
				{
					next_level = (struct Dir*)malloc(sizeof(struct Dir)*level[i].num_of_dirs);
					next_level_size = level[i].num_of_dirs;
				}
				else {
					cur_dir_num = next_level_size;											//����� �������
					next_level_size += level[i].num_of_dirs;
					next_level = (struct Dir*)realloc(next_level, next_level_size * sizeof(struct Dir));
				}
				for (int j = 0; cur_dir_num < next_level_size; cur_dir_num++, j++)
					next_level[cur_dir_num] = level[i].dirs[j];
			}
		}
		if (next_level_size == 0)
		{
			break;
		}
		level = next_level;
		level_size = next_level_size;
		for (int i = 0; i < level_size; i++)
		{
			printf("%s\t", level[i].name);
		}
		printf("\n");
		next_level_size = 0;
		next_level = NULL;
		cur_dir_num = 0;
	}
}

//��������� �������� ����(����������)(���������� ������ � �����)
char* getCurrentPath(struct Dir* dir)
{
	static char path[1000];
	if (dir->previous != NULL)								//���� �� ����� �� ��������� ��������, ��
	{
		strcat(getCurrentPath(dir->previous), dir->name);	//���������� ����� ������� ���������� � ������ �����, ������������ 
		strcat(path, "/");									//��� ����� ������� �������
	}
	else
	{														//���� ����� �� ��������� ��������, �� ������ ��� ����� � ����
		strcpy(path, dir->name);
		strcat(path, "/");
	}
	return path;
}

//�������� ����������(����������)
void delDir(struct Dir* pdir)
{
	if (pdir->dirs != NULL)
	{
		for (int i = 0; i < pdir->num_of_dirs; i++)			//����� �������� ��� ���� ������������
			delDir(&(pdir->dirs[i]));
	}
	//�������� ������ ����������(����� �� ������, �.�. � ������ ���������� ��� ������ ���������� � �����, ��� ���� ����������)
	//(��� �������� ����� ������ �� ������������)
	int j = 0;
	struct Dir* prevDirPtr = pdir->previous;				//��������� ��������� �� ���������� �������
	for (; j < prevDirPtr->num_of_dirs; j++)				//����� ������� �������� �� ����������� ��������
	{
		if (!strcmp(pdir->name, prevDirPtr->dirs[j].name)) break;
	}
	if (j == prevDirPtr->num_of_dirs)
	{
		puts("DELETE ERROR");
		return;
	}
	while (j < (prevDirPtr->num_of_dirs - 1))				//�������� ���� ������������ � ����������� �������� �����, ����� �������
	{														//���������� � ������ ��������
		prevDirPtr->dirs[j] = prevDirPtr->dirs[j + 1];
		j++;
	}
	prevDirPtr->num_of_dirs--;
	prevDirPtr->dirs = (struct Dir*)realloc(prevDirPtr->dirs, sizeof(struct Dir)*prevDirPtr->num_of_dirs);	//������������� ������
}

//������ �����(����������� ��������: 1 - �������, 0 - ������ ����� ���)
int readFile(struct Dir* pdir, const char* n, char* buf)
{
	int i = 0;
	for (; i < pdir->num_of_files; i++)						//����� ����� � ������� ������
	{
		if (!strcmp(pdir->filesInfo[i].name, n)) break;
	}
	if (i == pdir->num_of_files)
	{
		return 0;
	}
	FILE* FS = fopen("FS", "r+b");
	fsetpos(FS, &(pdir->filesInfo[i].position));			//����������� ��������� � "�����" �� ����� ������ �����
	fread(buf, sizeof(char), SIZE_OF_FILE, FS);				//������ ����� ���������
	fclose(FS);
	return 1;
}

//������ ������ � ����(������������ ��������: 1 - �������, 0 - ������ ����� ���)
int writeDataInFile(struct Dir* pdir, const char* n, const char* buf)
{
	int i = 0;
	for (; i < pdir->num_of_files; i++)						//����� ����� � ������� ������
	{
		if (!strcmp(pdir->filesInfo[i].name, n)) break;
	}
	if (i == pdir->num_of_files)
	{
		return 0;
	}
	FILE* FS = fopen("FS", "r+b");
	fsetpos(FS, &(pdir->filesInfo[i].position));			//����������� ��������� � "�����" �� ����� ������ �����
	fwrite(buf, sizeof(char), SIZE_OF_FILE, FS);			//������ ���������� � ����
	fclose(FS);
	return 1;
}

//���������� �����(������������ ��������: 1 - �������, 0 - ����� ���, -1 - ���� � ����� ������ ��� ����������,
//-2 - � ���������� ��� ������������ ���������� ������)
int addFile(struct Dir* pdir, const char* n)
{
	if (info.begin_of_free_space + SIZE_OF_FILE < SIZE_OF_THE_DISK)
	{
		int i = 0;
		for (; i < pdir->num_of_files; i++)
		{
			if (!strcmp(pdir->filesInfo[i].name, n)) return -1;
		}
		if (pdir->num_of_files == MAX_NUM_OF_FILES) return -2;
		FILE* FS = fopen("FS", "r+b");
		fsetpos(FS, &info.begin_of_free_space);				//���������� ��������� � "�����" �� ������ ���������� �����
		fseek(FS, SIZE_OF_FILE, SEEK_CUR);
		fgetpos(FS, &info.begin_of_free_space);				//��������� ��������� �� ����� ��������� �����
		pdir->filesInfo[pdir->num_of_files].position = info.begin_of_free_space;
		pdir->filesInfo[pdir->num_of_files].size = SIZE_OF_FILE;
		strcpy(pdir->filesInfo[pdir->num_of_files].name, n);
		pdir->num_of_files++;
		fclose(FS);
		char* zeroMem = (char*)calloc(SIZE_OF_FILE, sizeof(char));
		writeDataInFile(pdir, n, zeroMem);					//��������� ������
		free(zeroMem);
		return 1;
	}
	else return 0;
}

//�������� �����(������������ ��������: 1 - �������, 0 - ������ ����� ���)
int delFile(struct Dir* pdir, const char* n)
{
	int i = 0;
	for (; i < pdir->num_of_files; i++)						//���� ���� � ����������
	{
		if (!strcmp(pdir->filesInfo[i].name, n)) break;
	}
	if (i == pdir->num_of_files)
	{
		return 0;
	}
	while (i < (pdir->num_of_files - 1))					//�������� ���� � ����� � ������� �� 1 �����, ����� ������� ����������
	{														//�� ��������� �����
		pdir->filesInfo[i] = pdir->filesInfo[i + 1];
		i++;
	}
	strcpy(pdir->filesInfo[i].name, "");
	pdir->filesInfo[i].position = -1;
	pdir->filesInfo[i].size = -1;
	pdir->num_of_files--;
	return 1;
}

int main()
{
	struct Dir root;										//������ ������ ���������
	readFromFile(&root);									//������ �������� �� ����� � �����
	struct Dir* curDir = &root;								//������������� ��������� �� ������� ����������
	char buf[MAX_NAME];
	while (1)
	{
		printf("%s", getCurrentPath(curDir));
		gets_s(buf, MAX_NAME);								//���� �������
		if (!strcmp(buf, "exit")) break;
		else if (!strcmp(buf, "dir")) showDir(curDir);
		else if (!strcmp(buf, "addDir"))
		{
			gets_s(buf, MAX_NAME);
			int n =	addDir(curDir, buf);
			switch (n)
			{
			case 1: break;
			case 0: puts("this directory is full");
				break;
			case -1: puts("directory with this name has already exists");
				break;
			}
		}
		else if (!strcmp(buf, "..") && curDir->previous) curDir = curDir->previous;
		else if (!strcmp(buf, "delDir"))
		{
			gets_s(buf, MAX_NAME);
			int i = 0;
			for (; i < curDir->num_of_dirs; i++)
			{
				if (!strcmp(curDir->dirs[i].name, buf)) break;
			}
			if (i == curDir->num_of_dirs)
			{
				strcat(buf, " no such directory");
				puts(buf);
			}
			else
			{
				delDir(&(curDir->dirs[i]));
			}
		}
		else if (!strcmp(buf, "addFile"))
		{
			gets_s(buf, MAX_NAME);
			int n;
			n = addFile(curDir, buf);
			switch (n)
			{
			case 1: break;
			case 0: puts("There is no free space");
				break;
			case -1: puts("File with this name has already exists");
				break;
			case -2: puts("This directory is full");
				break;
			}
		}
		else if (!strcmp(buf, "showFile"))
		{
			char fileData[SIZE_OF_FILE];
			gets_s(buf, MAX_NAME);
			if (readFile(curDir, buf, fileData) == 0)
			{
				strcpy(buf, "no such file");
				puts(buf);
			}
			else
				puts(fileData);
		}
		else if (!strcmp(buf, "writeInFile"))
		{
			char fileData[SIZE_OF_FILE];
			printf("Enter name: ");
			gets_s(buf, MAX_NAME);
			printf("Enter data: ");
			gets_s(fileData, MAX_NAME);
			if (!writeDataInFile(curDir, buf, fileData))
			{
				puts("There is no such file");
			}
		}
		else if (!strcmp(buf, "delFile"))
		{
			gets_s(buf, MAX_NAME);
			if (!delFile(curDir, buf))
			{
				puts("There is no such file");
			}
		}
		else
		{
			int i = 0;
			for (; i < curDir->num_of_dirs; i++)
			{
				if (!strcmp(curDir->dirs[i].name, buf)) break;
			}
			if (i == curDir->num_of_dirs)
			{
				strcat(buf, " is wrong command");
				puts(buf);
			}
			else
			{
				curDir = &(curDir->dirs[i]);
			}
		}

	}
	writeInFile(&root);										//������ ���������� � ����
	system("pause");
	return 0;
}