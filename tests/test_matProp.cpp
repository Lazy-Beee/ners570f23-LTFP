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

    COMPARE<Real>(mp->getProperty<DENSITY>(-50.0f), 100.0f, 1e-3f, "density 1 (lower bound overflow)");
    COMPARE<Real>(mp->getProperty<DENSITY>(50.0f), 100.0f, 1e-3f, "density 2");
    COMPARE<Real>(mp->getProperty<DENSITY>(100.0f), 100.0f, 1e-3f, "density 3");
    COMPARE<Real>(mp->getProperty<DENSITY>(150.0f), 150.0f, 1e-3f, "density 4");
    COMPARE<Real>(mp->getProperty<DENSITY>(250.0f), 200.0f, 1e-3f, "density 5 (upper bound overflow)");

    COMPARE<Real>(mp->getProperty<SPECIFIC_HEAT>(-2.0f), 1.0f, 1e-3f, "specific heat 1 (lower bound overflow)");
    COMPARE<Real>(mp->getProperty<SPECIFIC_HEAT>(2.0f), 1.02f, 1e-3f, "specific heat 2");
    COMPARE<Real>(mp->getProperty<SPECIFIC_HEAT>(100.0f), 2.505f, 1e-3f, "specific heat 3");
    COMPARE<Real>(mp->getProperty<SPECIFIC_HEAT>(114.514f), 3.01f, 1e-3f, "specific heat 4");
    COMPARE<Real>(mp->getProperty<SPECIFIC_HEAT>(300.0f), 3.01f, 1e-3f, "specific heat 5 (upper bound overflow)");

    COMPARE<Real>(mp->getProperty<ENTHALPY>(-1.0f), 2.0f, 1e-3f, "enthalpy 1 (lower bound overflow)");
    COMPARE<Real>(mp->getProperty<ENTHALPY>(2.0f), 4.04f, 1e-3f, "enthalpy 2");
    COMPARE<Real>(mp->getProperty<ENTHALPY>(100.0f), 252.5f, 1e-3f, "enthalpy 3");
    COMPARE<Real>(mp->getProperty<ENTHALPY>(114.514f), 346.687f, 1e-3f, "enthalpy 4");
    COMPARE<Real>(mp->getProperty<ENTHALPY>(300.0f), 604.0f, 1e-3f, "enthalpy 5 (upper bound overflow)");

    COMPARE<Real>(mp->getProperty<CONDUCTIVITY>(2.0f), 33.0f, 1e-3f, "conductivity");

    COMPARE(mp->useEnthalpy(), "Enthalpy flag");
    COMPARE<Real>(mp->getTemperature(1.0f), 0.0f, 1e-3f, "temperature 1 (lower bound overflow)");
    COMPARE<Real>(mp->getTemperature(4.04f), 2.0f, 1e-3f, "temperature 2");
    COMPARE<Real>(mp->getTemperature(252.5f), 100.0f, 1e-3f, "temperature 3");
    COMPARE<Real>(mp->getTemperature(346.687f), 114.514f, 1e-3f, "temperature 4");
    COMPARE<Real>(mp->getTemperature(1000.0f), 200.0f, 1e-3f, "temperature 5 (upper bound overflow)");

    Real temp = 0.0f;
    mp->updateTempQdot(temp, -100.0f);
    COMPARE<Real>(temp, 0.0f, 1e-3f, "update temperature 1 (lower bound overflow)");
    mp->updateTempQdot(temp, 4.04f - 2.0f);
    COMPARE<Real>(temp, 2.0f, 1e-3f, "update temperature 2");
    mp->updateTempQdot(temp, 252.5f-4.04f);
    COMPARE<Real>(temp, 100.0f, 1e-3f, "update temperature 3");
    mp->updateTempQdot(temp, 346.687f - 252.5f);
    COMPARE<Real>(temp, 114.514f, 1e-3f, "update temperature 4");
    mp->updateTempQdot(temp, 1000.0f - 2.0f);
    COMPARE<Real>(temp, 200.0f, 1e-3f, "update temperature 5 (upper bound overflow)");

    COMPARE_summary();
}