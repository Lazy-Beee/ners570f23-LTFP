#ifndef __MaterialProperty__
#define __MaterialProperty__

#include "Common.hpp"
#include <string>

namespace LTFP
{
    enum PropertyType
    {
        DENSITY = 0,
        SPECIFIC_HEAT = 1,
        ENTHALPY = 2,
        CONDUCTIVITY = 3
    };
    static std::vector<std::string> PropTypeName = {
        "[Density]",
        "[Specific heat]",
        "[Enthalpy]",
        "[Conductivity]"};
    static const int PropTypeCount = 4;

    /// @brief Class managing material properties.
    /// @note This class is a singleton.
    /// @note For the enthalpy method, tabulation is enforced for reverse search. Specific heat will be computed from the temperature derivative.
    class MaterialProperty
    {
    private:
        static MaterialProperty *current;
        std::vector<bool> _tabulate;
        std::vector<bool> _propLoaded;
        std::vector<PiecewisePoly> _propPoly;
        std::vector<Table> _propTable;
        std::vector<std::vector<Real>> _tabulateStep;
        bool _useEnthalpy;

        

    public:
        MaterialProperty();
        MaterialProperty(const MaterialProperty &) = delete;
        MaterialProperty &operator=(const MaterialProperty &) = delete;
        ~MaterialProperty();

        static MaterialProperty *getCurrent();
        void init();

        template <PropertyType ptype>
        Real getProperty(const Real &temp);
        Real getKappa(const Real &temp);

        // Real getDensity(const Real &temp) { return getProperty(DENSITY, temp); };
        // Real getSpecificHeat(const Real &temp) { return getProperty(SPECIFIC_HEAT, temp); };
        // Real getEnthalpy(const Real &temp) { return getProperty(ENTHALPY, temp); };
        // Real getConductivity(const Real &temp) { return getProperty(CONDUCTIVITY, temp); };

        bool useEnthalpy() { return _useEnthalpy; };
        Real getTemperature(const Real &enthalpy);
        void updateTempDu(Real &temp, const Real &du);
    };
}

#endif
