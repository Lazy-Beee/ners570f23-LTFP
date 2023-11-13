#include <string>
#include <iostream>
#include <math.h>
#include <vector>

static int testCount = 0;
static std::vector<int> failedTests = {};

void COMPARE(bool compare, std::string message)
{
    if (compare)
        std::cout << "Test " << testCount << " passed: " + message << std::endl;
    else
    {
        std::cout << "Test " << testCount << " FAILED: " + message << std::endl;
        failedTests.push_back(testCount);
    }
    testCount++;
}

template <typename T>
void COMPARE(T val_1, T val_2, std::string message)
{
    if (val_1 == val_2)
        std::cout << "Test " << testCount << " passed: " + message << std::endl;
    else
    {
        std::cout << "Test " << testCount << " FAILED: " + message;
        std::cout << "\tvalue: " << val_1;
        std::cout << " != " << val_2 << std::endl;
        failedTests.push_back(testCount);
    }
    testCount++;
}

template <typename T>
void COMPARE(T val_1, T val_2, T eps, std::string message)
{
    if (fabs(val_1 - val_2) <= fabs(eps))
        std::cout << "Test " << testCount << " passed: " + message << std::endl;
    else
    {
        std::cout << "Test " << testCount << " FAILED: " + message;
        std::cout << "\tvalue: " << val_1;
        std::cout << " != " << val_2 << std::endl;
        failedTests.push_back(testCount);
    }
    testCount++;
}

void COMPARE_summary()
{
    if (failedTests.size() == 0)
        std::cout << "All " << testCount << " test passed!" << std::endl;
    else
    {
        std::cout << "Test ";
        for (int test : failedTests)
            std::cout << test << " ";
        std::cout << "Failed!" << std::endl;
    }
}