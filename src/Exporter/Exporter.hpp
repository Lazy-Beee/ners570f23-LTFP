#ifndef __Exporter__
#define __Exporter__

#include "src/Common.hpp"
#include <string>
#include <filesystem>
#include "src/SceneLoader.hpp"

namespace LTFP
{
	enum ExporterType
	{
		VTK_MESH = 0,
		CSV_MESH = 1
	};
	static std::vector<std::string> ExporterTypeName = {
		"[VTK mesh]",
		"[CSV mesh]"};

	/// @brief Base class of exporters.
	class Exporter
	{
	protected:
		const ExporterType _type;
		Real _exportPeriod;
		std::vector<std::string> _parameters;
		int _exportCount;
		Real _nextExportTime;
		std::filesystem::path _exportPath;
		int _outPrecision;

	public:
		Exporter(int type);
		virtual ~Exporter(){};

		virtual void init(SceneLoader::ExporterConfig config);
		bool timeToExport();
		/// @brief Export data to file
		virtual void exportData() = 0;
	};
}

#endif
