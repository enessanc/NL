add_executable(FakeServer
        test_main.cpp
)

target_link_libraries(FakeServer
        PRIVATE
        GTest::GTest
        NL)

target_include_directories(FakeServer PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(FakeServerTest FakeServer)