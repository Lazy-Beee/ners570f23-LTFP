add_library(lftp
    Common.hpp
    Common.cpp
    SceneLoader.hpp
    SceneLoader.cpp
    Simulator.hpp
    Simulator.cpp
    ThermalBoundary.hpp
    ThermalBoundary.cpp
    TimeManager.hpp
    TimeManager.cpp
)
add_dependencies(lftp
	utilities
)

target_link_libraries(lftp
	PUBLIC utilities
)