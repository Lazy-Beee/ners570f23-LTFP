#include "SceneLoader.hpp"
#include <fstream>
#include <filesystem>
#include "Simulator.hpp"
#include "../utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    SceneLoader *SceneLoader::current = nullptr;

    SceneLoader::SceneLoader()
    {
        _fatalError = false;
    }

    SceneLoader::~SceneLoader()
    {
        current = nullptr;
    }

    SceneLoader *SceneLoader::getCurrent()
    {
        if (current == nullptr)
            current = new SceneLoader();

        return current;
    }

    /// @brief Read Time section of scene file
    void SceneLoader::readTimeConfig()
    {
        if (_jsonData.find("Time") != _jsonData.end())
        {
            json config = _jsonData["Time"];
            _timeConfig = TimeConfig{};
            readValue(config["minTimeStepSize"], _timeConfig.minTimeStepSize);
            readValue(config["maxTimeStepSize"], _timeConfig.maxTimeStepSize);
            readValue(config["endTime"], _timeConfig.endTime);
            readValue(config["maxTimeSteps"], _timeConfig.maxTimeSteps);
        }
        else
            LOG_WARN << "Failed to load TimeConfig from scene file";
    }

    /// @brief Read Mesh section of scene file
    void SceneLoader::readMeshConfig()
    {
        if (_jsonData.find("Mesh") != _jsonData.end())
        {
            json config = _jsonData["Mesh"];
            _meshConfig = MeshConfig{};
            readVector(config["start"], _meshConfig.start);
            readVector(config["end"], _meshConfig.end);
            readValue(config["xCount"], _meshConfig.xCount);
            readValue(config["yCount"], _meshConfig.yCount);
            readValue(config["zCount"], _meshConfig.zCount);
            readValue(config["meshSize"], _meshConfig.meshSize);
            readValue(config["incrementThickness"], _meshConfig.incrementThickness);
            readValue(config["incrementPeriod"], _meshConfig.incrementPeriod);
        }
        else
            LOG_WARN << "Failed to load MeshConfig from scene file";
    }

    /// @brief Read Export section of scene file
    void SceneLoader::readExportConfig()
    {
        if (_jsonData.find("Export") != _jsonData.end())
        {
            json config = _jsonData["Export"];
            _exportConfig = ExportConfig{};
            readValue(config["enableVtkExport"], _exportConfig.enableVtkExport);
            readValue(config["printPeriod"], _exportConfig.printPeriod);
            readValue(config["exportPeriod"], _exportConfig.exportPeriod);
        }
        else
            LOG_WARN << "Failed to load ExportConfig from scene file";
    }

    /// @brief Read scene file and save the configurations
    void SceneLoader::readScene()
    {
        filesystem::path scenePath = Simulator::getCurrent()->getScenePath();
        ifstream input_file(scenePath);
        if (!input_file.is_open())
        {
            LOG_ERR << "Cannot open scene file: " << scenePath;
            exit(1);
        }
        try
        {
            _jsonData = json::parse(input_file);
        }
        catch (const exception &e)
        {
            LOG_ERR << "Cannot open scene file";
            LOG_ERR << e.what();
            exit(1);
        }

        readTimeConfig();
        readMeshConfig();
        readExportConfig();
    }
}