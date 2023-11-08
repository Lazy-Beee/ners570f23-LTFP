#ifndef __ThermalBoundaryDirichlet__
#define __ThermalBoundaryDirichlet__

#include <vector>
#include "src/Common.hpp"
#include "ThermalBoundary.hpp"

namespace LTFP
{
    /// @brief Class of Dirichlet thermal boundary.
    /// 
    class ThermalBoundaryDirichlet : public ThermalBoundary
    {
    private:
        std::vector<std::vector<Real>> _tempPolyCoeff;

    public:
        ThermalBoundaryDirichlet(int boundaryType);
        ~ThermalBoundaryDirichlet();

        void init();
        Real getFlux(const Vector3r &pos, const Real &temp);
        Real getTemp(const Vector3r &pos);
        std::vector<Real> getParam();
    };
}

#endif
