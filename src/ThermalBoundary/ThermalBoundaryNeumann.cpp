#include "ThermalBoundaryNeumann.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ThermalBoundaryNeumann::ThermalBoundaryNeumann(ConfigNeumann *config)
        : ThermalBoundary(config)
    {
        _fluxPolyCoeff.resize(3, {0});

        if ((_location != XPOSITIVE) && (_location != XNEGATIVE))
        {
            if (config->xFluxPoly.size() == 0)
                LOG_WARN << "X polynomial of [Neumann] boundary " << _index << " is empty";
            else
                _fluxPolyCoeff[0] = config->xFluxPoly;
        }

        if ((_location != YPOSITIVE) && (_location != YNEGATIVE))
        {
            if (config->yFluxPoly.size() == 0)
                LOG_WARN << "Y polynomial of [Neumann] boundary " << _index << " is empty";
            else
                _fluxPolyCoeff[1] = config->yFluxPoly;
        }
        if ((_location != ZPOSITIVE) && (_location != ZNEGATIVE))
        {
            if (config->zFluxPoly.size() == 0)
                LOG_WARN << "Z polynomial of [Neumann] boundary " << _index << " is empty";
            else
                _fluxPolyCoeff[2] = config->zFluxPoly;
        }
    }

    Real ThermalBoundaryNeumann::getFlux(const Vector3r &pos, [[maybe_unused]] const Real &temp)
    {
        return computePoly3r(pos, _fluxPolyCoeff).sum();
    }
}