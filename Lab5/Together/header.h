#if defined(_WIN32) || defined(_WIN64) 
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <locale.h>
#include "win_laba.h"

#else 
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <aio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include "laba_linux.h"
#endif 
