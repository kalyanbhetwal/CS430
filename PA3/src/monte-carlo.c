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