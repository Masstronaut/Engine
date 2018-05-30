#Creating detection_idiom header only library
add_library(detection_idiom INTERFACE)

#Adding the location where the files are located
target_include_directories(detection_idiom INTERFACE include/)