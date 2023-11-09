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
        _propPoly.resize(PropTypeCount, {});
        _propTable.resize(PropTypeCount, {});
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
            if (config.type < 0 || config.type >= PropTypeCount)
            {
                LOG_WARN << "Undefined material type " << config.type;
                continue;
            }
            if (_propLoaded[config.type])
            {
                LOG_WARN << "Repeated defintion of type " << config.type;
                continue;
            }

            vector<Real> tr = config.tempRange;
            vector<vector<Real>> ps = config.polynomials;

            if (tr.size() < 2 || ps.size() < 1 || (tr.size() - ps.size()) != 1)
            {
                LOG_WARN << "Incorrect size of tempRange or polynomials in type " << config.type;
                continue;
            }
            for (size_t i = 0; i < (tr.size() - 1); i++)
            {
                if (tr[i] >= tr[i + 1])
                {
                    LOG_WARN << "Reverse temperature range in type " << config.type;
                    continue;
                }
            }
            for (size_t i = 0; i < ps.size(); i++)
            {
                if (ps[i].size() == 0)
                {
                    LOG_WARN << "Empty polynomial in type " << config.type;
                    continue;
                }
            }

            _tabulate[config.type] = config.tabulate;
            _propPoly[config.type].first = config.tempRange;
            _propPoly[config.type].second = config.polynomials;
            _propLoaded[config.type] = true;
        }

        // If enthalpy is loaded, disable specific heat
        if (_propLoaded[2])
            _propLoaded[1] = false;

        // Generate tables
        tabulate();
    }

    /// @brief Generate temperature and property table
    void MaterialProperty::tabulate()
    {
        // TODO: generate table
    }

    /// @brief Compute material property from piecewise polynomial
    /// @param type material property type
    /// @param temp temperature of material
    /// @return Material property at given temperature
    /// @note Assuming there should be less than 10, binary search is not used when determining which 'piece' the temperature falls in.
    /// @note For temperature input out of range, the corresponding endpoint value is returned
    Real MaterialProperty::getProperty(PropertyType type, Real temp)
    {
        if (!_propLoaded[static_cast<int>(type)])
        {
            LOG_ERR << "Attempting to visit empty material property " << static_cast<int>(type);
            exit(1);
        }

        if (_tabulate[static_cast<int>(type)])
            return getPropertyPoly(type, temp);
        else
            return getPropertyTable(type, temp);
    }

    /// @brief Subfunction of getProperty()
    Real MaterialProperty::getPropertyPoly(PropertyType type, Real temp)
    {
        const vector<Real> &tempRange = _propPoly[static_cast<int>(type)].first;
        const vector<vector<Real>> &propPoly = _propPoly[static_cast<int>(type)].second;

        if (temp < tempRange.front())
            return computePoly(tempRange.front(), propPoly.front());

        for (size_t i = 1; i < tempRange.size(); i++)
            if (temp < tempRange[i])
                return computePoly(temp, propPoly[i - 1]);

        return computePoly(tempRange.back(), propPoly.back());
    }

    /// @brief Subfunction of getProperty()
    Real MaterialProperty::getPropertyTable(PropertyType type, Real temp)
    {
        // TODO: look up table

        return getPropertyPoly(type, temp);
    }
}