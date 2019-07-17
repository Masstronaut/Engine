#Specify all sample_test includes files
set(sample_test_includes
  "include/sample.h"
)

source_group("include" FILES ${sample_test_includes})

#Specify all sample_test source files
set(sample_test_srcs
  "src/sample.cpp"
  "src/unittest_sample.cpp"
)

source_group("src" FILES ${sample_test_srcs})

include_directories(${GTEST_INCLUDE_DIRS})

#Creating a static library called sample_test
#using sample_test_includes, sample_test_srcs to create the library with
add_library(sample_test 
  ${sample_test_includes}
  ${sample_test_srcs}
)

#Specifying sampe_test to be put in a filter in visual studio called tests
set_target_properties(sample_test PROPERTIES FOLDER pilcrow/tests)

#linking sample_test to libraries
target_link_libraries(sample_test ${GTEST_LIBRARY})