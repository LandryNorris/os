
file(GLOB_RECURSE LIBK_C_SOURCES ${CMAKE_SOURCE_DIR}/libk/**.c)

add_library(libk ${LIBK_C_SOURCES})

target_include_directories(libk PUBLIC ${CMAKE_SOURCE_DIR}/libk/include)

setupEmbeddedFlags(libk)

include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/libk/test/src/**.cpp)

add_executable(libk_test ${TEST_SOURCES} ${LIBK_C_SOURCES})
target_link_libraries(libk_test GTest::gtest)
target_include_directories(libk_test PUBLIC ${CMAKE_SOURCE_DIR}/libk/include)
