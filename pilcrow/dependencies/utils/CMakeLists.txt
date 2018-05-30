set(utils_include_files
  "include/EventArena.hpp"
  "include/Resource.hpp"
  "include/ThreadPool.hpp"
  "include/type_list.hpp"
  "include/ResourceSettings.h"
)

set(utils_src_files
  "src/Resource.cpp"
)

foreach(utils_include_file IN ITEMS ${utils_include_files})
  source_group("include" FILES "${utils_include_file}" )
endforeach()

foreach(utils_src_file IN ITEMS ${utils_src_files})
  source_group("src" FILES "${utils_src_file}" )
endforeach()

add_library(utils
  ${utils_src_files}
  ${utils_include_files}
)

#The filter the library will be in
SET_TARGET_PROPERTIES (utils PROPERTIES
    FOLDER modules)