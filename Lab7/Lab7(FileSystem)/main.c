//������ ������������ ����� = 200000 ����. ��������� ���������� �������� � ������ 100000 ������.
//���������� 100000 ���� ����������� �������.
#include "Header.h"
struct Info info;											//���������� ���������, ������� ������ ������ ���������� �����
struct Buffer buffer;

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
		dptr->filesInfo[i].num_of_segments = -1;
		dptr->filesInfo[i].begin_segment = -1;
		strcpy(dptr->filesInfo[i].name, "");
	}
}

void initInfo()
{
	for (int i = 0; i < MASK_SIZE; i++)
	{
		info.MASK[i] = 0;
	}
}

void showMask()
{
	printf("%d", info.MASK[0]);
	for (int i = 1; i < MASK_SIZE; i++)
	{
		if (i % 100 == 0) printf("\n");
		printf("%d", info.MASK[i]);
	}
	printf("\n");
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
	fwrite(&info, sizeof(struct Info), 1, FS);				//������ ���������� � �������
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
	for (int i = 0; i < pdir->num_of_files; i++)			//�������� ������ ����������
	{
		delFile(pdir, pdir->filesInfo[i].name);
	}
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
	fread(buf, sizeof(char), SIZE_OF_SEGMENT*pdir->filesInfo[i].num_of_segments, FS);	//������ ����� ���������
	fclose(FS);
	return 1;
}

//������ ������ � ����(������������ ��������: 1 - �������, 0 - ������ ����� ���)
int writeDataInFile(struct Dir* pdir, const char* name, const char* buf)
{
	int i = 0;
	for (; i < pdir->num_of_files; i++)						//����� ����� � ������� ������
	{
		if (!strcmp(pdir->filesInfo[i].name, name)) break;
	}
	if (i == pdir->num_of_files)
	{
		return 0;
	}
	if (pdir->filesInfo[i].num_of_segments*SIZE_OF_SEGMENT < strlen(buf))
	{
		delFile(pdir, name);
		addFile(pdir, name, strlen(buf));
	}
	for (i =0; i < pdir->num_of_files; i++)					//����� ����� � ������� ������
	{
		if (!strcmp(pdir->filesInfo[i].name, name)) break;
	}
	FILE* FS = fopen("FS", "r+b");
	fsetpos(FS, &(pdir->filesInfo[i].position));			//����������� ��������� � "�����" �� ����� ������ �����
	fwrite(buf, sizeof(char), strlen(buf), FS);				//������ ���������� � ����
	fclose(FS);
	return 1;
}

