#include "compare.hpp"
#include "src/Common.hpp"
#include "src/Simulator.hpp"
#include "src/MaterialProperty.hpp"
#include "src/SceneLoader.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator sim = Simulator();
    MaterialProperty *mp = MaterialProperty::getCurrent();

    sim.initUtilities("test.json");
    SceneLoader::getCurrent()->readScene(sim.getScenePath());
    mp->init();

    COMPARE(mp->getProperty<DENSITY>(-50.0f), 100.0f, 1e-3f, "density 1 (lower bound overflow)");
    COMPARE(mp->getProperty<DENSITY>(50.0f), 100.0f, 1e-3f, "density 2");
    COMPARE(mp->getProperty<DENSITY>(100.0f), 100.0f, 1e-3f, "density 3");
    COMPARE(mp->getProperty<DENSITY>(150.0f), 150.0f, 1e-3f, "density 4");
    COMPARE(mp->getProperty<DENSITY>(250.0f), 200.0f, 1e-3f, "density 5 (upper bound overflow)");

    COMPARE(mp->getProperty<SPECIFIC_HEAT>(-2.0f), 1.0f, 1e-3f, "specific heat 1 (lower bound overflow)");
    COMPARE(mp->getProperty<SPECIFIC_HEAT>(2.0f), 1.02f, 1e-3f, "specific heat 2");
    COMPARE(mp->getProperty<SPECIFIC_HEAT>(100.0f), 2.505f, 1e-3f, "specific heat 3");
    COMPARE(mp->getProperty<SPECIFIC_HEAT>(114.514f), 3.01f, 1e-3f, "specific heat 4");
    COMPARE(mp->getProperty<SPECIFIC_HEAT>(300.0f), 3.01f, 1e-3f, "specific heat 5 (upper bound overflow)");

    COMPARE(mp->getProperty<ENTHALPY>(-1.0f), 2.0f, 1e-3f, "enthalpy 1 (lower bound overflow)");
    COMPARE(mp->getProperty<ENTHALPY>(2.0f), 4.04f, 1e-3f, "enthalpy 2");
    COMPARE(mp->getProperty<ENTHALPY>(100.0f), 252.5f, 1e-3f, "enthalpy 3");
    COMPARE(mp->getProperty<ENTHALPY>(114.514f), 346.687f, 1e-3f, "enthalpy 4");
    COMPARE(mp->getProperty<ENTHALPY>(300.0f), 604.0f, 1e-3f, "enthalpy 5 (upper bound overflow)");

    COMPARE(mp->getProperty<CONDUCTIVITY>(2.0f), 33.0f, 1e-3f, "conductivity");

    COMPARE(mp->useEnthalpy(), "Enthalpy flag");
    COMPARE(mp->getTemperature(1.0f), 0.0f, 1e-3f, "temperature 1 (lower bound overflow)");
    COMPARE(mp->getTemperature(4.04f), 2.0f, 1e-3f, "temperature 2");
    COMPARE(mp->getTemperature(252.5f), 100.0f, 1e-3f, "temperature 3");
    COMPARE(mp->getTemperature(346.687f), 114.514f, 1e-3f, "temperature 4");
    COMPARE(mp->getTemperature(1000.0f), 200.0f, 1e-3f, "temperature 5 (upper bound overflow)");

    Real temp = 0.0f;
    mp->updateTempDu(temp, -100.0f);
    COMPARE(temp, 0.0f, 1e-3f, "update temperature 1 (lower bound overflow)");
    mp->updateTempDu(temp, 4.04f - 2.0f);
    COMPARE(temp, 2.0f, 1e-3f, "update temperature 2");
    mp->updateTempDu(temp, 252.5f-4.04f);
    COMPARE(temp, 100.0f, 1e-3f, "update temperature 3");
    mp->updateTempDu(temp, 346.687f - 252.5f);
    COMPARE(temp, 114.514f, 1e-3f, "update temperature 4");
    mp->updateTempDu(temp, 1000.0f - 2.0f);
    COMPARE(temp, 200.0f, 1e-3f, "update temperature 5 (upper bound overflow)");

    COMPARE_summary();
}