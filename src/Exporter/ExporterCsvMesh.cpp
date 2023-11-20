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
        _exportPath = Simulator::getCurrent()->getOutputPath() / "csv_mesh";
        try
        {
            filesystem::create_directories(_exportPath);
        }
        catch (filesystem::filesystem_error const &ex)
        {
            cerr << "Creat output and log directory failed" << endl;
            cerr << ex.what() << endl;
            exit(1);
        }
    }

    void ExporterCsvMesh::exportData()
    {
        Real time = TimeManager::getCurrent()->getTime();
        filesystem::path outfile = _exportPath;
        outfile /= "mesh_export_" + to_string(_exportCount) + "_" + to_string(time) + ".csv";

        // TODO: export to csv
    }
}