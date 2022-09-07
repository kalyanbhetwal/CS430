#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

unsigned long long fibonacci(int n) {
    if (n == 0) {
        return 0;
    }
    else if (n == 1) {
        return 1;
    }

    // set up initial variables
    unsigned long long c0 = 0;
    unsigned long long c1 = 1;
    unsigned long long c2 = 0;
    while (n >= 2) {
        // use fibonacci recursion each iteration
        c2 = c1 + c0;
        // update the variables
        c0 = c1;
        c1 = c2;
        n--;
    }
    return(c2);
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