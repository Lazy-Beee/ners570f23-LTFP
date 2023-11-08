#include "MaterialProperty.hpp"
#include "SceneLoader.hpp"
#include "utilities/Logger.hpp"

namespace LTFP
{
    MaterialProperty *MaterialProperty::current = nullptr;

    MaterialProperty::MaterialProperty()
    {
        _propPoly.resize(PropertyTypeCount);
        for (auto& pp : _propPoly)
            pp.clear();
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
        SceneLoader::TimeConfig timeConfig = SceneLoader::getCurrent()->getTimeConfig();
    }
}