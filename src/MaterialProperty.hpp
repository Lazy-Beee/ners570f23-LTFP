#ifndef __MaterialProperty__
#define __MaterialProperty__

#include "Common.hpp"

namespace LTFP
{
    enum PropertyType
    {
        DENSITY = 0,
        SPECIFIC_HEAT = 1,
        ENTHALPY = 2,
        CONDUCTIVITY = 3
    };

    static const int PropTypeCount = 4;
    static const Real TabulateSize = 0.1;

    /// @brief Class managing material properties.
    /// @note This class is a singleton.
    class MaterialProperty
    {
    private:
        static MaterialProperty *current;
        std::vector<bool> _tabulate;
        std::vector<bool> _propLoaded;
        /// @brief Pair of temperature points and contained polynomial
        std::vector<std::pair<std::vector<Real>, std::vector<std::vector<Real>>>> _propPoly;
        /// @brief Pair of temperature table and value table
        std::vector<std::pair<std::vector<Real>, std::vector<Real>>> _propTable;

        void tabulate();
        Real getProperty(PropertyType type, Real temp);
        Real getPropertyPoly(PropertyType type, Real temp);
        Real getPropertyTable(PropertyType type, Real temp);

    public:
        MaterialProperty();
        MaterialProperty(const MaterialProperty &) = delete;
        MaterialProperty &operator=(const MaterialProperty &) = delete;
        ~MaterialProperty();

        static MaterialProperty *getCurrent();
        void init();

        Real getDensity(Real temp) { return getProperty(DENSITY, temp); };
        Real getSpecificHeat(Real temp) { return getProperty(SPECIFIC_HEAT, temp); };
        Real getEnthalpy(Real temp) { return getProperty(ENTHALPY, temp); };
        Real getConductivity(Real temp) { return getProperty(CONDUCTIVITY, temp); };
    };
}

#endif
