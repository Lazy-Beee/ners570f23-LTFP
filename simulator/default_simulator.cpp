#include <iostream>
#include "src/Simulator.hpp"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        LTFP::Simulator::getCurrent()->runSimulation("1D_diffusion_test.json");
    }
    else if (argc == 2)
    {
        LTFP::Simulator::getCurrent()->runSimulation(argv[1]);
    }
    else
    {
        std::cerr << "Invalid number of command line inputs" << std::endl;
        return 1;
    }
}