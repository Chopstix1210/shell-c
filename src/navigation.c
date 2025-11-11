#include "navigation.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef MAX_BUFFER
#define MAX_BUFFER 200
#endif

void print_working_directory()
{
    char path[MAX_BUFFER];
    getcwd(path, MAX_BUFFER);
    printf("%s\n", path);
}