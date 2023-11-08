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

    ThermalBoundary/Boundary.hpp
    ThermalBoundary/BoundaryManager.hpp
    ThermalBoundary/BoundaryManager.cpp
    ThermalBoundary/ThermalBoundary.hpp
    ThermalBoundary/ThermalBoundary.cpp
    ThermalBoundary/ThermalBoundaryDirichlet.hpp
    ThermalBoundary/ThermalBoundaryDirichlet.cpp
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