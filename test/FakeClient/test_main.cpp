#include "gtest/gtest.h"

#define NL_TEST 1
#include "NL/NL.h"

using namespace NL::Test;
using namespace NL;


TEST(NLClientTest, FakeClientTest)
{
    ClientInterface<TestMsgType> ci;
    EXPECT_EQ(ci.Connect("127.0.0.1", 45555), true) << "Connection test was not successful.";

    TestStructA sent_struct_a{};
    sent_struct_a.data1 = 1234567;
    sent_struct_a.data2 = 30000;
    sent_struct_a.data3 = 3.009;

    TestStructB sent_struct_b{};
    sent_struct_b.data1 = 357.000000746;
    sent_struct_b.data2 = 9876123456789;

    std::string sent_string("Test String");
    strcpy(sent_struct_b.data3, sent_string.c_str());
    sent_struct_b.struct_data = sent_struct_a;

    bool is_validation_first = false;

    while (ci.IsConnected())
    {
        while(!ci.InComing().empty())
        {
            Message<TestMsgType> incoming_msg;
            incoming_msg = ci.InComing().pop_front().msg;
            switch (incoming_msg.header.id)
            {
                case TestMsgType::ClientValidated:
                {
                    EXPECT_EQ(is_validation_first, false)
                    << "[Validation Stage]: The server probably did not send validation message before any other message.";
                    is_validation_first = true;

                    Message<TestMsgType> message_A;
                    message_A.header.id = TestMsgType::EchoA;
                    message_A << sent_struct_a;
                    ci.Send(message_A);

                    break;
                }
                case TestMsgType::EchoA:
                {
                    TestStructA inc_struct_a{};
                    incoming_msg >> inc_struct_a;

                    EXPECT_EQ(sent_struct_a.data1, inc_struct_a.data1) << "[A Stage]: int phase was not succesfull.";
                    EXPECT_EQ(sent_struct_a.data2, inc_struct_a.data2) << "[A Stage]: short phase was not succesfull.";
                    EXPECT_EQ(sent_struct_a.data3, inc_struct_a.data3) << "[A Stage]: float phase was not succesfull.";

                    Message<TestMsgType> message_B;
                    message_B.header.id = TestMsgType::EchoB;
                    message_B << sent_struct_a << sent_struct_b;
                    ci.Send(message_B);
                    break;
                }
                case TestMsgType::EchoB:
                {
                    TestStructA inc_struct_a{};
                    TestStructB inc_struct_b{};

                    incoming_msg >> inc_struct_b >> inc_struct_a;

                    EXPECT_EQ(sent_struct_b.data1, inc_struct_b.data1) << "[B Stage]: double phase was not succesfull.";
                    EXPECT_EQ(sent_struct_b.data2, inc_struct_b.data2) << "[B Stage]: long phase was not succesfull.";
                    EXPECT_EQ(std::string(sent_struct_b.data3),
                              std::string(inc_struct_b.data3)) << "[B Stage]:  fixed char array phase was not succesfull.";


                    EXPECT_EQ(sent_struct_b.struct_data.data1,
                              inc_struct_b.struct_data.data1) << "[B Stage]: inner struct-int phase was not succesfull";
                    EXPECT_EQ(sent_struct_b.struct_data.data2,
                              inc_struct_b.struct_data.data2) << "[B Stage]: inner struct-short phase was not succesfull";
                    EXPECT_EQ(sent_struct_b.struct_data.data3,
                              inc_struct_b.struct_data.data3) << "[B Stage]: inner struct-float phase was not succesfull";

                    EXPECT_EQ(sent_struct_a.data1, inc_struct_a.data1) << "[B Stage]: outer struct-int phase was not succesfull.";
                    EXPECT_EQ(sent_struct_a.data2, inc_struct_a.data2) << "[B Stage]: outer struct-short phase was not succesfull.";
                    EXPECT_EQ(sent_struct_a.data3, inc_struct_a.data3) << "[B Stage]: outer struct-float phase was not succesfull.";

                    Message<TestMsgType> message_C;
                    message_C.header.id = TestMsgType::FinishSTR;
                    message_C << std::string("This is a test string from client.");
                    ci.Send(message_C);

                    break;
                }
                case TestMsgType::FinishSTR:
                {
                    std::string test_string_from_server("This is a test string from server.");
                    std::string inc_string;
                    incoming_msg >> inc_string;

                    EXPECT_EQ(test_string_from_server,inc_string) << "[C Stage]: string phase was not succesfull";

                    ci.Disconnect();

                    break;
                }
                default:
                {
                    FAIL() << "Incoming message header id could not be recognized. ";
                }
            }
        }
    }


}

int main(int argc, char** argv)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
