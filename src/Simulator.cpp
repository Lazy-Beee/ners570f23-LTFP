#include "Simulator.hpp"
#include <iostream>
#include "SceneLoader.hpp"
#include "TimeManager.hpp"
#include "../utilities/Counting.hpp"
#include "../utilities/Logger.hpp"
#include "../utilities/Timing.hpp"

INIT_LOGGING
INIT_TIMING
INIT_COUNTING

using namespace std;

namespace LTFP
{
    Simulator* Simulator::current = nullptr;

    Simulator::Simulator()
    {
        _execPath = filesystem::canonical("/proc/self/exe");
        _projectPath = _execPath.parent_path().parent_path();
		_scenePath = _projectPath/"scenes"/"default.json";
		_outputPath = _projectPath/"output"/"default";
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

    /// @brief Initialize file paths, logger, counter, and timer
    /// @param sceneFile scene file name
    void Simulator::initUtilities(string sceneFile)
    {
        // Initialize file paths
        _scenePath = _projectPath/"scenes"/sceneFile;
        string caseName = sceneFile.substr(0, sceneFile.find_last_of("."));
        _outputPath = _projectPath/"output"/caseName;

        // Create output and log directory
        filesystem::path logPath = _outputPath/"log";
        try
        {
            filesystem::create_directories(logPath);
        }
        catch (filesystem::filesystem_error const& ex)
        {
            cerr << "Creat output and log directory failed" << endl;
            cerr << ex.what() << endl;
            exit(1);
        }

        // Start logger
        filesystem::path logFilePath = logPath/"log.txt";
        Utilities::logger.addSink(unique_ptr<Utilities::ConsoleSink>(new Utilities::ConsoleSink(Utilities::LogLevel::DEBUG)));
        Utilities::logger.addSink(unique_ptr<Utilities::FileSink>(new Utilities::FileSink(Utilities::LogLevel::DEBUG, logFilePath)));
        
        LOG_INFO << "Laser additive manufacturing Thermal Field Prediction (LTFP) " << _version;
#ifdef USE_DOUBLE
        LOG_INFO << "LTPF is running in double precision mode";
#else
        LOG_INFO << "LTPF is running in single precision mode";
#endif
        LOG_DEBUG << "Project path:      " << _projectPath;
        LOG_DEBUG << "Executable path:   " << _execPath;
        LOG_DEBUG << "Scene file:        " << _scenePath;
        LOG_DEBUG << "Output path:       " << _outputPath;

        // Copy scene file to output directory
        try
        {
            filesystem::copy_file(_scenePath, logPath/"scene.json", filesystem::copy_options::overwrite_existing);
        }
        catch (filesystem::filesystem_error const& ex)
        {
            LOG_WARN << "Copy scene file to output directory failed.";
            LOG_WARN << ex.what();
        }
    }

    /// @brief Advance one time step
	void Simulator::step()
    {

    }

    /// @brief Wrap up simulation
    void Simulator::finalize()
    {
        LOG_INFO << "---------------------------------------------------------------------------";
		Utilities::Timing::printAverageTimes();
		Utilities::Timing::printTimeSums();
		Utilities::Counting::printAverageCounts();
		Utilities::Counting::printCounterSums();
    }

    /// @brief Run the simulation from beginning to end
    /// @param argc Number of command line inputs
    /// @param argv Command line inputs
    /// @note Only path the scene filename from command line. The scene file should be placed in ./scenes folder.
    void Simulator::runSimulation(int argc, char* argv[])
    {
        SceneLoader* sceneLoader = SceneLoader::getCurrent();
        TimeManager* timeManager = TimeManager::getCurrent();

        // Initialization
        if (argc == 1)
            initUtilities("default.json");
        else if (argc == 2)
            initUtilities(argv[1]);
        else
        {
            cout << "Invalid number of command line inputs" << endl;
            exit(1);
        }

        sceneLoader->readScene();
        timeManager->init();

        while(timeManager->advance())
        {
            
        }
        
        finalize();
    }
}