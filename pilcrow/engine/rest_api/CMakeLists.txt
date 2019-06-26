add_library(rest_api)

target_sources(rest_api 
PRIVATE
  RESTAPI.cpp
  RESTAPI.h
)

target_include_directories(rest_api 
PUBLIC
  ${PROJECT_SOURCE_DIR}
PRIVATE
  ${CPPREST_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/pilcrow/dependencies
)

target_link_libraries(rest_api 
PUBLIC
  engine
PRIVATE
  slot_map
  detection_idiom
)

pilcrow_source_group(CMAKE_CURRENT_LIST_DIR rest_api)

#The filter the library will be in
set_target_properties(rest_api PROPERTIES FOLDER pilcrow/engine)
