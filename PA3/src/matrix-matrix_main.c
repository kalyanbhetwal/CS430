
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
    struct matrixMatrix* res;
    res = malloc(sizeof(struct matrixMatrix));
    struct matrixMatrix* m1;
    m1 = malloc( sizeof(struct matrixMatrix));

    struct matrixMatrix* m2;
    m2 = malloc( sizeof(struct matrixMatrix));
    m1 = readMatrix(argv[1]);
    m2 = readTransposeMatrix(argv[2]);
    N = m1->nrows;

    struct timeval st, et;
  

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (N%size){   //Valid Communicator Size?
        printf("Invalid\n");
        MPI_Finalize();
        return(0);
    } 

    res->A = malloc( m1->nrows*m1->nrows * sizeof(double));
    double *aa, *cc;
    aa = malloc(N*N/size* sizeof(double));
    cc = malloc(N*N/size* sizeof(double));
    //scatter rows of first matrix to different processes     
    MPI_Scatter(m1->A, N*N/size, MPI_DOUBLE, aa, N*N/size, MPI_DOUBLE,0,MPI_COMM_WORLD);

    //broadcast second matrix to all processes
    MPI_Bcast(m2->A, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
     gettimeofday(&st,NULL);  

  for( i=0; i < N;i++){
        cc[i]=0; 
        for( j=0; j< N ;j++){  
          cc[i]= cc[i]+ aa[j]*m2->A[j];
          //printf("the val %f\n",cc[i]);
     }
    } 

    MPI_Gather(cc, N*N/size, MPI_DOUBLE, res->A, N*N/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);        
    MPI_Finalize();
    gettimeofday(&et,NULL);
    int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
    if(rank==0){
    printf("Elasped time for Matrix matrix=%d ms\n",elapsed); 
     free(m1);
    free(m2);
    free(res);
    }
  
}
