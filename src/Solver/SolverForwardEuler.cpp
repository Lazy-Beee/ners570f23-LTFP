#include "SolverForwardEuler.hpp"
#include "SceneLoader.hpp"
#include "utilities/Logger.hpp"
#include "utilities/Counting.hpp"
#include "TimeManager.hpp"
#include "Simulator.hpp"

namespace LTFP
{
    MeshData *MeshData::current = nullptr;

    MeshData::MeshData()
    {
        _xSize = 10;
        _ySize = 10;
        _zSize = 10;
        _xInterval = 0.1f;
        _yInterval = 0.1f;
        _zInterval = 0.1f;
    }

    MeshData::~MeshData()
    {
        current = nullptr;
    }

    MeshData *MeshData::getCurrent()
    {
        if (current == nullptr)
            current = new MeshData();

        return current;
    }

    /// @brief Load configuration from SceneLoader
    void MeshData::init()
    {
        SceneLoader::MeshConfig meshConfig = SceneLoader::getCurrent()->getMeshConfig();
        if (meshConfig.end[0] <= meshConfig.start[0] ||
            meshConfig.end[1] <= meshConfig.start[1] ||
            meshConfig.end[2] <= meshConfig.start[2])
        {
            LOG_ERR << "Mesh End must be greater than start!";
            exit(1);
        }
        _domainStart = meshConfig.start;
        _domainEnd = meshConfig.end;

        // x Size: Nx
        if (meshConfig.xCount > 0)
        {
            _xSize = meshConfig.xCount;
        }
        else if (_xInterval > 0.0)
        {
            _xSize = static_cast<size_t>((_domainEnd[0] - _domainStart[0]) / meshConfig.meshSize) + 1;
        }
        else
        {
            LOG_ERR << "Mesh x size or interval error!";
            exit(1);
        }

        // y Size: Ny
        if (meshConfig.yCount > 0)
        {
            _ySize = meshConfig.yCount;
        }
        else if (_yInterval > 0.0)
        {
            _ySize = static_cast<size_t>((_domainEnd[1] - _domainStart[1]) / meshConfig.meshSize) + 1;
        }
        else
        {
            LOG_ERR << "Mesh y size or interval error!";
            exit(1);
        }

        // z Size: Nz
        if (meshConfig.zCount > 0)
        {
            _zSize = meshConfig.zCount;
        }
        else if (_zInterval > 0.0)
        {
            _zSize = static_cast<size_t>((_domainEnd[2] - _domainStart[2]) / meshConfig.meshSize) + 1;
        }
        else
        {
            LOG_ERR << "Mesh z size or interval error!";
            exit(1);
        }

        // Re-calculate dx, dy, dz & resize the _centerPos
        _xInterval = (_domainEnd[0] - _domainStart[0]) / _xSize;
        _yInterval = (_domainEnd[1] - _domainStart[1]) / _ySize;
        _zInterval = (_domainEnd[2] - _domainStart[2]) / _zSize;
        LTFP::resizeMeshVector3r(_centerPos, _xSize, _ySize, _zSize);
        LTFP::resizeMeshReal(_temperature, _xSize, _ySize, _zSize);
        LTFP::resizeMeshReal(_temperatureOld, _xSize, _ySize, _zSize);
        LTFP::resizeMeshReal(_coolingRate, _xSize, _ySize, _zSize);
        LTFP::resizeMeshVector3r(_temperatureGrad, _xSize, _ySize, _zSize);

        for (size_t i = 0; i < _xSize; i++)
        {
            for (size_t j = 0; j < _ySize; j++)
            {
                for (size_t k = 0; k < _zSize; k++)
                {
                    _centerPos[i][j][k] = {_domainStart[0] + (i + 0.5f) * _xInterval,
                                           _domainStart[1] + (j + 0.5f) * _yInterval,
                                           _domainStart[2] + (k + 0.5f) * _zInterval};
                    _temperature[i][j][k] = meshConfig.initialTemp;
                    _temperatureOld[i][j][k] = meshConfig.initialTemp;
                }
            }
        }
    }

