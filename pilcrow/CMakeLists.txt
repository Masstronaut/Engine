include(../cmake/pilcrow_source_group.cmake)

add_subdirectory(dependencies)
add_subdirectory(engine)
add_subdirectory(modules)
add_subdirectory(runtimes)
add_subdirectory(tests)

# @@TODO Move this include to the root cmake file
# Doing so will require changes to clangfmt.cmake
include(../cmake/clangfmt.cmake)
include(../cmake/clangtidy.cmake)
