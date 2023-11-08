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

    COMPARE<Real>(sl->getTimeConfig().endTime, 1.2, 1e-3, "end time");
    COMPARE(sl->getTimeConfig().maxTimeSteps == 1000, "max time steps");
    COMPARE<Real>(sl->getTimeConfig().maxTimeStepSize, 0.1, 1e-3, "max time step size");
    COMPARE<Real>(sl->getTimeConfig().minTimeStepSize, 0.001, 1e-6, "min time step size");
}