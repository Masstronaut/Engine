# - Find GTEST
#
# GTEST_FOUND - Set to true if Google Test is Found
# GTEST_INCLUDE_DIRS - The include directories for Google Test
# GTEST_LIBRARY - Debug and Release Libs for Google Test

set(FIND_GTEST_PATHS 
    ${GTEST_ROOT}
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

find_path(
	GTEST_INCLUDE_DIRS #var name
  NAMES gtest
  PATH_SUFFIXES include 
  PATHS ${FIND_GTEST_PATHS}) 

find_library(
    GTEST_LIBRARY_DEBUG 
    NAMES gtestd
    PATHS ${FIND_GTEST_PATHS}
    PATH_SUFFIXES Debug
  )
  
find_library(
    GTEST_LIBRARY_RELEASE 
    NAMES gtest
    PATHS ${FIND_GTEST_PATHS}
    PATH_SUFFIXES Release
  )


  if(GTEST_LIBRARY_DEBUG AND GTEST_LIBRARY_RELEASE)
    set(GTEST_LIBRARY debug ${GTEST_LIBRARY_DEBUG} optimized ${GTEST_LIBRARY_RELEASE})
  endif(GTEST_LIBRARY_DEBUG AND GTEST_LIBRARY_RELEASE)  
  
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GTEST DEFAULT_MSG GTEST_LIBRARY GTEST_INCLUDE_DIRS)
