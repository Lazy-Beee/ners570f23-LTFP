#ifndef __ThermalBoundary__
#define __ThermalBoundary__

#include "Common.hpp"
#include <vector>

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
        const BoundaryType _boundaryType;

    public:
        ThermalBoundary(int boundaryType);
        virtual ~ThermalBoundary()=0;

        virtual void init() = 0;
        virtual Real getFlux(const Vector3r &pos, const Real &temp) = 0;
    };
}

#endif
