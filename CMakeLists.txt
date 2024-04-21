cmake_minimum_required(VERSION 3.28)
project(NL)

set(CMAKE_CXX_STANDARD 20)
set(TEST_STATUS ON)

add_library(
        NL
        INTERFACE
        include/NL/impl/ClientInterface.h
        include/NL/impl/Common.h
        include/NL/impl/Connection.h
        include/NL/impl/Message.h
        include/NL/impl/ServerInterface.h
        include/NL/impl/TSQueue.h
        include/NL/NL.h
)

target_include_directories(NL
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/external/asio/asio/include)


if(TEST_STATUS)
    message("Build progress for tests for NL is starting.")
    add_subdirectory(test)
endif()
