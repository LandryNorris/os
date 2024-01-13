
file(GLOB_RECURSE KERNEL_C_SOURCES ${CMAKE_SOURCE_DIR}/kernel/**.c)
file(GLOB_RECURSE KERNEL_ASM_SOURCES ${CMAKE_SOURCE_DIR}/kernel/**.asm)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/kernel/src/ld/linker.ld)

add_executable(kernel ${KERNEL_C_SOURCES} ${LINKER_SCRIPT} ${KERNEL_ASM_SOURCES})
target_include_directories(kernel PUBLIC ${CMAKE_SOURCE_DIR}/kernel/include ${CMAKE_SOURCE_DIR}/libk/include ${CMAKE_SOURCE_DIR}/libc/include)

add_dependencies(kernel libc libk)

target_link_libraries(kernel PRIVATE libc libk)

target_link_options(kernel PRIVATE "-T${LINKER_SCRIPT}")

