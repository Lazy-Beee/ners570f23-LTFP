#include "compare.hpp"
#include "src/Simulator.hpp"
#include "src/ThermalBoundary/Boundary.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator *sim = Simulator::getCurrent();
    SceneLoader *sl = SceneLoader::getCurrent();
    sim->initUtilities("test.json");
    sl->readScene();
}