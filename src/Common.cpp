#include "Common.hpp"

using namespace std;

namespace LTFP
{
    /// @brief Compute polynomial of any order
    /// @param x value of x
    /// @param coeff coefficient of polynomial listed from higher order to lower order
    /// @return computed polynomial value
    Real computePoly(const Real &x, const vector<Real> &coeff)
    {
        Real result = 0.0;

        for (const Real &c : coeff)
            result = result * x + c;

        return result;
    }

    /// @brief Compute 3 polynomial together
    /// @param x input vector
    /// @param coeff vector of coefficient in 3 directions
    /// @return output vector
    Vector3r computePoly3r(const Vector3r &x, const vector<vector<Real>> &coeff)
    {
        Vector3r result;
        
        for (int i = 0; i < 3; i++)
            result[i] = computePoly(x[i], coeff[i]);

        return result;
    }
}