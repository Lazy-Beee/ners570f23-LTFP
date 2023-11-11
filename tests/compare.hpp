#include <string>
#include <iostream>
#include <math.h>

static int testCount = 0;
static int failCount = 0;

void COMPARE(bool compare, std::string message)
{
    testCount++;
    if (compare)
        std::cout << "Test passed: " + message << std::endl;
    else
    {
        std::cout << "Test FAILED: " + message << std::endl;
        failCount++;
    }
}

template <typename T>
void COMPARE(T val_1, T val_2, std::string message)
{
    testCount++;
    if (val_1 == val_2)
        std::cout << "Test passed: " + message << std::endl;
    else
    {
        std::cout << "Test FAILED: " + message;
        std::cout << "\tvalue: " << val_1;
        std::cout << " != " << val_2 << std::endl;
        failCount++;
    }
}

template <typename T>
void COMPARE(T val_1, T val_2, T eps, std::string message)
{
    testCount++;
    if (fabs(val_1 - val_2) <= fabs(eps))
        std::cout << "Test passed: " + message << std::endl;
    else
    {
        std::cout << "Test FAILED: " + message;
        std::cout << "\tvalue: " << val_1;
        std::cout << " != " << val_2 << std::endl;
        failCount++;
    }
}

void COMPARE_summary()
{
    if (failCount == 0)
        std::cout << "All " << testCount << " test passed!" << std::endl;
    else
        std::cout << failCount << "/" << testCount << " test Failed!" << std::endl;
}