cmake_minimum_required(VERSION 3.13)
project(kernel C ASM)

find_program(CLANG_PATH_RAW clang)
string(STRIP ${CLANG_PATH_RAW} CLANG_PATH)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP
# This option was added in 3.24.0
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER ${CLANG_PATH})
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_EXTENSIONS asm)
set(CMAKE_VERBOSE_MAKEFILE true)

set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> \
     <FLAGS> -o <OBJECT> <SOURCE>")

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

include(test.cmake)
include(build_flags.cmake)

include(libc/libc.cmake)
include(libk/libk.cmake)
include(kernel/kernel.cmake)

message(${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(iso COMMAND ${CMAKE_SOURCE_DIR}/tools/iso.sh ${CMAKE_CURRENT_BINARY_DIR}/kernel DEPENDS kernel)
