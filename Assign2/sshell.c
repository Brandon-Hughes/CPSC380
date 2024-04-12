/// @brief Creates a shell script
/// @file sshell.c
/// @author Brandon Hughes
/// @course CPSC-380-01 Operating Systems
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80

int main(void) {
    char *args[MAX_LINE/2 + 1];
    int should_run = 1;

    while(should_run){

        printf("osh>");
        fflush(stdout);

        //splitting up the input line into an array
        int num_args = 0;
        char buffer[MAX_LINE];
        fgets(buffer, MAX_LINE, stdin);
        char *token = strtok(buffer, " \n");
        while (token != NULL) {
            args[num_args] = malloc(strlen(token) + 1);
            strcpy(args[num_args], token);
            num_args++;
            token = strtok(NULL, " \n");
        }

        //tests if there are any arguments or input if not go back to beginning
        if (num_args == 0){
            continue;
        }

        //tests "exit" to exit the program
        if(strcmp(args[0], "exit") == 0){
            should_run = 0;
            for(int i = 0; i < num_args; i++){
                free(args[i]);
            }
            break;
        }

        //removes the extra new line character
        args[num_args] = NULL;
        int executeNum;
        //forking the command into a child to tests input
        if(args[0] != NULL || (strcmp(args[0], " ") != 0)){
            pid_t pid;
            pid = fork();
            if (pid < 0){
                printf("Fork Failed");
                return 1;
            // if amphersand is included force the parent to continue
            } else if ((strcmp(args[num_args-1], "&")) == 0){
                args[num_args-1] = NULL;
                if(pid == 0){
                    executeNum = execvp(args[0], args);
                    if (executeNum == -1){
                        printf("Error Executing Command\n");
                    }
                    should_run = 0;
                }
            } else {
                if(pid == 0){
                    executeNum = execvp(args[0], args);
                    if (executeNum == -1){
                        printf("Error Executing Command\n");
                    }
                } else {
                    wait(NULL);
                }
            }
        }

        //frees the array at the end to use the right amount of storage again
        for(int i = 0; i < num_args; i++){
                free(args[i]);
        } 

    }
    return 0;
}