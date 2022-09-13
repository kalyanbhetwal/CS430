#include "../include/leibniz.h"

int main(int argc, char *argv[] ) {

    char* strIndex = argv[1];
    int digits = 0;
    // convert number of digits (as string) to integer
    if (strcmp(strIndex,"0")!=0) {
        digits = atoi(strIndex);
    }

    // calculate necessary precisison needed
    // for n correct digits (decimal places) of PI
    double f = get_pi(digits);
    printf("%.30f\n", f);
    return 0;
}