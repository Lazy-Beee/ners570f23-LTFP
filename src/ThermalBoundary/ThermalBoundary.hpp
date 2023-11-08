#ifndef __ThermalBoundary__
#define __ThermalBoundary__

#include "src/Common.hpp"

namespace LTFP
{
    /// @brief Thermal boundary type
    enum BoundaryType
    {
        Neumann = 0,
        Dirichlet = 1,
        Radiation = 2
    };

    /// @brief Thermal boundary location
    enum BoundaryLocation
    {
        xPositive = 0,
        xNegative = 1,
        yPositive = 2,
        yNegative = 3,
        zPositive = 4,
        zNegative = 5
    };

    /// @brief Base class of thermal boundaries.
    class ThermalBoundary
    {
    private:
    protected:
        int _boundaryIndex;
        const BoundaryType _boundaryType;

    public:
        ThermalBoundary(int boundaryType);
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