//���������� �����(������������ ��������: 1 - �������, 0 - ����� ���, -1 - ���� � ����� ������ ��� ����������,
//-2 - � ���������� ��� ������������ ���������� ������)
int addFile(struct Dir* pdir, const char* n, int fileSize)
{
	for (int i = 0; i < pdir->num_of_files; i++)
	{
		if (!strcmp(pdir->filesInfo[i].name, n)) return -1;
	}
	if (pdir->num_of_files == MAX_NUM_OF_FILES) return -2;
	int segNum;										//���������� ���������� ���������
	float num = (float)fileSize / SIZE_OF_SEGMENT;
	if (num == (int)fileSize / SIZE_OF_SEGMENT) segNum = (int)num;
	else segNum = num + 1;
	int memBeg, memEnd;								//����� ���������� �������� � �������� ����� ��������� ��������
	int i = 0;
	//����� �� ���� ������(�������� ������ ����������, �.�. � ������ ������ ������ ������ ���������� ����� ������)
	while (i < MASK_SIZE)
	{
		while (info.MASK[i] == 1 && i < MASK_SIZE) i++;	//������� ������� ���������
		if (i == MASK_SIZE) return 0;
		memBeg = i;										//��������� ������ ����������� ��������
		int sn = segNum;
		while ((info.MASK[i] == 0) && (i < MASK_SIZE) && (sn > 0))	//��������, ���������� �� ������� ����� ������ ������
		{
			sn--;
			i++;
		}
		if (i == MASK_SIZE && sn > 0) return 0;
		if (sn > 0) continue;
		else
		{
			memEnd = i;								//����� �������� ���������� �� ��������� ��������� �����. ������
			break;
		}
	}
	for (i = memBeg; i < memEnd; i++)				//��������� �����
	{
		info.MASK[i] = 1;
	}
		FILE* FS = fopen("FS", "r+b");
		fpos_t startFileMem = BEGIN_OF_FILES + memBeg*SIZE_OF_SEGMENT;
		fsetpos(FS, &startFileMem);				//���������� ��������� � "�����" �� ������ ���������� �����
		char* zeroMem = (char*)calloc(SIZE_OF_SEGMENT * segNum, sizeof(char));
		fwrite(zeroMem, sizeof(char), SIZE_OF_SEGMENT * segNum, FS);
		free(zeroMem);
		fclose(FS);
		pdir->filesInfo[pdir->num_of_files].position = startFileMem;
		pdir->filesInfo[pdir->num_of_files].size = fileSize;
		pdir->filesInfo[pdir->num_of_files].num_of_segments = segNum;
		pdir->filesInfo[pdir->num_of_files].begin_segment = memBeg;
		strcpy(pdir->filesInfo[pdir->num_of_files].name, n);
		pdir->num_of_files++;
		return 1;
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
	int segStart = pdir->filesInfo[i].begin_segment;
	int segEnd = segStart + pdir->filesInfo[i].num_of_segments;
	for (int j = segStart; j < segEnd; j++)					//���������� �����
	{
		info.MASK[j] = 0;
	}
	while (i < (pdir->num_of_files - 1))					//�������� ���� � ����� � ������� �� 1 �����, ����� ������� ����������
	{														//�� ��������� �����
		pdir->filesInfo[i] = pdir->filesInfo[i + 1];
		i++;
	}
	strcpy(pdir->filesInfo[i].name, "");
	pdir->filesInfo[i].position = -1;
	pdir->filesInfo[i].size = -1;
	pdir->filesInfo[i].num_of_segments = -1;
	pdir->filesInfo[i].begin_segment = -1;
	pdir->num_of_files--;
	return 1;
}

//�������� ���� � �������� �������.(����������: 1 - �������, 0 - ������ ����� ���,
//-1 - �� ������� ������� ���� � �������� �������, -2 - ������ ��������� ������)
int writeFileInFileSystem(struct Dir* pdir, char* path, char* fileName)
{
	int i = 0;
	for (; i < pdir->num_of_files; i++)						//���� ���� � ����������
	{
		if (!strcmp(pdir->filesInfo[i].name, fileName)) break;
	}
	if (i == pdir->num_of_files)
	{
		return 0;
	}
	int fileSize = pdir->filesInfo[i].num_of_segments*SIZE_OF_SEGMENT;
	char* buf = (char*)malloc(sizeof(char)*fileSize);
	if (!buf) return -2;
	if (!readFile(pdir, fileName, buf))
	{
		return 0;
	}
	char* fullPath[BUFFER_SIZE];
	strcpy(fullPath, path);
	strcat(fullPath, "/");
	strcat(fullPath, fileName);
	FILE* file = fopen(fullPath, "wb");
	if (!file) return -1;
	fwrite(buf, sizeof(char), fileSize, file);
	fclose(file);
	free(buf);
	return 1;
}

//��������� ���� �� �������� �������.(������������ ��������: 1 - �������, 0 - ����� ���,
//-1 - ���� � ����� ������ ��� ����������, -2 - � ����������� ��� ������������ ���������� ������,
//-3 - ���� � �������� ������� �� ��������,
int readFileFromFileSystem(struct Dir* pdir, char* path, char* fileName)
{
	char fullPath[BUFFER_SIZE];
	strcpy(fullPath, path);
	strcat(fullPath, "/");
	strcat(fullPath, fileName);
	FILE* file = fopen(fullPath, "rb");
	if (!file) return -3;
	int fileSize = filelength(fileno(file));
	char* buf = (char*)malloc(sizeof(char)*fileSize+1);
	buf[fileSize] = 0;
	fread(buf, sizeof(char), fileSize, file);
	fclose(file);
	int err = addFile(pdir, fileName, fileSize+1);
	if (err <= 0) return err;
	writeDataInFile(pdir, fileName, buf);
	free(buf);
	return 1;
}

