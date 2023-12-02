#include "LaserSource.hpp"
#include <filesystem>
#include "Simulator.hpp"
#include "SceneLoader.hpp"
#include "TimeManager.hpp"
#include "MeshData.hpp"
#include "utilities/Logger.hpp"
#include "utilities/Counting.hpp"
#include "utilities/json.hpp"

using namespace std;
using json = nlohmann::json;

namespace LTFP
{
    LaserSource *LaserSource::current = nullptr;

    LaserSource::LaserSource()
    {
        _laserActive = false;
    }

    LaserSource::~LaserSource()
    {
        current = nullptr;
    }

    LaserSource *LaserSource::getCurrent()
    {
        if (current == nullptr)
            current = new LaserSource();

        return current;
    }

    /// @brief Load configuration from SceneLoader
    void LaserSource::init()
    {
        vector<SceneLoader::LaserConfig> configs = SceneLoader::getCurrent()->getLaserConfig();

        for (SceneLoader::LaserConfig config : configs)
        {
            LaserData data = LaserData{};

            // Load configd
            data.index = config.index;
            data.type = static_cast<LaserType>(config.type);
            data.power = config.power;
            data.absorptivity = config.absorptivity;
            data.laserPath = readPathFile(config.laserPath);
            data.radius = config.radius;
            data.depth = config.depth;

            // Essential checks
            if (data.type < 0 || data.type >= LaserTypeName.size())
            {
                LOG_ERR << "Unidentified laser type " << data.type << " for laser " << data.index;
                exit(1);
            }
            if (data.power < 0.0f)
            {
                LOG_ERR << "Negative power value " << data.power << " for laser " << data.index;
                exit(1);
            }
            if (data.absorptivity < 0.0f || data.absorptivity > 1.0f)
            {
                LOG_ERR << "Illegal absorptivity " << data.absorptivity << " for laser " << data.index;
                exit(1);
            }
            if (data.laserPath.size() == 0)
            {
                LOG_ERR << "Laser path is empty for laser " << data.index;
                exit(1);
            }
            if (data.radius < 0.0f)
            {
                LOG_ERR << "Negative radius " << data.radius << " for laser " << data.index;
                exit(1);
            }

            // Compute static properties
            if (data.type == GAUSSIAN_2D || data.type == TOP_HEAD_2D)
                data.is2D = true;
            data.radiusSquared = config.radius * config.radius;
            if (data.type == GAUSSIAN_2D)
                data.coeff = 3.0f * data.absorptivity * data.power / (3.1415926f * data.radius * data.radius);
            else if (data.type == TOP_HEAD_2D)
                data.coeff = data.absorptivity * data.power / (2.0f * 3.1415926f * data.radius * data.radius);
            else
            {
                // TODO: add 3D gaussian equation
                LOG_WARN << "3D Gaussian Laser is not yet supported";
                exit(1);
            }

            // Add laser object to list
            _lasers.push_back(data);

            if (_lasers.size() == 0)
                _laserActive = false;
        }
    }

    /// @brief Load path from file
    vector<LaserSource::LaserPath> LaserSource::readPathFile(string file)
    {
        filesystem::path filePath = SceneLoader::getCurrent()->getScenePath().parent_path() / file;
        json config;
        ifstream input_file(filePath);
        if (!input_file.is_open())
        {
            LOG_ERR << "Cannot open path file: " << filePath;
            exit(1);
        }
        try
        {
            config = json::parse(input_file);
        }
        catch (const exception &e)
        {
            LOG_ERR << "Cannot open path file";
            LOG_ERR << e.what();
            exit(1);
        }

        json timeConfig = config["time"];
        json posConfig = config["position"];
        vector<LaserPath> paths;
        paths.resize(timeConfig.size());
        for (size_t i = 0; i < timeConfig.size(); i++)
        {
            SceneLoader::readValue(timeConfig[i][0], paths[i].t0);
            SceneLoader::readValue(timeConfig[i][1], paths[i].t1);
            SceneLoader::readVector(posConfig[i][0], paths[i].pos0);
            SceneLoader::readVector(posConfig[i][1], paths[i].pos1);
        }

        return paths;
    }

