#define NL_TEST 1
#include "NL/NL.h"

using namespace NL::Test;
using namespace NL;


bool test_fake_client()
{
    bool result = true;
    ClientInterface<TestMsgType> ci;
    if(!ci.Connect("127.0.0.1", 45555))
    {
        std::cout << "Connection test was not successful." << std::endl;
        result = false;
    }

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
                    if(is_validation_first)
                    {
                        std::cout << "[Validation Stage]: The server probably did not send validation message before any other message." << std::endl;
                        result = false;
                    }

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

                    if(sent_struct_a.data1 != inc_struct_a.data1)
                    {
                        std::cout << "[A Stage]: int phase was not succesfull." << std::endl;
                        result = false;
                    }

                    if(sent_struct_a.data2 != inc_struct_a.data2)
                    {
                        std::cout << "[A Stage]: short phase was not succesfull." << std::endl;
                        result = false;
                    }

                    if(sent_struct_a.data3 != inc_struct_a.data3)
                    {
                        std::cout << "[A Stage]: float phase was not succesfull." << std::endl;
                        result = false;
                    }

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

                    if(sent_struct_b.data1 != inc_struct_b.data1)
                    {
                        std::cout << "[B Stage]: double phase was not succesfull." << std::endl;
                        result = false;
                    }
                    if(sent_struct_b.data2 != inc_struct_b.data2)
                    {
                        std::cout << "[B Stage]: long phase was not succesfull." << std::endl;
                        result = false;
                    }

                    if(std::string(sent_struct_b.data3) != std::string(inc_struct_b.data3))
                    {
                        std::cout << "[B Stage]:  fixed char array phase was not succesfull." << std::endl;
                        result = false;
                    }

                    if(sent_struct_b.struct_data.data1 != inc_struct_b.struct_data.data1)
                    {
                        std::cout << "[B Stage]: inner struct-int phase was not succesfull" << std::endl;
                        result = false;
                    }

                    if(sent_struct_b.struct_data.data2 != inc_struct_b.struct_data.data2)
                    {
                        std::cout << "[B Stage]: inner struct-short phase was not succesfull" << std::endl;
                        result = false;
                    }

                    if(sent_struct_b.struct_data.data3 != inc_struct_b.struct_data.data3)
                    {
                        std::cout << "[B Stage]: inner struct-float phase was not succesfull" << std::endl;
                        result = false;
                    }

                    if(sent_struct_a.data1 != inc_struct_a.data1)
                    {
                        std::cout << "[B Stage]: outer struct-int phase was not succesfull." << std::endl;
                        result = false;
                    }

                    if(sent_struct_a.data2 != inc_struct_a.data2)
                    {
                        std::cout << "[B Stage]: outer struct-short phase was not succesfull." << std::endl;
                        result = false;
                    }

                    if(sent_struct_a.data3 !=  inc_struct_a.data3)
                    {
                        std::cout << "[B Stage]: outer struct-float phase was not succesfull." << std::endl;
                        result = false;
                    }

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

                    if(test_string_from_server != inc_string)
                    {
                        std::cout << "[C Stage]: string phase was not succesfull" << std::endl;
                    }

                    ci.Disconnect();

                    break;
                }
                default:
                {
                    std::cerr << "Incoming message header id could not be recognized. " << std::endl;
                    ci.Disconnect();
                    result = false;
                }
            }
        }
    }

    return result;
}

int main(int argc, char** argv)
{
    if(test_fake_client())
    {
        std::cout << "FakeClientTest passed." << std::endl;
    }
    else
    {
        std::cerr << "FakeServerTest did not passed." << std::endl;
    }

    return EXIT_SUCCESS;
}