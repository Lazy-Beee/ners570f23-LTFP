#include "compare.hpp"
#include "../utilities/Common.h"

int main(){
    Real v1 = static_cast<Real>(1.0);
    int v2 = 1;
    int v3 = 1;
    Real v4 = static_cast<Real>(2.0);
    Real v5 = static_cast<Real>(1.001);

    COMPARE(v2 == v3, "1.0 equals 1");
    COMPARE(v1 < v4, "1.0 is smaller than 2.0");
    COMPARE<float>(v1, v5, static_cast<float>(0.01), "1.0 is close to 1.001");
}