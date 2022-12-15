#define _POSIX_SOURCE
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/matrix-matrix.h"

int main(int argc, char *argv[]) {
  if(argc != 3){
    printf("Expected input: matmul input_filename output_filename\n");
    exit(0);
  }
  int rank, size, i, j, k, l, chunk, n, grid_size;
  //double *A, *B, *C, 
  double *localA, *localB, *localC, *buffA, *buffB, *tempA, *tempB;

  struct matrixMatrix* A, *B, *C;
  MPI_Status status;
   
  MPI_Request request, request_send, requests[4];

  MPI_Init(&argc, &argv);               // Initialize MPI 
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the number of processors
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get my number

  // Create a 2D Cartesian topology
  MPI_Comm comm_grid; 
  int dims[2], periods[2], reorder, grid_coordinates[2];
  dims[0] = dims[1] = sqrt(size); 
  periods[0] = periods[1] = 1; 
  reorder = 0;
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &comm_grid);
  MPI_Cart_coords(comm_grid, rank, 2, grid_coordinates);

  // Let rank 0 read input data
  if(rank == 0){

    
      A = malloc( sizeof(struct matrixMatrix));
      B = malloc( sizeof(struct matrixMatrix)); 
      C = malloc(sizeof(struct matrixMatrix));


      A = readMatrix(argv[1]);
      B = readMatrix(argv[2]);
      n = A->nrows;

    // Check assumption
    if(rank == 0){
      if((int)sqrt(size) * (int)sqrt(size) != size || n % (int)sqrt(size) != 0) {
        printf("ERROR: Matrix cannot be divided into submatrices.\n");
        exit(0);
      }
    }

    // Allocate memory for matrices
 //   A = (double*)malloc(n*n*sizeof(double));
   // B = (double*)malloc(n*n*sizeof(double));

    // Read input to matrices
   // readInput(A, B, n, stream_in);

    // Print matrices
    /*
    printf("A: \n");
    printMatrix(A, n, n, n);
    printf("\nB: \n");
    printMatrix(B, n, n, n);
    */
    
  }
  MPI_Barrier(MPI_COMM_WORLD);

  // Start timer
  double starttime = MPI_Wtime();

  // Send n and chunk to all processes
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  chunk = n/sqrt(size);

  // Send blocks (of size chunk*chunk) of A and B to processes
  MPI_Datatype newtype;
  int count = chunk, blocklen = chunk, stride = n;
  MPI_Type_vector(count, blocklen, stride, MPI_DOUBLE, &newtype);
  MPI_Type_commit(&newtype);

  // Allocate memory for local matrices
  localA = (double*)malloc(chunk*chunk*sizeof(double));
  localB = (double*)malloc(chunk*chunk*sizeof(double));

  // Send submatrices to processes
  grid_size = sqrt(size);
  if (rank==0) {
    int rank_counter = 0;
    for(i = 0; i < grid_size; i++){
      for(j = 0; j < grid_size; j++){
        MPI_Isend(&A->A[(i*chunk*n)+(j*chunk)], 1, newtype, rank_counter, 100+rank_counter, MPI_COMM_WORLD, &request);
        MPI_Isend(&B->A[(i*chunk*n)+(j*chunk)], 1, newtype, rank_counter, 200+rank_counter, MPI_COMM_WORLD, &request);
        rank_counter++;
      }
    }
  }
  MPI_Irecv(&localA[0], chunk*chunk, MPI_DOUBLE, 0, 100+rank, MPI_COMM_WORLD, &request);
  MPI_Wait(&request, &status);
  MPI_Irecv(&localB[0], chunk*chunk, MPI_DOUBLE, 0, 200+rank, MPI_COMM_WORLD, &request);
  MPI_Wait(&request, &status);

  // Perform matrix-matrix multiplication: Cannon's algorithm
  // START UP PHASE
  i = grid_coordinates[0];
  j = grid_coordinates[1];
  int up, left, down, right, source;
  if(i != 0){
    MPI_Cart_shift(comm_grid, 1, -i, &source, &left);
    MPI_Cart_shift(comm_grid, 1, i, &source, &right);
  }
  if(j != 0){
    MPI_Cart_shift(comm_grid, 0, -j, &source, &up);
    MPI_Cart_shift(comm_grid, 0, j, &source, &down);
  }

  buffA = (double*)calloc(chunk*chunk,sizeof(double));
  buffB = (double*)calloc(chunk*chunk,sizeof(double));

  if (i != 0) {
    MPI_Isend(&localA[0], chunk*chunk, MPI_DOUBLE, left, 300+left, comm_grid, &request_send);
    MPI_Irecv(&buffA[0], chunk*chunk, MPI_DOUBLE, right, 300+rank, comm_grid, &request);
    MPI_Wait(&request, &status);
    MPI_Wait(&request_send, &status);

    tempA = localA;
    localA = buffA;
    buffA = tempA;
  }
  if (j != 0) {
    MPI_Isend(&localB[0], chunk*chunk, MPI_DOUBLE, up, 400+up, comm_grid, &request_send);
    MPI_Irecv(&buffB[0], chunk*chunk, MPI_DOUBLE, down, 400+rank, comm_grid, &request);
    MPI_Wait(&request, &status);
    MPI_Wait(&request_send, &status);

    tempB = localB;
    localB = buffB;
    buffB = tempB;
  }
  
  // COMPUTE PHASE
  localC = (double*)calloc(chunk*chunk,sizeof(double));

  MPI_Cart_shift(comm_grid, 1, -1, &source, &left);
  MPI_Cart_shift(comm_grid, 1, 1, &source, &right);
  MPI_Cart_shift(comm_grid, 0, -1, &source, &up);
  MPI_Cart_shift(comm_grid, 0, 1, &source, &down);

  for(l = 0; l < grid_size; l++){

    MPI_Isend(&localA[0], chunk*chunk, MPI_DOUBLE, left, 500+left, comm_grid, &requests[0]);
    MPI_Irecv(&buffA[0], chunk*chunk, MPI_DOUBLE, right, 500+rank, comm_grid, &requests[1]);
    MPI_Isend(&localB[0], chunk*chunk, MPI_DOUBLE, up, 600+up, comm_grid, &requests[2]);
    MPI_Irecv(&buffB[0], chunk*chunk, MPI_DOUBLE, down, 600+rank, comm_grid, &requests[3]);

    // Do matrix-matrix multiplication
    for(int u = 0; u < chunk; u++){ // i 
      for(int v = 0; v < chunk; v++){ // k
        for(int s = 0; s < chunk; s++){ // j
          localC[u*chunk+s] += localA[u*chunk+v] * localB[v*chunk+s];
        }
      }
    }
    MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);

    tempA = localA;
    tempB = localB;

    localA = buffA;
    localB = buffB;

    buffA = tempA;
    buffB = tempB;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  
  // Send localC to rank 0
  MPI_Isend(&localC[0], chunk*chunk, MPI_DOUBLE, 0, 1000+rank, MPI_COMM_WORLD, &request_send);
  if(rank == 0){
    C->A = (double*)calloc(n*n,sizeof(double));
    int rank_counter = 0;
    for(k = 0; k < sqrt(size); k++){
      for(i = 0; i < sqrt(size); i++){
        MPI_Irecv(&C->A[k*chunk*n+i*chunk], 1, newtype, rank_counter, 1000+rank_counter, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        
        rank_counter++;
      }
    }
  }
  MPI_Wait(&request_send, &status);
  
  // Stop timer
  double execution_time = MPI_Wtime()-starttime; // stop timer
  double max_time;
	MPI_Reduce(&execution_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 

	if(rank==0){
    printf("Elapsed time %lf\n", max_time);
    char file1[2048];
    getcwd(file1,2048);
    strcat(file1,"/var/cannonresult.mm");
    FILE *stream_out;
    stream_out = fopen(file1,"w");
  
    if(stream_out == NULL){
      printf("Error: unable to open file: %s\n", file1);
      exit(0);
    }
    fprintf(stream_out,"%s","%%MatrixMarket matrix array real general\n%\n");
    fprintf(stream_out,"%d %d\n",n, n);
    for(int i = 0; i<n*n; i++){
      fprintf(stream_out, "%f\n", C->A[i]);
    }


  }

  // Clean up 
  if(rank == 0){
    free(A);
    free(B);
    free(C);
  }
  free(buffA);
  free(buffB);
  free(localA);
  free(localB);
  free(localC);
  MPI_Finalize();
  return 0;
}