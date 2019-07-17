
add_library(engine)

add_subdirectory(component)
add_subdirectory(components)
add_subdirectory(entity)
add_subdirectory(settings)
add_subdirectory(system)
add_subdirectory(systems)

target_sources(engine
PRIVATE
    Detectors.hpp
    RenderComponents.h
    RenderSystem.cpp
    RenderSystem.h
    SettingsFileReader.hpp
    Simulation.cpp
    Simulation.hpp
    World.cpp
    World.hpp
    World.inl
    WorldEvents.hpp
)

target_include_directories(engine 
PUBLIC
  ${PROJECT_SOURCE_DIR}
PRIVATE
  ${PROJECT_SOURCE_DIR}/pilcrow/dependencies
)

target_compile_features(engine PUBLIC cxx_std_17)

#Header only includes
target_link_libraries(engine 
PUBLIC
  glm
  detection_idiom
  slot_map
  glfw
  freetype
  Utils
PRIVATE 
)

pilcrow_source_group(CMAKE_CURRENT_LIST_DIR engine)

#The filter the library will be in
set_target_properties(engine PROPERTIES FOLDER pilcrow/engine)
