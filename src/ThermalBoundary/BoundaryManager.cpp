#include "BoundaryManager.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    BoundaryManager *BoundaryManager::current = nullptr;

    BoundaryManager::BoundaryManager()
    {
        _boundaries.resize(6);
    }

    BoundaryManager::~BoundaryManager()
    {
        current = nullptr;
    }

    BoundaryManager *BoundaryManager::getCurrent()
    {
        if (current == nullptr)
            current = new BoundaryManager();

        return current;
    }

    /// @brief Load configuration from SceneLoader
    void BoundaryManager::init()
    {
        vector<Config> boundaryConfig = SceneLoader::getCurrent()->getBoundaryConfig();

        for (Config bc : boundaryConfig)
        {
            if (bc.location < 0 || bc.location > 5)
            {
                LOG_WARN << "Unidentified boundary location " << bc.location << " with boundary index " << bc.index;
                continue;
            }

            if (bc.type == DIRICHLET)
            {
                ConfigDirichlet *bcd = static_cast<ConfigDirichlet *>(&bc);
                ThermalBoundaryDirichlet tbd = ThermalBoundaryDirichlet(*bcd);
                _boundaries[bcd->location].push_back(tbd);
            }
            else
            {
                LOG_WARN << "Unidentified boundary configuration with index " << bc.index << ", type " << bc.type << ", and location " << bc.location;
            }
        }
    }

}