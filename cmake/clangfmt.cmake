
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


find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  add_custom_target(
    Format-Code
    COMMENT "Running clang-format to format source files"
    COMMAND ${CLANG_FORMAT}
    #-help
    -i
    -style=file
    ${fmtfiles}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()