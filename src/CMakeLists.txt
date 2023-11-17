add_library(ltfp
  Common.hpp
  Common.cpp
  MaterialProperty.hpp
  MaterialProperty.cpp
  SceneLoader.hpp
  SceneLoader.cpp
  Simulator.hpp
  Simulator.cpp
  TimeManager.hpp
  TimeManager.cpp
  LaserSource.hpp
  LaserSource.cpp

  ThermalBoundary/BoundaryManager.hpp
  ThermalBoundary/BoundaryManager.cpp
  ThermalBoundary/ThermalBoundary.hpp
  ThermalBoundary/ThermalBoundary.cpp
  ThermalBoundary/ThermalBoundaryDirichlet.hpp
  ThermalBoundary/ThermalBoundaryDirichlet.cpp
  ThermalBoundary/ThermalBoundaryNeumann.hpp
  ThermalBoundary/ThermalBoundaryNeumann.cpp
  ThermalBoundary/ThermalBoundaryConvection.hpp
  ThermalBoundary/ThermalBoundaryConvection.cpp
  ThermalBoundary/ThermalBoundaryRadiation.hpp
  ThermalBoundary/ThermalBoundaryRadiation.cpp
  ThermalBoundary/ThermalBoundaryMirror.hpp
  ThermalBoundary/ThermalBoundaryMirror.cpp

  Exporter/ExportManager.hpp
  Exporter/ExportManager.cpp
  Exporter/Exporter.cpp
  Exporter/Exporter.cpp
  Exporter/ExporterVtkMesh.cpp
  Exporter/ExporterVtkMesh.cpp
  Exporter/ExporterCsvMesh.cpp
  Exporter/ExporterCsvMesh.cpp
)

add_dependencies(ltfp
	utilities
)

target_link_libraries(ltfp
	PUBLIC utilities
)

# include(CMakePrintHelpers)
# cmake_print_variables(CMAKE_SOURCE_DIR)

target_include_directories(ltfp PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)