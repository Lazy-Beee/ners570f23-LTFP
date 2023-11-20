#ifndef __ExporterVtkMesh__
#define __ExporterVtkMesh__

#include "src/Common.hpp"
#include <string>
#include "src/SceneLoader.hpp"
#include "Exporter.hpp"

namespace LTFP
{
	/// @brief Class that exports mesh data to CVS files.
	class ExporterVtkMesh : public Exporter
	{
	private:
		void exportVtkASCII();
		void exportVtkBinary();
		void temperatureASCII(std::ofstream &outfile);
		void coolingRateASCII(std::ofstream &outfile);
		void tempGradASCII(std::ofstream &outfile);
		void laserPowerASCII(std::ofstream &outfile);

	public:
		ExporterVtkMesh(int type);
		~ExporterVtkMesh(){};
		void exportData();
	};
}

#endif
