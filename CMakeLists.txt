cmake_minimum_required(VERSION 3.13)
project(kernel C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_EXTENSIONS asm)
set(CMAKE_VERBOSE_MAKEFILE true)

set(CMAKE_ASM_OBJECT_FORMAT elf32)

set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> \
     <FLAGS> -o <OBJECT> <SOURCE>")

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# Set target architecture
add_compile_options($<$<COMPILE_LANGUAGE:C>:--target=i386-pc-none-elf>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-ffreestanding>)
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-felf32>)

add_link_options(--target=i386-pc-none-elf)
add_link_options(-ffreestanding -nostdlib -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-3dnow -Wl,-no-pie)

include(libc/libc.cmake)
include(libk/libk.cmake)
include(kernel/kernel.cmake)

message(${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(iso COMMAND ${CMAKE_SOURCE_DIR}/tools/iso.sh ${CMAKE_CURRENT_BINARY_DIR}/kernel DEPENDS kernel)
