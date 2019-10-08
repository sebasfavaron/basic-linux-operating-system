#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#define MAX_ARGS 5

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int main(int argc, char *argv[]) {
  char * arguments[MAX_ARGS] = {0};
  char buffer[128] = {0};

  printf("shell-%d:~$> ", getpid());
  while(scanf("%s", buffer)) {
    int pid;
    int cantArgs = parseArguments(buffer, arguments);

    if (pid = run(buffer, cantArgs, arguments) == -1) {
      printf("%s: Invalid Module\n", arguments[0]);
    }

    printf("\nshell-%d:~$> ", getpid());
  }

  return 0;
}

int parseArguments(char * string, char * arguments[]) {
  int argc = 0;
  char * previous;
  char c;
  arguments[argc++] = string;
  while((c=*string) != ' ' && c != '\0')
    string++;
  *string = '\0';
  previous = ++string;

  if(c != '\0') {
    while((c=*string) != '\0') {
      if(c == ' ') {
        arguments[argc++] = previous;
        previous = string + 1;
        *string = '\0';
      }
      string++;
    }
    arguments[argc++] = previous;
  }
  return argc;
}
