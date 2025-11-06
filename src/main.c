#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *concat(const char *s1, const char *s2) {
  char *concat_string = malloc(strlen(s1) + strlen(s2) + 2);
  strcpy(concat_string, s1);
  strcat(concat_string, "/");
  strcat(concat_string, s2);
  return concat_string;
}

char *handle_path_search(char *command) {
  char *path = getenv("PATH");

  char *dirs[1024];
  char delimiter[] = ":";
  int num_dirs = 0;
  char *token = strtok(path, delimiter);
  while (token != NULL) {
    dirs[num_dirs] = malloc(strlen(token) + 1);
    strcpy(dirs[num_dirs], token);
    num_dirs++;
    token = strtok(NULL, delimiter);
  }
  dirs[num_dirs] = "\0";
  char *found = "\0";

  for (int i = 0; dirs[i] != NULL; i++) {
    DIR *dp;
    dp = opendir(dirs[i]);
    struct dirent *ep;
    if (dp != NULL) {
      while ((ep = readdir(dp)) != NULL) {
        if (strcmp(command, ep->d_name) == 0) {
          // printf("found: %s\n", dirs[i]);
          found = concat(dirs[i], ep->d_name);
          break;
        }
      }
      if (strcmp(found, "\0") != 0) {
        break;
      }
      (void)closedir(dp);
    }
  }
  return found;
}

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
  for (int i = 0; i < length; i++) {
    if (strcmp(command, builtin_list[i]) == 0) {
      found = 1;
      break;
    }
  }

  if (found == 1) {
    printf("%s is a shell builtin\n", command);
  } else {
    char *found_path = handle_path_search(command);
    if (strcmp(found_path, "\0") != 0) {
      printf("%s is %s\n", command, found_path);
    } else {
      printf("%s: not found\n", command);
    }
  }
}

int main(void) {
  setbuf(stdout, NULL);
  char *builtin_commands[] = {"echo", "exit", "type"};

  while (true) {
    printf("$ ");
    char command[1024];
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = '\0';

    char *token = strtok(command, " ");
    if (!token)
      continue;

    char *args = strtok(NULL, "");

    if (strcmp(token, "exit") == 0) {
      handle_exit(args);
    } else if (strcmp(token, "echo") == 0) {
      handle_echo(args);
    } else if (strcmp(token, "type") == 0) {
      int length = sizeof(builtin_commands) / sizeof(builtin_commands[0]);
      handle_type(args, builtin_commands, length);
    } else {
      printf("%s: command not found\n", token);
    }
  }
  return 0;
}
