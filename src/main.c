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

    // cmp known commands
    int exit_cmp = strcmp(token, "exit");
    int echo_cmp = strcmp(token, "echo");


    if (exit_cmp == 0) {
        int status = atoi(strtok(NULL, delimiter));
        exit(status);
    } else if (echo_cmp == 0) {
        char *token = strtok(NULL, "\n");
        printf("%s\n", token);
    } else {
        command[strcspn(command, "\n")] = '\0';
        printf("%s: command not found\n", command);
    }

  }
    return 0;
}
