#include <iostream>
#include "src/Simulator.hpp"

int main(int argc, char *argv[])
{
    LTFP::Simulator sim = LTFP::Simulator();

    if (argc == 1)
    {
        // sim.runSimulation("1D_diffusion_test.json");
        sim.runSimulation("Conservation_test.json"); // TODO: seg fault when ySize=zSize=1
    }
    else if (argc == 2)
    {
        sim.runSimulation(argv[1]);
    }
    else
    {
        std::cerr << "Invalid number of command line inputs" << std::endl;
        return 1;
    }
}