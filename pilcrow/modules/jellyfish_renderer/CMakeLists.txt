add_library(jellyfish_renderer)

add_subdirectory(GL)

target_sources(jellyfish_renderer
PRIVATE
  AssimpLoaders.hpp
  iWindow.h
  iMesh.h
  iShader.h
  iTexture.h
  Jellyfish.h
  Model.cpp
  Model.h
  RenderMain.cpp
  Vertex.cpp
  Vertex.h
  Util.h
  Jellyfish.h
)

target_include_directories(jellyfish_renderer
PUBLIC
  ${PROJECT_SOURCE_DIR}
PRIVATE
  ${GLM_INCLUDE_DIRS}
  ${ASSIMP_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/pilcrow/dependencies
)

target_link_libraries(jellyfish_renderer 
PUBLIC
  engine
  glfw
  freetype
)

pilcrow_source_group(CMAKE_CURRENT_LIST_DIR jellyfish_renderer)

#The filter the library will be in
set_target_properties(jellyfish_renderer PROPERTIES FOLDER pilcrow/modules)