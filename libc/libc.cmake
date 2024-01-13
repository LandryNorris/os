
file(GLOB_RECURSE LIBC_C_SOURCES ${CMAKE_SOURCE_DIR}/libc/**.c)

add_library(libc ${LIBC_C_SOURCES})
target_include_directories(libc PUBLIC ${CMAKE_SOURCE_DIR}/libc/include ${CMAKE_SOURCE_DIR}/libk/include)
