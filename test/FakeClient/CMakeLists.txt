add_executable(FakeClient
           test_main.cpp
)

target_link_libraries(FakeClient
        PRIVATE
        GTest::GTest
        NL)

target_include_directories(FakeClient PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(FakeClientTest FakeClient)