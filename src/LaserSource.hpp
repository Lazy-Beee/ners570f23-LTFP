#ifndef __LaserSource__
#define __LaserSource__

#include "Common.hpp"
#include <string>
#include <vector>

namespace LTFP
{
	enum LaserType
	{
		GAUSSIAN_2D = 0,
		TOP_HEAD_2D = 1,
		GAUSSIAN_3D = 2
	};
	static std::vector<std::string> LaserTypeName = {
		"[2D Gaussian]",
		"[2D Top Head]",
		"[3D Gaussian]"};

	/// @brief Class managing laser heat sources.
	/// @note This class is a singleton.
	class LaserSource
	{
	public:
		struct LaserPath
		{
			Real t0;
			Real t1;
			Vector3r pos0;
			Vector3r pos1;
		};

		struct LaserData
		{
			// Static configs
			int index;
			LaserType type;
			Real power;
			Real absorptivity;
			std::vector<LaserPath> laserPath;
			Real radius;
			Real depth;

			// Static properties
			bool is2D;
			Real coeff;
			Real radiusSquared;

			// Runtime properties
			bool on = false;
			size_t currentPath = 0;
			Vector3r currentPos = {0, 0, 0};
		};

	private:
		static LaserSource *current;
		bool _laserActive;

		std::vector<LaserData> _lasers;
		MeshReal _laserPower;

		std::vector<LaserPath> readPathFile(std::string file);
		Real getPowerDensity(const LaserData &laser, const Vector3r &pos);

	public:
		LaserSource();
		LaserSource(const LaserSource &) = delete;
		LaserSource &operator=(const LaserSource &) = delete;
		~LaserSource();

		static LaserSource *getCurrent();
		void init();
		void precomputePowerDistribution();
		const bool &laserActive() const { return _laserActive; };
		Real getLaserPower(const unsigned int &i, const unsigned int &j, const unsigned int &k) const { return _laserPower[i][j][k]; };

#ifndef NDEBUG
		LaserData *getLaserData(int i) { return &(_lasers[i]); };
#endif
	};
}

#endif
