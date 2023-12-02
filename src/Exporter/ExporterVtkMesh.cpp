#include "ExporterVtkMesh.hpp"
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
    ExporterVtkMesh::ExporterVtkMesh(int type) : Exporter(type)
    {
        _exportPath = ExportManager::getCurrent()->getExportPath() / "vtk_mesh";
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

    /// @note Reference: https://examples.vtk.org/site/VTKFileFormats/
    void ExporterVtkMesh::exportData()
    {
        exportVtkASCII();
        // exportVtkBinary();
    }

    void ExporterVtkMesh::exportVtkASCII()
    {
        TimeManager *tm = TimeManager::getCurrent();
        MeshData *mesh = MeshData::getCurrent();

        Real time = tm->getTime();
        int stepCount = tm->getTimeStepCount();
        filesystem::path outfilePath = _exportPath;
        outfilePath /= "mesh_export_" + to_string(_exportCount) + "_" + to_string(stepCount) + "_" + to_string(time) + ".vtk";

        ofstream outfile = ofstream(outfilePath, ios::binary);
        if (!outfile.is_open())
        {
            LOG_ERR << "Cannot open vtk output file: " << outfilePath;
            exit(1);
        }

        outfile << "# vtk DataFile Version 3.0\n";
        outfile << "LTFP data export\n";
        outfile << "ASCII\n";
        outfile << "DATASET STRUCTURED_GRID\n";
        outfile << "DIMENSIONS " << mesh->getSizeX() << " " << mesh->getSizeY() << " " << mesh->getSizeZ() << "\n";
        outfile << "POINTS " << mesh->getSizeX() * mesh->getSizeY() * mesh->getSizeZ() << " " << realStr << "\n";

        Real dx = mesh->getIntervalX();
        Real dy = mesh->getIntervalY();
        Real dz = mesh->getIntervalZ();
        Vector3r pos0 = mesh->getCenterPos(0, 0, 0);
        for (size_t k = 0; k < mesh->getSizeZ(); k++)
        {
            for (size_t j = 0; j < mesh->getSizeY(); j++)
            {
                for (size_t i = 0; i < mesh->getSizeX(); i++)
                {
                    outfile << pos0[0] + i * dx << " " << pos0[1] + j * dy << " " << pos0[2] + k * dz << " ";
                }
                outfile << "\n";
            }
        }
        outfile << "\n";

        int cellCount = mesh->getSizeX() * mesh->getSizeY() * mesh->getSizeZ();
        outfile << "POINT_DATA " << cellCount << "\n";
        for (string param : _parameters)
        {
            if (param == "temperature")
                temperatureASCII(outfile);
            else if (param == "coolingRate")
                coolingRateASCII(outfile);
            else if (param == "tempGrad")
                tempGradASCII(outfile);
            else if (param == "laserPower")
                laserPowerASCII(outfile);
        }

        outfile.close();
    }

    void ExporterVtkMesh::exportVtkBinary()
    {
        return;
    }

    void ExporterVtkMesh::temperatureASCII(ofstream &outfile)
    {
        MeshData *mesh = MeshData::getCurrent();

        outfile << "SCALARS Temperature float 1\n";
        outfile << "LOOKUP_TABLE default\n";
        for (size_t k = 0; k < mesh->getSizeZ(); k++)
        {
            for (size_t j = 0; j < mesh->getSizeY(); j++)
            {
                for (size_t i = 0; i < mesh->getSizeX(); i++)
                {
                    outfile << mesh->getTemperature(i, j, k) << " ";
                }
                outfile << "\n";
            }
        }
        outfile << "\n";
    }

    void ExporterVtkMesh::coolingRateASCII(ofstream &outfile)
    {
        MeshData *mesh = MeshData::getCurrent();

        outfile << "SCALARS Temperature float 1\n";
        outfile << "LOOKUP_TABLE default\n";
        for (size_t k = 0; k < mesh->getSizeZ(); k++)
        {
            for (size_t j = 0; j < mesh->getSizeY(); j++)
            {
                for (size_t i = 0; i < mesh->getSizeX(); i++)
                {
                    outfile << mesh->getCoolingRate(i, j, k) << " ";
                }
                outfile << "\n";
            }
        }
        outfile << "\n";
    }

    void ExporterVtkMesh::tempGradASCII(ofstream &outfile)
    {
        MeshData *mesh = MeshData::getCurrent();

        outfile << "SCALARS TemperatureGradientX float 1\n";
        outfile << "LOOKUP_TABLE default\n";
        for (size_t k = 0; k < mesh->getSizeZ(); k++)
        {
            for (size_t j = 0; j < mesh->getSizeY(); j++)
            {
                for (size_t i = 0; i < mesh->getSizeX(); i++)
                {
                    Vector3r tempGrad = mesh->getTemperatureGrad(i, j, k);
                    outfile << tempGrad[0] << " ";
                }
                outfile << "\n";
            }
        }
        outfile << "\n";

        outfile << "SCALARS TemperatureGradientY float 1\n";
        outfile << "LOOKUP_TABLE default\n";
        for (size_t k = 0; k < mesh->getSizeZ(); k++)
        {
            for (size_t j = 0; j < mesh->getSizeY(); j++)
            {
                for (size_t i = 0; i < mesh->getSizeX(); i++)
                {
                    Vector3r tempGrad = mesh->getTemperatureGrad(i, j, k);
                    outfile << tempGrad[1] << " ";
                }
                outfile << "\n";
            }
        }
        outfile << "\n";

        outfile << "SCALARS TemperatureGradientZ float 1\n";
        outfile << "LOOKUP_TABLE default\n";
        for (size_t k = 0; k < mesh->getSizeZ(); k++)
        {
            for (size_t j = 0; j < mesh->getSizeY(); j++)
            {
                for (size_t i = 0; i < mesh->getSizeX(); i++)
                {
                    Vector3r tempGrad = mesh->getTemperatureGrad(i, j, k);
                    outfile << tempGrad[2] << " ";
                }
                outfile << "\n";
            }
        }
        outfile << "\n";
    }

    void ExporterVtkMesh::laserPowerASCII(ofstream &outfile)
    {
        MeshData *mesh = MeshData::getCurrent();

        outfile << "SCALARS Temperature float 1\n";
        outfile << "LOOKUP_TABLE default\n";
        for (size_t k = 0; k < mesh->getSizeZ(); k++)
        {
            for (size_t j = 0; j < mesh->getSizeY(); j++)
            {
                for (size_t i = 0; i < mesh->getSizeX(); i++)
                {
                    outfile << LaserSource::getCurrent()->getLaserPower(i, j, k) << " ";
                }
                outfile << "\n";
            }
        }
        outfile << "\n";
    }
}