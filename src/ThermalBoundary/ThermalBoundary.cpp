#include "ThermalBoundary.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ThermalBoundary::ThermalBoundary(Config *config)
        : _index(config->index),
          _type(static_cast<BoundaryType>(config->type)),
          _location(static_cast<BoundaryLocation>(config->location))
    {
        LOG_DEBUG << "Thermal boundary " << _index
                  << " of type " << BoundaryTypeName[_type]
                  << " at location " << BoundaryLocationName[_location] << " is created";
    }
}