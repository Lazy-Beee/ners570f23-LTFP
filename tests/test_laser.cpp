#include "compare.hpp"
#include "src/Simulator.hpp"
#include "src/SceneLoader.hpp"
#include "src/LaserSource.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator *sim = Simulator::getCurrent();
    SceneLoader *sl = SceneLoader::getCurrent();
    LaserSource *ls = LaserSource::getCurrent();

    sim->initUtilities("test.json");
    sl->readScene();
    ls->init();

#ifndef NDEBUG
    LaserSource::LaserData *ld = ls->getLaserData(0);
    COMPARE(ld->index, 0, "laser index 1");
    COMPARE(ld->is2D, "is 2D 1");
#endif

    COMPARE_summary();
}