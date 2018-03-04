#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int moving_sum[5] = {0,0,0,0,0};

sem_t can_add;

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
    int previous = spot -1;
    

    for(i=1; i<=n; ++i){
            factorial *= i;              
    }
    
    if (spot == 0){
        moving_sum[spot] = factorial;
    }
    
    if(spot>0){
        if (moving_sum[previous]!=0){
            sem_wait(&can_add);
            moving_sum[spot] = moving_sum[previous] + factorial;
            sem_post(&can_add);
        }
        while(moving_sum[previous]==0){
            sem_wait(&can_add);
            moving_sum[spot] = moving_sum[previous] + factorial;
            sem_post(&can_add);
        }
    }
    
    return 0;
}
int main(void)
{
    pthread_t tid[5];
    
    if(sem_init(&can_add, 0, 1))
    {
        printf("Could not initialize a semaphore\n");
        return -1;
    }
    
    struct Data *my_data = malloc(5);
    if(my_data==NULL)    
        exit(1);
    
    for (int j = 0; j < 5; j++) {
        printf("Enter a number: \n");
        scanf("%d", &my_data[j].num);
        my_data[j].index = j;
        pthread_create(&tid[j], NULL, factorial, &my_data[j]);
    }
    for (int i = 0; i < 5; i++){
        if(pthread_join(tid[i], NULL)){
            printf("Could not join thread %d\n",i);
            return -1;
        }
    }
    
    for (int k = 0; k < 5; k++){
        printf("Element %d: %d\n",k,moving_sum[k]);
    }
    //free(my_data);
    my_data = NULL;
    return 0;  
}

