#ifdef USE_DOUBLE

typedef double Real;
#define REAL_MAX DBL_MAX
#define REAL_MIN DBL_MIN

#else

typedef float Real;
#define REAL_MAX FLT_MAX
#define REAL_MIN FLT_MIN

#endif