add_library(lftp
    Common.hpp
    
    exampleSingleton.hpp
    exampleSingleton.cpp
)
add_dependencies(lftp
	utilities
)

target_link_libraries(lftp
	PUBLIC utilities
)