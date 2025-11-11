#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "navigation.h"

char *concat(const char *s1, const char *s2) {
  char *concat_string = malloc(strlen(s1) + strlen(s2) + 2);
  strcpy(concat_string, s1);
  strcat(concat_string, "/");
  strcat(concat_string, s2);
  return concat_string;
}

bool is_executable(char *exe) {
  struct stat st;
  if (stat(exe, &st) != 0) return false;
  if (!S_ISREG(st.st_mode)) return 0;
  return access(exe, X_OK) == 0;
}

char *handle_path_search(char *command) {
  char *path_env = getenv("PATH");
  char *path = strdup(path_env);

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
  char *found = NULL;

  for (int i = 0; dirs[i] != NULL; i++) {
    DIR *dp;
    dp = opendir(dirs[i]);
    struct dirent *ep;
    if (dp != NULL) {
      while ((ep = readdir(dp)) != NULL) {
        if (strcmp(command, ep->d_name) == 0) {
          // printf("found: %s\n", dirs[i]);
          found = concat(dirs[i], ep->d_name);
          if (is_executable(found)) {
            free(path);
            return found;
          }
        }
      }
      (void)closedir(dp);
    }
  }
  free(path);
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
    if (found_path != NULL) {
      printf("%s is %s\n", command, found_path);
    } else {
      printf("%s: not found\n", command);
    }
  }
}


int handle_execs(char* exe, char *args) {
  char* exec_args[1024];

  char* program_name = strrchr(exe, '/');
  program_name = program_name ? program_name + 1 : exe;

  exec_args[0] = malloc(strlen(program_name) + 1);
  strcpy(exec_args[0], program_name);
  int num_args = 1;

  char* token = strtok(args, " ");
  while (token != NULL) {
    exec_args[num_args] = malloc(strlen(token) + 1);
    strcpy(exec_args[num_args], token);
    num_args++;
    token = strtok(NULL, " ");
  }
  exec_args[num_args] = NULL;


  pid_t child = fork();
  // printf("args %s\n", args);
  if (child == 0) {
    execvp(exe, exec_args);
    perror("execvp error");
    exit(EXIT_FAILURE);
  }
  if (child > 0) {
    wait(NULL);
    //printf("back to parent");
  } else {
    perror("fork error");
    exit(EXIT_FAILURE);
  }
  return 0;
}

int main(void) {
  setbuf(stdout, NULL);
  char *builtin_commands[] = {"echo", "exit", "type", "pwd"};

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
    } else if (strcmp(token, "pwd") == 0) {
      print_working_directory();
    } else if (strcmp(token, "type") == 0) {
      int length = sizeof(builtin_commands) / sizeof(builtin_commands[0]);
      handle_type(args, builtin_commands, length);
    } else {
      char* found = handle_path_search(command);
      if (found != NULL) {
        handle_execs(found, args);
      } else {
        printf("%s: command not found\n", token);
      }
    }
  }
  return 0;
}