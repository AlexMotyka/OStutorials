#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main() {
  int size = 100;
	int a[size][size];
	int b[size][size];
	int multi[size][size];
  int sum = 0;
  int result = 0;

  // Create matrix
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			a[i][j] = i + j;
			b[i][j] = i + j;
		}
	}

  // Serial
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      sum = 0; // Reset
      for(int k = 0; k < size; k++){
        sum += a[i][k] * b[k][j];
      }
      multi[i][j] = sum;
    }
  }
  for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
      result += multi[i][j];
  printf("Serial result = %d\n", result);

  // Parallel
  #ifdef _OPENMP
  omp_set_num_threads(8); // Say 8 threads
  #endif

  sum = 0; // Reset var
  result = 0; // Reset var

  for(int i = 0; i < size; i++){
    #pragma omp parallel for private (sum)
    for(int j = 0; j < size; j++){
    	sum = 0; // Reset
    	for(int k = 0; k < size; k++){
    		sum += a[i][k] * b[k][j];
    	}
    	multi[i][j] = sum;
    }
  }
	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
      result += multi[i][j];
  printf("Parallel result = %d\n", result);

	return 0;
}
