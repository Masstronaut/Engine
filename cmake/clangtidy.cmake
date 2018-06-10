
file(GLOB_RECURSE files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
set(tidyfiles "")
foreach(file_to_tidy ${files})
  get_filename_component(filepath ${file_to_tidy} DIRECTORY)
  get_filename_component(filename ${file_to_tidy} NAME)
  if(NOT ${filepath} MATCHES "(^dependencies|include|\.inl)")
    if(NOT ${filename} MATCHES "CMakeLists.txt")
      list(APPEND tidyfiles ${file_to_tidy})
    endif()
  endif()
endforeach()


find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
  add_custom_target(
    Tidy-Code
    COMMENT "Running clang-tidy to detect errors in source files"
    COMMAND ${CLANG_TIDY}
    #-help
    -checks=*,-clang-analyzer-alpha.*,-llvm-include-order
    ${tidyfiles}
    -fix-errors
    -p ${CMAKE_CURRENT_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
