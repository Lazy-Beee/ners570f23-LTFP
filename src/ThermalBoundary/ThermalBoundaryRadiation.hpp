#ifndef __ThermalBoundaryRadiation__
#define __ThermalBoundaryRadiation__

#include <vector>
#include "src/Common.hpp"
#include "ThermalBoundary.hpp"

using ConfigRadiation = LTFP::SceneLoader::BoundaryConfigRadiation;

namespace LTFP
{
    /// @brief Class of Radiation thermal boundary.
    /// 
    class ThermalBoundaryRadiation : public ThermalBoundary
    {
    private:
        Real _ambientTemp;
        Real _emissivityCoeff;
        Real _ambientTemp4;
        Real _coeff;

    public:
        ThermalBoundaryRadiation(ConfigRadiation *config);
        ~ThermalBoundaryRadiation() {};
        
        Real getFlux(const Vector3r &pos, const Real &temp);
    };
}

#endif
