#include "compare.hpp"
#include "src/Common.hpp"
#include "src/Simulator.hpp"
#include "src/MaterialProperty.hpp"
#include "src/SceneLoader.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator *sim = Simulator::getCurrent();
    MaterialProperty *mp = MaterialProperty::getCurrent();

    sim->initUtilities("test.json");
    SceneLoader::getCurrent()->readScene();
    mp->init();

    COMPARE<Real>(mp->getDensity(-50.0f), 100.0f, 1e-3f, "density 1");
    COMPARE<Real>(mp->getDensity(50.0f), 100.0f, 1e-3f, "density 2");
    COMPARE<Real>(mp->getDensity(100.0f), 100.0f, 1e-3f, "density 3");
    COMPARE<Real>(mp->getDensity(150.0f), 150.0f, 1e-3f, "density 4");
    COMPARE<Real>(mp->getDensity(250.0f), 200.0f, 1e-3f, "density 5");

    // COMPARE<Real>(mp->getSpecificHeat(1.0f), 5.0f, 1e-3f, "specific heat");
    COMPARE<Real>(mp->getEnthalpy(1.0f), 6.0f, 1e-3f, "enthalpy");
    COMPARE<Real>(mp->getConductivity(1.0f), 7.0f, 1e-3f, "conductivity");
}