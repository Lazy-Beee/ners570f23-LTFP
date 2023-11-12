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

    // Dirichlet
    ThermalBoundary *tb1 = bm->getThermalBoundary(XPOSITIVE, 0);
    COMPARE(tb1->getIndex() == 0, "Index 1");
    COMPARE(tb1->getLocation() == XPOSITIVE, "location 1");
    COMPARE(tb1->getType() == DIRICHLET, "Type 1");
    ThermalBoundaryDirichlet *tbd = static_cast<ThermalBoundaryDirichlet *>(tb1);
    Vector3r pos = {1.0f, 2.0f, 3.0f};
    COMPARE<Real>(tbd->getTemp(pos), 8.0f, 1e-3f, "Dirichlet getTemp");

    COMPARE_summary();
}