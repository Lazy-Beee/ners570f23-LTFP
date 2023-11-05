#include "compare.hpp"
#include "../src/Common.hpp"
#include "../src/Simulator.hpp"

using namespace std;
using namespace LTFP;

int main()
{
    Simulator* sim = Simulator::getCurrent();
    sim->initUtilities("test.json");


}