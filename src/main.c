#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (true) {
    printf("$ ");
    char command[1024];
    const char* delimiter = " ";

    fgets(command, sizeof(command), stdin);
    const char *token = strtok(command, delimiter);

    int exit_cmp = strcmp(token, "exit");
    if (exit_cmp == 0) {
        int status = atoi(strtok(NULL, delimiter));
        exit(status);
    }


    command[strcspn(command, "\n")] = '\0';
    printf("%s: command not found\n", command);

  }
    return 0;
}
