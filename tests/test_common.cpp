#include "compare.hpp"
#include "../src/Common.hpp"
#include <string>

using namespace std;
using namespace LTFP;

void compare(Real x, vector<Real> coeff)
{
    static int count = 0;
    count++;
    Real val = pow(x, 3) * coeff[0] + pow(x, 2) * coeff[1] + pow(x, 1) * coeff[2] + pow(x, 0) * coeff[3];
    COMPARE<Real>(computePoly(x, coeff), val, 1e-3, "polynomial " + to_string(count));
}

int main()
{
    compare(11.4, {1, 9, 1, 9});
    compare(5.14, {0, 8, 1, 0});
}