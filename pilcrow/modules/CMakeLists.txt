#Automatically add all directories as their own subdirectories
#This is a temporary solution to add modules to the engine

file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
set(dirlist "")
foreach(child ${children})
  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
    list(APPEND dirlist ${child})
  endif()
endforeach()

foreach(currDir ${dirlist})
  add_subdirectory(${currDir})
endforeach()
