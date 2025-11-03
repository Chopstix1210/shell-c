#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void handle_echo(char *args) {
    if (args)
        printf("%s\n", args);
}

void handle_exit(char *args) {
    int code = args ? atoi(args) : 0;
    exit(code);
}

void handle_type(char *command, char *builtin_list[], int length) {
    int found = 0;
    for (int i=0; i < length; i++) {
        if (strcmp(command, builtin_list[i]) == 0) {
            found = 1;
            break;
        }
    }

    if (found == 1) {
        printf("%s is a shell builtin\n", command);
    } else {
        printf("%s: not found\n", command);
    }
}

int main(void) {
    setbuf(stdout, NULL);
    char *builtin_commands[] = {"echo", "exit", "type"};

    while (true) {
        printf("$ ");
        char command[1024];
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // remove newline

        char *token = strtok(command, " ");
        if (!token) continue;

        char *args = strtok(NULL, ""); // everything after first token

        if (strcmp(token, "exit") == 0) {
            handle_exit(args);
        } else if (strcmp(token, "echo") == 0) {
            handle_echo(args);
        } else if (strcmp(token, "type") == 0) {
            int length = sizeof(builtin_commands) / sizeof(builtin_commands[0]);
            handle_type(args, builtin_commands, length);
        }else {
            printf("%s: command not found\n", token);
        }
    }
    return 0;
}
