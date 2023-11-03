#include <iostream>
#include <string>
#include "../utilities/Common.h"

void setSceneFile(std::string filename)
{
    std::cout << "Using " << filename << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
        setSceneFile("../scenes/default.json");
    else if (argc == 2)
        setSceneFile(argv[1]);
    else
        std::cerr << "Invalid number of command line inputs" << std::endl;

#ifdef USE_DOUBLE
    std::cout << "LTPF running in double precision." << std::endl;
#else
    std::cout << "LTPF running in single precision." << std::endl;
#endif

    return 0;
}