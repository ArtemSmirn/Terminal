#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define INTERP_BUFSIZE 64
#define ENTER 13
#define SIZE_PATH 256 

pid_t pid;  // Идентификатор родительского процесса
pid_t wpid; // Идентификатор дочернего процесса
char click;

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
  while(item != NULL)
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
  return items;
}

// Запуск процессов
int start_process(char** args)
{
  int status;

  //Создание нового процесса
  pid = fork();

  // Если дочерний процесс  
  if(pid == 0)
  {
    // Запуск новой команды
    if(execvp(args[0], args) == -1)
    {
      perror("ter");
    }
    
    exit(EXIT_FAILURE);
  } 
  else if(pid < 0)
  {
    perror("ter");
  }
  else
  {
    // Родительский процесс
    do
    {
      // Ожидание состояния процесса
      wpid = waitpid(pid, &status, WUNTRACED);
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  
  return 1;
}

// Реализация комады cd
int ter_cd(char** args)
{
  if(args[1] == NULL)
  {
    printf("Error: ожидается аргумент");
  }
  else
  {
    if(chdir(args[1]) != 0)
    {
      printf("Error: не удалось осуществить переход");
    }
    return 1;
  }
}

// Реализация команды exit
int ter_exit(char** args)
{
  return 0;
}

char* embed_command[] = {"cd", "exit"};

int (* fun_command[])(char **) = {&ter_cd, &ter_exit};

// Работа с процессами
int join_procces(char** args)
{
  int i;
  
  // Вводится пустая команда
  if(args[0] == NULL)
  {
    return 1;
  }
  
  // Поиск втроенной команды (cd и exit)
  for(i=0; i < (sizeof(embed_command) / sizeof(char* )); i++)
  {
    if(strcmp(args[0], embed_command[i]) == 0)
    {
      return(*fun_command[i])(args);
    }
  }
  
  return start_process(args);
}


void ouch(int sig)
{
  if(pid == 0)
  {
    // Передача сигнала процессу
    kill(pid, sig);
  }
  
  printf("\n");
  click = getchar();
}

void terminal(void)
{
  char* line;  // Указатель на введенную строку 
  char** args; // Указатель на элементы строки
  int status;  //
  char Path[SIZE_PATH]; // Массив для указания пути

  struct sigaction act;
  act.sa_handler = ouch;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  do
  {
    // Определение и вывод текущего пути
    getcwd(Path, sizeof(Path));
    printf("~%s $", Path);

    // Обработка прерывания при получении сигнала
    sigaction(SIGINT, &act, 0);
    if(click == ENTER)
    {
      printf("~%s $", Path);
    }
    
    // Обработка введенной команды 
    line = read_command();
    args = interpretation_command(line);
    status = join_procces(args);

    // Очищение выделенной памяти
    free(line);
    free(args);
  }while(status);
}

int main(int argc, char** argv)
{
  terminal();
  
  return EXIT_SUCCESS;
}














