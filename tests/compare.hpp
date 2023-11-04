#include <string>
#include <iostream>
#include <math.h>
#include "../src/Common.hpp"

void COMPARE(bool compare, std::string message)
{
    if (compare)
        std::cout << "Test passed: " + message << std::endl;
    else
        std::cout << "Test failed: " + message << std::endl;
}

template <typename T>
void COMPARE(T val_1, T val_2, T eps, std::string message)
{
    if (fabs(val_1 - val_2) < fabs(eps))
        std::cout << "Test passed: " + message << std::endl;
    else
        std::cout << "Test failed: " + message << std::endl;
}