#include "compare.hpp"
#include "src/Common.hpp"
#include "src/Simulator.hpp"
#include "src/SceneLoader.hpp"
#include "src/Exporter/ExportManager.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator *sim = Simulator::getCurrent();
    sim->initUtilities("test.json");

    SceneLoader *sl = SceneLoader::getCurrent();
    ExportManager *em = ExportManager::getCurrent();

    sl->readScene();
    em->init();

    em->step();

    COMPARE_summary();
}