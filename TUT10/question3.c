#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

// Define process 0 as MASTER
#define MASTER 0

#define CHUNK_SIZE 100 // Matrix row/column size
#define DATA_SIZE 10000 // Make sure is CHUNK_SIZE**2


// Serial matrix multiplication to verify the answer
// From TUT9 question4.c
void serial_matrix_multiplication(){
  int a[CHUNK_SIZE][CHUNK_SIZE];
  int b[CHUNK_SIZE][CHUNK_SIZE];
  int multi[CHUNK_SIZE][CHUNK_SIZE];
  int sum = 0;
  // Create matrix
  for(int i = 0; i < CHUNK_SIZE; i++){
    for(int j = 0; j < CHUNK_SIZE; j++){
      a[i][j] = i + j;
      b[i][j] = i + j;
    }
  }
  // Multi
  for(int i = 0; i < CHUNK_SIZE; i++){
    for(int j = 0; j < CHUNK_SIZE; j++){
      sum = 0; // Reset
      for(int k = 0; k < CHUNK_SIZE; k++){
        sum += a[i][k] * b[k][j];
      }
      multi[i][j] = sum;
    }
  }
  // Print
  printf("\n==== Serial to verify ====\n");
  for (int i = 0; i < CHUNK_SIZE; i++) {
      for (int j = 0; j < CHUNK_SIZE; j++) {
          printf("%5d ",multi[i][j]);
      }
      puts("");
  }
}


void master(int n_proc)
{
    int chunk[CHUNK_SIZE] = { 0 };  // The chunk to store results
    int n_sent = 0, n_recv = 0;        // The number of the data chunks sent/recv
    int total_chunks = DATA_SIZE / CHUNK_SIZE;
    int proc = 0;                      // The process that returned data
    MPI_Status status;                 // MPI status struct
    int A[CHUNK_SIZE][CHUNK_SIZE];
    int B[CHUNK_SIZE][CHUNK_SIZE];
    int C[CHUNK_SIZE][CHUNK_SIZE];

    // Initialize A & B
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            A[i][j] = j + i;
            B[i][j] = j + i;
        }
    }

    // Print A
    printf("\n==== A ====\n");
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for(int j = 0; j < CHUNK_SIZE; j++) {
            printf("%5d ", A[i][j]);
        }
        puts("");
    }

    // Print B
    printf("\n==== B ====\n");
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for(int j = 0; j < CHUNK_SIZE; j++) {
            printf("%5d ", B[i][j]);
        }
        puts("");
    }

    // Broadcast B to slaves
    MPI_Bcast(&B[0][0], DATA_SIZE, MPI_INT, MASTER, MPI_COMM_WORLD);

    // "Map" the data to slave processes
    // i MUST start from 1, since MASTER is process 0!
    for (int i = 1; i < n_proc; ++i)
    {
        // Send CHUNK_SIZE of data to each process initially
        // Sends data, of type double to process 'i'
        MPI_Send(&A[n_sent][0], CHUNK_SIZE, MPI_INT, i, n_sent, MPI_COMM_WORLD);
        n_sent++;
    }

    // Receive EACH of the chunks from the slave processes
    for (int i = 0; i < total_chunks; ++i)
    {
        // Receive the computed chunk back from the slave
        MPI_Recv(chunk, CHUNK_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // Get the process that sent the data and send it the next chunk
        proc = status.MPI_SOURCE;
        n_recv = status.MPI_TAG;

        // Copy the results from the slave into C
        for (int i = 0; i < CHUNK_SIZE; ++i)
        {
            C[n_recv][i] = chunk[i];
        }

        if (n_sent < total_chunks)
        {
            MPI_Send(&A[n_sent][0], CHUNK_SIZE, MPI_INT, proc, n_sent, MPI_COMM_WORLD);
            n_sent++;
        }
    }

    // Send all the slave processes STOP signal, (TAG of CHUNK_SIZE)
    for (int i = 1; i < n_proc; ++i)
    {
        MPI_Send(chunk, CHUNK_SIZE, MPI_INT, i, CHUNK_SIZE, MPI_COMM_WORLD);
    }

    // Print C
    printf("\n==== C ====\n");
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            printf("%5d ",C[i][j]);
        }
        puts("");
    }

    // Print Serial
    serial_matrix_multiplication();
  }


void slave(int proc_id)
{
    int chunk[CHUNK_SIZE] = { 0 };  // The chunk to receive for calculations
    int result[CHUNK_SIZE] = { 0 }; // A calculated row of the matrix multiplication
    int n_recv = 0;                    // The number of the data chunk received
    MPI_Status status;                 // MPI status struct

    // Recieve broadcast B from the master
    int B[CHUNK_SIZE][CHUNK_SIZE];
    MPI_Bcast(&B[0][0], DATA_SIZE, MPI_INT, MASTER, MPI_COMM_WORLD);

    // Receive the chunk to calculate from MASTER
    MPI_Recv(chunk, CHUNK_SIZE, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    n_recv = status.MPI_TAG;

    // Calculate all results provided until "STOP" signal recieved (CHUNK_SIZE)
    while (n_recv < CHUNK_SIZE)
    {
        for (int i = 0; i < CHUNK_SIZE; ++i) {
            result[i] = 0; // Reset
            for (int j = 0; j < CHUNK_SIZE; j++)
                result[i] += chunk[j] * B[j][i]; // Calc result
        }

        // Send the results back to MASTER, include in TAG the chunk that was calculated
        MPI_Send(result, CHUNK_SIZE, MPI_INT, MASTER, n_recv, MPI_COMM_WORLD);

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
