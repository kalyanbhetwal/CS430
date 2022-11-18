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
float monteCarlo(long long maxIterations)
{
    long long hits = 0;
    // init seed for thread
    unsigned int seed = time(0);
    rand_r(&seed);
    float radius = 3.0f;
    for (long long iterationsLeft = maxIterations; iterationsLeft > 0; iterationsLeft--)
    {
        if (square(getRand(radius)) + square(getRand(radius)) <= square(radius))
        {
            hits++;
        }
    }
    return 4 * ((float)hits / (float)maxIterations);
}

int main(int argc, char *argv[])
{
    int doneCounter;
    // init MPI
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    printf("Rank %d out of %d processors\n", world_rank, world_size);
    if (world_rank == 0)
    {
        doneCounter = 0;
    }
    // get start time
    struct timeval start, end;
    gettimeofday(&start, NULL);
    float total = 0;
    total = (total + monteCarlo(9999)) / 2;
    doneCounter++;
    if (world_rank == 0)
    {
        while (doneCounter < world_size)
        {
            sleep(0.01);
        }

        printf("%f\n", total);

        gettimeofday(&end, NULL);
        long int time_in_microseconds = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;
        printf("Total time: %ld microseconds\n", time_in_microseconds);
        printf("Or %ld milliseconds\n", time_in_microseconds / 1000);
    }
    MPI_Finalize();
}