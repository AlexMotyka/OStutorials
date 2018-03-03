#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int main(void)
{
    pid_t pid;
    
    
    pid = fork();

    if(pid==0){
        printf("Child process\n");
        exit(0);
    }else{
        int returnStatus;
        waitpid(pid, &returnStatus, 0);

        if (returnStatus==0){
            printf("The child process terminated correctly.\n");
        }

        if (returnStatus==1){
            printf("The child process terminated with an error.\n");
        }
    }

    return 0;

    
}