#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>

// to compile and run:

// module load openmpi/gcc

// mpicc monte-carlo-main.c -o monte-carlo-main.o

// mpirun ./monte-carlo-main.o

/**
 * Initialize the random number generator using the current time as the seed
 */
void initRand()
{
    srand(time(0));
}

/**
 * Get a random number with 2 places after the decimal
 * @param upper - Upper limit of random number
 * @return - Random number between 0-upper with 4 places past the decimal
 */
float getRand(float upper)
{
    upper = upper * 100;
    float res = ((float)((int)rand() % (int)upper)) / (float)100;
    return res;
}

/**
 * Square a number
 * @param num - Number to square
 * @return - Square of given number
 */
float square(float num)
{
    return num * num;
}

/**
 * Perform a Monte Carlo estimation of the value of pi
 * @param maxIterations - Number of points to test
 * @return - Estimated value of pi
 */
long long monteCarlo(long long maxIterations, int thread_num)
{
    long long hits = 0;
    // init seed for thread
    unsigned int seed = time(0) ^ (unsigned int)thread_num;
    float radius = 10000.0f;
    for (long long iterationsLeft = maxIterations; iterationsLeft > 0; iterationsLeft--)
    {
        float rand_1 = (float)(((int)rand_r(&seed) % (int)(radius * 100)) / (float)100);
        float rand_2 = (float)(((int)rand_r(&seed) % (int)(radius * 100)) / (float)100);
        if (rand_1 * rand_1 + rand_2 * rand_2 <= radius * radius)
        {
            hits++;
        }
    }
    return hits;
}

int main(int argc, char *argv[])
{
    // init MPI
    MPI_Init(&argc, &argv);
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