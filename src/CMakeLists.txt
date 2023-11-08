add_library(lftp
    Common.hpp
    Common.cpp
    SceneLoader.hpp
    SceneLoader.cpp
    Simulator.hpp
    Simulator.cpp
    TimeManager.hpp
    TimeManager.cpp

    ThermalBoundary/ThermalBoundary.hpp
    ThermalBoundary/ThermalBoundary.cpp
    ThermalBoundary/BoundaryManager.hpp
    ThermalBoundary/BoundaryManager.cpp
)

add_dependencies(lftp
	utilities
)

target_link_libraries(lftp
	PUBLIC utilities
)

target_include_directories(lftp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)