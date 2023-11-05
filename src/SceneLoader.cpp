#include "SceneLoader.hpp"
#include <fstream>
#include <filesystem>
#include "Simulator.hpp"
#include "../utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    SceneLoader* SceneLoader::current = nullptr;

    SceneLoader::SceneLoader()
    {
        _fatalError = false;
    }

    SceneLoader::~SceneLoader()
    {
        current = nullptr;
    }

    SceneLoader* SceneLoader::getCurrent()
    {
        if (current == nullptr)
            current = new SceneLoader();

        return current;
    }

    /// @brief Read Time section from scene file
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

            bool exitEnsured = false;

            _timeConfig.endTime = REAL_MAX;
            if (readValue(config["endTime"], _timeConfig.endTime))
                exitEnsured = true;

            _timeConfig.maxTimeSteps = INT_MAX;
            if (readValue(config["maxTimeSteps"], _timeConfig.maxTimeSteps))
                exitEnsured = true;

            if (!exitEnsured)
            {
                LOG_ERR << "Both end time and maximum time steps are undefined.";
                _fatalError = true;
            }
        }
        else
        {
            LOG_ERR << "Cannot load TimeConfig from scene file";
            _fatalError = true;
        }
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
        catch (const exception& e)
        {
            LOG_ERR << "Cannot load scene file to jsonData.";
            LOG_ERR << e.what();
            exit(1);
        }

        readTimeConfig();

        if (_fatalError)
        {
            LOG_ERR << "Failed to load essential configuration(s)";
            exit(1);
        }
    }
}