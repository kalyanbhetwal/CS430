#include "fibonacci.h"

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
    printf("The fibonacci number is %llu\n", fib);
}
