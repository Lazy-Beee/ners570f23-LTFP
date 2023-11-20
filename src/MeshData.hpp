#ifndef __MeshData__
#define __MeshData__

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "Common.hpp"

namespace LTFP
{	

	using MeshReal = std::vector<std::vector<std::vector<Real>>>;
	using MeshVector = std::vector<std::vector<std::vector<Vector3r>>>;

	/// @brief Class managing mesh and data.
	/// @note This class is a singleton.
	class MeshData
	{
	private:
		static MeshData *current;
		size_t _xSize;
		size_t _ySize;
		size_t _zSize;
		Real _xInterval;
		Real _yInterval;
		Real _zInterval;
		Vector3r _domainStart;
		Vector3r _domainEnd;
		MeshVector _centerPos;
		MeshReal _temperature;
		MeshReal _temperatureOld;
		std::vector<Real> _incrementTime;
		std::vector<Real> _incrementThickness;
		
		void readLayerFile(std::string file);

	public:
		MeshData();
		MeshData(const MeshData &) = delete;
		MeshData &operator=(const MeshData &) = delete;
		~MeshData();

		static MeshData *getCurrent();
		void init();
		void stepIncrement();

		inline const Vector3r &getCenterPos(const int &i, const int &j, const int &k) const { return _centerPos[i][j][k]; };
		inline void setTemp(const int &i, const int &j, const int &k, const Real &value) { _temperature[i][j][k] = value; };
		
	};
}

#endif
