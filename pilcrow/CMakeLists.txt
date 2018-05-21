add_subdirectory(dependencies)
add_subdirectory(engine)
add_subdirectory(runtimes)
add_subdirectory(modules)
add_subdirectory(tests)



file(GLOB_RECURSE files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
set(fmtfiles "")
foreach(formattedfile ${files})
  get_filename_component(filepath ${formattedfile} DIRECTORY)
  get_filename_component(filename ${formattedfile} NAME)
  if(NOT ${filepath} MATCHES "^dependencies")
    if(NOT ${filename} MATCHES "CMakeLists.txt")
      list(APPEND fmtfiles ${formattedfile})
    endif()
  endif()
endforeach()
