set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_test(test_sceneLoader.cpp)
add_test(test_utility.cpp)
add_test(test_timeManager.cpp)
add_test(test_common.cpp)
add_test(test_matProp.cpp)
add_test(test_boundary.cpp)
add_test(test_laser.cpp)