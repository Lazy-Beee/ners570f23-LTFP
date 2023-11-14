#include "ThermalBoundaryConvection.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ThermalBoundaryConvection::ThermalBoundaryConvection(ConfigConvection *config)
        : ThermalBoundary(config)
    {
        _ambientTemp = config->ambientTemp;
        if (_ambientTemp < 0.0f)
            LOG_WARN << "Ambient temperature of [Convection] boundary " << this->_index << " is negative";

        _convectionCoeff = config->convectionCoeff;
        if (_convectionCoeff < 0.0f)
            LOG_WARN << "Convection coefficient of [Convection] boundary " << this->_index << " is negative";
    }

    Real ThermalBoundaryConvection::getFlux([[maybe_unused]] const Vector3r &pos, const Real &temp)
    {
        return (_ambientTemp - temp) * _convectionCoeff;
    }
}