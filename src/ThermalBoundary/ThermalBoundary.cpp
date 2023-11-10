#include "ThermalBoundary.hpp"
#include "utilities/Logger.hpp"

namespace LTFP
{
    ThermalBoundary::ThermalBoundary(Config config) : _index(config.index),
                                                      _type(static_cast<BoundaryType>(config.type)),
                                                      _location(static_cast<BoundaryLocation>(config.location))
    {
        LOG_DEBUG << "Thermal boundary " << _index << " of type " << BoundaryTypeName[_type] << " at location " << BoundaryLocationName[_location] << " is created";
    }

    /// @brief Compute the simple flux thought the boundary
    /// @param pos Position of the boundary neighboring cell
    /// @param temp Temperature of boundary neighboring cell
    /// @return Flux though the boundary
    Real ThermalBoundary::getFlux(const Vector3r &pos, const Real &temp)
    {
#ifndef NDEBUG
        LOG_WARN << "getFlux function in ThermalBoundary base class is called";
        exit(1);
#endif
        return Real();
    }

    /// @brief Get the temeprature of the ghost boundary cell
    /// @param pos Position of the boundary neighboring cell
    /// @return Temperature of the boundary
    Real ThermalBoundary::getTemp(const Vector3r &pos)
    {
#ifndef NDEBUG
        LOG_WARN << "getTemp function in ThermalBoundary base class is called";
        exit(1);
#endif
        return Real();
    }
}