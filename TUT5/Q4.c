#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int total_grade = 0;
pthread_mutex_t mutex;

void* class_total(void *arg) {
  int value = * (int*) arg;
  pthread_mutex_lock(&mutex); // lock
  total_grade += value;
  pthread_mutex_unlock(&mutex); // unlock
  return NULL;
}

int main() {
  pthread_t tid[10];

  for (int i = 0; i < 10; i++) {
    int grade;
    printf ("Enter grade for student %d: ", i);
    scanf("%d", &grade);
    pthread_create(&tid[i], NULL, class_total, &grade);
  }

  for (int i = 0; i < 10; i++) {
    pthread_join(tid[i], NULL);
  }

  printf("Total grade: %d\n", total_grade);

  return 0;
}
