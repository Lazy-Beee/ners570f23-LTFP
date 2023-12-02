#include <chrono>
#include <thread>
#include "src/Common.hpp"
#include "src/Simulator.hpp"
#include "utilities/Counting.hpp"
#include "utilities/Logger.hpp"
#include "utilities/Timing.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator sim = Simulator();
    sim.initUtilities("blank.json");

    for (int i = 0; i < 100; i++)
    {
        START_TIMING("TEST1");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        STOP_TIMING_AVG;

        INCREASE_COUNTER("Test2", 0.1 * i);
    }

    sim.finalize();
}