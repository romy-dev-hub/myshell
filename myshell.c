#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main(){
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    pid_t pid;
    int status;

    while(1){
        //prompt
        printf("myshell> ");
        fflush(stdout);

        //read input
        if(fgets(input, sizeof(input), stdin) == NULL){
            perror("fgets failed");
            exit(1);
        }

        //remove newline at the end
        input[strcspn(input, "\n")] = 0;

        //exit command
        if(strcmp(input, "exit") == 0){
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