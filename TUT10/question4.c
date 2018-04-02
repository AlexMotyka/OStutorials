#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>

// Define process 0 as MASTER
#define MASTER 0

#define DATA_SIZE 100000
#define CHUNK_SIZE 10


// Prime density function to get the number of primes expect to find in search space
int prime_density_function(int start, int end)
{
	if (start <= 1)
		return end / log(end) + end / 10;
	else
		return end / log(end) - start / log(start) + end / 10;
}


void master(int n_proc)
{
  int data[CHUNK_SIZE] = { 0 };
	int chunk[DATA_SIZE] = { 0 };
	int n_sent = 0;
  int proc = 0;
	MPI_Status status;

	// Populate arrays
  int* range_size = (int *) malloc(sizeof(int) * n_proc);
	for (int i = 1; i < n_proc; ++i) {
		int range = DATA_SIZE / (n_proc - 1);
		data[0] = n_sent * range + 1;
		data[1] = (n_sent + 1) * range;
		range_size[i] = prime_density_function(data[0], data[1]);
		MPI_Send(&data[0], CHUNK_SIZE, MPI_INT, i, n_sent, MPI_COMM_WORLD);
		n_sent++;
	}

	// Receive slave processes
  int* results[n_proc];
  int slave_size[n_proc];
	for (int i = 1; i < n_proc; ++i){
		MPI_Recv(chunk, DATA_SIZE, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		proc = status.MPI_SOURCE;
		MPI_Get_count(&status, MPI_INT, &slave_size[i]);
		results[proc] = (int *) malloc (sizeof(int) * slave_size[i]);
		for (int j = 0; j < slave_size[i]; ++j)
			if (chunk[j] != 0)
					results[proc][j] = chunk[j]; // Store results
	}

  // Send all the slave processes STOP signal, (TAG of CHUNK_SIZE)
	for (int i = 1; i < n_proc; ++i )
		MPI_Send(chunk, CHUNK_SIZE, MPI_INT, i, range_size[i], MPI_COMM_WORLD);

	// Results
	for (int i = 1; i < n_proc; ++i)
		for (int j = 0; j < slave_size[i]; j++)
			printf("%d\n", results[i][j]);
}


void slave()
{
	int chunk[CHUNK_SIZE];
  int n_recv = 0;
	MPI_Status status;

	// Receive the chunk to calculate from MASTER
	MPI_Recv(chunk, CHUNK_SIZE, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	n_recv = status.MPI_TAG;

	int number_of_primes = prime_density_function(chunk[0], chunk[1]);
	int* primes = (int *) malloc(sizeof(int)*number_of_primes);
  bool isPrime = true;
  int number_of_results = 0;

	while (n_recv < number_of_primes) {
		for (int i = chunk[0]; i <= chunk[1]; i++) {
			for (int j = 2; j < i; j++) {
				isPrime = true;
				primes[number_of_results] = 0;
				if (i % j == 0) { // Not prime
					isPrime = false;
					break;
				}
			}
			if (isPrime) { // Prime
				primes[number_of_results] = i; // Store
				number_of_results++;
			}
		}

    // Send the results back to MASTER, include in TAG the chunk that was calculated
		MPI_Send(&primes[0], number_of_results, MPI_INT, MASTER, n_recv, MPI_COMM_WORLD);

    // Wait for the next chunk of data to be received
		MPI_Recv(chunk, CHUNK_SIZE, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		n_recv = status.MPI_TAG;
	}
}


int main (int argc, char* argv[])
{
    int proc_id;            // Process rank
    int n_proc;             // Number of processes

    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the current number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    // Get the current process id
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);


    if (proc_id == MASTER)
    {
        master(n_proc);
    }
    else
    {
        slave(proc_id);
    }

    // Required to terminate all MPI processes
    MPI_Finalize();
}
