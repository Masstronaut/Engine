# - Find ASSIMP
#  
#  ASSIMP_FOUND - Set to true if Assimp is found
#  ASSIMP_INCLUDE_DIRS - the Assimp include directories
#  ASSIMP_LIBRARY - Debug and Release Library of Assimp


if(WIN32)
  set(FIND_ASSIMP_PATHS
      ${ASSIMP_ROOT}
      ~/Library/Frameworks
      /Library/Frameworks
      /usr/local
      /usr
      /sw
      /opt/local
      /opt/csw
      /opt)

  find_path( ASSIMP_INCLUDE_DIRS
    NAMES assimp
    PATHS ${FIND_ASSIMP_PATHS}
    PATH_SUFFIXES include
  )

  find_library(
    ASSIMP_LIBRARY_DEBUG 
    NAMES assimp-vc140-mt
    PATHS ${FIND_ASSIMP_PATHS}
    PATH_SUFFIXES Debug 
  )

  find_library(
    ASSIMP_LIBRARY_RELEASE 
    NAMES assimp-vc140-mt
    PATHS ${FIND_ASSIMP_PATHS}
    PATH_SUFFIXES Release
  )
  if(ASSIMP_LIBRARY_DEBUG AND ASSIMP_LIBRARY_RELEASE)
    set(ASSIMP_LIBRARY debug ${ASSIMP_LIBRARY_DEBUG} optimized ${ASSIMP_LIBRARY_RELEASE})
  endif(ASSIMP_LIBRARY_DEBUG AND ASSIMP_LIBRARY_RELEASE)

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(ASSIMP DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_DIRS)
	
else(UNIX)
	find_path(
	  ASSIMP_INCLUDE_DIRS
	  NAMES assimp
	  PATHS ${FIND_ASSIMP_PATHS}
	  PATH_SUFFIXES include
	)

	find_library(
	  ASSIMP_LIBRARY
	  NAMES assimp
	  PATHS ${FIND_ASSIMP_PATHS}
    PATH_SUFFIXES lib
	)
	
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ASSIMP DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_DIRS)

endif(WIN32)