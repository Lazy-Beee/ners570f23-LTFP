#ifndef __ThermalBoundaryDirichlet__
#define __ThermalBoundaryDirichlet__

#include <vector>
#include "src/Common.hpp"
#include "ThermalBoundary.hpp"

using ConfigDirichlet = LTFP::SceneLoader::BoundaryConfigDirichlet;

namespace LTFP
{
    /// @brief Class of Dirichlet thermal boundary.
    /// 
    class ThermalBoundaryDirichlet : public ThermalBoundary
    {
    private:
        std::vector<std::vector<Real>> _tempPolyCoeff;

    public:
        ThermalBoundaryDirichlet(ConfigDirichlet *config);
        ~ThermalBoundaryDirichlet() {};
        
        Real getTemp(const Vector3r &pos, const Real &temp);
    };
}

#endif
