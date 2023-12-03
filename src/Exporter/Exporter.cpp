#include "Exporter.hpp"
#include "src/TimeManager.hpp"
#include "utilities/Logger.hpp"

using namespace std;

namespace LTFP
{
    Exporter::Exporter(int type) : _type(static_cast<ExporterType>(type))
    {
        LOG_DEBUG << "Exporter " << ExporterTypeName[type] << " is created";
        _exportPeriod = REAL_MAX;
        _exportCount = 0;
        _parameters = {};
        _nextExportTime = 0.0f;
        _outPrecision = 6;
    }

    /// @brief Determine whether it's time to export
    bool Exporter::timeToExport()
    {
        if (TimeManager::getCurrent()->getTime() >= _nextExportTime)
        {
            _nextExportTime += _exportPeriod;
            return true;
        }
        else
            return false;
    }

    /// @brief load configuration from scene
    void Exporter::init(SceneLoader::ExporterConfig config)
    {
        _exportPeriod = config.period;
        _parameters = config.parameters;
        _outPrecision = config.outPrecision;

        if (_exportPeriod < 0)
        {
            LOG_ERR << "Negative export period for Exporter " << ExporterTypeName[_type];
            exit(1);
        }
        if (_parameters.size() == 0)
        {
            LOG_WARN << "Empty parameter set for Exporter " << ExporterTypeName[_type];
        }
        for (string param : _parameters)
        {
            if (param != "temperature" && param != "coolingRate" && param != "tempGrad" && param != "laserPower" && param != "enthalpy")
                LOG_WARN << "Unidentified output parameter [" << param << "]";
        }
    }
}