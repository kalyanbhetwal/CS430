#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066

double get_pi(int term) {
    // compute PI to a given number of terms in the Leibniz series
    // PARALLEL COMPUTATION BEGINS HERE //
    
    double start; 
    double end;
    start = omp_get_wtime();
    double f = 0.0; // private serial variable
    double pf = 0.0; // private threaded variable
    // Calculate the partial sum of the Leibniz series up to the number of terms
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
    end = omp_get_wtime();
    printf("Work took %f seconds\n", end - start);
    
    // PARALLEL COMPUTATION ENDS HERE //
    return f;
}
