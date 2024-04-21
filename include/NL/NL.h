#pragma once

#include "NL/impl/Common.h"
#include "NL/impl/Message.h"
#include "NL/impl/TSQueue.h"
#include "NL/impl/Connection.h"
#include "NL/impl/ServerInterface.h"
#include "NL/impl/ClientInterface.h"

#ifdef NL_TEST

namespace NL::Test
{
    enum class TestMsgType : uint8_t
    {
        ClientValidated = 0,
        EchoA,
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
}

#endif