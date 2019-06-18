add_library(physics)

add_subdirectory(systems)
add_subdirectory(components)

target_include_directories(physics 
PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE 
  ${GLM_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/pilcrow/engine/core
  ${PROJECT_SOURCE_DIR}/pilcrow/dependencies
)

target_link_libraries(physics slot_map)
target_link_libraries(physics detection_idiom)

#The filter the library will be in
set_target_properties(physics PROPERTIES FOLDER pilcrow/modules)
