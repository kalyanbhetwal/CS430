#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char *argv[] ) {
    // get index for fibonacci number from Command Line
    char* strIndex = argv[1];
    int index = 0;
    // convert string index to integer
    if (strcmp(strIndex,"0")!=0) {
        index = atoi(strIndex);
    }
    // check if index is negative
    // if so, return -1 and exit
    if (index < 0) {
        fprintf(stderr, "%s", "Index must be a positive integer\n");
        return(-1);
    }

    unsigned long long fib = fibonacci(index);
    printf("%llu", fib);
}
