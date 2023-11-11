cmake_minimum_required(VERSION 3.20)

project(LTFP VERSION 1.0.0 LANGUAGES CXX)

# Set module path to find our macros
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Accommodate the greatlakes environment
option(GREATLAKES_ENVIRONMENT "OFF") 
if(GREATLAKES_ENVIRONMENT STREQUAL "ON")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGREATLAKES")
endif()

# Check that the compiler supports the C++17 standard
# We assume you are using GNU compilers
if(GREATLAKES_ENVIRONMENT STREQUAL "OFF")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "11.0.0")
      message(FATAL_ERROR "GCC version must be at least 11.0.0")
    endif()
  else()
    message(WARNING "You are using an unsupported compiler! This might not work.")
  endif()
endif()

if(GREATLAKES_ENVIRONMENT STREQUAL "OFF")
  find_package(Eigen3 3.4.0 REQUIRED)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

option(USE_DOUBLE_PRECISION "OFF")
if(USE_DOUBLE_PRECISION STREQUAL "ON")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_DOUBLE")
endif()

# Enable a long list of warnings to ensure correctness
if(GREATLAKES_ENVIRONMENT STREQUAL "OFF")
  include(gnu_warning_flags)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GNU_WARNING_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++")

include(add_simulator)
include(add_test)
add_subdirectory(src)
add_subdirectory(utilities)
add_subdirectory(simulator)
add_subdirectory(tests)
