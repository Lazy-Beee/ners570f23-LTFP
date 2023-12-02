#include "ExporterCsvMesh.hpp"
#include <filesystem>
#include "ExportManager.hpp"
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
        _exportPath = ExportManager::getCurrent()->getExportPath() / "csv_mesh";
        filesystem::remove_all(_exportPath);
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
        MeshData *mesh = MeshData::getCurrent();
        LaserSource *laser = LaserSource::getCurrent();

        filesystem::path outfilePath = _exportPath;
        outfilePath /= "mesh_export_" + to_string(_exportCount) + ".csv";
        _exportCount++;

        ofstream outfile(outfilePath);
        if (!outfile.is_open())
        {
            LOG_ERR << "Cannot open csv output file: " << outfilePath;
            exit(1);
        }

        // Write header
        outfile << "Index X,Index Y,Index Z,Pos X,Pos Y,Pos Z";
        for (string param : _parameters)
        {
            if (param == "temperature")
                outfile << ",Temperature";
            else if (param == "coolingRate")
                outfile << ",Cooling Rate";
            else if (param == "tempGrad")
                outfile << ",Temperature Gradient X,Temperature Gradient Y,Temperature Gradient Z";
            else if (param == "laserPower")
                outfile << ",Laser Power";
        }
        outfile << "\n";

        // Write data
        outfile << std::setprecision(_outPrecision);
        for (size_t i = 0; i < mesh->getSizeX(); i++)
        {
            for (size_t j = 0; j < mesh->getSizeY(); j++)
            {
                for (size_t k = 0; k < mesh->getSizeZ(); k++)
                {
                    outfile << i << "," << j << "," << k;
                    Vector3r pos = mesh->getCenterPos(i, j, k);
                    outfile << "," << pos[0] << "," << pos[1] << "," << pos[2];
                    for (string param : _parameters)
                    {
                        if (param == "temperature")
                            outfile << "," << mesh->getTemperature(i, j, k);
                        else if (param == "coolingRate")
                            outfile << "," << mesh->getCoolingRate(i, j, k);
                        else if (param == "tempGrad")
                        {
                            Vector3r tempGrad = mesh->getTemperatureGrad(i, j, k);
                            outfile << "," << tempGrad[0] << "," << tempGrad[1] << "," << tempGrad[2];
                        }
                        else if (param == "laserPower")
                            outfile << "," << laser->getLaserPower(i, j, k);
                    }
                    outfile << "\n";
                }
            }
        }

        outfile.close();
    }
}