    void MeshData::stepIncrement()
    {
        Real time = TimeManager::getCurrent()->getTime();
        static size_t increaseCount = 0;
        size_t y_increment1, y_increment2;
        if (time >= _incrementTime[increaseCount])
        {
            if (increaseCount == 0)
            {
                y_increment1 = 0;
                y_increment2 = static_cast<size_t>(_incrementThickness[increaseCount] / _yInterval) + 1;
            }
            else
            {
                y_increment1 = static_cast<size_t>(_incrementThickness[increaseCount - 1] / _yInterval) + 1;
                y_increment2 = static_cast<size_t>(_incrementThickness[increaseCount] / _yInterval) + 1;
            }
            size_t ySizeOld = _ySize;
            _ySize += (y_increment2 - y_increment1);

            MeshVector centerPosTemp = _centerPos;
            MeshReal _temperatureTemp = _temperature;
            MeshReal _temperatureOldTemp = _temperatureOld;
            LTFP::resizeMeshVector3r(_centerPos, _xSize, _ySize, _zSize);
            LTFP::resizeMeshReal(_temperature, _xSize, _ySize, _zSize);
            LTFP::resizeMeshReal(_temperatureOld, _xSize, _ySize, _zSize);

            for (size_t i = 0; i < _xSize; i++)
            {
                for (size_t j = 0; j < _ySize; j++)
                {
                    for (size_t k = 0; k < _zSize; k++)
                    {
                        _centerPos[i][j][k] = {_domainStart[0] + (i + 0.5f) * _xInterval,
                                               _domainStart[1] + (j + 0.5f) * _yInterval,
                                               _domainStart[2] + (k + 0.5f) * _zInterval};
                        if (j < ySizeOld) // Assign previous value for the existing layers
                        {
                            _temperature[i][j][k] = _temperatureTemp[i][j][k];
                            _temperatureOld[i][j][k] = _temperatureOldTemp[i][j][k];
                        }
                        else // The same as the top value for the new layers
                        {
                            _temperature[i][j][k] = _temperatureTemp[i][ySizeOld - 1][k];
                            _temperatureOld[i][j][k] = _temperatureTemp[i][ySizeOld - 1][k];
                        }
                    }
                }
            }
            increaseCount++;
        }
    }

    // FIXME Should be moved to the thermal solver part
    void MeshData::calCoolingRate()
    {
        LTFP::resizeMeshReal(_coolingRate, _xSize, _ySize, _zSize);
        Real timestep = TimeManager::getCurrent()->getTimeStepSize();
        for (size_t i = 0; i < _xSize; i++)
        {
            for (size_t j = 0; j < _ySize; j++)
            {
                for (size_t k = 0; k < _zSize; k++)
                {
                    _coolingRate[i][j][k] = (_temperature[i][j][k] - _temperatureOld[i][j][k]) / timestep;
                }
            }
        }
    }

    // FIXME Should be moved to the thermal solver part
    void MeshData::calTemperatureGrad()
    {
        LTFP::resizeMeshVector3r(_temperatureGrad, _xSize, _ySize, _zSize);
        Real TempGradX, TempGradY, TempGradZ;
        for (size_t i = 0; i < _xSize; i++)
        {
            for (size_t j = 0; j < _ySize; j++)
            {
                for (size_t k = 0; k < _zSize; k++)
                {
                    if (i == 0) // Boundary (left x)
                    {
                        TempGradX = (_temperature[i + 1][j][k] - _temperature[i][j][k]) / _xInterval;
                    }
                    else if (i == _xSize - 1) // Boundary (right x)
                    {
                        TempGradX = (_temperature[i][j][k] - _temperature[i - 1][j][k]) / _xInterval;
                    }
                    else // Center x
                    {
                        TempGradX = (_temperature[i + 1][j][k] - _temperature[i - 1][j][k]) / _xInterval * 0.5;
                    }
                    if (j == 0) // Boundary (bottom y)
                    {
                        TempGradY = (_temperature[i][j + 1][k] - _temperature[i][j][k]) / _yInterval;
                    }
                    else if (j == _ySize - 1) // Boundary (top y)
                    {
                        TempGradY = (_temperature[i][j][k] - _temperature[i][j - 1][k]) / _yInterval;
                    }
                    else // Center y
                    {
                        TempGradY = (_temperature[i][j + 1][k] - _temperature[i][j - 1][k]) / _yInterval * 0.5;
                    }
                    if (k == 0) // Boundary (left z)
                    {
                        TempGradZ = (_temperature[i][j][k + 1] - _temperature[i][j][k]) / _zInterval;
                    }
                    else if (k == _zSize - 1) // Boundary (right z)
                    {
                        TempGradZ = (_temperature[i][j][k] - _temperature[i][j][k - 1]) / _zInterval;
                    }
                    else // Center z
                    {
                        TempGradZ = (_temperature[i][j][k + 1] - _temperature[i][j][k - 1]) / _zInterval * 0.5;
                    }
                    _temperatureGrad[i][j][k] = {TempGradX, TempGradY, TempGradZ};
                }
            }
        }
    }

    /// @brief Load layer data from file
    void MeshData::readLayerFile(std::string file)
    {
        if (file != "")
        {
            std::filesystem::path filePath = Simulator::getCurrent()->getScenePath().parent_path() / file;
            json config;
            std::ifstream input_file(filePath);
            if (!input_file.is_open())
            {
                LOG_ERR << "Cannot open path file: " << filePath;
                exit(1);
            }
            try
            {
                config = json::parse(input_file);
            }
            catch (const std::exception &e)
            {
                LOG_ERR << "Cannot open path file";
                LOG_ERR << e.what();
                exit(1);
            }

            json durationConfig = config["layer duration"];
            json thicknessConfig = config["layer increment"];

            for (size_t i = 1; i < durationConfig.size(); i++)
            {
                Real tempDy, tempT1, tempT2;
                SceneLoader::readValue(thicknessConfig[i], tempDy);
                _incrementThickness.push_back(tempDy);
                SceneLoader::readValue(durationConfig[i - 1][1], tempT1);
                SceneLoader::readValue(durationConfig[i][0], tempT2);
                _incrementTime.push_back(0.5f * (tempT1 + tempT2));
            }
        }
    }

}