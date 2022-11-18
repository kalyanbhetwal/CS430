#include "../include/leibniz.h"

#define N 1000

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
        int split = N / nproc;
        int lower, upper;
        
        // processes 1 ... number of process - 1
        for (i = nproc-1; i > 0; i--) {
            lower = N - split*(nproc-i) + 1;
            upper = N - split*(nproc-i-1);
            MPI_Send(&upper, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&lower, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        
        // initial process get remaining work + remainder after dividing n by number of threads
        lower = 0;
        upper = N - split*(nproc-1);
        double total;
        total = get_pi(N, lower, upper);
        
        double tmp;
        for (i = 1; i < nproc; i++) {
            MPI_Recv(&tmp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            total += tmp;
        }
        
        printf("%.30f\n", total);
    }
    else {
       int upper, lower;
       double tmp;
       MPI_Recv(&upper, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
       MPI_Recv(&lower, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
       tmp = get_pi(N, lower, upper);
       MPI_Send(&tmp, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}
