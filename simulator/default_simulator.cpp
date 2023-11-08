#include <iostream>
#include <string>
#include "src/Common.hpp"
#include "src/Simulator.hpp"
#include "utilities/Counting.hpp"
#include "utilities/Logger.hpp"
#include "utilities/Timing.hpp"

int main(int argc, char *argv[])
{
    if (argc > 2)
        std::cerr << "Invalid number of command line inputs" << std::endl;

    LTFP::Simulator::getCurrent()->runSimulation(argc, argv);

    return 0;
}