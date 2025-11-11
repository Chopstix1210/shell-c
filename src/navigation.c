#include "navigation.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#ifndef MAX_BUFFER
#define MAX_BUFFER 200
#endif

void print_working_directory() {
    char path[MAX_BUFFER];
    getcwd(path, MAX_BUFFER);
    printf("%s\n", path);
}

void change_directory(char* directory) {

    char home_var = directory ? directory[0] : '\0';
    printf("This is the first alpha: %c\n", home_var);

    struct stat sb;
    if (home_var == '~') {
        if (strlen(directory) == 1) {
            chdir(getenv("HOME"));
        } else {
           printf("not implemented yet\n");
        }
    } else if (home_var == '.') {
       if (directory[1] == '.') {
           if (chdir("..") != 0) {
               perror("cd ..");
           }
       }
    }
    else if (stat(directory, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        chdir(directory);
    } else {
        printf("cd: %s: No such file or directory\n", directory);
    }
}