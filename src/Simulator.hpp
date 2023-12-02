#ifndef __Simulator__
#define __Simulator__

#include "Common.hpp"
#include <vector>
#include <string>
#include <filesystem>
#include "ThermalSolver/ThermalSolver.hpp"

namespace LTFP
{
	/// @brief Class managing simulator work flow.
	/// @details Run the simulation in default mode with runSimulation(). Or assemble the simulation with provided functions.
	class Simulator
	{
	private:
		std::string _version = "v1.0.0";
		std::filesystem::path _projectPath;
		std::filesystem::path _execPath;
		std::filesystem::path _scenePath;
		std::filesystem::path _exportPath;
		int _printPeriod;
		ThermalSolver *_thermalSolver;

	public:
		Simulator();
		~Simulator();

		void initUtilities(std::string sceneFile);
		void initialize();
		void step();
		void finalize();
		void runSimulation(std::string sceneFile);

		inline std::filesystem::path getScenePath() const { return _scenePath; };
		inline std::filesystem::path getExportPath() const { return _exportPath; };
	};
}

#endif