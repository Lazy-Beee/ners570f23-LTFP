#ifndef __BoundaryManager__
#define __BoundaryManager__

#include <vector>
#include "src/Common.hpp"
#include "ThermalBoundary.hpp"

namespace LTFP
{
	/// @brief Class managing thermal boundary objects.
	/// @note This class is a singleton.
	class BoundaryManager
	{
	private:
		static BoundaryManager *current;
		const size_t _numLoc = 6;
		/// @brief Boundary objects
		/// @note First-level: location x+ / x- / y+ / y- / z+ / z-
		std::vector<std::vector<ThermalBoundary *>> _boundaries;
		std::vector<bool> _tempBC;

	public:
		BoundaryManager();
		BoundaryManager(const BoundaryManager &) = delete;
		BoundaryManager &operator=(const BoundaryManager &) = delete;
		~BoundaryManager();

		static BoundaryManager *getCurrent();
		void init();

		Real getTempBC(BoundaryLocation loc, const Vector3r &pos, const Real &temp);
		Real getFluxBC(BoundaryLocation loc, const Vector3r &pos, const Real &temp);
		inline bool isTempBC(BoundaryLocation i) const { return _tempBC[i]; };

#ifndef NDEBUG
		/// @warning for debug only
		ThermalBoundary *getThermalBoundary(BoundaryLocation location, size_t index) const { return _boundaries[static_cast<size_t>(location)][index]; };
#endif
	};
}

#endif
