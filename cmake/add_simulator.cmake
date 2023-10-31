macro(add_simulator FILENAME)

  set(EXECNAME ${FILENAME})
  get_filename_component(EXECNAME ${EXECNAME} NAME_WE)
  get_filename_component(EXECNAME ${EXECNAME} NAME_WLE)
  set(EXECNAME ${EXECNAME}.exe)

  add_executable(${EXECNAME} ${FILENAME})
  
  target_include_directories(${EXECNAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src>
    $<INSTALL_INTERFACE:include>
  )

  target_link_libraries(${EXECNAME} lftp)
endmacro()