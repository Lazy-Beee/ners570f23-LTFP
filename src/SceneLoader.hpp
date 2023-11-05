#ifndef __SceneLoader__
#define __SceneLoader__

#include "Common.hpp"
#include <vector>
#include "../utilities/json.hpp"

using json = nlohmann::json;

namespace LTFP
{
	/// @brief Class reads the scene file and store the configurations.
	/// @note This class is a singleton.
	class SceneLoader
	{
	public:
		/// @brief Time advancement configurations
		struct TimeConfig
		{
			Real minTimeStepSize; ///< Minimum time step size
			Real maxTimeStepSize; ///< Maximum time step size
			Real endTime;		  ///< Simulation end time
			int maxTimeSteps;	  ///< Maximum number of time steps
		};

		/// @brief Mesh configurations
		struct MeshConfig
		{
			Vector3r start;			 ///< Minimum domain coordinate
			Vector3r end;			 ///< Maximum domain coordinate
			int xCount;				 ///< Number of mesh in X
			int yCount;				 ///< Number of mesh in Y
			int zCount;				 ///< Number of mesh in Z
			Real meshSize;			 ///< Size of cubic mesh
			Real incrementThickness; ///< Thickness of each domain increment
			Real incrementPeriod;	 ///< Period between domain increment
		};

		/// @brief Exportation configurations
		struct ExportConfig
		{
			bool enableVtkExport; ///< Export mesh and data to vtk file
			int consolePeriod;	  ///< Period between printing simulation status to console
			Real exportPeriod;	  ///< Period between exporting to file
		};

	private:
		static SceneLoader *current;
		json _jsonData;
		bool _fatalError; ///< Mark whether the scene file is missing essential info
		TimeConfig _timeConfig;
		MeshConfig _meshConfig;
		ExportConfig _exportConfig;

		void readTimeConfig();
		void readMeshConfig();
		void readExportConfig();

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
		void readScene(bool terminateOnError = true);

		TimeConfig getTimeConfig() const { return _timeConfig; };
	};
}

#endif
