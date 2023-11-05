#ifndef __Simulator__
#define __Simulator__

#include "Common.hpp"
#include <vector>
#include <string>
#include <filesystem>

namespace LTFP
{
	/// @brief Class managing simulator work flow.
	/// @details Run the simulation in default mode with runSimulation(). Or assemble the simulation with provided functions.
	/// @note This class is a singleton.
	class Simulator
	{
	private:
		static Simulator *current;
		std::string _version = "v1.0.0";
		std::filesystem::path _projectPath;
		std::filesystem::path _execPath;
		std::filesystem::path _scenePath;
		std::filesystem::path _outputPath;

	public:
		Simulator();
		Simulator(const Simulator &) = delete;
		Simulator &operator=(const Simulator &) = delete;
		~Simulator();

		static Simulator *getCurrent();
		void initUtilities(std::string sceneFile);
		void step();
		void finalize();
		void runSimulation(int argc, char *argv[]);

		inline std::filesystem::path getOutputPath() const { return _outputPath; };
		inline std::filesystem::path getScenePath() const { return _scenePath; };
	};
}

#endif