set(jfr_includes
  "include/GL/GLWindow.h"
  "include/GL/GLMesh.h"
  "include/GL/GLTexture.h"
  "include/GL/GLText.h"
  "include/GL/GLShader.h"
  "include/GL/GLProgram.h"
  "include/iWindow.h"
  "include/iMesh.h"
  "include/iShader.h"
  "include/iTexture.h"
  "include/AssimpLoaders.hpp"
  "include/Model.h"
  "include/Vertex.h"
  "include/Util.h"
  "include/Jellyfish.h"
)

set(jfr_srcs
  "src/GL/GLWindow.cpp"
  "src/GL/GLMesh.cpp"
  "src/GL/GLTexture.cpp"
  "src/GL/GLText.cpp"
  "src/GL/GLShader.cpp"
  "src/GL/GLProgram.cpp"
  "src/Model.cpp"
  "src/Vertex.cpp"
  "src/RenderMain.cpp"
)

foreach(jfr_include IN ITEMS ${jfr_includes})
  source_group("include" FILES "${jfr_include}" )
endforeach()

foreach(jfr_src IN ITEMS ${jfr_srcs})
  source_group("src" FILES "${jfr_src}" )
endforeach()


include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories("${PROJECT_SOURCE_DIR}/pilcrow/dependencies")

add_library(jellyfish_renderer
  ${jfr_includes}
  ${jfr_srcs}
)

target_link_libraries(jellyfish_renderer ${GLFW_LIBRARIES})

#The filter the library will be in
SET_TARGET_PROPERTIES (jellyfish_renderer PROPERTIES
    FOLDER libraries)