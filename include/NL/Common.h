#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#ifdef TEST_COMMON
    enum class TestMsgType : uint8_t
    {
        EchoA = 0,
        EchoB,
        FinishSTR
    };

    struct TestStructA
    {
        int data1;
        short data2;
        float data3;
    };

    struct TestStructB
    {
        TestStructA struct_data;
        double data1;
        long data2;
        char data3[30];
    };
#endif