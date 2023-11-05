#include "compare.hpp"
#include "../src/Common.hpp"
#include "../src/Simulator.hpp"
#include "../src/TimeManager.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator* sim = Simulator::getCurrent();
    sim->initUtilities("blank.json");

    TimeManager* tm = TimeManager::getCurrent();

    Real dt1 = 1.2;
    Real dt2 = 0.5;
    int n1 = 100;
    int n2 = 65;

    tm->setTimeStepSize(dt1);

    for (int i = 0; i < n1; i++)
    {
        tm->advance();
    }

    COMPARE<Real>(tm->getTimeStepSize(), dt1, 1e-3, "Time step size 1");
    COMPARE(tm->getTimeStepCount() == n1, "Time step count 1");
    COMPARE<Real>(tm->getTime(), dt1 * static_cast<Real>(n1), 1e-3, "Current time 1");

    
    tm->setTimeStepSize(dt2);

    for (int i = 0; i < n2; i++)
    {
        tm->advance();
    }

    COMPARE<Real>(tm->getTimeStepSize(), dt2, 1e-3, "Time step size 2");
    COMPARE(tm->getTimeStepCount() == (n1 + n2), "Time step count 2");
    COMPARE<Real>(tm->getTime(), dt1 * static_cast<Real>(n1) + dt2 * static_cast<Real>(n2), 1e-3, "Current time 2");
    
    sim->finalize();
}