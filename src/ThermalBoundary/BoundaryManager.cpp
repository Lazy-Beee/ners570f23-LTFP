#include "BoundaryManager.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"
#include "ThermalBoundaryNeumann.hpp"
#include "ThermalBoundaryDirichlet.hpp"
#include "ThermalBoundaryConvection.hpp"
#include "ThermalBoundaryRadiation.hpp"
#include "ThermalBoundaryMirror.hpp"

using namespace std;

namespace LTFP
{
    BoundaryManager *BoundaryManager::current = nullptr;

    BoundaryManager::BoundaryManager()
    {
        _boundaries.resize(_numLoc);
        for (size_t i = 0; i < _numLoc; i++)
            _boundaries[i].clear();
        _tempBC.resize(_numLoc, false);
    }

    BoundaryManager::~BoundaryManager()
    {
        current = nullptr;

        for (size_t i = 0; i < _boundaries.size(); i++)
            for (size_t j = 0; j < _boundaries[i].size(); j++)
                delete _boundaries[i][j];
    }

    BoundaryManager *BoundaryManager::getCurrent()
    {
        if (current == nullptr)
            current = new BoundaryManager();

        return current;
    }

    /// @brief Load configuration from SceneLoader and create boundary objects
    void BoundaryManager::init()
    {
        vector<Config *> boundaryConfig = SceneLoader::getCurrent()->getBoundaryConfig();

        for (Config *bc : boundaryConfig)
        {
            if (bc->location < 0 || bc->location > (_numLoc - 1))
            {
                LOG_WARN << "Unidentified boundary location " << bc->location << " with boundary index " << bc->index;
                continue;
            }

            if (bc->type == NEUMANN)
            {
                ConfigNeumann *bcd = static_cast<ConfigNeumann *>(bc);
                ThermalBoundaryNeumann *tbd = new ThermalBoundaryNeumann(bcd);
                _boundaries[bcd->location].push_back(tbd);
            }
            else if (bc->type == DIRICHLET)
            {
                ConfigDirichlet *bcd = static_cast<ConfigDirichlet *>(bc);
                ThermalBoundaryDirichlet *tbd = new ThermalBoundaryDirichlet(bcd);
                _boundaries[bcd->location].push_back(tbd);
            }
            else if (bc->type == CONVECTION)
            {
                ConfigConvection *bcd = static_cast<ConfigConvection *>(bc);
                ThermalBoundaryConvection *tbd = new ThermalBoundaryConvection(bcd);
                _boundaries[bcd->location].push_back(tbd);
            }
            else if (bc->type == RADIATION)
            {
                ConfigRadiation *bcd = static_cast<ConfigRadiation *>(bc);
                ThermalBoundaryRadiation *tbd = new ThermalBoundaryRadiation(bcd);
                _boundaries[bcd->location].push_back(tbd);
            }
            else if (bc->type == MIRROR)
            {
                ConfigMirror *bcd = static_cast<ConfigMirror *>(bc);
                ThermalBoundaryMirror *tbd = new ThermalBoundaryMirror(bcd);
                _boundaries[bcd->location].push_back(tbd);
            }
            else
            {
                LOG_WARN << "Unidentified boundary configuration with index " << bc->index << ", type " << bc->type << ", and location " << bc->location;
            }
        }

        // Check B.C. combination
        for (size_t i = 0; i < _numLoc; i++)
        {
            for (ThermalBoundary *bc : _boundaries[i])
            {
                if (bc->getType() == DIRICHLET || bc->getType() == MIRROR)
                    _tempBC[i] = true;
            }

            if (_tempBC[i] && _boundaries[i].size() > 1)
            {
                LOG_ERR << "Illegal thermal boundary condition combination in location " << BoundaryLocationName[i];
                exit(1);
            }
        }
    }

    /// @brief Get boundary temperature
    /// @note No need to verify the boundary type, they will be checked in the BC classes
    Real BoundaryManager::getTempBC(BoundaryLocation loc, const Vector3r &pos, const Real &temp)
    {
        return _boundaries[loc][0]->getTemp(pos, temp);
    }

    /// @brief Get boundary flux
    /// @note No need to verify the boundary type, they will be checked in the BC classes
    Real BoundaryManager::getFluxBC(BoundaryLocation loc, const Vector3r &pos, const Real &temp)
    {
        Real flux = 0.0f;
        for (ThermalBoundary *bc : _boundaries[loc])
            flux += bc->getFlux(pos, temp);

        return flux;
    }
}