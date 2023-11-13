#ifndef __ThermalBoundaryConvection__
#define __ThermalBoundaryConvection__

#include <vector>
#include "src/Common.hpp"
#include "ThermalBoundary.hpp"

using ConfigConvection = LTFP::SceneLoader::BoundaryConfigConvection;

namespace LTFP
{
    /// @brief Class of Convection thermal boundary.
    /// 
    class ThermalBoundaryConvection : public ThermalBoundary
    {
    private:
        Real _ambientTemp;
        Real _convectionCoeff;

    public:
        ThermalBoundaryConvection(ConfigConvection *config);
        ~ThermalBoundaryConvection() {};
        
        Real getFlux(const Vector3r &pos, const Real &temp);
    };
}

#endif
