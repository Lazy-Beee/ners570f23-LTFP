#include "ThermalBoundaryRadiation.hpp"
#include <math.h>
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ThermalBoundaryRadiation::ThermalBoundaryRadiation(ConfigRadiation *config)
        : ThermalBoundary(config)
    {
        _ambientTemp = config->ambientTemp;
        if (_ambientTemp < 0.0f)
            LOG_WARN << "Ambient temperature of [Radiation] boundary " << this->_index << " is negative";

        _emissivityCoeff = config->emissivityCoeff;
        if (_emissivityCoeff < 0.0f)
            LOG_WARN << "Radiation coefficient of [Radiation] boundary " << this->_index << " is negative";

        _ambientTemp4 = pow(_ambientTemp, 4);
        _coeff = _emissivityCoeff * 5.67e-8f;
    }

    Real ThermalBoundaryRadiation::getFlux([[maybe_unused]] const Vector3r &pos, const Real &temp)
    {
        return (pow(temp, 4) - _ambientTemp4) * _coeff;
    }
}