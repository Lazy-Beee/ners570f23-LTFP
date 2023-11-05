cmake_minimum_required(VERSION 3.20)

project(LTFP VERSION 1.0.0 LANGUAGES CXX)

# Set module path to find our macros
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Check that the compiler supports the C++17 standard
# We assume you are using GNU compilers
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "8.0.0")
    message(FATAL_ERROR "GCC version must be at least 8.0.0")
  endif()
else()
  message(WARNING "You are using an unsupported compiler! This might not work.")
endif()

find_package(Eigen3 3.4.0 REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(USE_DOUBLE_PRECISION "OFF")
if(USE_DOUBLE_PRECISION STREQUAL "ON")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_DOUBLE")
endif()

# Enable a long list of warnings to ensure correctness
include(gnu_warning_flags)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GNU_WARNING_FLAGS}")

include(add_simulator)
include(add_test)
add_subdirectory(src)
add_subdirectory(utilities)
add_subdirectory(simulator)
add_subdirectory(tests)
