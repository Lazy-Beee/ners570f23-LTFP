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

            _timeConfig.minTimeStepSize = REAL_MIN;
            readValue(config["minTimeStepSize"], _timeConfig.minTimeStepSize);

            _timeConfig.maxTimeStepSize = REAL_MAX;
            readValue(config["maxTimeStepSize"], _timeConfig.maxTimeStepSize);

            _timeConfig.endTime = REAL_MAX;
            readValue(config["endTime"], _timeConfig.endTime);

            _timeConfig.maxTimeSteps = INT_MAX;
            readValue(config["maxTimeSteps"], _timeConfig.maxTimeSteps);
        }
        else
        {
            LOG_ERR << "Failed to load TimeConfig from scene file.";
            _fatalError = true;
        }
    }

    /// @brief Read Mesh section of scene file
    void SceneLoader::readMeshConfig()
    {
        if (_jsonData.find("Mesh") != _jsonData.end())
        {
            json config = _jsonData["Mesh"];
            _meshConfig = MeshConfig{};

            if (!readVector(config["start"], _meshConfig.start))
            {
                LOG_ERR << "Failed to load domain start.";
                _fatalError = true;
            }

            if (!readVector(config["end"], _meshConfig.end))
            {
                LOG_ERR << "Failed to load domain end.";
                _fatalError = true;
            }

            _meshConfig.xCount = -1;
            readValue(config["xCount"], _meshConfig.xCount);

            _meshConfig.yCount = -1;
            readValue(config["yCount"], _meshConfig.xCount);

            _meshConfig.zCount = -1;
            readValue(config["zCount"], _meshConfig.xCount);

            _meshConfig.meshSize = -1.0;
            readValue(config["meshSize"], _meshConfig.meshSize);

            _meshConfig.incrementThickness = 0.0;
            readValue(config["incrementThickness"], _meshConfig.incrementThickness);

            _meshConfig.incrementPeriod = REAL_MAX;
            readValue(config["incrementPeriod"], _meshConfig.incrementPeriod);
        }
        else
        {
            LOG_ERR << "Failed to load TimeConfig from scene file.";
            _fatalError = true;
        }
    }

    /// @brief Read scene file and save the configurations
    /// @param terminateOnError Terminate simulation on missing essential info
    void SceneLoader::readScene(bool terminateOnError)
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
            LOG_ERR << "Cannot load scene file to jsonData.";
            LOG_ERR << e.what();
            exit(1);
        }

        readTimeConfig();

        if (_fatalError && terminateOnError)
        {
            LOG_ERR << "Failed to load essential configuration(s), aborting simulation.";
            exit(1);
        }
    }
}