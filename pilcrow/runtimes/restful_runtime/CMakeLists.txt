add_executable(restful_runtime)

#Specify all restful_runtime source files
target_sources(restful_runtime
PRIVATE
  main.cpp
)

target_include_directories(restful_runtime
PRIVATE
  ${PROJECT_SOURCE_DIR}/pilcrow/dependencies
  ${CPPREST_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
  ${ASSIMP_INCLUDE_DIRS}
)

target_link_libraries(restful_runtime
PRIVATE
  ${ASSIMP_LIBRARY}
  ${CPPREST_LIBRARY}
  ${OPENGL_LIBRARIES}
  jellyfish_renderer
  physics
  rest_api
  Utils
  engine

  #glad and stb_image are temporary libraries they are fully moved to graphics
  glad
  stb_image
)

pilcrow_source_group(CMAKE_CURRENT_LIST_DIR restful_runtime)

#Set Visual Studio Project(restful_runtime) to be inside filter "executables"
set_target_properties(restful_runtime PROPERTIES FOLDER pilcrow/runtimes)

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