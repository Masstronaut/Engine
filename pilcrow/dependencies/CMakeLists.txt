add_subdirectory(assimp)
#add_subdirectory(cpprestsdk)
add_subdirectory(detection_idiom)
add_subdirectory(freetype)
add_subdirectory(glfw)
add_subdirectory(glm)
#add_subdirectory(googletest)
add_subdirectory(slot_map)
add_subdirectory(Utils)

#Will eventually be removed and exist in graphics
add_subdirectory(glad)
add_subdirectory(stb_image)

#################################################
# Turn the warnings off on our dependencies.
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  set(disableWarning /W0)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL AppleClang)
  # Also, this is an easy place to force color output for me in this moment
  # Future maintainers of this file may judge me, but I will do this thing.
  set(disableWarning -Wno-everything -fcolor-diagnostics)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(disableWarning -w)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Intel)
  set(disableWarning -w0)
endif()

target_compile_options(freetype PRIVATE ${disableWarning})
target_compile_options(glfw PRIVATE ${disableWarning})
