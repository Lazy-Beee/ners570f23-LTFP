#include "SceneLoader.hpp"
#include <fstream>
#include <filesystem>
#include "Simulator.hpp"
#include "ThermalBoundary/BoundaryManager.hpp"
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

        for (BoundaryConfig *bc : _boundaryConfig)
            delete bc;
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
            readValue(config["initialTemp"], _meshConfig.initialTemp);
            readValue(config["layerFile"], _meshConfig.layerFile);
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
            readValue(config["consolePeriod"], _exportConfig.consolePeriod);

            for (json expoConfig : config["exporters"])
            {
                ExporterConfig exporter = ExporterConfig{};
                readValue(expoConfig["type"], exporter.type);
                readValue(expoConfig["period"], exporter.period);
                readValue(expoConfig["outPrecision"], exporter.outPrecision);
                readVector(expoConfig["parameters"], exporter.parameters);
                _exportConfig.exporters.push_back(exporter);
            }
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
            readValue(config["type"], bType);

            if (bType == NEUMANN)
            {
                BoundaryConfigNeumann *bc = new BoundaryConfigNeumann{};
                readValue(config["index"], bc->index);
                readValue(config["type"], bc->type);
                readValue(config["location"], bc->location);
                readVector(config["xFluxPoly"], bc->xFluxPoly);
                readVector(config["yFluxPoly"], bc->yFluxPoly);
                readVector(config["zFluxPoly"], bc->zFluxPoly);
                _boundaryConfig.push_back(bc);
            }
            else if (bType == DIRICHLET)
            {
                BoundaryConfigDirichlet *bc = new BoundaryConfigDirichlet{};
                readValue(config["index"], bc->index);
                readValue(config["type"], bc->type);
                readValue(config["location"], bc->location);
                readVector(config["xTempPoly"], bc->xTempPoly);
                readVector(config["yTempPoly"], bc->yTempPoly);
                readVector(config["zTempPoly"], bc->zTempPoly);
                _boundaryConfig.push_back(bc);
            }
            else if (bType == CONVECTION)
            {
                BoundaryConfigConvection *bc = new BoundaryConfigConvection{};
                readValue(config["index"], bc->index);
                readValue(config["type"], bc->type);
                readValue(config["location"], bc->location);
                readValue(config["ambientTemp"], bc->ambientTemp);
                readValue(config["convectionCoeff"], bc->convectionCoeff);
                _boundaryConfig.push_back(bc);
            }
            else if (bType == RADIATION)
            {
                BoundaryConfigRadiation *bc = new BoundaryConfigRadiation{};
                readValue(config["index"], bc->index);
                readValue(config["type"], bc->type);
                readValue(config["location"], bc->location);
                readValue(config["ambientTemp"], bc->ambientTemp);
                readValue(config["emissivityCoeff"], bc->emissivityCoeff);
                _boundaryConfig.push_back(bc);
            }
            else // Mirror BC is also in this branch
            {
                BoundaryConfig *bc = new BoundaryConfig{};
                readValue(config["index"], bc->index);
                readValue(config["type"], bc->type);
                readValue(config["location"], bc->location);
                _boundaryConfig.push_back(bc);
            }
        }
    }

    /// @brief Read Laser section of scene file
    void SceneLoader::readLaserConfig()
    {
        if (_jsonData.find("Laser") != _jsonData.end())
        {
            json configs = _jsonData["Laser"];

            for (size_t i = 0; i < configs.size(); i++)
            {
                json config = configs[i];
                LaserConfig lc = LaserConfig{};

                readValue(config["index"], lc.index);
                readValue(config["type"], lc.type);
                readValue(config["power"], lc.power);
                readValue(config["absorptivity"], lc.absorptivity);
                readValue(config["laserPath"], lc.laserPath);
                readValue(config["radius"], lc.radius);
                readValue(config["depth"], lc.depth);

                _laserConfig.push_back(lc);
            }
        }
        else
            LOG_WARN << "Failed to load LaserConfig from scene file";
    }

    /// @brief Read ThermalSolver section of scene file
    void SceneLoader::readThermalSolverConfig()
    {
        if (_jsonData.find("ThermalSolver") != _jsonData.end())
        {
            json config = _jsonData["ThermalSolver"];
            _thermalSolverConfig = ThermalSolverConfig{};

            readValue(config["type"], _thermalSolverConfig.type);
            readValue(config["cflNumber"], _thermalSolverConfig.cflNumber);
        }
        else
            LOG_WARN << "Failed to load ThermalSolverConfig from scene file";
    }

    /// @brief Read scene file and save the configurations
    void SceneLoader::readScene(filesystem::path scenePath)
    {
        _scenePath = scenePath;
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
        readLaserConfig();
        readThermalSolverConfig();
    }
}