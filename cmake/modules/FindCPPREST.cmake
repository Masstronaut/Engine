# - Find CPPREST
#  
#  CPPREST_FOUND - Set to true if CppRest is found
#  CPPREST_INCLUDE_DIRS - the CppRest include directories
#  CPPREST_LIBRARY - Debug and Release Library of CppRest

set(FIND_CPPREST_PATHS 
    ${CPPREST_ROOT}
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

find_path(
	CPPREST_INCLUDE_DIRS #var name
  NAMES cpprest
  PATH_SUFFIXES include #include/cpprest
  PATHS ${FIND_CPPREST_PATHS}) 

find_library(
    CPPREST_LIBRARY_DEBUG 
    NAMES cpprest141_2_10
    PATHS ${FIND_CPPREST_PATHS}
    PATH_SUFFIXES Debug
)
  
find_library(
    CPPREST_LIBRARY_RELEASE 
    NAMES cpprest141_2_10
    PATHS ${FIND_CPPREST_PATHS}
    PATH_SUFFIXES Release
)
  
if(CPPREST_LIBRARY_DEBUG AND CPPREST_LIBRARY_RELEASE)
  set(CPPREST_LIBRARY debug ${CPPREST_LIBRARY_DEBUG} optimized ${CPPREST_LIBRARY_RELEASE})
endif(CPPREST_LIBRARY_DEBUG AND CPPREST_LIBRARY_RELEASE)  
  
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CPPREST DEFAULT_MSG CPPREST_LIBRARY CPPREST_INCLUDE_DIRS)