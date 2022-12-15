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
 * @param threads - Number of threads to use
 * @return - Estimated value of pi
 */
float monteCarlo(long long maxIterations, int threads)
{
    // get start time
    struct timeval start, end;
    gettimeofday(&start, NULL);
    // long long hits = 0;
    omp_set_num_threads(threads);
    long long hits[threads];
#pragma omp parallel
    {
        int thread_num = omp_get_thread_num();
        #pragma omp single
            printf("Number of threads = %d\n", omp_get_num_threads());
        hits[thread_num] = 0;
        // init seed for thread
        unsigned int seed = time(0) ^ (unsigned int)thread_num;
        float radius = 10000.0f;
        long long limit = maxIterations / threads;
        for (long long iterationsLeft = limit; iterationsLeft > 0; iterationsLeft--)
        {
            float rand_1 = (float)(((int)rand_r(&seed) % (int)(radius * 100)) / (float)100);
            float rand_2 = (float)(((int)rand_r(&seed) % (int)(radius * 100)) / (float)100);
            if (rand_1 * rand_1 + rand_2 * rand_2 <= radius * radius)
            {
                hits[thread_num]++;
            }
        }
    }
    float total = 0;
    for (int i = 0; i < threads; i++)
    {
        total = total + hits[i];
    }
    gettimeofday(&end, NULL);
    // time_t elapsed_time = end.tv_usec - start.tv_usec;
    long int time_in_microseconds = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;
    printf("Total time: %ld microseconds\n", time_in_microseconds);
    printf("Or %ld milliseconds\n", time_in_microseconds / 1000);
    return 4 * (total / (float)maxIterations);
}

int main(int argc, char *argv[])
{
    printf("pi = %f\n", monteCarlo(strtol(argv[1], NULL, 10), strtoi(argv[2], NULL, 10)));
}
