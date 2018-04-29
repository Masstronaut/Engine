#variable that contains all the sources of unit_test_runtime
set(unit_test_runtime_srcs
  "src/main.cpp"
)
#Specifying all the unit test runtime sources to be in a filter called "src"
source_group("src" FILES ${unit_test_runtime_srcs})

#Specify all core_test includes files
set(core_test_includes
  "include/core/AllCoreTests.h"
)
#Specify all core_test source files
set(core_test_srcs
  "src/core/simulation_class_tests.cpp"
)
#Set the filters for all the core test src and include files
foreach(core_test_include IN ITEMS ${core_test_includes})
  source_group("include\\core" FILES "${core_test_include}" )
endforeach()
foreach(core_test_src IN ITEMS ${core_test_srcs})
  source_group("src\\core" FILES "${core_test_src}" )
endforeach()

set(event_arena_test_includes
  "include/utils/AllEventArenaTests.h"
)
set(event_arena_test_srcs
  "src/utils/EventArenaLambdaTests.cpp"
  "src/utils/EventArenaCallOnceTests.cpp"
)
foreach(EA_Test IN ITEMS ${event_arena_test_includes})
  source_group("include\\utils" FILES ${EA_Test})
endforeach()

source_group("src\\utils" FILES ${event_arena_test_srcs})




#add google test include directory to unit_test_runtime project
include_directories(${GTEST_INCLUDE_DIRS})
#the include folder has a header that includes all the tests, making it easy to include them in main
include_directories(${PROJECT_SOURCE_DIR}/pilcrow/runtimes/unit_test_runtime/include)
include_directories(${PROJECT_SOURCE_DIR}/pilcrow/engine/core/include)
include_directories(${PROJECT_SOURCE_DIR}/pilcrow/dependencies)

add_executable(unit_test_runtime 
  ${unit_test_runtime_srcs}
  ${core_test_includes}
  ${core_test_srcs}
  ${event_arena_test_includes}
  ${event_arena_test_srcs}
)

#Specifying unit_test_runtime to be put in a filter in visual studio called executables
set_target_properties(unit_test_runtime PROPERTIES
    FOLDER executables)

#core engine
add_dependencies(unit_test_runtime engine)

#linking unit_test_runtime to libraries
target_link_libraries(unit_test_runtime ${GTEST_LIBRARY})

#linking  engine core 
target_link_libraries(unit_test_runtime engine)