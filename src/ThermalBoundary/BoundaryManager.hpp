#ifndef __BoundaryManager__
#define __BoundaryManager__

#include <vector>
#include "src/Common.hpp"
#include "ThermalBoundary.hpp"
#include "ThermalBoundaryDirichlet.hpp"

namespace LTFP
{
	/// @brief Class managing thermal boundary objects.
	/// @note This class is a singleton.
	class BoundaryManager
	{
	private:
		static BoundaryManager *current;
		/// @brief Boundary objects
		/// @note First-level: location x+ / x- / y+ / y- / z+ / z-
		std::vector<std::vector<ThermalBoundary *>> _boundaries;

	public:
		BoundaryManager();
		BoundaryManager(const BoundaryManager &) = delete;
		BoundaryManager &operator=(const BoundaryManager &) = delete;
		~BoundaryManager();

		static BoundaryManager *getCurrent();
		void init();

#ifndef NDEBUG
		/// @warning for debug only
		ThermalBoundary *getThermalBoundary(BoundaryLocation location, size_t index) const { return _boundaries[static_cast<size_t>(location)][index]; };
#endif
	};
}

#endif
