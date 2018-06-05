#VS 2017 introduced in this version
cmake_minimum_required(VERSION 3.8.2)

#Name of the project
project(Engine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Adding path for custom cmake modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
)

#Adding a global variable where the dependencies of the project lives
set(dependencies "${CMAKE_CURRENT_SOURCE_DIR}/pilcrow/dependencies")

#Specify the root directory of the libraries 
#if(MSVC)
  set(GLFW_ROOT "${dependencies}/glfw/glfw-3.2.1")
  set(ASSIMP_ROOT "${dependencies}/assimp/assimp-4.0.1")
  set(GTEST_ROOT "${dependencies}/googletest/googletest-1.8.0")
  set(GLM_ROOT "${dependencies}/glm/glm-0.9.9.0")
  set(FREETYPE_ROOT "${dependencies}/freetype/freetype-2.9")
  set(CPPREST_ROOT "${dependencies}/cpprestsdk/cpprestsdk-2.10.0")
  set(DETECTION_IDIOM_ROOT "${dependencies}/detection_idiom")
  set(SLOT_MAP_ROOT "${dependencies}/slot_map")
  
#endif(MSVC)


#Finding the specified libraries to be included later
find_package(GLFW)
find_package(ASSIMP)
find_package(FREETYPE)
find_package(CPPREST)
find_package(GTEST)
find_package(OpenGL)

#Set Visual Studio startup project to "restful_runtime"
set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "restful_runtime" )
#Specify compiler flags for MSVC to use the latest c++ 
if (MSVC_VERSION GREATER_EQUAL "1900")
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
  if (_cpp_latest_flag_supported)
    add_compile_options("/std:c++latest")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  #add_compile_options("-stdlib=libc++")
  message("C++17 support")
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_subdirectory(pilcrow)

# Including custom cmake rules
#generate the compile commands used by clang-tidy and other tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

