#include "navigation.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#ifndef MAX_BUFFER
#define MAX_BUFFER 200
#endif

void print_working_directory() {
    char path[MAX_BUFFER];
    getcwd(path, MAX_BUFFER);
    printf("%s\n", path);
}

void change_directory(char* directory) {
    struct stat sb;

    if (stat(directory, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        chdir(directory);
    } else {
        printf("cd: %s: No such file or directory\n", directory);
    }
}