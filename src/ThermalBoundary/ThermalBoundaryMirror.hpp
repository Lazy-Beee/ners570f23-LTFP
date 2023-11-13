#ifndef __ThermalBoundaryMirror__
#define __ThermalBoundaryMirror__

#include <vector>
#include "src/Common.hpp"
#include "ThermalBoundary.hpp"

using ConfigMirror = LTFP::SceneLoader::BoundaryConfig;

namespace LTFP
{
    /// @brief Class of Mirror thermal boundary.
    ///
    class ThermalBoundaryMirror : public ThermalBoundary
    {
    private:
    public:
        ThermalBoundaryMirror(ConfigMirror *config);
        ~ThermalBoundaryMirror(){};

        Real getTemp(const Vector3r &pos, const Real &temp);
    };
}

#endif
