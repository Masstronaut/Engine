set(phy_includes
  "include/Mathematics.h"
)

set(phy_srcs
  "src/Mathematics.cpp"
)

foreach(phy_include IN ITEMS ${phy_includes})
  source_group("include" FILES "${phy_include}" )
endforeach()

foreach(phy_src IN ITEMS ${phy_srcs})
  source_group("src" FILES "${phy_src}" )
endforeach()

set(phy_components_includes
  "include/components/Body.h"
  "include/components/Manifold.h"
)

set(phy_components_srcs
  "src/components/Body.cpp"
  "src/components/Manifold.cpp"
)

foreach(phy_components_include IN ITEMS ${phy_components_includes})
  source_group("include\\components" FILES "${phy_components_include}" )
endforeach()

foreach(phy_components_src IN ITEMS ${phy_components_srcs})
  source_group("src\\components" FILES "${phy_components_src}" )
endforeach()


set(phy_systems_includes
  "include/systems/CollisionDetection.h"
  "include/systems/Integration.h"
  "include/systems/PhysicsSystems.h"
  "include/systems/Resolution.h"
)

set(phy_systems_srcs
  "src/systems/CollisionDetection.cpp"
  "src/systems/Integration.cpp"
  "src/systems/Resolution.cpp"
)

foreach(phy_systems_include IN ITEMS ${phy_systems_includes})
  source_group("include\\systems" FILES "${phy_systems_include}" )
endforeach()

foreach(phy_systems_src IN ITEMS ${phy_systems_srcs})
  source_group("src\\systems" FILES "${phy_systems_src}" )
endforeach()

include_directories("${GLM_INCLUDE_DIRS}")
include_directories("${PROJECT_SOURCE_DIR}/pilcrow/engine/core/include")
include_directories("${PROJECT_SOURCE_DIR}/pilcrow/dependencies")

add_library(physics 
  ${phy_includes} 
  ${phy_srcs}
  ${phy_components_includes}
  ${phy_components_srcs}
  ${phy_systems_includes}
  ${phy_systems_srcs}
)

target_link_libraries(physics slot_map)
target_link_libraries(physics detection_idiom)

#The filter the library will be in
SET_TARGET_PROPERTIES (physics PROPERTIES
    FOLDER libraries)
