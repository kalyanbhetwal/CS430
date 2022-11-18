#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>

// dev note: to compile:
// gcc monte-carlo.c -o monte-carlo -fopenmp
// to run:
// ./monte-carlo

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
float monteCarlo(long long maxIterations, int threads)
{
    // get start time
    struct timeval start, end;
    gettimeofday(&start, NULL);
    long long hits = 0;
    omp_set_num_threads(threads);
    printf("Number of threads = %d\n", threads);
    // init seed for thread
    int seed = time(0);
    rand_r(&seed);
    float radius = 3.0f;
    float sqRad = square(radius);
#pragma omp parallel for reduction(+: hits)
    for (long long iterationsLeft = maxIterations; iterationsLeft > 0; iterationsLeft--)
    {
        if (square(getRand(radius)) + square(getRand(radius)) <= sqRad)
        {
            hits++;
        }
    }
    gettimeofday(&end, NULL);
    time_t elapsed_time = end.tv_usec - start.tv_usec;
    long int time_in_microseconds = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;
    printf("Total time: %ld microseconds\n", time_in_microseconds);
    printf("Or %ld milliseconds\n", time_in_microseconds / 1000);
    return 4 * ((float)hits / (float)maxIterations);
}

int main(int argc, char *argv[])
{
    // // int threads[] = {1, 2, 4, 6, 8, 12, 16, 20, 24, 28};
    // int threads[] = {1, 2, 4, 6, 8, 12};
    // // long long iterations[] = {99999999, 9999999999, 999999999999};
    // long long iterations[] = {9999999999};
    // for (int t = 0; t < sizeof(threads) / sizeof(int); t++)
    //     for (int i = 0; i < sizeof(iterations) / sizeof(long long); i++)
    //     {
    //         printf("\n\nIterations: %lld\nThreads: %d\n", iterations[i], threads[t]);
    //         printf("pi = %f\n", monteCarlo(iterations[i], threads[t]));
    //     }
    char *e;
    printf("\n\nIterations: %lld\nThreads: %d\n", strtoll(argv[2], &e, 0), atoi(argv[1]));
    printf("pi = %f\n", monteCarlo(strtoll(argv[2], &e, 0), atoi(argv[1])));
}