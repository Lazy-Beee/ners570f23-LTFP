add_library(lftp
    exampleSingleton.hpp
    exampleSingleton.cpp
)
add_dependencies(lftp
	utilities
)

target_link_libraries(lftp
	PUBLIC utilities
)