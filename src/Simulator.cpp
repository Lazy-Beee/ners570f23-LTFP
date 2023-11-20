#include "Simulator.hpp"
#include <iostream>
#include <omp.h>
#include "Common.hpp"
#include "SceneLoader.hpp"
#include "TimeManager.hpp"
#include "MaterialProperty.hpp"
#include "LaserSource.hpp"
#include "ThermalBoundary/BoundaryManager.hpp"
#include "utilities/Counting.hpp"
#include "utilities/Logger.hpp"
#include "utilities/Timing.hpp"

INIT_LOGGING
INIT_TIMING
INIT_COUNTING

using namespace std;

namespace LTFP
{
    Simulator *Simulator::current = nullptr;

    Simulator::Simulator()
    {
        _execPath = filesystem::canonical("/proc/self/exe");
        _projectPath = _execPath.parent_path().parent_path();
        _scenePath = _projectPath / "scenes" / "default.json";
        _outputPath = _projectPath / "output" / "default";
        _printPeriod = INT_MAX;
    }

    Simulator::~Simulator()
    {
        current = nullptr;
    }

    Simulator *Simulator::getCurrent()
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
        _scenePath = _projectPath / "scenes" / sceneFile;
        string caseName = sceneFile.substr(0, sceneFile.find_last_of("."));
        _outputPath = _projectPath / "output" / caseName;

        // Create output and log directory
        filesystem::path logPath = _outputPath / "log";
        try
        {
            filesystem::create_directories(logPath);
        }
        catch (filesystem::filesystem_error const &ex)
        {
            cerr << "Creat output and log directory failed" << endl;
            cerr << ex.what() << endl;
            exit(1);
        }

        // Start logger
        filesystem::path logFilePath = logPath / "log.txt";
#ifndef NDEBUG
        Utilities::logger.addSink(unique_ptr<Utilities::ConsoleSink>(new Utilities::ConsoleSink(Utilities::LogLevel::DEBUG)));
#else
        Utilities::logger.addSink(unique_ptr<Utilities::ConsoleSink>(new Utilities::ConsoleSink(Utilities::LogLevel::INFO)));
#endif
        Utilities::logger.addSink(unique_ptr<Utilities::FileSink>(new Utilities::FileSink(Utilities::LogLevel::DEBUG, logFilePath)));

        LOG_INFO << "Laser additive manufacturing Thermal Field Prediction (LTFP) " << _version;
#ifdef USE_DOUBLE
        LOG_INFO << "LTPF is running in double precision mode";
#else
        LOG_INFO << "LTPF is running in single precision mode";
#endif
        LOG_INFO << "Available OpenMP threads: " << omp_get_max_threads();
        LOG_DEBUG << "Project path: " << _projectPath;
        LOG_DEBUG << "Executable:   " << _execPath;
        LOG_DEBUG << "Scene file:   " << _scenePath;
        LOG_DEBUG << "Output path:  " << _outputPath;

        // Copy scene file to output directory
        try
        {
            filesystem::copy_file(_scenePath, logPath / "scene.json", filesystem::copy_options::overwrite_existing);
        }
        catch (filesystem::filesystem_error const &ex)
        {
            LOG_WARN << "Cannot copy scene file to output directory";
            LOG_WARN << ex.what();
        }
    }

    /// @brief Initialize simulation
    void Simulator::initialize()
    {
        SceneLoader *sl = SceneLoader::getCurrent();

        sl->readScene();

        if (sl->getExportConfig().consolePeriod > 0)
            _printPeriod = sl->getExportConfig().consolePeriod;

        TimeManager::getCurrent()->init();
        MaterialProperty::getCurrent()->init();
        BoundaryManager::getCurrent()->init();
        LaserSource::getCurrent()->init();
    }

    /// @brief Advance one time step
    void Simulator::step()
    {
        TimeManager *tm = TimeManager::getCurrent();

        // Print info to consol periodically
        static int nextPrint = 0;
        if (tm->getTimeStepCount() >= nextPrint)
        {
            LOG_INFO << "Step: " << tm->getTimeStepCount() << " \tStep size: " << tm->getTimeStepSize() * 1000 << " ms";
            nextPrint += _printPeriod;
        }

        START_TIMING("LaserSourcePrecompute");
        LaserSource::getCurrent()->precomputePowerDistribution();
        STOP_TIMING_AVG;

        // TODO: domain increment

        // TODO: Solve Thermal equation

        // TODO: Export data
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
    void Simulator::runSimulation(int argc, char *argv[])
    {
        // Create modules
        SceneLoader *sceneLoader = SceneLoader::getCurrent();
        TimeManager *timeManager = TimeManager::getCurrent();
        MaterialProperty *materialProperty = MaterialProperty::getCurrent();
        LaserSource *laserSource = LaserSource::getCurrent();
        BoundaryManager *boundaryManager = BoundaryManager::getCurrent();

        // Setup simulation with scene file path
        if (argc == 1)
            initUtilities("default.json");
        else if (argc == 2)
            initUtilities(argv[1]);
        else
        {
            cout << "Invalid number of command line inputs" << endl;
            exit(1);
        }

        // Run simulator
        initialize();

        while (timeManager->advance())
            step();

        finalize();

        // Destroy modules
        delete sceneLoader;
        delete timeManager;
        delete materialProperty;
        delete laserSource;
        delete boundaryManager;
    }
}