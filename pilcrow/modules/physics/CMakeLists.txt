add_library(physics)

add_subdirectory(systems)
add_subdirectory(components)

target_include_directories(physics 
PUBLIC
  ${PROJECT_SOURCE_DIR}
PRIVATE 
  ${GLM_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/pilcrow/dependencies
)

target_link_libraries(physics 
PUBLIC
  engine
  slot_map 
  detection_idiom
PRIVATE 
)

pilcrow_source_group(CMAKE_CURRENT_LIST_DIR physics)

#The filter the library will be in
set_target_properties(physics PROPERTIES FOLDER pilcrow/modules)
