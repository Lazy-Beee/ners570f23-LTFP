#include "ThermalBoundaryDirichlet.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ThermalBoundaryDirichlet::ThermalBoundaryDirichlet(ConfigDirichlet config) : ThermalBoundary(config)
    {
        _tempPolyCoeff.resize(3, {0});

        if (!_location == XPOSITIVE && !_location == XNEGATIVE)
        {
            if (config.xTemp.size() == 0)
                LOG_WARN << "X polynomial of boundary " << _index << " is empty";
            else
                _tempPolyCoeff[0] = config.xTemp;
        }

        if (!_location == YPOSITIVE && !_location == YNEGATIVE)
        {
            if (config.yTemp.size() == 0)
                LOG_WARN << "Y polynomial of boundary " << _index << " is empty";
            else
                _tempPolyCoeff[1] = config.yTemp;
        }

        if (!_location == ZPOSITIVE && !_location == ZNEGATIVE)
        {
            if (config.zTemp.size() == 0)
                LOG_WARN << "Z polynomial of boundary " << _index << " is empty";
            else
                _tempPolyCoeff[2] = config.zTemp;
        }
    }

    Real ThermalBoundaryDirichlet::getTemp(const Vector3r &pos)
    {
        return computePoly3r(pos, _tempPolyCoeff).sum();
    }
}