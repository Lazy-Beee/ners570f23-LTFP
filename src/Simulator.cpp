#include "Simulator.hpp"
#include <iostream>
#include "../utilities/Counting.h"
#include "../utilities/Logger.h"
#include "../utilities/Timing.h"

INIT_LOGGING
INIT_TIMING
INIT_COUNTING

using namespace std;

namespace LTFP
{
    Simulator* Simulator::current = nullptr;

    Simulator::Simulator()
    {
        cout << "Simulator object created." << endl;
    }

    Simulator::~Simulator()
    {
        current = nullptr;
    }

    Simulator* Simulator::getCurrent()
    {
        if (current == nullptr)
            current = new Simulator();

        return current;
    }

    /// @brief Initialize simulation environment and utilities
	/// @param sceneFile scene file name
    void Simulator::init(string sceneFile)
    {
        _execPath = filesystem::canonical("/proc/self/exe");
        _scenePath = filesystem::canonical(_execPath.parent_path()/".."/"scenes"/sceneFile);
        string caseName = sceneFile.substr(0, sceneFile.find_last_of("."));
        _outputPath = filesystem::canonical(_execPath.parent_path()/".."/"output"/caseName);
        filesystem::path logPath = _outputPath/"log";
        
        try
        {
            filesystem::create_directory(logPath);
        }
        catch (filesystem::filesystem_error const& ex)
        {
            cout << "Creat output and log directory failed: " << logPath << endl;
            cout << ex.what() << endl;
            exit(1);
        }

        filesystem::path logFilePath = logPath/"log.txt";
        Utilities::logger.addSink(unique_ptr<Utilities::ConsoleSink>(new Utilities::ConsoleSink(Utilities::LogLevel::INFO)));
        Utilities::logger.addSink(unique_ptr<Utilities::FileSink>(new Utilities::FileSink(Utilities::LogLevel::DEBUG, logFilePath)));
        
        LOG_INFO << "Laser additive manufacturing Thermal Field Prediction (LTFP) solver activated";
        LOG_INFO << "Scene file:        " << _scenePath;
        LOG_INFO << "Executable path:   " << _execPath;
        LOG_INFO << "Output path:       " << _outputPath;

#ifdef USE_DOUBLE
        LOG_INFO << "LTPF is running in double precision mode";
#else
        LOG_INFO << "LTPF is running in single precision mode";
#endif
    }

    /// @brief Advance one time step
	void Simulator::step()
    {

    }

    /// @brief Wrap up simulation
    void Simulator::finalize()
    {
        LOG_INFO << "---------------------------------------------------------------------------";
		// LOG_INFO << "Time: " << TimeManager::getCurrent()->getTime();
		Utilities::Timing::printAverageTimes();
		Utilities::Timing::printTimeSums();
		Utilities::Counting::printAverageCounts();
		Utilities::Counting::printCounterSums();
    }

    /// @brief Run the simulation from beginning to end
    /// @param argc Number of command line inputs
    /// @param argv Command line inputs
    void Simulator::runSimulation(int argc, char* argv[])
    {
        if (argc == 1)
            init("default.json");
        else if (argc == 2)
            init(argv[1]);
        else
        {
            cout << "Invalid number of command line inputs: " << to_string(argc) << endl;
            exit(1);
        }
        

        finalize();
    }
}