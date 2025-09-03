#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024    //chars in a line
#define MAX_ARGS 64       //max args in a command

int main(){
    char input[MAX_INPUT];         // buffer to store what the user types
    char *args[MAX_ARGS];          // array of strings (each word of the command)
    pid_t pid;                     
    int status;                    //to store exit status of child process

    while(1){
        //prompt
        printf("myshell> ");
        fflush(stdout);            //makes sure the text shows immediately, doesn't wait in buffer

        //read input
        if(fgets(input, sizeof(input), stdin) == NULL){      //fgets --> reads what u type (until enter), stores it in input
            perror("fgets failed");
            exit(1);
        }

        //remove newline at the end
        input[strcspn(input, "\n")] = 0;

        //exit command
        if(strcmp(input, "exit") == 0){             //strcmp compares strings
            break;
        }

        //parse input into args
        int i = 0;
        char *token = strtok(input, " ");
        while(token != NULL && i < MAX_ARGS - 1){
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;    //null terminate the args array

        if(args[0] == NULL){
            continue;  //empty command
        }

        //handle "cd" as a built-in
        if (strcmp(args[0], "cd") == 0){
            if (args[1] == NULL){
                //no path provided --> go to home directory
                char *home = getenv("HOME");
                if (home != NULL){
                    chdir(home);
                }
            }else{
                //change the specified directory
                if(chdir(args[1]) != 0){
                    perror("cd failed");
                }
            }
            continue; //skip fork/exec for cd 
        }

        //fork and exec
        pid = fork();
        if(pid == 0){
            //child process
            execvp(args[0], args);           
            perror("exec failed");  //if execvp returns, there was an error
            exit(1);
        }else if(pid > 0){
            //parent process
            waitpid(pid, &status, 0);
        }else{
            perror("fork failed");
        }
    }

    return 0;
}