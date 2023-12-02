#include "MaterialProperty.hpp"
#include <algorithm>
#include "SceneLoader.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    MaterialProperty *MaterialProperty::current = nullptr;

    MaterialProperty::MaterialProperty()
    {
        _tabulate.resize(PropTypeCount, false);
        _propLoaded.resize(PropTypeCount, false);
        _propPoly.resize(PropTypeCount);
        _propTable.resize(PropTypeCount);
        _tabulateStep.resize(PropTypeCount);
        _useEnthalpy = false;
    }

    MaterialProperty::~MaterialProperty()
    {
        current = nullptr;
    }

    MaterialProperty *MaterialProperty::getCurrent()
    {
        if (current == nullptr)
            current = new MaterialProperty();

        return current;
    }

    /// @brief Load configuration from SceneLoader
    void MaterialProperty::init()
    {
        vector<SceneLoader::MatPropConfig> configs = SceneLoader::getCurrent()->getMatPropConfig();

        // Load scene config
        for (SceneLoader::MatPropConfig config : configs)
        {
            // Pre-checks
            if (config.type < 0 || config.type >= PropTypeCount)
            {
                LOG_WARN << "Undefined material property with id " << config.type;
                continue;
            }
            if (_propLoaded[config.type])
            {
                LOG_WARN << "Repeated defintion of property " << PropTypeName[config.type];
                continue;
            }

            // Load config
            _propLoaded[config.type] = true;
            _tabulate[config.type] = config.tabulate;
            _propPoly[config.type].first = config.tempRange;
            _propPoly[config.type].second = config.polynomials;
            if (config.tabulate || config.type == ENTHALPY)
            {
                if (config.tabulateStep.empty())
                    _tabulateStep[config.type].resize(config.polynomials.size(), 0.1f);
                else
                    _tabulateStep[config.type] = config.tabulateStep;
            }

            const vector<Real> &tr = _propPoly[config.type].first;
            const vector<vector<Real>> &ps = _propPoly[config.type].second;
            const vector<Real> &ts = _tabulateStep[config.type];

            // Allow specific heat to be loaded in tabulate switch mode
            if (config.type == SPECIFIC_HEAT && tr.size() == 0 && ps.size() == 0 && ts.size() == 0)
            {
                LOG_INFO << "Property " << PropTypeName[config.type] << " loaded in tabulate switch mode, values will be interpolated from the enthalpy curve";
                _propLoaded[config.type] = false;
                continue;
            }

            // Check piecewise polynomial
            if (tr.size() < 2 || ps.size() < 1 || (tr.size() - ps.size()) != 1)
            {
                LOG_WARN << "Size mismatch in piecewise polynomial of property " << PropTypeName[config.type];
                _propLoaded[config.type] = false;
            }
            if (_tabulate[config.type] && ts.size() != ps.size() && !ts.empty())
            {
                LOG_WARN << "Size mismatch in tabulate step size of property " << PropTypeName[config.type];
                _propLoaded[config.type] = false;
            }
            for (size_t i = 0; i < (tr.size() - 1); i++)
            {
                if (tr[i] >= tr[i + 1])
                {
                    LOG_WARN << "Reverse temperature range in property " << PropTypeName[config.type];
                    _propLoaded[config.type] = false;
                }
            }
            for (size_t i = 0; i < ps.size(); i++)
            {
                if (ps[i].size() == 0)
                {
                    LOG_WARN << "Empty polynomial in property " << PropTypeName[config.type];
                    _propLoaded[config.type] = false;
                }
            }
        }

        // If enthalpy is loaded, override specific heat. Tabulation is still controlled by specific heat config
        if (_propLoaded[ENTHALPY])
        {
            _useEnthalpy = true;
            _tabulate[ENTHALPY] = true; // Force tabulation for reverse lookup

            _propLoaded[SPECIFIC_HEAT] = true;
            _propPoly[SPECIFIC_HEAT].first = _propPoly[ENTHALPY].first;
            _propPoly[SPECIFIC_HEAT].second = _propPoly[ENTHALPY].second;
            for (size_t i = 0; i < _propPoly[SPECIFIC_HEAT].second.size(); i++)
                _propPoly[SPECIFIC_HEAT].second[i].pop_back();
            if (_tabulate[SPECIFIC_HEAT])
                _tabulateStep[SPECIFIC_HEAT] = _tabulateStep[ENTHALPY];
        }

        // Check if properties are defined
        for (int i = 0; i < PropTypeCount; i++)
        {
            if (i == ENTHALPY && _propLoaded[SPECIFIC_HEAT])
                continue;

            if (!_propLoaded[i])
            {
                LOG_WARN << "Property " << PropTypeName[i] << " is undefined, abort simulation";
                exit(1);
            }
        }

        // Generate tables
        for (int i = 0; i < PropTypeCount; i++)
            if (_tabulate[i])
                _propTable[i] = tabulatePiecewisePoly(_propPoly[i], _tabulateStep[i]);

        // Check monotonically increment of enthalpy
        const vector<Real> &enthalpyTable = _propTable[ENTHALPY].second;
        volatile bool reverse = false;
#pragma omp parallel for shared(reverse)
        for (int i = 1; i < static_cast<int>(enthalpyTable.size()); i++)
        {
            if (!reverse && enthalpyTable[i] <= enthalpyTable[i - 1])
            {
#pragma omp critical
                reverse = true;
            }
        }
        if (reverse)
        {
            LOG_ERR << "Non-increasing enthalpy detected, abort simulation";
            exit(1);
        }
    }

    /// @brief Compute material property from piecewise polynomial
    /// @param ptype material property type
    /// @param temp temperature of the material
    /// @return Material property at given temperature
    /// @note For temperature input out of range, the corresponding endpoint value is returned.
    template <PropertyType ptype>
    Real MaterialProperty::getProperty(const Real &temp)
    {
#ifndef NDEBUG
        if (!_propLoaded[ptype])
        {
            LOG_ERR << "Attempting to visit empty material property " << PropTypeName[ptype];
            exit(1);
        }
#endif

        if (_tabulate[ptype])
            return lookupTable(temp, _propTable[ptype]);
        else
            return computePiecewisePoly(temp, _propPoly[ptype]);
    }

    Real MaterialProperty::getTemperature(const Real &enthalpy)
    {
#ifndef NDEBUG
        if (!_useEnthalpy)
        {
            LOG_ERR << "Attempting to inverse interpolate temperature without setting enthalpy";
            exit(1);
        }
#endif

        return lookupTable(enthalpy, _propTable[ENTHALPY], true);
    }

    /// @brief Get diffusion coefficient k/(rho*cp)
    Real MaterialProperty::getKappa(const Real &temp)
    {
        // TODO: the diffusion coefficient can be tabulated
        return getProperty<CONDUCTIVITY>(temp) / (getProperty<DENSITY>(temp) * getProperty<SPECIFIC_HEAT>(temp));
    }

    /// @brief Compute new temperature from enthalpy change
    /// @param temp temperature
    /// @param du enthalpy change
    void MaterialProperty::updateTempDu(Real &temp, const Real &du)
    {
        if (_useEnthalpy)
            temp = getTemperature(getProperty<ENTHALPY>(temp) + du);
        else
            temp += du / getProperty<SPECIFIC_HEAT>(temp);
    }

    template Real MaterialProperty::getProperty<DENSITY>(const Real &temp);
    template Real MaterialProperty::getProperty<SPECIFIC_HEAT>(const Real &temp);
    template Real MaterialProperty::getProperty<ENTHALPY>(const Real &temp);
    template Real MaterialProperty::getProperty<CONDUCTIVITY>(const Real &temp);
}