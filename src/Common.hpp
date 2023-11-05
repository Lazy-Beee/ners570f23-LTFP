#ifndef __Common_h__
#define __Common_h__

#include <cfloat>
#include <eigen3/Eigen/Dense>

#ifdef USE_DOUBLE

typedef double Real;
#define REAL_MAX DBL_MAX
#define REAL_MIN DBL_MIN

#else

typedef float Real;
#define REAL_MAX FLT_MAX
#define REAL_MIN FLT_MIN

#endif
#endif

using Vector2r = Eigen::Matrix<Real, 2, 1, Eigen::DontAlign>;
using Vector3r = Eigen::Matrix<Real, 3, 1, Eigen::DontAlign>;

using Vector2i = Eigen::Matrix<int, 2, 1, Eigen::DontAlign>;
using Vector3i = Eigen::Matrix<int, 3, 1, Eigen::DontAlign>;