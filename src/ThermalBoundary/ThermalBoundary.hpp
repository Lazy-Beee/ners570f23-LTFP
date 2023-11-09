#ifndef __ThermalBoundary__
#define __ThermalBoundary__

#include "src/Common.hpp"

namespace LTFP
{
    /// @brief Thermal boundary type
    enum BoundaryType
    {
        NEUMANN = 0,
        DIRICHLET = 1,
        RADIATION = 2
    };

    /// @brief Thermal boundary location
    enum BoundaryLocation
    {
        XPOSITIVE = 0,
        XNEGATIVE = 1,
        YPOSITIVE = 2,
        YNEGATIVE = 3,
        ZPOSITIVE = 4,
        ZNEGATIVE = 5
    };

    /// @brief Base class of thermal boundaries.
    class ThermalBoundary
    {
    protected:
        const int _boundaryIndex;
        const BoundaryType _boundaryType;

    public:
        ThermalBoundary(int boundaryIndex, int boundaryType);
        virtual ~ThermalBoundary() = 0;

        /// @brief Initialize thermal boundary
        virtual void init() = 0;

        /// @brief Compute the simple flux thought the boundary
        /// @param pos Position of the boundary neighboring cell
        /// @param temp Temperature of boundary neighboring cell
        /// @return Flux though the boundary
        virtual Real getFlux(const Vector3r &pos, const Real &temp) = 0;

        /// @brief Get the thermal boundary parameters
        /// @return Vector of parameters
        virtual std::vector<Real> getParam() = 0;
    };
}

#endif
