#include "../include/monte-carlo.h"

int main(int argc, char *argv[])
{
     if (argc < 3) {
     fprintf(stderr, "Usage: %s [<iterations> <num of threads>]\n", argv[0]);
       return(-1);
    }
    printf("pi = %f\n", monteCarlo(20000000000, 28));
}