#include "Solver.hpp"
#include "utilities/Logger.hpp"
#include "TimeManager.hpp"
#include "MeshData.hpp"
#include "LaserSource.hpp"
#include "ThermalBoundary/BoundaryManager.hpp"
#include "MaterialProperty.hpp"

namespace LTFP
{
  Solver::Solver(Config *config)
      : _index(config->index),
        _type(static_cast<SolverType>(config->type))
  {
    LOG_DEBUG << "Solver " << _index
              << " of type " << SolverTypeName[_type];
  }

  void Solver::SolverFE1()
  {
    MeshData *mesh = MeshData::getCurrent();
    LaserSource *laser = LaserSource::getCurrent();
    Real currentTime = TimeManager::getCurrent()->getTime();
    Real timeStep = TimeManager::getCurrent()->getTimeStepSize();
    MaterialProperty *matProp = MaterialProperty::getCurrent();
    BoundaryManager *boundaryManager = BoundaryManager::getCurrent();
    
    size_t i, j, k;
    MeshReal tmpMeshTemp;
    size_t nx = mesh->getSizeX, ny = mesh->getSizeY, nz = mesh->getSizeZ; 
    Real dx = mesh->getIntervalX, dy = mesh->getIntervalY, dz = mesh->getIntervalZ;
    resizeMeshReal(tmpMeshTemp, nx, ny, nz);
    for (i = 1; i < nx-1; i++){
      for (j = 1; j < mesh->ny-1; j++){
        for (k = 1; k < mesh->nz-1; k++){
          tmpTemp[i][j][k] = mesh->getTemperature(i, j, k) + timeStep * ( 
              matProp->getProperty<CONDUCTIVITY>(mesh->getTemperature(i, j, k)) 
            / matProp->getProperty<DENSITY>(mesh->getTemperature(i, j, k)) 
            / matProp->getProperty<SPECIFIC_HEAT>(mesh->getTemperature(i, j, k))
            * ( (mesh->getTemperature(i-1, j, k) - 2.0f*mesh->getTemperature(i, j, k) + mesh->getTemperature(i+1, j, k))/(dx*dx)
              + (mesh->getTemperature(i, j-1, k) - 2.0f*mesh->getTemperature(i, j, k) + mesh->getTemperature(i, j+1, k))/(dy*dy)
              + (mesh->getTemperature(i, j, k-1) - 2.0f*mesh->getTemperature(i, j, k) + mesh->getTemperature(i, j, k+1))/(dz*dz) )
            + laser->getLaserPower(i, j, k) / matProp->getProperty<SPECIFIC_HEAT>(mesh->getTemperature(i, j, k)) );
        }
      }
    }

    for (i = 1; i < nx-1; i++){
      for (j = 1; j < mesh->ny-1; j++){
        for (k = 1; k < mesh->nz-1; k++){
          mesh->setTemperature(i, j, k, tmpTemp[i][j][k]);
        }
      }
    }

  }
}
