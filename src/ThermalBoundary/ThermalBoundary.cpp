#include "ThermalBoundary.hpp"
#include "utilities/Logger.hpp"

namespace LTFP
{
    ThermalBoundary::ThermalBoundary(int boundaryType) : _boundaryType(static_cast<BoundaryType>(boundaryType))
    {
        LOG_DEBUG << "Thermal boundary " << _boundaryIndex << " of type " << _boundaryType << " is created";
    }
}