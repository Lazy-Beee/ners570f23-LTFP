#ifndef __ExporterCsvMesh__
#define __ExporterCsvMesh__

#include "src/Common.hpp"
#include <string>
#include "src/SceneLoader.hpp"
#include "Exporter.hpp"

namespace LTFP
{
	/// @brief Class that exports mesh data to CVS files.
	class ExporterCsvMesh : public Exporter
	{
	public:
		ExporterCsvMesh(int type);
		~ExporterCsvMesh(){};
		void exportData();
	};
}

#endif
