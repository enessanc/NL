add_executable(FakeClient
           test_main.cpp
)

target_link_libraries(FakeClient
        PRIVATE
        NL)

target_include_directories(FakeClient PRIVATE ${CMAKE_SOURCE_DIR}/include)

