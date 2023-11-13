#ifndef __ThermalBoundary__
#define __ThermalBoundary__

#include "src/Common.hpp"
#include <string>
#include "src/SceneLoader.hpp"

using Config = LTFP::SceneLoader::BoundaryConfig;

namespace LTFP
{
    /// @brief Thermal boundary type
    enum BoundaryType
    {
        NEUMANN = 0,
        DIRICHLET = 1,
        CONVECTION = 2,
        RADIATION = 3,
        MIRROR = 4
    };
    static std::vector<std::string> BoundaryTypeName = {
        "[Neumann]",
        "[Dirichlet]",
        "[Convection]",
        "[Radiation]",
        "[Mirror]"};

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
    static std::vector<std::string> BoundaryLocationName = {
        "[X+]",
        "[X-]",
        "[Y+]",
        "[Y-]",
        "[Z+]",
        "[Z-]"};

    /// @brief Base class of thermal boundaries.
    class ThermalBoundary
    {
    protected:
        const int _index;
        const BoundaryType _type;
        const BoundaryLocation _location;

    public:
        ThermalBoundary(Config *config);
        virtual ~ThermalBoundary(){};

        virtual Real getTemp(const Vector3r &pos, const Real &temp);
        virtual Real getFlux(const Vector3r &pos, const Real &temp);

        inline int getIndex() const { return _index; };
        inline BoundaryType getType() const { return _type; };
        inline BoundaryLocation getLocation() const { return _location; };
    };
}

#endif
