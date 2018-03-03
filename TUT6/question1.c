#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int main(void)
{

    FILE * fp;
    FILE * fp2;
    pid_t pid;
    pid_t pid2;
    int i;
    /* open the file for writing*/
    fp = fopen("child1.txt","w");

    fprintf(fp,"child 1\n");
    fclose(fp);

    fp2 = fopen("child2.txt","w");

    fprintf(fp2,"child 2 \n");
    fclose(fp2);
    
    pid = fork();
    

    if(pid==0){
        
        printf("child 1 reading... \n");
        fp = fopen("child1.txt","r");
        if (fp){
            while ((i = getc(fp)) != EOF)
                putchar(i);
            fclose(fp);
        }
        sleep(1000);
    }else{
        pid2 = fork();
        if(pid2==0){
            printf("child 2 reading... \n");
            fp2 = fopen("child2.txt","r");
            if (fp2){
                while ((i = getc(fp2)) != EOF)
                    putchar(i);
                fclose(fp2);
            }
            sleep(1000);
        }

    }

    
    

    return 0;

    
}