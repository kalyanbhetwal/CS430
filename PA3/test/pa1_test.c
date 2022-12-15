//
// Created by Kalyan on 9/8/22.
//
#define _POSIX_SOURCE
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include "gtest/gtest.h"


extern "C" {
#include "../include/matrix-vector.h"
#include "../include/matrix-matrix.h"
}

TEST(TESTCannon, myTestCannon){
    float array[9] = {90.0,100.0,110,120,202,228,254,280};
    char file1[2048];
    getcwd(file1,2048);
    strcat(file1,"/var/cannonresult.mm");
    struct matrixMatrix* result;
    result = readMatrix(file1);
    for (int i = 0; i < result->nrows; ++i) {
        EXPECT_EQ(result->A[i], array[i]) << "Vectors x and y differ at index " << i;
    }

}

TEST(TESTMatrixVector, myTest1)
{   
    struct matrixVector* result;

    float array[4] = {30.000000 ,70.000000 ,110.000000,150.00};
    char file3[2048];
    getcwd(file3,2048);
    strcat(file3,"/var/mvresult.mm");
    result = readMatrixVector(file3);


    // printf("The result of  matrix vector Multiplication \n");
    // for(int i = 0 ; i< result->nrows;i++){
    //     printf("%lf\n", result->A[i]);
    // }

    for (int i = 0; i < result->nrows; ++i) {
        EXPECT_EQ(result->A[i], array[i]) << "Vectors x and y differ at index " << i;
    }
} 


TEST(TESTMatrixMatrix, myTestMatMUL)
{   
    struct matrixMatrix* result2;

    float array[9] = {90.000000,100.000000,110.000000,120.000000};
    char file2[2048];
    getcwd(file2,2048);
    strcat(file2,"/var/mmresult.mm");
    

    result2 = readMatrix(file2);

    // printf("The result of  matrix vector Multiplication \n");
    // for(int i = 0 ; i< result->nrows;i++){
    //     printf("%lf\n", result->A[i]);
    // }

    for (int i = 0; i < result2 ->nrows; ++i) {
        EXPECT_EQ(result2->A[i], array[i]) << "Vectors x and y differ at index " << i;
    }
} 


// TEST(TESTleibniz, myTest3)
// {   
//     double a  = get_pi(100000);
//     std::cout <<"the value of a is "<< a<<std::endl;
//     EXPECT_NEAR(a, 3.142,0.1); 
// } 

// TEST(TESTMontecarlo, myTest4)
// {   
//     double a  = monteCarlo(10000);
//     EXPECT_NEAR(a, 3.14,0.1); 
// } 

