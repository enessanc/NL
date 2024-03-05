cmake_minimum_required(VERSION 3.25)
project(NL)

set(CMAKE_CXX_STANDARD 20)
set(TEST_STATUS ON)

add_subdirectory(include)

if(TEST_STATUS)
    enable_testing()
    message("Tests for NL is starting.")
    add_subdirectory(test)
endif()