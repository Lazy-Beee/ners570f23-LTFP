#include "compare.hpp"
#include "../src/Common.hpp"

int main(){
    Real v1 = 1.0;
    int v2 = 1;
    int v3 = 1;
    Real v4 = 2.0;
    Real v5 = 1.001;

    COMPARE(v2 == v3, "1.0 equals 1");
    COMPARE(v1 < v4, "1.0 is smaller than 2.0");
    COMPARE<float>(v1, v5, 0.01, "1.0 is close to 1.001");
}