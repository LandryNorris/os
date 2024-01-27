
file(GLOB_RECURSE KERNEL_C_SOURCES ${CMAKE_SOURCE_DIR}/kernel/**.c)
file(GLOB_RECURSE KERNEL_ASM_SOURCES ${CMAKE_SOURCE_DIR}/kernel/**.asm)
file(GLOB_RECURSE KERNEL_TEST_SOURCES ${CMAKE_SOURCE_DIR}/kernel/test/**.cpp)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/kernel/src/ld/linker.ld)

add_executable(kernel ${KERNEL_C_SOURCES} ${LINKER_SCRIPT} ${KERNEL_ASM_SOURCES})
target_include_directories(kernel PUBLIC ${CMAKE_SOURCE_DIR}/kernel/include ${CMAKE_SOURCE_DIR}/libk/include ${CMAKE_SOURCE_DIR}/libc/include)

add_dependencies(kernel libc libk)

target_link_libraries(kernel PRIVATE libc libk)

target_link_options(kernel PRIVATE "-T${LINKER_SCRIPT}")

setupEmbeddedFlags(kernel)

include(GoogleTest)

file(GLOB_RECURSE SERIAL_SOURCES ${CMAKE_SOURCE_DIR}/kernel/src/c/serial/**.c)
file(GLOB_RECURSE IDT_SOURCES ${CMAKE_SOURCE_DIR}/kernel/src/c/idt/**.c)

add_executable(kernel_test ${KERNEL_TEST_SOURCES} ${SERIAL_SOURCES} ${IDT_SOURCES})
target_link_libraries(kernel_test GTest::gtest)

target_compile_definitions(kernel_test PRIVATE TESTING)

target_include_directories(kernel_test PRIVATE ${CMAKE_SOURCE_DIR}/kernel/include)
target_include_directories(kernel_test PRIVATE ${CMAKE_SOURCE_DIR}/kernel/test/include)
