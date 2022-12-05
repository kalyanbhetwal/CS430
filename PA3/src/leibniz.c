#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/leibniz.h"

double get_pi(int term, int lower, int upper) {
    // compute PI to a given number of terms in the Leibniz series

    double pf = 0.0; // 
    // Calculate the partial sum of the Leibniz series from the lower bound to upper bound
    for (int i = lower; i < (upper); i++) {
        if (i % 2 == 0) {
                pf += (double) 4 / (2*i+1);
        }
        else {
                pf -= (double) 4 / (2*i+1);
        }
    }

    return pf;
}
