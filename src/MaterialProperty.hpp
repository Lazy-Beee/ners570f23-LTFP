#ifndef __MaterialProperty__
#define __MaterialProperty__

#include "Common.hpp"

namespace LTFP
{
    enum PropertyType
    {
        DENSITY = 0,
        SPECFIC_HEAT = 1,
        ENTHALPY = 2,
        CONDUCTIVITY = 3
    };
    static const size_t PropertyTypeCount = 4;

    /// @brief Class managing material properties.
    /// @note This class is a singleton.
    class MaterialProperty
    {
    private:
        static MaterialProperty *current;
        std::vector<std::vector<Real>> _propPoly;

        Real getProperty(PropertyType type, const Real &temp);

    public:
        MaterialProperty();
        MaterialProperty(const MaterialProperty &) = delete;
        MaterialProperty &operator=(const MaterialProperty &) = delete;
        ~MaterialProperty();

        static MaterialProperty *getCurrent();
        void init();

        const Real &getDensity(const Real &temp) { getProperty(DENSITY, temp); };
        const Real &getSpecficHeat(const Real &temp) { getProperty(SPECFIC_HEAT, temp); };
        const Real &getEnthalpy(const Real &temp) { getProperty(ENTHALPY, temp); };
        const Real &getConductivity(const Real &temp) { getProperty(CONDUCTIVITY, temp); };
    };
}

#endif
