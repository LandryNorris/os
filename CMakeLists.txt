cmake_minimum_required(VERSION 3.13)
project(kernel C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_EXTENSIONS asm)
set(CMAKE_VERBOSE_MAKEFILE true)

set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> \
     <FLAGS> -o <OBJECT> <SOURCE>")

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

include(build_flags.cmake)
include(libc/libc.cmake)
include(libk/libk.cmake)
include(kernel/kernel.cmake)

message(${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(iso COMMAND ${CMAKE_SOURCE_DIR}/tools/iso.sh ${CMAKE_CURRENT_BINARY_DIR}/kernel DEPENDS kernel)
