# - Find FREETYPE
#  
#  FREETYPE_FOUND - Set to true if FreeType is found
#  FREETYPE_INCLUDE_DIRS - the FreeType include directories
#  FREETYPE_LIBRARY - Debug and Release Library of FreeType

set(FIND_FREETYPE_PATHS 
    ${FREETYPE_ROOT}
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)
    
find_path(
	FREETYPE_INCLUDE_DIRS #var name
  NAMES freetype
  PATH_SUFFIXES include
  PATHS ${FIND_FREETYPE_PATHS}) 
  
FIND_LIBRARY(
    FREETYPE_LIBRARY_DEBUG 
    NAMES freetype
    PATHS ${FIND_FREETYPE_PATHS}
    PATH_SUFFIXES Debug
)

FIND_LIBRARY(
    FREETYPE_LIBRARY_RELEASE
    NAMES freetype
    PATHS ${FIND_FREETYPE_PATHS}
    PATH_SUFFIXES Release
)

if(FREETYPE_LIBRARY_DEBUG AND FREETYPE_LIBRARY_RELEASE)
  set(FREETYPE_LIBRARY debug ${FREETYPE_LIBRARY_DEBUG} optimized ${FREETYPE_LIBRARY_RELEASE})
endif(FREETYPE_LIBRARY_DEBUG AND FREETYPE_LIBRARY_RELEASE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FREETYPE DEFAULT_MSG FREETYPE_LIBRARY FREETYPE_INCLUDE_DIRS)