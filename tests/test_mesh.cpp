#include "compare.hpp"
#include "src/Simulator.hpp"
#include "src/SceneLoader.hpp"
#include "src/MeshData.hpp"

using namespace std;
using namespace LTFP;

int main(){
    
    Simulator *sim = Simulator::getCurrent();
    SceneLoader *sl = SceneLoader::getCurrent();
    MeshData *meshData = MeshData::getCurrent();

    sim->initUtilities("test.json");
    sl->readScene();
    meshData->init();

#ifndef NDEBUG
    
    // For Nx, Ny, Nz
    int nx=11, ny=10, nz=21;
    COMPARE(meshData->getSizeX(), static_cast<size_t>(nx), static_cast<size_t>(0), "xSize:Nx");
    COMPARE(meshData->getSizeY(), static_cast<size_t>(ny), static_cast<size_t>(0), "ySize:Ny");
    COMPARE(meshData->getSizeZ(), static_cast<size_t>(nz), static_cast<size_t>(0), "zSize:Nz");

    // For dx, dy, dz
    float interval = 0.1f;
    COMPARE(meshData->getIntervalX(), interval, 1e-3f, "xInterval:dx");
    COMPARE(meshData->getIntervalY(), interval, 1e-3f, "yInterval:dy");
    COMPARE(meshData->getIntervalZ(), interval, 1e-3f, "zInterval:dz");

    // For x, y, z Position
    int i = 1, j = 1, k = 1;
    float xpos = (i+0.5f)*interval, ypos = (j+0.5f)*interval, zpos = (k+0.5f)*interval;
    Vector3r xyzMeshpos = meshData->getCenterPos(i, j, k);
    COMPARE(xyzMeshpos[0], xpos, 1e-3f, "xpos");
    COMPARE(xyzMeshpos[1], ypos, 1e-3f, "ypos");
    COMPARE(xyzMeshpos[2], zpos, 1e-3f, "zpos");

    // For Cooling Rate
    Real coolingRate = 0.01;
    meshData->setCoolingRate(i, j, k, coolingRate);
    COMPARE(meshData->getCoolingRate(i, j, k), coolingRate, static_cast<Real>(1e-3), "cooling rate");

    // For temperature
    Real tempT = 200.0, initT = 300.0;
    meshData->setTemperature(i, j, k, tempT);
    COMPARE(meshData->getTemperature(2, 2, 2), initT, static_cast<Real>(1e-3), "temperature (init)");
    COMPARE(meshData->getTemperature(i, j, k), tempT, static_cast<Real>(1e-3), "temperature (get/set)");
    COMPARE(meshData->getTemperature(i, j, k), tempT+10.0f, static_cast<Real>(10.0+1e-3), "temperature (hand-tune get/set)");

    // For temperture gradient
    Vector3r setGradT = {1.0, 2.0, 3.0};
    meshData->setTemperatureGrad(i, j, k, setGradT);
    Vector3r getGradT = meshData->getTemperatureGrad(i, j, k);
    COMPARE(getGradT[0], setGradT[0], 1e-3f, "temperature gradient along x");
    COMPARE(getGradT[1], setGradT[1], 1e-3f, "temperature gradient along y");
    COMPARE(getGradT[2], setGradT[2], 1e-3f, "temperature gradient along z");

#endif

    COMPARE_summary();

}