#include "ThermalBoundaryDirichlet.hpp"
#include "src/SceneLoader.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ThermalBoundaryDirichlet::ThermalBoundaryDirichlet(int boundaryIndex, int boundaryType) : ThermalBoundary(boundaryIndex, boundaryType)
    {
    }

    ThermalBoundaryDirichlet::~ThermalBoundaryDirichlet()
    {
    }

    void ThermalBoundaryDirichlet::init()
    {
        // SceneLoader* sl = SceneLoader::getCurrent();
        
    }

    Real ThermalBoundaryDirichlet::getFlux(const Vector3r &pos, const Real &temp)
    {
        Real tempBoundary = getTemp(pos);
        return 0;
    }

    Real ThermalBoundaryDirichlet::getTemp(const Vector3r &pos)
    {
        return computePoly3r(pos, _tempPolyCoeff).sum();
    }

    vector<Real> ThermalBoundaryDirichlet::getParam()
    {
        return vector<Real>{};
    }
}