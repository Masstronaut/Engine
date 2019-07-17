cmake_minimum_required(VERSION 3.14)

project(Engine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Adding path for custom cmake modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
)

#Adding a global variable where the dependencies of the project lives
set(dependencies ${CMAKE_CURRENT_SOURCE_DIR}/pilcrow/dependencies)
set(toolsDirectory ${CMAKE_CURRENT_SOURCE_DIR}/Tools)

#Specify the root directory of the libraries 
set(ASSIMP_ROOT "${dependencies}/assimp/assimp-4.0.1")
set(GTEST_ROOT "${dependencies}/googletest/googletest-1.8.0")
set(GLM_ROOT "${dependencies}/glm/glm-0.9.9.0")
set(CPPREST_ROOT "${dependencies}/cpprestsdk/cpprestsdk-2.10.0")
set(DETECTION_IDIOM_ROOT "${dependencies}/detection_idiom")
set(SLOT_MAP_ROOT "${dependencies}/slot_map")


#Finding the specified libraries to be included later
find_package(ASSIMP)
find_package(CPPREST)
find_package(GTEST)
find_package(OpenGL)

add_subdirectory(pilcrow)

#Generate the compile commands used by clang-tidy and other tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Set Visual Studio startup project to "restful_runtime"
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT restful_runtime)