    /// @brief Determine laser state and precompute power distribution
    void LaserSource::precomputePowerDistribution()
    {
        if (!_laserActive)
            return;

        MeshData *mesh = MeshData::getCurrent();
        Real currentTime = TimeManager::getCurrent()->getTime();

        resizeMeshReal(_laserPower, mesh->getSizeX(), mesh->getSizeY(), mesh->getSizeZ());
#pragma omp parallel for collapse(3) schedule(static)
        for (size_t i = 0; i < mesh->getSizeX(); i++)
            for (size_t j = 0; j < mesh->getSizeY(); j++)
                for (size_t k = 0; k < mesh->getSizeZ(); k++)
                    _laserPower[i][j][k] = 0.0f;

        for (size_t laserId = 0; laserId < _lasers.size(); laserId++)
        {
            LaserData &laser = _lasers[laserId];
            vector<LaserPath> &paths = laser.laserPath;
            LaserPath &currentPath = paths[laser.currentPath];

            // Check laser state
            while (laser.currentPath < laser.laserPath.size())
            {
                if (currentTime < currentPath.t0)
                {
                    laser.on = false;
                    break;
                }
                else if (currentTime < currentPath.t1)
                {
                    laser.on = true;
                    break;
                }
                else
                {
                    laser.currentPath++;
                    currentPath = paths[laser.currentPath];
                }
            }

            if (!laser.on)
                continue;

            // Get current position
            laser.currentPos = (currentTime - currentPath.t0) / (currentPath.t1 - currentPath.t0) * (currentPath.pos1 - currentPath.pos0) + currentPath.pos0;

            // Compute power distribution
            vector<vector<vector<Real>>> cellPower;
            resizeMeshReal(cellPower, mesh->getSizeX(), mesh->getSizeY(), mesh->getSizeZ());
            Real totalPower = 0.0f;

            if (laser.is2D)
            {
                size_t j = mesh->getSizeY() - 1;
                Real area = mesh->getIntervalX() * mesh->getIntervalZ();

#pragma omp parallel for reduction(+ : totalPower) collapse(2) schedule(static)
                for (size_t i = 0; i < mesh->getSizeX(); i++)
                {
                    for (size_t k = 0; k < mesh->getSizeZ(); k++)
                    {
                        Real p = getPowerDensity(laser, mesh->getCenterPos(i, j, k)) * area;
                        cellPower[i][j][k] = p;
                        totalPower += p;
                    }
                }
            }
            else
            {
                // TODO: compute power distribution for 3D laser source
            }

            Real correctionFactor = laser.power * laser.absorptivity / totalPower;

            if (correctionFactor > 1.1 || correctionFactor < 0.9)
                LOG_WARN << "Correction factor of laser " << laser.index << " is out of range [0.9, 1.1] at simulation time " << currentTime;

#pragma omp parallel for collapse(3) schedule(static)
            for (size_t i = 0; i < mesh->getSizeX(); i++)
                for (size_t j = 0; j < mesh->getSizeY(); j++)
                    for (size_t k = 0; k < mesh->getSizeZ(); k++)
                        _laserPower[i][j][k] += cellPower[i][j][k] * correctionFactor;
        }
    }

    Real LaserSource::getPowerDensity(const LaserData &laser, const Vector3r &pos)
    {
        if (laser.type == GAUSSIAN_2D)
        {
            Vector2r vecXZ = {pos[0] - laser.currentPos[0], pos[2] - laser.currentPos[2]};
            Real disXZ2 = vecXZ.squaredNorm();

            if (disXZ2 > laser.radiusSquared)
                return 0.0f;
            else
                return laser.coeff * exp(-3.0f * disXZ2 / laser.radiusSquared);
        }
        else if (laser.type == TOP_HEAD_2D)
        {
            return laser.coeff;
        }
        else if (laser.type == GAUSSIAN_3D)
        {
            // TODO: add 3D gaussian equation
            return 0.0f;
        }
        else
        {
            LOG_WARN << "Unidentified laser type " << laser.type;
            return 0.0f;
        }
    }
}