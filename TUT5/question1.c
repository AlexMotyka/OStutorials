#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void* hello_world() {
  sleep(rand()%2 + 1); // Sleep 1-3 sec
  printf("hello world\n");
  return NULL;
}

void* goodbye() {
  sleep(rand()%2 + 1); // Sleep 1-3 sec
  printf("goodbye\n");
  return NULL;
}

int main() {
  srand ( time(NULL) );

  pthread_t tid[2];

  pthread_create(&tid[0], NULL, hello_world, NULL);
  pthread_create(&tid[1], NULL, goodbye, NULL);

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);

  return 0;
}
