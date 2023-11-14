#include "compare.hpp"
#include "src/Simulator.hpp"
#include "src/ThermalBoundary/BoundaryManager.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator *sim = Simulator::getCurrent();
    SceneLoader *sl = SceneLoader::getCurrent();
    BoundaryManager *bm = BoundaryManager::getCurrent();

    sim->initUtilities("test.json");
    sl->readScene();
    bm->init();

#ifndef NDEBUG
    // Neumann
    ThermalBoundary *tb1 = bm->getThermalBoundary(XPOSITIVE, 0);
    COMPARE(tb1->getIndex(), 0, "Index 1");
    COMPARE(tb1->getLocation(), XPOSITIVE, "location 1");
    COMPARE(tb1->getType(), NEUMANN, "Type 1");
    Vector3r pos1 = {1.0f, 2.0f, 3.0f};
    Real temp1 = 10.0f;
    COMPARE<Real>(tb1->getTemp(pos1, temp1), 0.0f, 1e-3f, "getTemp 1");
    COMPARE<Real>(tb1->getFlux(pos1, temp1), 8.0f, 1e-3f, "getFlux 1");

    // Dirichlet
    ThermalBoundary *tb2 = bm->getThermalBoundary(YNEGATIVE, 0);
    COMPARE(tb2->getIndex(), 1, "Index 2");
    COMPARE(tb2->getLocation(), YNEGATIVE, "location 2");
    COMPARE(tb2->getType(), DIRICHLET, "Type 2");
    Vector3r pos2 = {1.0f, 2.0f, 3.0f};
    Real temp2 = 10.0f;
    COMPARE<Real>(tb2->getTemp(pos2, temp2), 41.0f, 1e-3f, "getTemp 2");
    COMPARE<Real>(tb2->getFlux(pos2, temp2), 0.0f, 1e-3f, "getFlux 2");

    // Convection
    ThermalBoundary *tb3 = bm->getThermalBoundary(XPOSITIVE, 1);
    COMPARE(tb3->getIndex(), 2, "Index 3");
    COMPARE(tb3->getLocation(), XPOSITIVE, "location 3");
    COMPARE(tb3->getType(), CONVECTION, "Type 3");
    Vector3r pos3 = {1.0f, 2.0f, 3.0f};
    Real temp3 = 310.0f;
    COMPARE<Real>(tb3->getTemp(pos3, temp3), 0.0f, 1e-3f, "getTemp 3");
    COMPARE<Real>(tb3->getFlux(pos3, temp3), -150.0f, 1e-3f, "getFlux 3");

    // Radiation
    ThermalBoundary *tb4 = bm->getThermalBoundary(ZNEGATIVE, 0);
    COMPARE(tb4->getIndex(), 3, "Index 4");
    COMPARE(tb4->getLocation(), ZNEGATIVE, "location 4");
    COMPARE(tb4->getType(), RADIATION, "Type 4");
    Vector3r pos4 = {1.0f, 2.0f, 3.0f};
    Real temp4 = 310.0f;
    COMPARE<Real>(tb4->getTemp(pos4, temp4), 0.0f, 1e-3f, "getTemp 4");
    COMPARE<Real>(tb4->getFlux(pos4, temp4), -643.664f, 1e-3f, "getFlux 4");

    // Mirror
    ThermalBoundary *tb5 = bm->getThermalBoundary(ZPOSITIVE, 0);
    COMPARE(tb5->getIndex(), 4, "Index 5");
    COMPARE(tb5->getLocation(), ZPOSITIVE, "location 5");
    COMPARE(tb5->getType(), MIRROR, "Type 5");
    Vector3r pos5 = {1.0f, 2.0f, 3.0f};
    Real temp5 = 310.0f;
    COMPARE<Real>(tb5->getTemp(pos5, temp5), 310.0f, 1e-3f, "getTemp 5");
    COMPARE<Real>(tb5->getFlux(pos5, temp5), 0.0f, 1e-3f, "getFlux 5");
#endif

    COMPARE(bm->isTempBC(XPOSITIVE), false, "isTempBC 1");
    COMPARE(bm->isTempBC(XNEGATIVE), false, "isTempBC 2");
    COMPARE(bm->isTempBC(YPOSITIVE), false, "isTempBC 3");
    COMPARE(bm->isTempBC(YNEGATIVE), true, "isTempBC 4");
    COMPARE(bm->isTempBC(ZPOSITIVE), true, "isTempBC 5");
    COMPARE(bm->isTempBC(ZNEGATIVE), false, "isTempBC 6");

    COMPARE(bm->getFluxBC(XPOSITIVE, {1.0f,2.0f,3.0f}, 310.0f), -392.0f, 1e-3f, "getBC 1");
    COMPARE(bm->getFluxBC(XNEGATIVE, {1.0f,2.0f,3.0f}, 310.0f), 0.0f, 1e-3f, "getBC 2");
    COMPARE(bm->getFluxBC(YPOSITIVE, {1.0f,2.0f,3.0f}, 310.0f), 0.0f, 1e-3f, "getBC 3");
    COMPARE(bm->getTempBC(YNEGATIVE, {1.0f,2.0f,3.0f}, 10.0f), 41.0f, 1e-3f, "getBC 4");
    COMPARE(bm->getTempBC(ZPOSITIVE, {1.0f,2.0f,3.0f}, 310.0f), 310.0f, 1e-3f, "getBC 5");
    COMPARE(bm->getFluxBC(ZNEGATIVE, {1.0f,2.0f,3.0f}, 310.0f), -643.664f, 1e-3f, "getBC 6");
    
    COMPARE_summary();
}