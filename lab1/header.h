#pragma once
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN64)
	#include <Windows.h>
#endif

struct Info
    {
#if defined(_WIN32) || defined(_WIN64)
        STARTUPINFO stInfo;
        PROCESS_INFORMATION prInfo;
#else
        pid_t pid;
#endif
    };

int createProc(struct Info*);
void deleteProc(struct Info*);