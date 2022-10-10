#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/mmio.h"
#include <omp.h>

unsigned long long fibonacci(int n) {
    if (n == 0) {
        return 0;
    }
    else if (n == 1) {
        return 1;
    }
    else {
        unsigned long long i;
        unsigned long long j;
        #pragma omp task shared(i, j)  if (n>33)
        i = fibonacci(n-1);
        j = fibonacci(n-2);
        #pragma omp taskwait
        return i+j;
    }
}
