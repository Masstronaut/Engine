function (pilcrow_list_to_string aResult aDelimiter aList)
  set(temporaryString "")

  foreach(item ${${aList}})
    #message(item: ${item})
    set(temporaryString ${temporaryString}${aDelimiter}${item})
  endforeach()

  string(LENGTH ${aDelimiter} lengthOfDelim)

  string(SUBSTRING ${temporaryString} ${lengthOfDelim} -1 outputString)

  #set(${aResult} ${temporaryString} PARENT_SCOPE)
  set(${aResult} ${outputString} PARENT_SCOPE)
endfunction()

function (pilcrow_process_sources aTarget aList aResult)
  get_target_property(targetSourceDir ${aTarget} SOURCE_DIR)
  set(temporaryList "")
  foreach(item ${${aList}})
    if(IS_ABSOLUTE ${item})
      list(APPEND temporaryList ${item})
      #message(STATUS ${item})
    else()
      set(fullPath ${targetSourceDir}/${item})

      if (EXISTS ${fullPath})
        list(APPEND temporaryList ${fullPath})
        #message(STATUS ${fullPath})
      else()
        message(SEND_ERROR "Can't find absolute path to ${item}")
      endif()
    endif()
  endforeach()

  set(${aResult} ${temporaryList} PARENT_SCOPE)
endfunction()

function(pilcrow_populate_include_list aTarget aIncludeDirectoriesVariable)
  # This is the start of the actual include directory when looking at BUILD_INTERFACE
  # generator expressions.    
  set(startOfIncludeDirectory 18)

  #Add on the original directories.
  list(APPEND initialIncludeDirectories ${${aIncludeDirectoriesVariable}})

  # Inteface libraries use different properties for their link libraries and 
  # include directories. So we'll look up if this target is one so we can
  # set the property type we're looking for.
  get_target_property(targetType ${aTarget} TYPE)

  if (NOT targetType STREQUAL "INTERFACE_LIBRARY")
    set(includeDirectoriesProperty INCLUDE_DIRECTORIES)
    set(linkLibrariesProperty LINK_LIBRARIES)
  else()
    set(includeDirectoriesProperty INTERFACE_INCLUDE_DIRECTORIES)
    set(linkLibrariesProperty INTERFACE_LINK_LIBRARIES)
  endif()

  # Now append on this targets include directories
  get_target_property(targetIncludeDirectories ${aTarget} ${includeDirectoriesProperty})

  if(NOT targetIncludeDirectories STREQUAL "targetIncludeDirectories-NOTFOUND")
    list(APPEND initialIncludeDirectories ${targetIncludeDirectories})
  endif()

  # Now we recurse to each target underneath this one.
  get_target_property(targetLinkLibraries ${aTarget} ${linkLibrariesProperty})

  if(NOT targetLinkLibraries STREQUAL "targetLinkLibraries-NOTFOUND")
    foreach(linkLibrary ${targetLinkLibraries})
      pilcrow_populate_include_list(${linkLibrary} initialIncludeDirectories)
    endforeach()
  endif()

  # We need to convert BUILD_INTERFACE includes to normal directories, and remove
  # INSTALL_INTERFACE directories.
  foreach(includeDirectory ${initialIncludeDirectories})
    if(includeDirectory MATCHES "\\$<INSTALL_INTERFACE:.*>")
      continue()
    elseif(includeDirectory MATCHES "^\\$<BUILD_INTERFACE:.*>$")
      # Compute the length - 19, so we can get the length of _only_ the include directory
      # itself. This way we don't get any of the $<BUILD_INTERFACE:> part of the string.
      string(LENGTH ${includeDirectory} lengthOfString)
      math(EXPR finalStringLength "${lengthOfString} - 19")

      string(SUBSTRING ${includeDirectory} ${startOfIncludeDirectory} ${finalStringLength} newIncludeDirectory)
      list(APPEND finalIncludeDirectories ${newIncludeDirectory})
    else()
      list(APPEND finalIncludeDirectories ${includeDirectory})
    endif()
  endforeach()

  # Deduplicate the list
  list(REMOVE_DUPLICATES finalIncludeDirectories)
  
  # Return the list.
  set(${aIncludeDirectoriesVariable} ${finalIncludeDirectories} PARENT_SCOPE)
endfunction()

function(pilcrow_create_reflection_target aTarget) 
  get_target_property(targetBinaryDir ${aTarget} BINARY_DIR)
  get_target_property(targetSourceDir ${aTarget} SOURCE_DIR)
  get_target_property(targetSources ${aTarget} SOURCES)

  pilcrow_populate_include_list(${aTarget} targetIncludeDirectories)
  
  #message(STATUS ${targetIncludeDirectories})

  list(FILTER targetSources INCLUDE REGEX "^.*\.(c(pp)?)$")
  
  pilcrow_process_sources(${aTarget} targetSources processedTargetSources)

  list(JOIN processedTargetSources ";" targetSourcesList)
  list(JOIN targetIncludeDirectories ";" targetIncludeDirectoriesList)

  add_custom_command(TARGET ${aTarget} PRE_BUILD
    # Runs ReflectionExporter
    COMMAND ${toolsDirectory}/ReflectionExporter/ReflectionExporter.exe
    # Includes
    --include ${targetIncludeDirectoriesList}
    # Sources
    --sources ${targetSourcesList}
    # Output file
    --outputFile "\"${targetBinaryDir}/ReflectionCode.cpp\""
  )
endfunction() 

function(pilcrow_source_group aRoot aTarget) 
  get_target_property(targetBinaryDir ${aTarget} BINARY_DIR)
  get_target_property(targetSourceDir ${aTarget} SOURCE_DIR)
  get_target_property(targetSources ${aTarget} SOURCES)

  # This will determine if the given files are in a folder or not and separate 
  # them on that alone. 
  foreach(aFile ${targetSources}) 
    file(TO_CMAKE_PATH ${aFile} resultFile) 
    get_filename_component(nameComponent ${resultFile} NAME) 
    get_filename_component(fullPath ${nameComponent} ABSOLUTE) 
    
    if(IS_ABSOLUTE ${aFile})
      # It's only safe to call RELATIVE_PATH if the path begins with our "root" dir.
      string(FIND "${aFile}" "${targetSourceDir}" checkIfRelativeToSourceDir)
      string(FIND "${aFile}" "${targetBinaryDir}" checkIfRelativeToBinaryDir)

      if ("${checkIfRelativeToSourceDir}" EQUAL 0)
        file(RELATIVE_PATH relativePath ${targetSourceDir} ${aFile})
      elseif ("${checkIfRelativeToBinaryDir}" EQUAL 0)
        file(RELATIVE_PATH relativePath ${targetBinaryDir} ${aFile})
        set(fullPath ${targetBinaryDir}/${nameComponent})
      endif()
    else()
      set(relativePath ${aFile})
    endif()

    if(EXISTS ${fullPath}) 
      set(notInAFolder ${notInAFolder} ${relativePath}) 
    else()
      set(inAFolder ${inAFolder} ${relativePath}) 
    endif() 
  endforeach() 

  # We use a no space prefix with files from folders, otherwise the filters  
  # don't get made. 
  source_group(TREE ${${aRoot}}  
               PREFIX "" 
               FILES ${inAFolder}) 

  # We use a one space prefix with files not in folders, otherwise the files 
  # are put into "Source Files" and "Header Files" filters. 
  source_group(TREE ${${aRoot}}  
               PREFIX " " 
               FILES ${notInAFolder}) 
endfunction() 