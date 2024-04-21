#include "gtest/gtest.h"
#include "NL/ClientInterface.h"


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

TEST(NLClientTest, FakeClientTest)
{
    NL::ClientInterface<TestMsgType> ci;
    //ASSERT_EQ(ci.Connect("127.0.0.1",63555),true) << "Connection test was not passed.";
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
