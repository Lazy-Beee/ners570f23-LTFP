#include "ThermalBoundaryDirichlet.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

namespace LTFP
{
    ThermalBoundaryDirichlet::ThermalBoundaryDirichlet(int boundaryType) : ThermalBoundary(boundaryType)
    {
    }

    ThermalBoundaryDirichlet::~ThermalBoundaryDirichlet()
    {
    }

    void ThermalBoundaryDirichlet::init()
    {
        SceneLoader* sl = SceneLoader::getCurrent();
        
    }

    Real ThermalBoundaryDirichlet::getFlux(const Vector3r &pos, const Real &temp)
    {
        return Real();
    }

    std::vector<Real> ThermalBoundaryDirichlet::getParam()
    {
        return std::vector<Real>();
    }
}