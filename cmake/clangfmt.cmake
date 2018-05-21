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
