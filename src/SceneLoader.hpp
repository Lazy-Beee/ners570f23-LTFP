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
			Real incrementThickness = -1.0;
			Real incrementPeriod = REAL_MAX;
		};

		/// @brief Exportation configurations
		struct ExportConfig
		{
			bool enableVtkExport = false;
			int printPeriod = INT_MAX;
			Real exportPeriod = REAL_MAX;
		};

		/// @brief Base thermal boundary configurations
		struct BoundaryConfig
		{
			int index = -1;
			int type = -1;
			int location = -1;
		};

		/// @brief Dirichlet thermal boundary configurations
		struct BoundaryConfigDirichlet : public BoundaryConfig
		{
			std::vector<Real> xTempPoly = {};
			std::vector<Real> yTempPoly = {};
			std::vector<Real> zTempPoly = {};
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

	private:
		static SceneLoader *current;
		json _jsonData;
		bool _fatalError; ///< Mark whether the scene file is missing essential info
		TimeConfig _timeConfig;
		MeshConfig _meshConfig;
		ExportConfig _exportConfig;
		std::vector<MatPropConfig> _matPropConfig;
		std::vector<BoundaryConfig *> _boundaryConfig;

		void readTimeConfig();
		void readMeshConfig();
		void readExportConfig();
		void readMatProp();
		void readBoundary();

		template <typename T>
		bool readValue(const json &jsonData, T &val)
		{
			if (jsonData.is_null())
				return false;

			val = jsonData.get<T>();
			return true;
		}

		template <typename T>
		bool readVector(const json &jsonData, std::vector<T> &vec)
		{
			if (jsonData.is_null())
				return false;

			vec = jsonData.get<std::vector<T>>();
			return true;
		}

		template <typename T, int size>
		bool readVector(const json &jsonData, Eigen::Matrix<T, size, 1, Eigen::DontAlign> &vec)
		{
			if (jsonData.is_null())
				return false;

			std::vector<T> values = jsonData.get<std::vector<T>>();
			for (unsigned int i = 0; i < values.size(); i++)
				vec[i] = values[i];
			return true;
		}

	public:
		SceneLoader();
		SceneLoader(const SceneLoader &) = delete;
		SceneLoader &operator=(const SceneLoader &) = delete;
		~SceneLoader();

		static SceneLoader *getCurrent();
		void readScene();
		void readScene(std::filesystem::path scenePath);

		inline TimeConfig getTimeConfig() const { return _timeConfig; };
		inline MeshConfig getMeshConfig() const { return _meshConfig; };
		inline ExportConfig getExportConfig() const { return _exportConfig; };
		inline std::vector<MatPropConfig> getMatPropConfig() const { return _matPropConfig; };
		inline std::vector<BoundaryConfig *> getBoundaryConfig() const { return _boundaryConfig; };
	};
}

#endif
