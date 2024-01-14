
file(GLOB_RECURSE LIBC_C_SOURCES ${CMAKE_SOURCE_DIR}/libc/src/c/**.c)
file(GLOB_RECURSE LIBK_C_SOURCES ${CMAKE_SOURCE_DIR}/libk/src/c/**.c)
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/libc/test/src/**.cpp)

add_library(libc_test_lib STATIC ${LIBC_C_SOURCES} ${LIBK_C_SOURCES})
add_library(libc ${LIBC_C_SOURCES})

target_include_directories(libc_test_lib PRIVATE ${CMAKE_SOURCE_DIR}/libc/include ${CMAKE_SOURCE_DIR}/libk/include)

target_include_directories(libc PRIVATE ${CMAKE_SOURCE_DIR}/libc/include ${CMAKE_SOURCE_DIR}/libk/include)

setupEmbeddedFlags(libc)

target_compile_definitions(libc_test_lib PUBLIC TESTING)

add_executable(libc_test ${TEST_SOURCES})
target_link_libraries(libc_test GTest::gtest)
target_link_libraries(libc_test libc_test_lib)

target_include_directories(libc_test PRIVATE ${CMAKE_SOURCE_DIR}/libc/test/include ${CMAKE_SOURCE_DIR})

include(GoogleTest)
