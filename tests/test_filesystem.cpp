#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    std::filesystem::path p;
    p = std::filesystem::canonical("/proc/self/exe");
    std::cout << p << std::endl;
    std::cout << p.parent_path().parent_path() << std::endl;

    return 0;
}