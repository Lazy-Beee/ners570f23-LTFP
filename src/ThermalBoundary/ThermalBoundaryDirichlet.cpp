#include "ThermalBoundaryDirichlet.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ThermalBoundaryDirichlet::ThermalBoundaryDirichlet(ConfigDirichlet *config)
        : ThermalBoundary(config)
    {
        _tempPolyCoeff.resize(3, {0});

        if ((_location != XPOSITIVE) && (_location != XNEGATIVE))
        {
            if (config->xTempPoly.size() == 0)
                LOG_WARN << "X polynomial of boundary " << _index << " is empty";
            else
                _tempPolyCoeff[0] = config->xTempPoly;
        }

        if ((_location != YPOSITIVE) && (_location != YNEGATIVE))
        {
            if (config->yTempPoly.size() == 0)
                LOG_WARN << "Y polynomial of boundary " << _index << " is empty";
            else
                _tempPolyCoeff[1] = config->yTempPoly;
        }
        if ((_location != ZPOSITIVE) && (_location != ZNEGATIVE))
        {
            if (config->zTempPoly.size() == 0)
                LOG_WARN << "Z polynomial of boundary " << _index << " is empty";
            else
                _tempPolyCoeff[2] = config->zTempPoly;
        }
    }

    Real ThermalBoundaryDirichlet::getTemp(const Vector3r &pos, [[maybe_unused]] const Real &temp)
    {
        return computePoly3r(pos, _tempPolyCoeff).sum();
    }
}