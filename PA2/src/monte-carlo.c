#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>

/**
 * Initialize the random number generator using the current time as the seed
 */
void initRand() {
    srand(time(0));
}

/**
 * Get a random number with 2 places after the decimal
 * @param upper - Upper limit of random number
 * @return - Random number between 0-upper with 4 places past the decimal
 */
float getRand(float upper) {
    upper = upper * 100;
    float res = ((float) ((int) rand() % (int) upper)) / (float) 100;
    return res;
}

/**
 * Square a number
 * @param num - Number to square
 * @return - Square of given number
 */
float square(float num) {
    return num * num;
}

/**
 * Perform a Monte Carlo estimation of the value of pi
 * @param maxIterations - Number of points to test
 * @return - Estimated value of pi
 */
float monteCarlo(long long maxIterations) {
    // get start time
    struct timeval start, end;
    gettimeofday(&start, NULL);
    // num of threads to use
	int threads = 6;
    long long hits = 0;
	omp_set_num_threads(threads);
	#pragma omp parallel
	{
        #pragma omp single
            printf("Number of threads = %d\n", omp_get_num_threads());
		// init seed for thread
		int seed = time(0);
		rand_r(&seed);
        float radius = 3.0f;
        #pragma omp for reduction(+:hits)
            for (long long iterationsLeft = maxIterations; iterationsLeft > 0; iterationsLeft--) {
                if (square(getRand(radius)) + square(getRand(radius)) <= square( radius)) {
                    hits++;
                }
            }
	}
    gettimeofday(&end, NULL);
    time_t elapsed_time = end.tv_usec - start.tv_usec;
    long int time_in_microseconds = ((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec;
    printf("Total time: %ld microseconds\n", time_in_microseconds);
    printf("Or %ld milliseconds\n", time_in_microseconds / 1000);
    return 4 * ((float) hits / (float) maxIterations);
}

int main(int argc, char *argv[]) {
    printf("%f\n", monteCarlo(99999999));
}