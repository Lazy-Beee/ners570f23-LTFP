#ifndef __src_Common__
#define __src_Common__

#include <cfloat>
#include <vector>
#include <eigen3/Eigen/Dense>

namespace LTFP
{
#ifdef USE_DOUBLE

    typedef double Real;
#define REAL_MAX DBL_MAX
#define REAL_MIN DBL_MIN
#define TEMP_MAX 3000.0
#define TEMP_MIN -100.0

#else

    typedef float Real;
#define REAL_MAX FLT_MAX
#define REAL_MIN FLT_MIN
#define TEMP_MAX 3000.0f
#define TEMP_MIN -100.0f

#endif



    using Vector2r = Eigen::Matrix<Real, 2, 1, Eigen::DontAlign>;
    using Vector3r = Eigen::Matrix<Real, 3, 1, Eigen::DontAlign>;

    using Vector2i = Eigen::Matrix<int, 2, 1, Eigen::DontAlign>;
    using Vector3i = Eigen::Matrix<int, 3, 1, Eigen::DontAlign>;

    Real computePoly(const Real &x, const std::vector<Real> &coeff);
    Vector3r computePoly3r(const Vector3r &x, const std::vector<std::vector<Real>> &coeff);
}

#endif