#include "compare.hpp"
#include "src/Common.hpp"
#include "src/Simulator.hpp"
#include "src/TimeManager.hpp"
#include "src/SceneLoader.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator *sim = Simulator::getCurrent();
    TimeManager *tm = TimeManager::getCurrent();

    sim->initUtilities("test.json");
    SceneLoader::getCurrent()->readScene();
    tm->init();

    Real maxDt = 0.2;
    Real minDt = 0.001;
    int maxSteps = 1000;
    // Real endTime = 120.0;

    Real dt1 = 1.0;
    Real dt2 = 0.05;
    Real dt3 = 0.0001;
    int n1 = 100;
    int n2 = 65;

    tm->setTimeStepSize(dt1);
    tm->advance();
    for (int i = 0; i < n1; i++)
    {
        tm->advance();
    }
    COMPARE<Real>(tm->getTimeStepSize(), maxDt, 1e-3, "Time step size 1");
    COMPARE(tm->getTimeStepCount() == n1, "Time step count 1");
    COMPARE<Real>(tm->getTime(), maxDt * n1, 1e-3, "Current time 1");

    tm->setTimeStepSize(dt2);
    for (int i = 0; i < n2; i++)
    {
        tm->advance();
    }
    COMPARE<Real>(tm->getTimeStepSize(), dt2, 1e-3, "Time step size 2");
    COMPARE(tm->getTimeStepCount() == (n1 + n2), "Time step count 2");
    COMPARE<Real>(tm->getTime(), maxDt * n1 + dt2 * n2, 1e-3, "Current time 2");

    tm->setTimeStepSize(dt3);
    while (tm->advance())
    {
    }
    COMPARE<Real>(tm->getTimeStepSize(), minDt, 1e-3, "Time step size 3");
    COMPARE(tm->getTimeStepCount() == 1000, "Time step count 3");
    COMPARE<Real>(tm->getTime(), maxDt * n1 + dt2 * n2 + (maxSteps - n1 - n2) * minDt, 1e-3, "Current time 3");

    sim->finalize();

    COMPARE_summary();
}