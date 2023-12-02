#ifndef __MeshData__
#define __MeshData__

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "Common.hpp"

namespace LTFP
{	
	/// @brief Class managing mesh and data.
	/// @note This class is a singleton.
	class MeshData
	{
	private:
		static MeshData *current;
		bool _incrementActive;

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
		MeshReal _coolingRate;
		MeshVector _temperatureGrad;
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
		void copyTempToOld();

		inline const Vector3r &getCenterPos(const int &i, const int &j, const int &k) const { return _centerPos[i][j][k]; };
		inline const Real &getTemperature(const int &i, const int &j, const int &k) const { return _temperature[i][j][k]; };
		inline void setTemperature(const int &i, const int &j, const int &k, const Real &value) { _temperature[i][j][k] = value; };
		inline const Real &getTemperatureOld(const int &i, const int &j, const int &k) const { return _temperatureOld[i][j][k]; };
		inline void setTemperatureOld(const int &i, const int &j, const int &k, const Real &value) { _temperatureOld[i][j][k] = value; };
		inline const size_t &getSizeX() const { return _xSize; };
		inline const size_t &getSizeY() const { return _ySize; };
		inline const size_t &getSizeZ() const { return _zSize; };
		inline const Real &getIntervalX() const { return _xInterval; };
		inline const Real &getIntervalY() const { return _yInterval; };
		inline const Real &getIntervalZ() const { return _zInterval; };
		inline const Real &getCoolingRate(const int &i, const int &j, const int &k) const { return _coolingRate[i][j][k]; };
		inline void setCoolingRate(const int &i, const int &j, const int &k, const Real &value) { _coolingRate[i][j][k] = value; };
		inline const Vector3r &getTemperatureGrad(const int &i, const int &j, const int &k) const { return _temperatureGrad[i][j][k]; };
		inline void setTemperatureGrad(const int &i, const int &j, const int &k, const Vector3r &value) { _temperatureGrad[i][j][k] = value; };

	};
}

#endif
