#include "Common.hpp"
#include <iostream>
#include "utilities/Logger.hpp"

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

        for (size_t i = 0; i < 3; i++)
            result[i] = computePoly(x[i], coeff[i]);

        return result;
    }

    /// @brief Compute piecewise polynomial
    /// @param x value of x
    /// @param poly the piecewise poly
    /// @return computed value
    /// @note Assuming there should be less than 10 pieces, binary search is not used.
    Real computePiecewisePoly(const Real &x, const PiecewisePoly &poly)
    {
        const vector<Real> &xRange = poly.first;
        const vector<vector<Real>> &coeff = poly.second;

        if (x < xRange.front())
        {
#ifndef NDEBUG
            LOG_WARN << "Range overflow (lower bound) during piecewise poly evaluation";
#endif
            return computePoly(xRange.front(), coeff.front());
        }

        for (size_t i = 1; i < xRange.size(); i++)
            if (x <= xRange[i])
                return computePoly(x, coeff[i - 1]);

#ifndef NDEBUG
        LOG_WARN << "Range overflow (upper bound) during piecewise poly evaluation";
#endif
        return computePoly(xRange.back(), coeff.back());
    }

    /// @brief Tabulate a piece of polynomial
    /// @param range x range
    /// @param coeff polynomial coefficient
    /// @param stepSize tabulation step size
    void tabulatePoly(Vector2r range, std::vector<Real> coeff, Real stepSize, Table &table)
    {
        vector<Real> &x = table.first;
        vector<Real> &y = table.second;

        // If table is empty, fill in the first value
        if (x.empty())
        {
            x.push_back(range[0]);
            y.push_back(computePoly(range[0], coeff));
        }
        else
        {
            y.back() = 0.5 * y.back() + 0.5 * computePoly(range[0], coeff);
        }

        // Resize table
        Real pieceLength = range[1] - range[0];
        size_t pieceSize = ceil(pieceLength / stepSize);
        size_t oldSize = x.size();
        size_t newSize = oldSize + pieceSize;
        x.resize(newSize);
        y.resize(newSize);

        // Compute table
#pragma omp parallel for
        for (int i = 0; i < static_cast<int>(pieceSize - 1); i++)
        {
            x[oldSize + i] = range[0] + stepSize * (i + 1);
            y[oldSize + i] = computePoly(x[oldSize + i], coeff);
        }

        x[newSize - 1] = range[1];
        y[newSize - 1] = computePoly(range[1], coeff);
    }

    /// @brief Tabulate a piecewise polynomial
    /// @param poly piecewise polynomial
    /// @param stepSize tabulation step size
    /// @return Generated table
    Table tabulatePiecewisePoly(PiecewisePoly poly, std::vector<Real> stepSize)
    {
        Table table;

        for (size_t i = 0; i < (poly.first.size() - 1); i++)
        {
            Vector2r range = {poly.first[i], poly.first[i + 1]};
            tabulatePoly(range, poly.second[i], stepSize[i], table);
        }

        return table;
    }

    /// @brief Look up table using binary search and linear interpolation
    /// @param x value to look up
    /// @param table the table
    /// @param inverse reverse x and y column of table
    /// @return value obtained from table
    Real lookupTable(const Real &x, const Table &table, bool inverse)
    {
        const vector<Real> *xTable = &table.first;
        const vector<Real> *yTable = &table.second;

        if (inverse)
            swap(xTable, yTable);

        if (x < xTable->front())
        {
#ifndef NDEBUG
            LOG_WARN << "Range overflow (lower bound) during tabular interpolation";
#endif
            return yTable->front();
        }

        if (x > xTable->back())
        {
#ifndef NDEBUG
            LOG_WARN << "Range overflow (upper bound) during tabular interpolation";
#endif
            return yTable->back();
        }

        int low = 0;
        int high = xTable->size();
        int mid = (low + high) / 2;

        while ((high - low) > 1)
        {
            if ((*xTable)[mid] < x)
                low = mid;
            else
                high = mid;

            mid = (low + high) / 2;
        }

        const Real &x1 = (*xTable)[low];
        const Real &x2 = (*xTable)[low + 1];
        const Real &y1 = (*yTable)[low];
        const Real &y2 = (*yTable)[low + 1];

        return (y2 - y1) * (x - x1) / (x2 - x1) + y1;
    }

    /// @brief To resize the meshReal with xSize, ySize, and zSize (Nx, Ny, Nz)
    void resizeMeshReal(MeshReal &meshin, const size_t xSize, const size_t ySize, const size_t zSize)
    {
        meshin.resize(xSize);
        for (auto &xVec : meshin)
        {
            xVec.resize(ySize);
            for (auto &yVec : xVec)
            {
                yVec.resize(zSize);
            }
        }
    }

    /// @brief To resize the meshVector3r with xSize, ySize, and zSize (Nx, Ny, Nz)
    void resizeMeshVector3r(MeshVector &meshin, const size_t xSize, const size_t ySize, const size_t zSize)
    {
        meshin.resize(xSize);
        for (auto &xVec : meshin)
        {
            xVec.resize(ySize);
            for (auto &yVec : xVec)
            {
                yVec.resize(zSize);
            }
        }
    }

}