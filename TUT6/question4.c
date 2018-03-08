#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define NUMBERS 10
int buffer[5] = {0,0,0,0,0};
sem_t sem;

void* producer(void* arg) {
  int *temp = (int *) arg; // Pass array to temp
  int index = 0;
  int count = 0;

  while (index < NUMBERS) {
    sleep(rand()%1 + 1); // Sleep 1-2 sec
    if (count < 6) {
      sem_wait(&sem); // sem
      if (buffer[count] == 0) {
        printf("Produced %d\n", temp[index]);
        buffer[count] = temp[index];
        index++;
      }
      else
        count++;
      sem_post(&sem); // sem
    }
    else
      count = 0;
  }
  return 0;
}

void* consumer() {
  int index = 0;
  int count = 0;

  while (index < NUMBERS) {
    sleep(rand()%1 + 1); // Sleep 1-2 sec
    if (count < 6) {
      sem_wait(&sem); // sem
      if (buffer[count] != 0) {
        printf("Consumed %d\n", buffer[count]);
        buffer[count] = 0;
        index++;
      }
      else
        count++;
      sem_post(&sem); // sem
    }
    else
      count = 0;
  }
  return 0;
}

int main(void) {
    // For random sleep time
    srand ( time(NULL) );

    // Scan numbers
    int in[NUMBERS];
    for (int i = 0; i < NUMBERS; i++) {
        printf("Enter a number: ");
        scanf("%d", &in[i]);
    }

    sem_init(&sem, 0, 1);
    pthread_t tid[2];
    pthread_create(&tid[0], NULL, producer, &in);
    pthread_create(&tid[1], NULL, consumer, NULL);

    for (int i = 0; i < 2; i++)
        pthread_join(tid[i], NULL);
    sem_destroy(&sem);

    // Print the buffer at the end
    printf("Buffer: ");
    for (int i = 0; i < 5; i++)
        printf("%d ", buffer[i]);
    printf("\n");

    return 0;
}
