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
		std::vector<std::vector<ThermalBoundary>> _boundaries;

	public:
		BoundaryManager();
		BoundaryManager(const BoundaryManager &) = delete;
		BoundaryManager &operator=(const BoundaryManager &) = delete;
		~BoundaryManager();

		static BoundaryManager *getCurrent();
		void init();
		
	};
}

#endif
