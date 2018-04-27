# - Find GLFW
#  
#  GLFW_FOUND - Set to true if GLFW is found
#  GLFW_INCLUDE_DIRS - the GLFW include directories
#  GLFW_LIBRARY - Debug and Release Library of GLFW

SET(FIND_GLFW_PATHS 
    ${GLFW_ROOT}
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)
#for 1 include path
FIND_PATH(
	GLFW_INCLUDE_DIRS #var name
  NAMES GLFW
  PATH_SUFFIXES include #include/GLFW
  PATHS ${FIND_GLFW_PATHS}) 

#for 1 lib
FIND_LIBRARY(
    GLFW_LIBRARY_DEBUG 
    NAMES glfw3
    PATHS ${FIND_GLFW_PATHS}
    PATH_SUFFIXES Debug
  )
  
FIND_LIBRARY(
    GLFW_LIBRARY_RELEASE 
    NAMES glfw3
    PATHS ${FIND_GLFW_PATHS}
    PATH_SUFFIXES Release
  )
  
IF(GLFW_LIBRARY_DEBUG AND GLFW_LIBRARY_RELEASE)
  SET(GLFW_LIBRARY debug ${GLFW_LIBRARY_DEBUG} optimized ${GLFW_LIBRARY_RELEASE})
ENDIF(GLFW_LIBRARY_DEBUG AND GLFW_LIBRARY_RELEASE)  
  
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW DEFAULT_MSG GLFW_LIBRARY GLFW_INCLUDE_DIRS)