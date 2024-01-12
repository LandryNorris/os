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

file(GLOB_RECURSE KERNEL_C_SOURCES ${CMAKE_SOURCE_DIR}/kernel/**.c)
file(GLOB_RECURSE KERNEL_ASM_SOURCES ${CMAKE_SOURCE_DIR}/kernel/**.asm)
file(GLOB_RECURSE LIBC_C_SOURCES ${CMAKE_SOURCE_DIR}/libc/**.c)
file(GLOB_RECURSE LIBK_C_SOURCES ${CMAKE_SOURCE_DIR}/libk/**.c)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/kernel/src/ld/linker.ld)

add_library(libc ${LIBC_C_SOURCES})
add_library(libk ${LIBK_C_SOURCES})

add_executable(kernel ${KERNEL_C_SOURCES} ${LINKER_SCRIPT} ${KERNEL_ASM_SOURCES})

target_include_directories(kernel PUBLIC ${CMAKE_SOURCE_DIR}/kernel/include ${CMAKE_SOURCE_DIR}/libk/include ${CMAKE_SOURCE_DIR}/libc/include)
target_include_directories(libc PUBLIC ${CMAKE_SOURCE_DIR}/libc/include ${CMAKE_SOURCE_DIR}/libk/include)
target_include_directories(libk PUBLIC ${CMAKE_SOURCE_DIR}/libk/include)

add_dependencies(kernel libc libk)

target_link_libraries(kernel PRIVATE libc libk)

target_link_options(kernel PRIVATE "-T${LINKER_SCRIPT}")

message(${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(iso COMMAND ${CMAKE_SOURCE_DIR}/tools/iso.sh ${CMAKE_CURRENT_BINARY_DIR}/kernel DEPENDS kernel)
