include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(NL_TEST test_main.cpp)

target_link_libraries(NL_TEST
        PRIVATE
        GTest::GTest
        NL)

target_include_directories(NL_TEST PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NL_TEST_CMAKE NL_TEST)