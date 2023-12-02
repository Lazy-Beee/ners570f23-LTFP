#ifndef __SceneLoader__
#define __SceneLoader__

#include "Common.hpp"
#include <vector>
#include <string>
#include "utilities/json.hpp"

using json = nlohmann::json;

namespace LTFP
{
	/// @brief Class reads the scene file and store the configurations.
	/// This module does not perform any check on the configuration, default values
	/// will be used for missing terms. For meaning of Config struct parameters,
	/// refer to `scenes/readme.md`.
	/// @note This class is a singleton.
	class SceneLoader
	{
	public:
		/// @brief Time advancement configurations
		struct TimeConfig
		{
			Real minTimeStepSize = REAL_MIN;
			Real maxTimeStepSize = REAL_MAX;
			Real endTime = -1.0;
			int maxTimeSteps = -1;
		};

		/// @brief Mesh configurations
		struct MeshConfig
		{
			Vector3r start = {0, 0, 0};
			Vector3r end = {1, 1, 1};
			int xCount = -1;
			int yCount = -1;
			int zCount = -1;
			Real meshSize = -1.0;
			Real initialTemp = -1.0;
			std::string layerFile = "";
		};

		/// @brief Exporter configurations
		struct ExporterConfig
		{
			int type = -1;
			Real period = -1.0f;
			std::vector<std::string> parameters = {};
			int outPrecision = 6;
		};

		/// @brief Exportation configurations
		struct ExportConfig
		{
			int consolePeriod = INT_MAX;
			std::vector<ExporterConfig> exporters = {};
		};

		/// @brief Base thermal boundary configurations
		struct BoundaryConfig
		{
			int index = -1;
			int type = -1;
			int location = -1;
		};

		/// @brief Neumann thermal boundary configurations
		struct BoundaryConfigNeumann : public BoundaryConfig
		{
			std::vector<Real> xFluxPoly = {};
			std::vector<Real> yFluxPoly = {};
			std::vector<Real> zFluxPoly = {};
		};

		/// @brief Dirichlet thermal boundary configurations
		struct BoundaryConfigDirichlet : public BoundaryConfig
		{
			std::vector<Real> xTempPoly = {};
			std::vector<Real> yTempPoly = {};
			std::vector<Real> zTempPoly = {};
		};

		/// @brief Convection thermal boundary configurations
		struct BoundaryConfigConvection : public BoundaryConfig
		{
			Real ambientTemp = 300.0f;
			Real convectionCoeff = -1.0f;
		};

		/// @brief Radiation thermal boundary configurations
		struct BoundaryConfigRadiation : public BoundaryConfig
		{
			Real ambientTemp = -1.0f;
			Real emissivityCoeff = -1.0f;
		};

		/// @brief Material Property configurations
		struct MatPropConfig
		{
			int type = -1;
			bool tabulate = false;
			std::vector<Real> tempRange = {};
			std::vector<std::vector<Real>> polynomials = {};
			std::vector<Real> tabulateStep = {};
		};

		/// @brief Laser Property configurations
		struct LaserConfig
		{
			int index = -1;
			int type = -1;
			Real power = -1.0f;
			Real absorptivity = -1.0f;
			std::string laserPath = "";
			Real radius = -1.0f;
			Real depth = -1.0f;
		};

		/// @brief Base thermal solver configurations
		struct ThermalSolverConfig
		{
			int type = -1;
			Real cflNumber = 1.0;
		};

		template <typename T>
		static bool readValue(const json &jsonData, T &val)
		{
			if (jsonData.is_null())
				return false;

			val = jsonData.get<T>();
			return true;
		}

		template <typename T>
		static bool readVector(const json &jsonData, std::vector<T> &vec)
		{
			if (jsonData.is_null())
				return false;

			vec = jsonData.get<std::vector<T>>();
			return true;
		}

		template <typename T, int size>
		static bool readVector(const json &jsonData, Eigen::Matrix<T, size, 1, Eigen::DontAlign> &vec)
		{
			if (jsonData.is_null())
				return false;

			std::vector<T> values = jsonData.get<std::vector<T>>();
			for (unsigned int i = 0; i < values.size(); i++)
				vec[i] = values[i];
			return true;
		}

	private:
		static SceneLoader *current;
		std::filesystem::path _scenePath;
		json _jsonData;
		bool _fatalError; ///< Mark whether the scene file is missing essential info
		TimeConfig _timeConfig;
		MeshConfig _meshConfig;
		ExportConfig _exportConfig;
		std::vector<MatPropConfig> _matPropConfig;
		std::vector<BoundaryConfig *> _boundaryConfig;
		std::vector<LaserConfig> _laserConfig;
		ThermalSolverConfig _thermalSolverConfig;

		void readTimeConfig();
		void readMeshConfig();
		void readExportConfig();
		void readMatProp();
		void readBoundary();
		void readLaserConfig();
		void readThermalSolverConfig();

	public:
		SceneLoader();
		SceneLoader(const SceneLoader &) = delete;
		SceneLoader &operator=(const SceneLoader &) = delete;
		~SceneLoader();

		static SceneLoader *getCurrent();
		void readScene(std::filesystem::path scenePath);

		inline TimeConfig getTimeConfig() const { return _timeConfig; };
		inline MeshConfig getMeshConfig() const { return _meshConfig; };
		inline ExportConfig getExportConfig() const { return _exportConfig; };
		inline std::vector<MatPropConfig> getMatPropConfig() const { return _matPropConfig; };
		inline std::vector<BoundaryConfig *> getBoundaryConfig() const { return _boundaryConfig; };
		inline std::vector<LaserConfig> getLaserConfig() const { return _laserConfig; };
		inline ThermalSolverConfig getThermalSolverConfig() const { return _thermalSolverConfig; };
		inline std::filesystem::path getScenePath() const { return _scenePath; };
	};
}

#endif
