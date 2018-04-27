set(restAPI_include
  "include/RESTAPI.h"
)

set(restAPI_src
  "src/RESTAPI.cpp"
)

foreach(restAPI_include_file IN ITEMS ${restAPI_include})
  source_group("include" FILES "${restAPI_include_file}" )
endforeach()

foreach(restAPI_src_file IN ITEMS ${restAPI_src})
  source_group("src" FILES "${restAPI_src_file}" )
endforeach()

include_directories(${CPPREST_INCLUDE_DIRS})
include_directories("${PROJECT_SOURCE_DIR}/pilcrow/engine/core/include")
include_directories("${PROJECT_SOURCE_DIR}/pilcrow/dependencies")

add_library(rest_api ${restAPI_include} ${restAPI_src})

target_link_libraries(rest_api slot_map)
target_link_libraries(rest_api detection_idiom)

#The filter the library will be in
SET_TARGET_PROPERTIES (rest_api PROPERTIES
    FOLDER libraries)