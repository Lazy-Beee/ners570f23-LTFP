#ifndef __ThermalBoundaryDirichlet__
#define __ThermalBoundaryDirichlet__

#include "src/Common.hpp"
#include "ThermalBoundary.hpp"

namespace LTFP
{
    /// @brief Class of Dirichlet thermal boundary.
    /// 
    class ThermalBoundaryDirichlet : public ThermalBoundary
    {
    private:
        Real _temp;

    public:
        ThermalBoundaryDirichlet(int boundaryType);
        ~ThermalBoundaryDirichlet();

        void init();
        Real getFlux(const Vector3r &pos, const Real &temp);
        std::vector<Real> getParam();
    };
}

#endif
