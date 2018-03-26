#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main() {
	double t0, t1, t2, t3; // For timer
  srand(time(NULL)); // For random number

	int n = 100000000;
	int *x = malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		x[i] = rand() % 100 + 1; // 0~100

  // Serial
  int norm = 0;
	t0 = omp_get_wtime();
	for (int i = 0; i < n; ++i)
		norm += fabs(x[i]);
	t1 =  omp_get_wtime();

  // Parallel
	norm = 0;
  omp_set_num_threads(8); // Say 8 threads
	t2 = omp_get_wtime();
  #pragma omp parallel for reduction(+:norm)
	for (int i = 0; i < n; i++)
			norm += fabs(x[i]);
	t3 = omp_get_wtime();

  printf("Serial time = %lf\n", (t1 - t0));
	printf("Parallel time = %lf\n", (t3 - t2));

  return 0;
}
