#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUMBERS 5

sem_t sem;
int total_sum;
int status; // For the child process status

void* factorial(void* arg){
    int *temp = (int *) arg; // Pass int to temp
    int fac = 1;
    for (int i = *temp; i > 0; --i)
        fac *= i; // Calc factorial
    sem_wait(&sem); // semaphores
    total_sum += fac;
    sem_post(&sem); // semaphores
    printf("Factorial of %d = %d\n", *temp, fac);
    return 0;
}

int main(void) {
    int in[NUMBERS];

    // Prompt 5 numbers and assign to in[5] and save to file
    FILE *f = fopen("numbers.txt", "w");
    for (int i = 0; i < NUMBERS; i++) {
      printf("Enter a number: ");
      scanf("%d", &in[i]);
      fprintf(f, "%d\n", in[i]);
    }
    fclose(f);

    // Fork
    pid_t pid;
    pid = fork();
    if (pid < 0) // If fails
      return 1;

    sem_init(&sem, 0, 1); // semaphores

    if (pid == 0) { // Child
      int in2[NUMBERS];

      // Read from file and save to in2
      FILE *fp = fopen("numbers.txt", "r");
      for (int i = 0; i < NUMBERS; i++)
        fscanf(fp, "%d\n", &in2[i]);
      fclose(fp);

      // 5 threads
      pthread_t tid[NUMBERS];
      for (int i = 0; i < NUMBERS; i++)
        pthread_create(&tid[i], NULL, factorial, &in2[i]); // Only pass one #

      // Wait threads to join
      for (int i = 0; i < NUMBERS; i++)
        pthread_join(tid[i], NULL);

      sem_destroy(&sem); // semaphores

      // Write total_sum to file
      FILE *fp2 = fopen("sum.txt", "w");
      fprintf(fp2, "%d", total_sum);
      fclose(fp2);

      return 0;
    }
    else { // Parent
      waitpid(pid, &status, 0); // Wait for child process to terminate
      if (status == 0) {
        // Read the file and print
        int total;
        FILE *fp3 = fopen("sum.txt", "r");
        fscanf(fp3, "%d", &total);
        printf("Total sum = %d\n", total);
      }
    }

    return 0;
}
