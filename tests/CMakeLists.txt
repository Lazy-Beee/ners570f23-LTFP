set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

add_test(test_example.cpp)
add_test(test_loadScene.cpp)
add_test(test_utility.cpp)
add_test(test_timeManager.cpp)