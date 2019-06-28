
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

function (ListToString result delim)
    list(GET ARGV 2 temp)
    math(EXPR N "${ARGC}-1")
    foreach(IDX RANGE 3 ${N})
        list(GET ARGV ${IDX} STR)
        set(temp "${temp}${delim}${STR}")
    endforeach()
    set(${result} "${temp}" PARENT_SCOPE)
endfunction(ListToString)



function(pilcrow_source_group aRoot aTarget) 
  get_target_property(targetBinaryDir ${aTarget} BINARY_DIR)
  get_target_property(targetSourceDir ${aTarget} SOURCE_DIR)
  get_target_property(targetSources ${aTarget} SOURCES)
  get_target_property(targetIncludeDirectories ${aTarget} INCLUDE_DIRECTORIES)
  

  list(FILTER targetSources INCLUDE REGEX "^.*\.(c(pp)?)$")

  ListToString(targetSourcesList " " ${targetSources})
  ListToString(targetIncludeDirectoriesList " " ${targetIncludeDirectories})

  message(Sources: "--sources ${targetSourcesList}")
  
  message(Includes: "--includes ${targetIncludeDirectoriesList}")
  

endfunction() 