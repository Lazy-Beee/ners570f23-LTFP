#include "ExporterVtkMesh.hpp"
#include <filesystem>
#include "src/Simulator.hpp"
#include "src/TimeManager.hpp"
#include "src/LaserSource.hpp"
#include "src/MeshData.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ExporterVtkMesh::ExporterVtkMesh(int type) : Exporter(type)
    {
        _exportPath = Simulator::getCurrent()->getOutputPath() / "vtk_mesh";
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

    void ExporterVtkMesh::exportData()
    {
        Real time = TimeManager::getCurrent()->getTime();
        filesystem::path outfile = _exportPath;
        outfile /= "mesh_export_" + to_string(_exportCount) + "_" + to_string(time) + ".vtk";

        // TODO: export to vtk
    }
}