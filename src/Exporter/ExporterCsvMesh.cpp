#include "ExporterCsvMesh.hpp"
#include <filesystem>
#include "src/Simulator.hpp"
#include "src/TimeManager.hpp"
#include "src/LaserSource.hpp"
#include "src/MeshData.hpp"
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
        Real stepCount = TimeManager::getCurrent()->getTimeStepCount();
        filesystem::path outfilePath = _exportPath;
        outfilePath /= "mesh_export_" + to_string(_exportCount) + "_" + to_string(stepCount) + "_" + to_string(time) + ".csv";

        ofstream csvFile(outfilePath);
        if (!csvFile.is_open())
        {
            LOG_ERR << "Cannot open csv output file: " << outfilePath;
            exit(1);
        }

        // Write header
        csvFile << "Index X,Index Y,Index Z,Pos X,Pos Y,Pos Z";
        for (string param : _parameters)
        {
            if (param == "temperature")
                csvFile << ",Temperature";
            else if (param == "coolingRate")
                csvFile << ",Cooling Rate";
            else if (param == "tempGrad")
                csvFile << ",Temperature Gradient X,Temperature Gradient Y,Temperature Gradient Z";
            else
                LOG_WARN << "Unidentified output parameter [" << param << "]";
        }
        csvFile << endl;

        // Write data
        MeshData *mesh = MeshData::getCurrent();
        csvFile << std::setprecision(_outPrecision);
        for (size_t i = 0; i < mesh->getSizeX(); i++)
        {
            for (size_t j = 0; j < mesh->getSizeY(); j++)
            {
                for (size_t k = 0; k < mesh->getSizeZ(); k++)
                {
                    
                }
            }
        }
    }
}