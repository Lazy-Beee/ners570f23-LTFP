add_library(utilities
    Common.h
    Counting.h
    Logger.h
    Timing.h
    StringTools.h
)

set_target_properties(utilities PROPERTIES LINKER_LANGUAGE CXX)