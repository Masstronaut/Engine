#Find the path to the glm version specified
find_path(
  ASSIMP_INCLUDE_DIRS
  NAMES assimp
  PATH_SUFFIXES include
  PATHS assimp-4.0.1
)

#Creating assimp library
add_library(assimp INTERFACE)

#Adding the location where the files are located
target_include_directories(assimp INTERFACE ${ASSIMP_INCLUDE_DIRS})