#include "compare.hpp"
#include "src/Simulator.hpp"
#include "src/SceneLoader.hpp"
#include "src/LaserSource.hpp"
#include "src/TimeManager.hpp"

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
    COMPARE(ld->type, GAUSSIAN_2D, "type 1");
    COMPARE(ld->power, 10.0f, 1e-3f, "power 1");
    COMPARE(ld->absorptivity, 0.5f, 1e-3f, "absorptivity 1");
    COMPARE(ld->laserPath[0].t0, 0.0f, 1e-3f, "laser path 1.1");
    COMPARE(ld->laserPath[0].t1, 0.5f, 1e-3f, "laser path 1.2");
    COMPARE(ld->laserPath[0].pos0[0], 0.05f, 1e-3f, "laser path 1.3");
    COMPARE(ld->laserPath[0].pos1[0], 1.05f, 1e-3f, "laser path 1.4");
    COMPARE(ld->laserPath[1].pos0[2], 0.15f, 1e-3f, "laser path 1.5");
    COMPARE(ld->radius, 0.12f, 1e-3f, "radius 1");

    COMPARE(ld->is2D, "is 2D 1");
    Real coeff1 = 3.0f * 0.5f * 10.0f / (3.1415926f * 0.12f * 0.12f);
    COMPARE(ld->coeff, coeff1, 1e-3f, "coeff 1");
    COMPARE(ld->radiusSquared, 0.0144f, 1e-3f, "radius 1");

    TimeManager::getCurrent()->setTime(-1.0f);
    ls->precomputePowerDistribution();
    COMPARE(ld->on, false, "on/off state 1.1");

    TimeManager::getCurrent()->setTime(14.5f);
    ls->precomputePowerDistribution();
    COMPARE(ld->on, true, "on/off state 1.2");
    COMPARE<size_t>(ld->currentPath, 20, "current path 1");
    COMPARE(ld->currentPos[0], 0.05f, 1e-3f, "current pos 1.1");
    COMPARE(ld->currentPos[1], 1.2f, 1e-3f, "current pos 1.2");
    COMPARE(ld->currentPos[2], 1.05f, 1e-3f, "current pos 1.3");
#endif

    COMPARE_summary();
}