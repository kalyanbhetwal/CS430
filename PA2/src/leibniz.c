#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066

double get_pi(int digits) {

    // calculate the minimum number of terms needed
    // to compute PI accurate to the specified number of digits
    int term = 1;
    for (int i = 0; i < digits; i++) {
        term *= 10;
    }
    term *= 2;


    // PARALLEL COMPUTATION BEGINS HERE //
    
    
    double f = 0.0; // private serial variable
    double pf = 0.0; // private threaded variable
    // Calculate the partial sum of the Leibniz series up to the minimum number of terms
    #pragma omp parallel private(pf) shared(f)
    {
        #pragma omp for
        for (int i = 0; i < (term); i++) {
            if (i % 2 == 0) {
                pf += (double) 4 / (2*i+1);
            }
            else {
                pf -= (double) 4 / (2*i+1);
            }
        }
        
        #pragma omp critical
        {
            f += pf;
        }
    }
    
    
    // PARALLEL COMPUTATION ENDS HERE //
    
    
    // coninue calculating terms of the Leibniz series until
    // the overestimate and underestimate are within the 
    // desired approximation range
    // e.x. for two digits, the partial sums should be computed
    // until all further partial sums are between 3.14 and 3.15
    int accurate = 0;
    double acc = (double) ((int) (PI * (term / 2)));
    acc = (2*acc) / term;
    double acc2 = acc + (double) 2 / term;
    double next = 0.0;
    printf("%.9f, %.9f\n", acc, acc2);
    while (accurate == 0) {
        // get the next term
        if (term % 2 == 0) {
            next = f + (double) 4 / (2*term+1);
        }
        else {
            next = f - (double) 4 / (2*term+1);
        }
        
        // determine if all further partial sums will be within the 
        // correct approximation range
        // If so, exit the while loop before the next iteration
        if ((f < acc2) && (f > acc) && (next < acc2) && (next > acc)) {
            accurate++;
        }
        term++;
        f = next;
    }

    return f;
}
