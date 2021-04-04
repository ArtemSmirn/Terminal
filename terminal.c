#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>

//Считывание команды из консоли
char *read_command(void)
{
  char* line = NULL;
  ssize_t linesize = 0;
  getline(&line, &linesize, stdin);
  return line;
}
