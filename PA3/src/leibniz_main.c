#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"
#include "../include/leibniz.h"

// double get_pi(int term, int lower, int upper) {
//     // compute PI to a given number of terms in the Leibniz series

//     double pf = 0.0; // 
//     // Calculate the partial sum of the Leibniz series from the lower bound to upper bound
//     for (int i = lower; i < (upper); i++) {
//         if (i % 2 == 0) {
//                 pf += (double) 4 / (2*i+1);
//         }
//         else {
//                 pf -= (double) 4 / (2*i+1);
//         }
//     }

//     return pf;
// }

int main(int argc, char** argv) {
    int nproc, pid, i;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Status status;

  	if (nproc < 1) {
  		  printf("Need at least one processor running.\n");
  		  MPI_Abort(MPI_COMM_WORLD, 1);
  		  exit(1);
  	}
   
    if (pid==0) {
    
        if (argc == 1) {
          printf("Please input a positive number\n");
          return(-1);
        }
        char* strIndex = argv[1];
        int N = 0;
        // convert number of digits (as string) to integer
        if (strcmp(strIndex,"0")!=0) {
           N = atoi(strIndex);
        }
    
        int split = N / nproc;
        int lower, upper;
        

        
        double start; 
        double end;
        start = MPI_Wtime();
        
        // processes 1 ... number of process - 1
        for (i = nproc-1; i > 0; i--) {
            lower = N - split*(nproc-i) + 1;
            upper = N - split*(nproc-i-1);
            MPI_Send(&upper, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&lower, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&N, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
        }
        
        // initial process get remaining work + remainder after dividing n by number of threads
        lower = 0;
        upper = N - split*(nproc-1);
        double total;
        total = get_pi( lower, upper);
        
        double tmp;
        for (i = 1; i < nproc; i++) {
            MPI_Recv(&tmp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            total += tmp;
        }
        
        
        printf("%.30f\n", total);
        
        end = MPI_Wtime();
        printf("Work took %f seconds\n", end - start);

    }
    else {
       int upper, lower, NT;
       double tmp;
       MPI_Recv(&NT, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
       
       MPI_Recv(&upper, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
       MPI_Recv(&lower, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
       tmp = get_pi( lower, upper);
       MPI_Send(&tmp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}
