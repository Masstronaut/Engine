#Specify all restful_runtime source files
set(restful_runtime_src
  "src/main.cpp"
)

#Specifying all the restful runtime sources to be in a filter called "src"
source_group("src" FILES ${restful_runtime_src})

include_directories(${PROJECT_SOURCE_DIR}/pilcrow/dependencies)
include_directories(${PROJECT_SOURCE_DIR}/pilcrow/engine/core/include)
include_directories(${PROJECT_SOURCE_DIR}/pilcrow/engine/rest_api/include)
include_directories(${PROJECT_SOURCE_DIR}/pilcrow/modules/jellyfish_renderer/include)
include_directories(${PROJECT_SOURCE_DIR}/pilcrow/modules/physics/include)
include_directories(${CPPREST_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIRS})


add_executable(restful_runtime ${restful_runtime_src})

#Set Visual Studio Project(restful_runtime) to be inside filter "executables"
set_target_properties(restful_runtime PROPERTIES
    FOLDER executables)

add_dependencies(restful_runtime jellyfish_renderer)
add_dependencies(restful_runtime physics)
add_dependencies(restful_runtime rest_api)
add_dependencies(restful_runtime utils)
add_dependencies(restful_runtime engine)

#glad and stb_image are temporary libraries they are fully moved to graphics
add_dependencies(restful_runtime glad)
add_dependencies(restful_runtime stb_image)
target_link_libraries(restful_runtime glad)
target_link_libraries(restful_runtime stb_image)
############################################################################

target_link_libraries(restful_runtime ${GLFW_LIBRARY})
target_link_libraries(restful_runtime ${ASSIMP_LIBRARY})
target_link_libraries(restful_runtime ${CPPREST_LIBRARY})
target_link_libraries(restful_runtime ${FREETYPE_LIBRARY})
target_link_libraries(restful_runtime ${OPENGL_LIBRARIES})

target_link_libraries(restful_runtime jellyfish_renderer)
target_link_libraries(restful_runtime physics)
target_link_libraries(restful_runtime rest_api)
target_link_libraries(restful_runtime utils)
target_link_libraries(restful_runtime engine)

#Copy cpprest sdk's dll to restful_runtime output directory
add_custom_command(TARGET restful_runtime POST_BUILD   # Adds a post-build event to restful_runtime
    COMMAND ${CMAKE_COMMAND} -E copy_if_different     # which executes "cmake - E copy_if_different..."
    "${dependencies}/cpprestsdk/cpprestsdk-2.10.0/$<CONFIGURATION>/cpprest141$<$<CONFIG:Debug>:d>_2_10.dll" # <--this is in-file
    $<TARGET_FILE_DIR:restful_runtime>)            # <--this is out-file path

#Copy assimp's dll to restful_runtime output directory
add_custom_command(TARGET restful_runtime POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${dependencies}/assimp/assimp-3.3.1/$<CONFIGURATION>/assimp-vc140-mt.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:restful_runtime>)                 # <--this is out-file path