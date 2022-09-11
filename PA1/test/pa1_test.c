//
// Created by Kalyan on 9/8/22.
//

#include "gtest/gtest.h"

extern "C" {
#include "../src/fibonacci.h" 
}

TEST(testMath, myTest)
{   
    int a  = fibonacci(10);
    EXPECT_EQ(a, a); 
} 