#ifndef __SceneLoader__
#define __SceneLoader__

#include "Common.hpp"
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
			Real minTimeStepSize;		///< Minimum time step size
			Real maxTimeStepSize;		///< Maximum time step size
			Real endTime;				///< Simulation end time
			int maxTimeSteps;			///< Maximum number of time steps
		};

		/// @brief Mesh configurations
		struct MeshConfig
		{
			AlignedBox3r domain;		///< Domain of mesh
			int xCount;					///< Number of mesh in X
			int yCount;					///< Number of mesh in Y
			int zCount;					///< Number of mesh in Z
			Real incrementThickness;	///< Thickness of each domain increment
			Real incrementPeriod;		///< Period between domain increment
		};

		/// @brief Exportation configurations
		struct ExportConfig
		{
			bool enableVtkExport;		///< Export mesh and data to vtk file
			int consolePeriod;			///< Period between printing simulation status to console
			Real exportPeriod;			///< Period between exporting to file
		};

	private:
        static SceneLoader* current;
		json _jsonData;
		bool _fatalError;
		TimeConfig _timeConfig;
		MeshConfig _meshConfig;
		ExportConfig _exportConfig;

		void readTimeConfig();

		template <typename T>
		bool readValue(const json &j, T &v)
		{
			if (j.is_null())
				return false;

			v = j.get<T>();
			return true;
		}

		template <typename T>
		bool readVector(const json &j, std::vector<T> &vec)
		{
			if (j.is_null())
				return false;

			vec = j.get<std::vector<T>>();
			return true;
		}	

	public:
		SceneLoader();
		SceneLoader(const SceneLoader&) = delete;
		SceneLoader& operator=(const SceneLoader&) = delete;
		~SceneLoader();

		static SceneLoader* getCurrent();
        void readScene();

		TimeConfig getTimeConfig() const { return _timeConfig; };
	};
}

#endif
