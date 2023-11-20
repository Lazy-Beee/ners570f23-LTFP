#ifndef __ExportManager__
#define __ExportManager__

#include "src/Common.hpp"
#include "Exporter.hpp"
#include "ExporterVtkMesh.hpp"
#include "ExporterCsvMesh.hpp"

namespace LTFP
{
	/// @brief Class managing data export.
	/// @note This class is a singleton.
	class ExportManager
	{
	private:
		static ExportManager *current;
		std::vector<Exporter *> _exporters;

	public:
		ExportManager();
		ExportManager(const ExportManager &) = delete;
		ExportManager &operator=(const ExportManager &) = delete;
		~ExportManager();

		static ExportManager *getCurrent();
		void init();
		void step();
	};
}

#endif
