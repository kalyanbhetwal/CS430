
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stddef.h>
#include "mpi.h"
#include "../include/matrix-matrix.h"


int main(int argc, char *argv[])
{
    if (argc < 3)
	{
		fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
		exit(1);
	}
    int i, j,rank, size,N;
    struct matrixMatrix* res, *m1, *m2;
    double *recA , *recC;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Let rank 0 read input data


        m1 = malloc( sizeof(struct matrixMatrix));
        m2 = malloc( sizeof(struct matrixMatrix)); 
        res = malloc(sizeof(struct matrixMatrix));
       
        m1 = readMatrix(argv[1]);
        m2 = readTransposeMatrix(argv[2]);
        N = m1->nrows;

        res->A = (double*) malloc(N* N* sizeof(double));

        if(N%size!= 0) {
            printf("ERROR: Matrix cannot be divided into submatrices.\n");
            exit(0);
        }
    
    MPI_Barrier(MPI_COMM_WORLD); /* barrier is needed if no necessary synchronization for the timing is ensured yet */
	double start = MPI_Wtime(); /* only 1 process needs to do this */
		
    // first broad cast the n to all others
    MPI_Bcast (&N, 1 , MPI_DOUBLE , 0 , MPI_COMM_WORLD ) ;
    //printf ("  n / p is %d \n " , N/size  ) ;

    recA = (double*) malloc( N* N/size * sizeof( double));
    recC = (double*) malloc( N * N/size * sizeof( double ));

    //printf (" process %d create rec buffer \n " , pid  )


	// now p0 scatter matrix sendA to all 
    MPI_Scatter( m1->A , N * N/size , MPI_DOUBLE , recA , N * N/size , MPI_DOUBLE , 0, MPI_COMM_WORLD ) ;
    
    // for (int i=0; i < (N * N/size); i++)
    //         printf (" process %d A [%d] = % f \n " , rank , i , recA[i] ) ;

            //now p0 broadcast sendB to all others
     MPI_Bcast ( m2->A, N*N , MPI_DOUBLE , 0 , MPI_COMM_WORLD ) ;

    // for (int i=0; i < (N*N) ; i++)
	//  	printf (" process %d sendB [%d] = % f \n " , rank , i , m2->A[i]) ;

    	// all do this part to calculate recC as multiplicated matrix	
        
        for (  i=0 ; i< (N/size) ; i++ ){
                for ( j = 0 ; j< N ; j++ )
                {
                        recC[i*N+j] = 0 ;
                        for ( int k = 0 ; k<N ; k++ )
                                recC[i*N+j] += recA[i*N+k]*m2->A[j*N+k];
                                //printf (" process %d recC [%d] = % f \n" , rank , i*N+j , recC[i*N+j] ) ;
                }
            }
    // nopw p0 will gather all result data from all prcesses
     
     MPI_Gather( recC , N*N/size , MPI_DOUBLE , res->A , N*N/size , MPI_DOUBLE, 0, MPI_COMM_WORLD ) ;

	// here is the last point of calculating the time		
	MPI_Barrier(MPI_COMM_WORLD); /* barrier is needed if no necessary synchronization for the timing is ensured yet */
	if(rank==0){
        double end = MPI_Wtime(); /* only 1 process needs to do this */
        double time = end - start ;
        printf ("Elapsed  time %d : time = % f \n" , rank , time ) ;
    }

    MPI_Finalize();
    free(recA);
    free(recC);
	return 0;
  
}
