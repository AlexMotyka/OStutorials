#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* hello_world() {
  sleep(1);
  printf("hello world\n");
  return NULL;
}

void* goodbye() {
  sleep(1);
  printf("goodbye\n");
  return NULL;
}

int main() {
  pthread_t tid[2];

  for (int i = 0; i < 2; i++) {
    if (i == 0)
      pthread_create(&tid[i], NULL, hello_world, NULL);
    else
      pthread_create(&tid[i], NULL, goodbye, NULL);
  }

  for (int i = 0; i < 2; i++) {
    pthread_join(tid[i], NULL);
  }

  return 0;
}