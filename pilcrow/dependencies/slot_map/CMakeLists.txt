#Creating slot_map header only library
add_library(slot_map INTERFACE)

#Adding the location where the files are located
target_include_directories(slot_map INTERFACE include/)