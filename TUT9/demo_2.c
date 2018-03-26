#include <stdio.h>
#include <omp.h>
int main (int argc, char *argv[])
{
    int num = 0; // Watch out for race conditions!
    int thread_num, nthreads = 16;
    // Creates 16 threads if it is compiled using -fopenmp command
    #ifdef _OPENMP
    omp_set_num_threads(nthreads);
    #endif
    printf("Testing OpenMP, you should see each thread print...\n");
    // Execute on each of the 16 threads in parallel
    #pragma omp parallel
    {
        thread_num = omp_get_thread_num();
        // This is critical section, execute only one thread at a time
        #pragma omp critical
        {
            // This is a OPENMP section, incremrnt and prints only if it is compiled using -fopenmp command
            #ifdef _OPENMP
            num += 1; // THREAD SAFE INCREMENT
            printf("This thread = %d, num = %d\n", thread_num, num);
            #endif
        }
    }
    return 0;
}
