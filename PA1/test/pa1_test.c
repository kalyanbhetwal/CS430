//
// Created by Kalyan on 9/8/22.
//

#include "gtest/gtest.h"

extern "C" {
#include "../src/hellomake.h"
}

TEST(testMath, myTest)
{   
    int a  = myPrintHelloMake();
    EXPECT_EQ(10000, a); 
} 