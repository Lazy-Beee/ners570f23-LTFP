#include "ExporterCsvMesh.hpp"
#include <filesystem>
#include "src/Simulator.hpp"
#include "src/TimeManager.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ExporterCsvMesh::ExporterCsvMesh(int type) : Exporter(type)
    {
    }

    void ExporterCsvMesh::exportData()
    {
        Real time = TimeManager::getCurrent()->getTime();
        filesystem::path outfile = Simulator::getCurrent()->getOutputPath() / "csv_mesh";
        outfile /= "mesh_export_" + to_string(_exportCount) + "_" + to_string(time) + ".csv";

        // TODO: export to csv
    }
}