//�������� ���� � ������(������������ ��������: 1 - �������, 0 - ������ ����� ���, -1 - ������ ��������� ������)
int copy(struct Dir* pdir, char* fileName)
{
	int i = 0;
	for (; i < pdir->num_of_files; i++)						//���� ���� � ����������
	{
		if (!strcmp(pdir->filesInfo[i].name, fileName)) break;
	}
	if (i == pdir->num_of_files)
	{
		return 0;
	}
	buffer.bufferSize = pdir->filesInfo[i].num_of_segments*SIZE_OF_SEGMENT;
	buffer.fileInfo = (char*)malloc(sizeof(char)*buffer.bufferSize);
	if (!buffer.fileInfo) return -1;
	readFile(pdir, fileName, buffer.fileInfo);
	strcpy(buffer.fileName, fileName);
	return 1;
}

//��������� ���� � �����������(������������ ��������: 1 - �������, 0 - ����� ���, -1 - ���� � ����� ������
//��� ����������, -2 - � ����������� ��� ������������ ���������� �����, -3 - ������ ����, -4 - ��������������
//������)
int paste(struct Dir* pdir)
{
	if (buffer.fileInfo == NULL || strlen(buffer.fileName) == 0 || buffer.bufferSize ==0) return -3;
	int err = addFile(pdir, buffer.fileName, buffer.bufferSize);
	if (err <= 0) return err;
	err = writeDataInFile(pdir, buffer.fileName, buffer.fileInfo);
	if (!err) return -4;
	strcpy(buffer.fileName, "");
	free(buffer.fileInfo);
	buffer.fileInfo = NULL;
	return 1;
}

int main()
{
	struct Dir root;										//������ ������ ���������
	readFromFile(&root);									//������ �������� �� ����� � �����
	//��� ������� ������� ������������(� ����������� ������� ����):
	//initDir(&root, NULL, "root");
	//initInfo();
	struct Dir* curDir = &root;								//������������� ��������� �� ������� ����������
	strcpy(buffer.fileName, "");
	buffer.fileInfo = NULL;
	buffer.bufferSize = 0;
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
			int n = addDir(curDir, buf);
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
			n = addFile(curDir, buf, SIZE_OF_SEGMENT);
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
			char fileData[100000];
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
			char fileData[SIZE_OF_SEGMENT];
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
		else if (!strcmp(buf, "writeFileInFileSystem"))
		{
			char* path[BUFFER_SIZE];
			int err;
			puts("Write name of file:");
			gets_s(buf, MAX_NAME);
			puts("Write path:");
			gets_s(path, BUFFER_SIZE);
			err = writeFileInFileSystem(curDir, path, buf);
			if (err == 0) puts("There is no such file");
			else if (err == -1) puts("Can't create file in file system");
		}
		else if (!strcmp(buf, "readFileFromFileSystem"))
		{
			char path[BUFFER_SIZE];
			int err;
			puts("Write path to directory:");
			gets_s(path, BUFFER_SIZE);
			puts("Write name of file:");
			gets_s(buf, MAX_NAME);
			err = readFileFromFileSystem(curDir, path, buf);
			switch (err)
			{
			case 1:puts("File added");
				break;
			case 0:puts("There is no free space");
				break;
			case -1:puts("There is file with this name");
				break;
			case -2:puts("Directory is full");
				break;
			case -3:puts("Can't open file in file system");
				break;
			}
		}
		else if (!strcmp(buf, "copy"))
		{
			puts("Enter file name");
			gets_s(buf, BUFFER_SIZE);
			if (!copy(curDir, buf))
				puts("There is no such file");
		}
		else if (!strcmp(buf, "paste"))
		{
			int err = paste(curDir);
			switch (err)
			{
			case 1: break;
			case 0: puts("There is no empty space");
				break;
			case -1: puts("There is such file");
				break;
			case -2: puts("Directory is full");
				break;
			case -3: puts("Buffer is empty");
				break;
			case -4: puts("unidentifed error");
				break;
			}
		} else if (!strcmp(buf, "showMask"))
		{
			showMask();
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
	if (!buffer.fileInfo) free(buffer.fileInfo);
	system("pause");
	return 0;
}