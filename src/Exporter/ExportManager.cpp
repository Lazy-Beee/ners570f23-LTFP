#include "ExportManager.hpp"
#include "src/SceneLoader.hpp"
#include "src/TimeManager.hpp"
#include "utilities/Logger.hpp"

namespace LTFP
{
    ExportManager *ExportManager::current = nullptr;

    ExportManager::ExportManager()
    {
        _exporters = {};
    }

    ExportManager::~ExportManager()
    {
        current = nullptr;

        for (Exporter *exporter : _exporters)
            delete exporter;
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

        for (SceneLoader::ExporterConfig config : exportConfig.exporters)
        {
            if (config.type == VTK_MESH)
            {
                ExporterVtkMesh *exporter = new ExporterVtkMesh(config.type);
                exporter->init(config);
                _exporters.push_back(exporter);
            }
            else if (config.type == CSV_MESH)
            {
                ExporterCsvMesh *exporter = new ExporterCsvMesh(config.type);
                exporter->init(config);
                _exporters.push_back(exporter);
            }
            else
            {
                LOG_WARN << "Unidentified boundary type " << config.type;
            }
        }
    }

    /// @brief Check export period and export
    void ExportManager::step()
    {
        for (Exporter *exporter : _exporters)
            if (exporter->timeToExport())
                exporter->exportData();
    }
}