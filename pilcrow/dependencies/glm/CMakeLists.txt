#Find the path to the glm version specified
find_path(
  GLM_INCLUDE_DIRS
  NAMES glm
  PATH_SUFFIXES include
  PATHS glm-0.9.8.5
)

#Creating glm header only library
add_library(glm INTERFACE)

#Adding the location where the files are located
target_include_directories(glm INTERFACE ${GLM_INCLUDE_DIRS})