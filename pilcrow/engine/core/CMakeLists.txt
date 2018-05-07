
#The files in the component folder to compile
set(Component_include_files
  "include/component/ComponentAggregate.hpp"
  "include/component/ComponentPool.hpp"
  "include/component/ComponentTraits.hpp"
)

set(Component_files
  "src/component/ComponentAggregate.cpp"
  "src/component/ComponentAggregate.inl"
  "src/component/ComponentPool.inl"
)

#Adding the correct filters to the component files
foreach(Comp_include_file IN ITEMS ${Component_include_files})
  source_group("include\\component" FILES "${Comp_include_file}" )
endforeach()

#Adding the correct filters to the component files
foreach(Comp_file IN ITEMS ${Component_files})
  source_group("src\\component" FILES "${Comp_file}" )
endforeach()

#The files in the components folder to compile
set(Components_include_files
  "include/components/Transform.h"
  "include/components/WeaponComponents.h"
)

set(Components_src_files
  ""
)

foreach(Comps_include_file IN ITEMS ${Components_include_files})
  source_group("include\\components" FILES "${Comps_include_file}" )
endforeach()

foreach(Comps_src_file IN ITEMS ${Components_src_files})
  source_group("src\\components" FILES "${Comps_src_file}" )
endforeach()

#The files in the Entity folder to compile
set(Entity_include_files
  "include/entity/ArchetypeRef.hpp"
  "include/entity/ConstrainedEntityRef.hpp"
  "include/entity/EntitiesWith.hpp"
  "include/entity/EntityRef.hpp"
  "include/entity/EntityID.hpp"
  "include/entity/Entity.hpp"
)

set(Entity_files
  "src/entity/ArchetypeRef.cpp"
  "src/entity/ArchetypeRef.inl"
  "src/entity/ConstrainedEntityRef.inl"
  "src/entity/EntitiesWith.inl"
  "src/entity/Entity.cpp"
  "src/entity/Entity.inl"
  "src/entity/EntityRef.inl"
)

foreach(Entity_include_file IN ITEMS ${Entity_include_files})
  source_group("include\\entity" FILES "${Entity_include_file}" )
endforeach()

foreach(Entity_file IN ITEMS ${Entity_files})
  source_group("src\\entity" FILES "${Entity_file}" )
endforeach()

#The files in the settings folder to compile
set(Settings_include_files
  "include/settings/WindowSettings.h"
)

set(Settings_files
  ""
)

foreach(Settings_include_file IN ITEMS ${Settings_include_files})
  source_group("include\\settings" FILES "${Settings_include_file}" )
endforeach()

foreach(Settings_file IN ITEMS ${Settings_files})
  source_group("src\\settings" FILES "${Settings_file}" )
endforeach()

#The files in the system folder to compile
set(System_include_files
  "include/system/System.hpp"
  "include/system/SystemTraits.hpp"
)

set(System_files
  "src/system/System.inl"
)

foreach(System_include_file IN ITEMS ${System_include_files})
  source_group("include\\system" FILES "${System_include_file}" )
endforeach()

foreach(System_file IN ITEMS ${System_files})
  source_group("src\\system" FILES "${System_file}" )
endforeach()

#The files in the systems folder to compile
set(Systems_include_files
  "include/systems/WeaponSystems.h"
)

set(Systems_files
  ""
)

foreach(Systems_include_file IN ITEMS ${Systems_include_files})
  source_group("include\\systems" FILES "${Systems_file}" )
endforeach()

foreach(Systems_file IN ITEMS ${Systems_files})
  source_group("src\systems" FILES "${Systems_file}" )
endforeach()

#The files in the src directory to compile
set(include_files
  "include/Camera.hpp"
  "include/Detectors.hpp"
  "include/RenderComponents.h"
  "include/RenderSystem.h"
  "include/SettingsFileReader.hpp"
  "include/Simulation.hpp"
  "include/World.hpp"
  "include/WorldEvents.hpp"
)

set(src_files
  "src/EntityRef.cpp"
  "src/RenderSystem.cpp"
  "src/Simulation.cpp"
  "src/World.cpp"
  "src/World.inl"
)

foreach(include_file IN ITEMS ${include_files})
  source_group("include" FILES "${include_file}")
endforeach()

foreach(src_file IN ITEMS ${src_files})
  source_group("src" FILES "${src_file}")
endforeach()


include_directories(${PROJECT_SOURCE_DIR}/pilcrow/dependencies)
include_directories(${PROJECT_SOURCE_DIR}/pilcrow/engine/rest_api/include)
include_directories(${PROJECT_SOURCE_DIR}/pilcrow/modules/jellyfish_renderer/include)
include_directories(${CPPREST_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})

add_library(engine 
  ${src_files}
  ${include_files} 
  ${System_include_files}
  ${Systems_include_files}
  ${Systems_files} 
  ${Systems_include_files}  
  ${Component_include_files}
  ${Component_files}
  ${Components_include_files}
  ${Components_files}
  ${Entity_include_files}
  ${Entity_files}
  ${Settings_include_files}
  ${Settings_files} 
)

#Header only includes
target_link_libraries(engine slot_map)
target_link_libraries(engine detection_idiom)
target_link_libraries(engine glm)

#The filter the library will be in
SET_TARGET_PROPERTIES (engine PROPERTIES
    FOLDER libraries)