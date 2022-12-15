#include "../include/monte-carlo.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    // init MPI
    MPI_Init(&argc, &argv);

    if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [no of iterations]\n", argv[0]);
		exit(1);
	}
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Status status;
    if (world_size == 1)
    {
        printf("Running %ld iterations on %d thread(s)\n", strtol(argv[1], NULL, 10), world_size);
        // get start time
        struct timeval start, end;
        gettimeofday(&start, NULL);
        long long hits = monteCarlo(strtol(argv[1], NULL, 10), world_rank);
        gettimeofday(&end, NULL);
        // time_t elapsed_time = end.tv_usec - start.tv_usec;
        long int time_in_microseconds = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;
        printf("Total time: %ld microseconds\n", time_in_microseconds);
        printf("Or %ld milliseconds\n", time_in_microseconds / 1000);
        printf("pi = %f\n", (float)(4 * ((float)hits / (float)strtol(argv[1], NULL, 10))));
    }
    else if (world_rank == 0)
    {
        printf("Running %ld iterations on %d thread(s)\n", strtol(argv[1], NULL, 10), world_size);
        // get start time
        struct timeval start, end;
        gettimeofday(&start, NULL);
        long long totalHits = 0;
        for (int i = 1; i < world_size; i++)
        {
            long long receivedHits;
            MPI_Recv(&receivedHits, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            totalHits = totalHits + receivedHits;
        }
        gettimeofday(&end, NULL);
        // time_t elapsed_time = end.tv_usec - start.tv_usec;
        long int time_in_microseconds = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;
        printf("Total time: %ld microseconds\n", time_in_microseconds);
        printf("Or %ld milliseconds\n", time_in_microseconds / 1000);
        printf("pi = %f\n", (float)(4 * ((float)totalHits / (float)strtol(argv[1], NULL, 10))));
    }
    else
    {
        long long hits = monteCarlo(strtol(argv[1], NULL, 10) / (world_size - 1), world_rank);
        MPI_Send(&hits, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
