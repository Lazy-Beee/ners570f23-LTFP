#include "compare.hpp"
#include "src/Simulator.hpp"
#include "src/SceneLoader.hpp"
#include "src/LaserSource.hpp"
#include "src/TimeManager.hpp"
#include "src/Exporter/ExportManager.hpp"
#include "src/MeshData.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator sim = Simulator();
    SceneLoader *sl = SceneLoader::getCurrent();
    LaserSource *ls = LaserSource::getCurrent();

    sim.initUtilities("test.json");
    sl->readScene(sim.getScenePath());
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
#endif

    MeshData::getCurrent()->init();
    ExportManager::getCurrent()->init(sim.getExportPath());
    TimeManager::getCurrent()->setTime(1.0f);
    ls->precomputePowerDistribution();
    ExportManager::getCurrent()->step();

    COMPARE_summary();
}