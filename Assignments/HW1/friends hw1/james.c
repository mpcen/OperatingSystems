#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token sparators


/*----------------------------------------------------------------------------------------
 *   Arguments:   (char *):  Shell command to execute
 *                (char **): Arguments to cmd command
 *
 *      Return:   (int)      Returns the child_pid of the spawned process
 * Description:   This function spawns a new process via `fork` which returns
 *                the child process's ID to he parent, while
 *                returning a process ID of 0 to the child process (child_pid).
 *                This value is used by the function to either return the child_pid
 *                or exec a cmd via execvp
 *----------------------------------------------------------------------------------------*/
int spawn( char * cmd, char ** arg_list );
//void handler(int sigal_number);
int run(char ** args);
int background(char ** args);
int murder(pid_t pid);
int a2i(char *s);

//sig_atomic_t atomic = 0 ;
//sig_atomic_t ChildExitStatus = 0 ;


int main (int argc, char ** argv){
    char buf[MAX_BUFFER];                      // line buffer
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;                               // working pointer thru args
    char prompt[] = "==> " ;                    // shell prompt
    int child_pid ;
/*
    // set up the signal
    struct sigaction sa ;          // sigaction sets the signal disposition
    memset(&sa, 0, sizeof(sa));    // Set memory area sa with size of sa to 0
    sa.sa_handler = &handler;      // Give struct member address to function to execute
    sigaction(SIGCHLD, &sa, NULL); // Signal SIGCHLD to parent after exec handler when child process completes
*/
    // while not EOF (from input stream) or 'quit'
    while (!feof(stdin)) {

        // Print a prompt and wait (fgets) until input
        fputs (prompt, stdout);                 // prompt > stdout
        if (fgets (buf, MAX_BUFFER, stdin )) {  // read from input until '\n' or max buffer size reached

            arg = args;                         // use arg to iterate through args so args pointer not lost

            /* -----------------------------------------------------------------------------------
             *    strtok:  From the Docs
             * -----------------------------------------------------------------------------------
             * On the first call to strtok() the string to be parsed should be specified in str.
             * In each subsequent call that should parse the same string, str must be NULL.      */

            // Tokenize buf at the separator and assign to arg.
            // Each call to strtok returns a word until none left; return NULL
            *arg++ = strtok(buf,SEPARATORS);
            while ((*arg++ = strtok(NULL,SEPARATORS)));

            // If anything was read other than newline/whitespace/tab
            if (args[0]) {

                /* check for commands [clear, quit, run, background] */
                if (strcmp(args[0], "clear") == 0 ) {                // "clear" command
                    system("clear"); // call the shell cmd
                    continue;
                }
                else if ( strcmp(args[0],"quit") == 0 ) break;        // "quit"  command

                else if ( strcmp(args[0],"run") == 0 ){               // "run"   command
                    // if command was supplied
                    if ( args[1] ) {
                        child_pid = run(args);
                        fprintf(stdout, "Exited with: %d", child_pid);
                    }
                    // else tell user we need cmd arg
                    else{
                        fputs("Requires command to run", stderr);
                        continue ;
                    }
                }

                else if ( strcmp(args[0],"background") == 0 ){        // "background"   command
                    // if command was supplied
                    if ( args[1] ){
                        pid_t child_pid = background(args);
                        fprintf(stdout, "Started with: %d", child_pid);
                    }

                    // else tell user we need cmd arg
                    else{
                        fputs("Requires command to run", stderr);
                        continue ;
                    }
                }
                else if ( strcmp(args[0],"murder") == 0 ){              // "murder" command
                    if ( args[1] ) {
                        int stat = murder(atoi(args[1]));
                        if ( stat != 0 ){
                            fprintf(stdout, "Murdah'd %s", args[1]);
                        }
                    }
                    else{
                        fputs("Requires command to run", stderr);
                        continue ;
                    }
                }

                else{                                                   // any other commands get echoed
                    arg = args;                                         // Move ptr back to front of array
                    while (*arg) fprintf(stdout,"%s ",*arg++);          // Print *arg until null
                }

                fputs ("\n", stdout);                                   // print a newline and repeat
            }
        }
    }
    return 0;
}

/*
void handler(int signal_number){
    int exitStatus = 0 ;
    wait(&exitStatus);
    ChildExitStatus = exitStatus;
    fprintf(stdout, "Process exited");
}*/

int murder(pid_t pid){
    int stat = kill(pid, SIGTERM);
    if ( stat != 0 ) {
        fprintf(stdout, "Error\n");
        return 0;
    }
    return stat ;
}
int run(char ** args){
    int child_wait;
    int child_pid = spawn( *(args+1), args+1 );
    int rtn ;

    wait(&child_wait);
    if ( WIFEXITED (child_wait) ){
        rtn = WEXITSTATUS(child_wait);
    }
    return child_pid;
}

pid_t background(char ** args){
    pid_t child_pid = spawn( *(args+1), args+1 );
    return ( child_pid != 0 ) ? child_pid : -1 ;
}


pid_t spawn( char * cmd, char ** arg_list ){
    // Declare and initialize.
    pid_t child_pid = fork(); // Fork a process

    /* check who's who and react accordingly */
    if ( child_pid != 0 ){
        return child_pid ;
    }
    else{
        execvp(cmd, arg_list);
        // if control reached here, there was an error.
        fprintf(stderr, "Error occurred using execvp\n");
        abort();
        return 0;
    }
}