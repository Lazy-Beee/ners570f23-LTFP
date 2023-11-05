add_library(utilities
    Counting.hpp
    Logger.hpp
    Timing.hpp
    json.hpp
)

set_target_properties(utilities PROPERTIES LINKER_LANGUAGE CXX)