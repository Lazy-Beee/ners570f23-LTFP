#ifndef __ThermalBoundaryNeumann__
#define __ThermalBoundaryNeumann__

#include <vector>
#include "src/Common.hpp"
#include "ThermalBoundary.hpp"

using ConfigNeumann = LTFP::SceneLoader::BoundaryConfigNeumann;

namespace LTFP
{
    /// @brief Class of Dirichlet thermal boundary.
    /// 
    class ThermalBoundaryNeumann : public ThermalBoundary
    {
    private:
        std::vector<std::vector<Real>> _fluxPolyCoeff;

    public:
        ThermalBoundaryNeumann(ConfigNeumann *config);
        ~ThermalBoundaryNeumann() {};
        
        Real getFlux(const Vector3r &pos, const Real &temp);
    };
}

#endif
