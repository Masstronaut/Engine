
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
    EntityRef.cpp
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
PRIVATE
  ${PROJECT_SOURCE_DIR}/pilcrow/dependencies
  ${PROJECT_SOURCE_DIR}/pilcrow/engine/rest_api/include
  ${PROJECT_SOURCE_DIR}/pilcrow/modules/jellyfish_renderer
  ${CPPREST_INCLUDE_DIRS}
  ${ASSIMP_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
)

#Header only includes
target_link_libraries(engine 
PUBLIC
  glm
PRIVATE 
  detection_idiom
  slot_map
)

#The filter the library will be in
set_target_properties(engine PROPERTIES FOLDER pilcrow/engine)
