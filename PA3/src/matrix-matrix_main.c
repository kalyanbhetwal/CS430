#define N 3
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stddef.h>
#include "mpi.h"
#include "../include/matrix-matrix.h"


int main(int argc, char *argv[])
{
    int i, j,rank, size;
    struct matrixMatrix* res;
    res = malloc(sizeof(struct matrixMatrix));
    struct matrixMatrix* m1;
    m1 = malloc( sizeof(struct matrixMatrix));
  //m1 = readMatrix(file1);
    struct matrixMatrix* m2;
    m2 = malloc( sizeof(struct matrixMatrix));
    m1 = readMatrix(argv[1]);
    m2 = readTransposeMatrix(argv[2]);
    struct timeval st, et;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    res->A = malloc( m1->nrows*m2->ncolumns * sizeof(double));
    double *aa, *cc;
    aa = malloc(N* sizeof(double));
    cc = malloc(N* sizeof(double));
    //scatter rows of first matrix to different processes   
   
    MPI_Scatter(m1->A, N*N/size, MPI_DOUBLE, aa, N*N/size, MPI_DOUBLE,0,MPI_COMM_WORLD);

    //broadcast second matrix to all processes
    MPI_Bcast(m2->A, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
     gettimeofday(&st,NULL);  

  for( i=0; i < m1->ncolumns ;i++){
        cc[i]=0; 
        for( j=0; j< m2->ncolumns ;j++){  
               cc[i]= cc[i]+ aa[j]*m2->A[i*m2->ncolumns+j];   
     }
    } 

    MPI_Gather(cc, N*N/size, MPI_DOUBLE, res->A, N*N/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);        
    MPI_Finalize();
    gettimeofday(&et,NULL);
    int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
    printf("Elasped time for Matrix matrix=%d ms\n",elapsed); 
}
