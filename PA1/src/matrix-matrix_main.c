#include <stdio.h>
#include <stdlib.h>
#include "../include/matrix-matrix.h"

int main(int argc, char *argv[]){
    if (argc < 3)
	{
		fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
		exit(1);
	}
    struct matrixMatrix* result;
    result = matrixMultiply(argv[1], argv[2]);
   
    printf("The result of  matrix vector Multiplication \n");
    for(int i = 0 ; i< result->nrows;i++){
        printf("%lf\n", result->A[i]);
    }

}