#include "compare.hpp"
#include "src/Common.hpp"
#include "src/Simulator.hpp"
#include "src/SceneLoader.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator *sim = Simulator::getCurrent();
    sim->initUtilities("test.json");

    SceneLoader *sl = SceneLoader::getCurrent();

    sl->readScene();

    COMPARE(sl->getTimeConfig().endTime, 120.0f, 1e-3f, "end time");
    COMPARE(sl->getTimeConfig().maxTimeSteps, 1000, "max time steps");
    COMPARE(sl->getTimeConfig().maxTimeStepSize, 0.2f, 1e-3f, "max time step size");
    COMPARE(sl->getTimeConfig().minTimeStepSize, 0.001f, 1e-6f, "min time step size");

    COMPARE_summary();
}