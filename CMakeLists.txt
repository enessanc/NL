cmake_minimum_required(VERSION 3.28)
project(NL)

set(CMAKE_CXX_STANDARD 20)
set(TEST_STATUS ON)

add_library(
        NL
        INTERFACE
        include/NL/ClientInterface.h
        include/NL/Common.h
        include/NL/Connection.h
        include/NL/Message.h
        include/NL/ServerInterface.h
        include/NL/TSQueue.h
)

target_include_directories(NL
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/external/asio/asio/include)


if(TEST_STATUS)
    enable_testing()
    message("Build progress for tests for NL is starting.")
    add_subdirectory(test)
endif()
