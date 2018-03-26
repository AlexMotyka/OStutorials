#include <stdlib.h>
#include <stdio.h>
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
	double *y = malloc(sizeof(double) * n);
	double dx = 1.0 / (n + 1); // 1.0 is for double value

	double x = 0;
	#pragma omp parallel for private(x)
	for(int i = 0; i < n; i++) {
		x = i * dx;
		y[i] = exp(x) * cos(x) * sin(x) * sqrt(5 * x + 6.0);
  }

	return 0;
}
