#ifndef __src_Common__
#define __src_Common__

#include <cfloat>
#include <vector>

#ifdef GREATLAKES
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include <Eigen/Dense>
#pragma GCC diagnostic pop
#else
#include <eigen3/Eigen/Dense>
#endif

namespace LTFP
{
#ifdef USE_DOUBLE

    typedef double Real;
#define REAL_MAX DBL_MAX
#define REAL_MIN DBL_MIN

#else

    typedef float Real;
#define REAL_MAX FLT_MAX
#define REAL_MIN FLT_MIN

#endif

    using Vector2r = Eigen::Matrix<Real, 2, 1, Eigen::DontAlign>;
    using Vector3r = Eigen::Matrix<Real, 3, 1, Eigen::DontAlign>;

    using Vector2i = Eigen::Matrix<int, 2, 1, Eigen::DontAlign>;
    using Vector3i = Eigen::Matrix<int, 3, 1, Eigen::DontAlign>;

    /// @brief Piecewise polynomial, pair(x range, polynomial)
    using PiecewisePoly = std::pair<std::vector<Real>, std::vector<std::vector<Real>>>;
    /// @brief Table, pair(x values, y values)
    using Table = std::pair<std::vector<Real>, std::vector<Real>>;

    Real computePoly(const Real &x, const std::vector<Real> &coeff);
    Vector3r computePoly3r(const Vector3r &x, const std::vector<std::vector<Real>> &coeff);
    Real computePiecewisePoly(const Real &x, const PiecewisePoly &poly);
    void tabulatePoly(Vector2r range, std::vector<Real> coeff, Real stepSize, Table &table);
    Table tabulatePiecewisePoly(PiecewisePoly poly, std::vector<Real> stepSize);
    Real lookupTable(const Real &x, const Table &table, bool inverse = false);
    void resizeMeshReal(std::vector<std::vector<std::vector<Real>>> &meshin,
                        const size_t xSize, const size_t ySize, const size_t zSize);
    void resizeMeshVector3r(std::vector<std::vector<std::vector<Vector3r>>> &meshin,
                            const size_t xSize, const size_t ySize, const size_t zSize);
}

#endif