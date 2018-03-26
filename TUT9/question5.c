#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Execute with a argument! (The number of threads)\n");
		exit(1);
	}

  int nthreads = atoi(argv[1]); // String to int
  #ifdef _OPENMP
  omp_set_num_threads(nthreads);
  #endif

  int n = 100000000;
  double dx = 1.0 / (n + 1);

	FILE *f = fopen("calculations.txt", "w");

  double x = 0, y = 0;
	#pragma omp parallel for private(x) private(y)
	for (int i = 0; i < n; i++){
		x = i * dx;
		y = exp(x) * cos (x) * sin (x) * sqrt (5 * x + 6.0);
		if ((i % 1000000) == 0) { // For every 1 millionth calculation
			#pragma omp critical
			fprintf(f, "i = %d | x = %lf | y = %lf\n", i, x, y);
		}
	}

	fclose(f);
	return 0;
}
