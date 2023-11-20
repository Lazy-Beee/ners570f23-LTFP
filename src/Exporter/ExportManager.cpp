#include "ExportManager.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

namespace LTFP
{
    ExportManager *ExportManager::current = nullptr;

    ExportManager::ExportManager()
    {
    }

    ExportManager::~ExportManager()
    {
        current = nullptr;
    }

    ExportManager *ExportManager::getCurrent()
    {
        if (current == nullptr)
            current = new ExportManager();

        return current;
    }

    /// @brief Load configuration from SceneLoader
    void ExportManager::init()
    {
        SceneLoader::ExportConfig exportConfig = SceneLoader::getCurrent()->getExportConfig();

        // TODO: create exporters
    }

    /// @brief Check export period and export
    void ExportManager::step()
    {
        // TODO: check time and call export
        
    }
}