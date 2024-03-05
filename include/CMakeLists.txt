add_library(
        NL
        INTERFACE
        ClientInterface.h
        Common.h
        Connection.h
        Message.h
        Server.h
        TSQueue.h
)

target_include_directories(NL
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../external/asio/asio/include)
