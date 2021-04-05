#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>

#define INTERP_BUFSIZE 64

//Считывание команды из консоли
char *read_command(void)
{
  char* line = NULL; // Буфер для содержимого строки
  ssize_t line_size = 0; // Размер буфера
  
  //Считывание содержимого из консоли
  getline(&line, &line_size, stdin);
  
  return line;
}

// Разбиение строки на элементы
char** interpretation_command(char* line)
{
  int bufsize = INTERP_BUFSIZE;
  int position = 0; // Положение элемента в строке
  char** items = malloc(bufsize * sizeof(char*)); // Массив элементов строки
  char* item; // Элемент строки
  
  // Проверка на выделение памяти под массив элементов строки
  if(!items)
  {
    fprintf(stderr, "Ошибка выделения памяти\n");
    exit(EXIT_FAILURE);
  }
  
  // Выделение элемента строки
  item = strtok(line, " \t\r\n\a");
  
  // Разбиваем строку
  while(token != NULL)
  {
    // Добавляем элемент строки в массив
    items[position] = item;
    position++;
    
    // Если привысили текущий размер буфера
    if(position >= bufsize)
    {
      // Увеличиваем  размер массива
      bufsize += INTERP_BUFSIZE;
      
      // Перераспределяем блоки памяти
      items = realloc(items, bufsize * sizeof(char*));
      
      // Проверка на выделение памяти под массив элементов строки
      if(!items)
      {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
      }
    }
    
    // Выделяем новый элемент
    item = strtok(NULL, " \t\r\n\a");
  }
  
  items[position] = NULL;
  return tokens;
}
















