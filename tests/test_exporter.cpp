#include "compare.hpp"
#include "src/Common.hpp"
#include "src/Simulator.hpp"
#include "src/SceneLoader.hpp"
#include "src/TimeManager.hpp"
#include "src/MeshData.hpp"
#include "src/Exporter/ExportManager.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator sim = Simulator();
    ExportManager *em = ExportManager::getCurrent();
    sim.initUtilities("test.json");
    SceneLoader::getCurrent()->readScene(sim.getScenePath());
    MeshData::getCurrent()->init();
    em->init(sim.getExportPath());
    TimeManager::getCurrent()->setTime(11.0f);
    em->step();
}