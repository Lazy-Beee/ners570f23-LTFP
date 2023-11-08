#include "ThermalBoundary.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

namespace LTFP
{
    ThermalBoundary::ThermalBoundary(int boundaryType) : _boundaryType(static_cast<BoundaryType>(boundaryType))
    {
        LOG_DEBUG << "Thermal boundary of type " << _boundaryType << " is created";
    }
}