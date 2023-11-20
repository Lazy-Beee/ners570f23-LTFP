#include "ExporterVtkMesh.hpp"
#include <filesystem>
#include "src/Simulator.hpp"
#include "src/TimeManager.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    ExporterVtkMesh::ExporterVtkMesh(int type) : Exporter(type)
    {
    }

    void ExporterVtkMesh::exportData()
    {
        Real time = TimeManager::getCurrent()->getTime();
        filesystem::path outfile = Simulator::getCurrent()->getOutputPath() / "vtk_mesh";
        outfile /= "mesh_export_" + to_string(_exportCount) + "_" + to_string(time) + ".vtk";

        // TODO: export to vtk
    }
}