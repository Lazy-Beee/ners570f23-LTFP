#ifndef __BoundaryManager__
#define __BoundaryManager__

#include "src/Common.hpp"

namespace LTFP
{
	/// @brief Class managing thermal boundary objects.
	/// @note This class is a singleton.
	class BoundaryManager
	{
	private:
		static BoundaryManager *current;

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
