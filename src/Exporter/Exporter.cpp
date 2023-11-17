#include "Exporter.hpp"
#include "utilities/Logger.hpp"

namespace LTFP
{
    Exporter::Exporter(int type) : _type(static_cast<ExporterType>(type))
    {
        LOG_DEBUG << "Exporter " << ExporterTypeName[type] << " is created";
        _exportPeriod = REAL_MAX;
        _exportCount = 0;
        _parameters = {};
    }

    /// @brief load configuration from scene
    void Exporter::init(SceneLoader::ExporterConfig config)
    {
        _exportPeriod = config.period;
        _parameters = config.parameters;

        if (_exportPeriod < 0)
        {
            LOG_ERR << "Negative export period for Exporter " << ExporterTypeName[_type];
            exit(1);
        }
        if (_parameters.size() == 0)
        {
            LOG_WARN << "Empty parameter set for Exporter " << ExporterTypeName[_type];
        }
    }
}