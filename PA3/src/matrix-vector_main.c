#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stddef.h>
#include "mpi.h"
#include "../include/matrix-vector.h"


int main(int argc, char *argv[])
{
  if (argc < 3)
	{
		fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
		exit(1);
	}
    int i, j,rank, size,N;
    struct matrixVector* res;
    res = malloc(sizeof(struct matrixVector));
    struct matrixVector* m1;
    m1 = malloc( sizeof(struct matrixVector));
  //m1 = readMatrix(file1);
    struct matrixVector* m2;
    m2 = malloc( sizeof(struct matrixVector));

    m1 = readMatrixVector(argv[1]);
    m2 = readMatrixVector(argv[2]);
    N = m1->nrows;
    //struct timeval st, et;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

     
    if (N%size){   //Valid Communicator Size?
        printf("Invalid\n");
        MPI_Finalize();
        return(0);
    } 

    res->A = malloc( m1->nrows * sizeof(double));
    double *aa, *cc;
    aa = malloc(N*N*sizeof(double));
    cc = malloc(N/size* sizeof(double));
    //scatter rows of first matrix to different processes   
   
    MPI_Scatter(m1->A, N*N/size, MPI_DOUBLE, aa, N*N/size, MPI_DOUBLE,0,MPI_COMM_WORLD);

    //broadcast second matrix to all processes
    MPI_Bcast(m2->A, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime(); 
    // gettimeofday(&st,NULL);  

     for(i=0;i < m1->nrows/size;i++){
         cc[i] = 0;
        for(j = 0;j <m2-> nrows ;j++){
           cc[i]+=aa[i*m1->ncolumns+j]*m2->A[j]; 
        }
      // printf("%lf\n", res->A[i]);
    }

  // for( i=0; i < m1->ncolumns/size ;i++){
  //       cc[i]=0; 
  //       for( j=0; j< m2->nrows ;j++){  
  //              cc[i]=cc[i]+ aa[j]* m2->A[j];   
  //             //printf("aa  %d;; rank %d  data2 %f  result %f\n",i*m1->ncolumns+j,rank,  m1->A[i*m1->ncolumns+j], cc[i]);
  //    }
  //   // printf("the value of cc %f\n",cc[i]);
  //   // printf("---------------------\n");
  //   } 

    MPI_Gather(cc, N/size, MPI_DOUBLE, res->A, N/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD); 
    if(rank==0){
       double end = MPI_Wtime(); /* only 1 process needs to do this */
        double time = end - start ;
        printf ("Elapsed  time MV %d : time = % f \n" , rank , time ) ;
    }       
    MPI_Finalize();
    //if(rank==0){
    //   gettimeofday(&et,NULL);
    //   int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
    //   printf("Elasped time for Matrix vector=%d ms\n",elapsed); 

    //   char file1[2048];
    //   getcwd(file1,2048);
    //   strcat(file1,"/var/mvresult.mm");
    //   FILE *stream_out;
    //   stream_out = fopen(file1,"w");
    
    //   if(stream_out == NULL){
    //     printf("Error: unable to open file: %s\n", file1);
    //     exit(0);
    //   }
    //   fprintf(stream_out,"%s","%%MatrixMarket matrix array real general\n%\n");
    //   fprintf(stream_out,"%d %d\n",N,1);
    //   for(int i = 0; i<N; i++){
    //     fprintf(stream_out, "%f\n", res->A[i]);
    //   }
    // }
    free(aa);
    free(cc);
}