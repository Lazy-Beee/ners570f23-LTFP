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
    ExportManager *em = ExportManager::getCurrent();
    Simulator::getCurrent() ->initUtilities("test.json");
    SceneLoader::getCurrent()->readScene();
    MeshData::getCurrent()->init();
    em->init();
    TimeManager::getCurrent()->setTime(11.0f);
    em->step();
}