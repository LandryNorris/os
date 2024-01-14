
file(GLOB_RECURSE LIBK_C_SOURCES ${CMAKE_SOURCE_DIR}/libk/**.c)

add_library(libk ${LIBK_C_SOURCES})

target_include_directories(libk PUBLIC ${CMAKE_SOURCE_DIR}/libk/include)

setupEmbeddedFlags(libk)
