#include "SceneLoader.hpp"
#include <fstream>
#include <filesystem>
#include "Simulator.hpp"
#include "ThermalBoundary/Boundary.hpp"
#include "utilities/Logger.hpp"

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

    /// @brief Read MatProp section of scene file
    void SceneLoader::readMatProp()
    {
        json configs;
        _matPropConfig = {};

        if (_jsonData.find("MatProp") != _jsonData.end())
            configs = _jsonData["MatProp"];
        else
            LOG_WARN << "Failed to load MatPropConfig from scene file";

        for (size_t i = 0; i < configs.size(); i++)
        {
            json config = configs[i];
            MatPropConfig mpc = MatPropConfig{};

            readValue(config["type"], mpc.type);
            readValue(config["tabulate"], mpc.tabulate);
            readVector(config["tempRange"], mpc.tempRange);
            readVector(config["tabulateStep"], mpc.tabulateStep);

            json polyConfig = config["polynomials"];
            for (size_t j = 0; j < polyConfig.size(); j++)
            {
                vector<Real> vec = {};
                readVector(polyConfig[j], vec);
                mpc.polynomials.push_back(vec);
            }

            _matPropConfig.push_back(mpc);
        }
    }

    /// @brief Read Boundary section of scene file
    void SceneLoader::readBoundary()
    {
        json configs;
        _boundaryConfig = {};

        if (_jsonData.find("Boundary") != _jsonData.end())
            configs = _jsonData["Boundary"];
        else
            LOG_WARN << "Failed to load BoundaryConfig from scene file";

        for (size_t i = 0; i < configs.size(); i++)
        {
            json config = configs[i];

            int bType = -1;
            readValue(config["index"], bType);

            if (bType == DIRICHLET)
            {
                BoundaryConfigDirichlet bc = BoundaryConfigDirichlet{};
                readValue(config["index"], bc.index);
                readValue(config["type"], bc.type);
                readValue(config["location"], bc.location);
                readVector(config["xTemp"], bc.xTemp);
                readVector(config["yTemp"], bc.yTemp);
                readVector(config["zTemp"], bc.zTemp);
                _boundaryConfig.push_back(bc);
            }
            else
            {
                BoundaryConfig bc = BoundaryConfig{};
                readValue(config["index"], bc.index);
                readValue(config["type"], bc.type);
                readValue(config["location"], bc.location);
                _boundaryConfig.push_back(bc);
            }
        }
    }

    /// @brief Read scene file and save the configurations using scene path in Simulator
    void SceneLoader::readScene()
    {
        readScene(Simulator::getCurrent()->getScenePath());
    }

    /// @brief Read scene file and save the configurations
    void SceneLoader::readScene(filesystem::path scenePath)
    {
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
        readMatProp();
        readBoundary();
    }
}