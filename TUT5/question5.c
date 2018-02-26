#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 10

int total_grade = 0;
double total_bellcurve = 0;
int grade_array[NUM_THREADS];

pthread_mutex_t mutex;
pthread_mutex_t mutex2;


void* save_bellcurve(void *arg) {
  int value = * (int*) arg;
  double newMark = value * 1.50;

  pthread_mutex_lock(&mutex); // lock
  total_grade += value;
  pthread_mutex_unlock(&mutex); // unlock

  pthread_mutex_lock(&mutex2); // lock
  total_bellcurve += newMark;
  pthread_mutex_unlock(&mutex2); // unlock

  FILE *fp;
  fp = fopen("bellcurve.txt", "a");
  fprintf(fp, "%.1f\n", newMark);

  return NULL;
}


void* read_grades() {
  FILE *f;
  f = fopen("grades.txt", "r");
  for (int i = 0; i < NUM_THREADS; i++)
      fscanf(f, "%d", &grade_array[i]);
  return NULL;
}


int main(){
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&mutex2, NULL);

  pthread_t read;
  pthread_create(&read, NULL, read_grades, NULL);
  pthread_join(read, NULL);

  pthread_t tid[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; i++)
    pthread_create(&tid[i], NULL, save_bellcurve, &grade_array[i]);
  for (int i = 0; i < NUM_THREADS; i++)
    pthread_join(tid[i], NULL);

  printf("Total grade before bellcurve: %d\n", total_grade);
  printf("Average grade before bellcurve: %f\n", (double) total_grade/NUM_THREADS);

  printf("Total grade after bellcurve: %f\n", total_bellcurve);
  printf("Average grade after bellcurve: %f\n", total_bellcurve/NUM_THREADS);

  return 0;
}
