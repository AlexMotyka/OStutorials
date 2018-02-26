#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<pthread.h>




void* bellcurve(void *arg) {
    double newMark = *(int*)arg * 1.50;
    printf("Your new mark is: %f \n", newMark);
    return NULL;

}

int main()
{
    pthread_t tid[5];
    int num_students = 5;
    int grades[5]; 

    for (int j = 0; j < num_students; j++) {
      printf("Enter mark: \n");
      scanf("%d", &grades[j]);
    }

    for (int k=0; k<num_students; k++){
        pthread_create(&tid[k], NULL, bellcurve, &grades[k]);
    }

    for (int i = 0; i < num_students; i++) {
        pthread_join(tid[i], NULL);
    }

    

    return 0;
}