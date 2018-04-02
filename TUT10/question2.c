#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

// Define process 0 as MASTER
#define MASTER 0

#define DATA_SIZE 10000000
#define CHUNK_SIZE 1
#define NUM_OF_TIMES 1000


void master(int n_proc)
{
	double *data = malloc(sizeof(double) * DATA_SIZE);
  MPI_Status status;

	for (int i = 0; i < DATA_SIZE; i++) {
		double r = rand() % 100;
		data[i] = r;
	}

  double start_time = MPI_Wtime();

	for (int i = 0; i < NUM_OF_TIMES; i++) {
		MPI_Send(&data, CHUNK_SIZE, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
    printf("Master: Sent\n");
		MPI_Recv(&data, CHUNK_SIZE, MPI_DOUBLE, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("Master: Received\n");
	}

  double end_time = MPI_Wtime();
	printf("=== Master running time = %lf ===\n", end_time - start_time);
}


void slave(int proc_id)
{
	double *data = malloc(sizeof(double) * DATA_SIZE);
	MPI_Status status;

	double start_time = MPI_Wtime();

	for (int i = 0; i < NUM_OF_TIMES; i++) {
		MPI_Recv(&data, CHUNK_SIZE, MPI_DOUBLE, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("Slave: Received\n");
		MPI_Send(&data, CHUNK_SIZE, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    printf("Slave: Sent\n");
	}

	double end_time = MPI_Wtime();
	printf("=== Slave running time = %lf ===\n", end_time - start_time);
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
