#include "ThermalBoundaryMirror.hpp"
#include <math.h>
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ThermalBoundaryMirror::ThermalBoundaryMirror(ConfigMirror *config)
        : ThermalBoundary(config)
    {
    }

    Real ThermalBoundaryMirror::getTemp([[maybe_unused]] const Vector3r &pos, const Real &temp)
    {
        return temp;
    }
}