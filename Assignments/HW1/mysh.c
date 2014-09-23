/*
 * Emmanuel Martinez
 * PID: e2904661
 * NID: em170306
 * University of Central Florida
 * Operating Systems [COP4600] Fall 2014
 * Homework Assignment 1: Create a Linux shell that replaces the current one
 *
 * */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_BUFFER 1024                         // max line buffer
#define MAX_ARGS 64                             // max # args
#define SEPARATORS " \t\n"                      // token separators

/*
 *
*/
int runProcess(char **args);
void welcome();

int main (int argc, char ** argv){
  welcome();
  printf("\n\n");
  char buf[MAX_BUFFER];                         // line buffer
  char *args[MAX_ARGS];                         // pointers to arg strings
  char **arg;                                   // working pointer through args
  char *prompt = "# ";                         // shell prompt
  int returnCode, killer;

  /* Keep reading input until "quit" command or EOF of redirected input */

  while (!feof(stdin)){

    // get command line from input

    fputs (prompt, stdout);                     // write prompt
    if (fgets (buf, MAX_BUFFER, stdin)){        // read a line

      // tokenize the input into args array

      arg = args;
      *arg++ = strtok(buf, SEPARATORS);         // tokenize input

      while ((*arg++ = strtok(NULL,SEPARATORS)))
        ; // Last entry will be NULL

      if(args[0]){                              // if there's anything there

        // check for internal/external command

        if(strcmp(args[0], "clear") == 0){          // "clear" command
          system("clear");
          continue;
        }

        else if(strcmp(args[0], "quit") == 0)            // "exit" command
          break;

        else if(strcmp(args[0], "run") == 0){
          if(args[1]){
            returnCode = runProcess(args);

            printf("Child exited with PID of: %d", returnCode);
          }
          else{
            printf("Enter a command and try again\n");
            continue;
          }
        }

        // background
        else if(strcmp(args[0], "background") == 0){
          if(args[1]){
            returnCode = background(args);
            printf("PID Started: %d\n", returnCode);
          }
          else{
            printf("Enter a command and try again\n");
            continue;
          }
        }

        // Murder
        else if(strcmp(args[0], "murder") == 0){
          if(args[1]){
            killer = murder(atoi(args[1]));
            if(killer != 0)
              printf("Process %s killed", args[1]);
          }
          else{
            printf("Enter a command and try again\n");
            continue;
          }

        }

        // else pass command onto OS and reset arg
        else{
          arg = args;
          while (*arg)
            fprintf(stdout, "%s is not a valid statement.\n"
                "Try using 'run' followed by the command.\n", *arg++);

        }

        printf("\n");

      }

    }

  }

  return 0;

}

int runProcess(char **args){
  int pid;
  int foo;
  int waiter;

    // fork the process
    pid = fork();

    // if parent
    if(pid != 0){
      wait(&waiter);

      if(WIFEXITED (waiter))
        foo = WEXITSTATUS(waiter);

      return pid;
    }
    // else child
    else{
      execvp(*(args+1), args+1);
      abort();
      return 0;
    }
  }

int background(char **args){
  int pid;
  int foo;
  int waiter;

  pid = fork();

  if(pid != 0)
    return pid;

  else{
    execvp(*(args+1), args+1);
    printf("Error executing\n");
    abort();
    return -1;
  }
}

int murder(int pid){
  int killer;
  killer = kill(pid, SIGTERM);
  if(killer != 0){
    printf("Error executing\n");
    return 0;
  }
  printf("Process killed\n");
  return killer;
}

void welcome(){
  printf("Welcome to Manny's Shell!\n"
      "Use 'run <command> <arg1> <arg2> ... <argn>' to execute commands\n"
      "Example: run mkdir ./myNewFolder\n"
      "Use 'clear' to clear shell\n"
      "Use 'quit' to close shell\n");
}
