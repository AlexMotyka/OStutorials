#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int moving_sum[5] = {0,0,0,0,0};

struct Data {
    int num;
    int index;
};

void *factorial(void *threadarg){
    struct Data *my_data;
    my_data = (struct Data *) threadarg;
    
    int n = my_data->num;
    int spot = my_data->index;
    int factorial=1;
    int i;

    for(i=1; i<=n; ++i){
            factorial *= i;              
        }
        printf("Factorial of %d = %d\n", n, factorial);
    moving_sum[spot]=factorial;
    while(moving_sum[spot-1]==0){
        //try to change it by checking the semaphore lock
        
    }
    
    return 0;
}
int main(void)
{
    pthread_t tid[5];
    
    struct Data *my_data = malloc(5);
    if(my_data==NULL)    
        exit(1);
    
    for (int j = 0; j < 5; j++) {
        printf("Enter a number: \n");
        scanf("%d", &my_data[j].num);
        my_data[j].index = j;
        pthread_create(&tid[j], NULL, factorial, &my_data[j]);
    }
    for (int i = 0; i < 5; i++)
        pthread_join(tid[i], NULL);
    
    //free(my_data);
    my_data = NULL;
    return 0;  
}

