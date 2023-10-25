add_executable(lftp_main main.cpp)

target_include_directories(lftp_main PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(lftp_main lftp)