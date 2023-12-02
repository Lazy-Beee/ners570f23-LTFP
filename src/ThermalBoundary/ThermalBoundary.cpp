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

    /// @brief Compute the simple flux thought the boundary
    /// @param pos Position of the boundary neighboring cell
    /// @param temp Temperature of boundary neighboring cell
    /// @return Flux though the boundary
    Real ThermalBoundary::getFlux([[maybe_unused]] const Vector3r &pos, [[maybe_unused]] const Real &temp)
    {
        LOG_WARN << "Function in ThermalBoundary base class is called, getFlux is not supported by the boundary type " << BoundaryTypeName[_type];
        return 0.0f;
    }

    /// @brief Get boundary temperature
    /// @param pos Position of the boundary neighboring cell
    /// @param temp Temperature of boundary neighboring cell
    /// @return Ghost cell temperature
    Real ThermalBoundary::getTemp([[maybe_unused]] const Vector3r &pos, [[maybe_unused]] const Real &temp)
    {
        LOG_WARN << "Function in ThermalBoundary base class is called, getTemp is not supported by the boundary type " << BoundaryTypeName[_type];
        return 0.0f;
    }
}