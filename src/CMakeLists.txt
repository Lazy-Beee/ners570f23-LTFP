add_library(lftp
    Common.hpp
    
    Simulator.hpp
    Simulator.cpp
    TimeManager.hpp
    TimeManager.cpp
)
add_dependencies(lftp
	utilities
)

target_link_libraries(lftp
	PUBLIC utilities
)