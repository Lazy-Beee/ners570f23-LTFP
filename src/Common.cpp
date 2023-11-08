#include "Common.hpp"

namespace LTFP
{
    /// @brief Compute polynomial of any order
    /// @param x value of x
    /// @param coeff coefficient of polynomial listed from higher order to lower order
    /// @return computed polynomial value
    Real computePoly(const Real &x, const std::vector<Real> &coeff)
    {
        Real result = 0.0;

        for (const Real &c : coeff)
            result = result * x + c;

        return result;
    }
}