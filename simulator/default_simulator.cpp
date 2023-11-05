#include <iostream>
#include <string>
#include "../src/Common.hpp"
#include "../src/Simulator.hpp"
#include "../utilities/Counting.h"
#include "../utilities/Logger.h"
#include "../utilities/Timing.h"

int main(int argc, char *argv[])
{
    if (argc > 2)
        std::cerr << "Invalid number of command line inputs" << std::endl;

    LTFP::Simulator::getCurrent()->runSimulation(argc, argv);

    return 0;
}