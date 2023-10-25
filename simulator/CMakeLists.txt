add_executable(simualtor.exe main.cpp)

target_include_directories(simualtor.exe PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(simualtor.exe lftp)