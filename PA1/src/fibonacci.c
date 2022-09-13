#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/fibonacci.h"
unsigned long long fibonacci(int n) {
    // base case #1
    if (n == 0) {
        return 0;
    }
    // base case #2
    else if (n == 1) {
        return 1;
    }
    // recursive function call
    else {
        return fibonacci(n-1) + fibonacci(n-2);
    }
}
