#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<pthread.h>




void* bellcurve(int* grade) {
    printf("Made it into bellcurve\n");
    double newMark = *grade * 1.50;
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

    printf("Got out.\n");

    for (int i=0; i<num_students; i++){
        printf("Trying to create\n");
        pthread_create(&tid[i], NULL, bellcurve(&(grades[i])), NULL);
        printf("Created\n");
    }

    for (int i = 0; i < num_students; i++) {
        printf("Joining");
        pthread_join(tid[i], NULL);
    }

    

    return 0;
}