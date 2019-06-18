#variable that contains all the sources of unit_test_runtime
set(unit_test_runtime_srcs
  "src/main.cpp"
)

#Specifying all the unit test runtime sources to be in a filter called "src"
source_group("src" FILES ${unit_test_runtime_srcs})

#add google test include directory to unit_test_runtime project
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(unit_test_runtime ${unit_test_runtime_srcs})

#Specifying unit_test_runtime to be put in a filter in visual studio called executables
set_target_properties(unit_test_runtime PROPERTIES FOLDER pilcrow/runtimes)

add_dependencies(unit_test_runtime sample_test)

#linking unit_test_runtime to libraries
target_link_libraries(unit_test_runtime sample_test)
target_link_libraries(unit_test_runtime ${GTEST_LIBRARY})