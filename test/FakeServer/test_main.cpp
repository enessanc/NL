#include <cstring>
#define NL_TEST 1
#include "NL/NL.h"

using namespace NL;
using namespace NL::Test;
using namespace std::chrono_literals;

class FakeServer : public ServerInterface<TestMsgType>
{

public:
    explicit FakeServer(uint16_t port) : ServerInterface<TestMsgType>(port)
    {
        test_struct_a.data1 = 1234567;
        test_struct_a.data2 = 30000;
        test_struct_a.data3 = 3.009;

        test_struct_b.data1 = 357.000000746;
        test_struct_b.data2 = 9876123456789;

        std::string test_string("Test String");
        strcpy(test_struct_b.data3, test_string.c_str());
        test_struct_b.struct_data = test_struct_a;

        test_finish_string = std::string("This is a test string from client.");
    }


    bool Result()
    {
        return result;
    }

protected:
    bool OnClientConnect(std::shared_ptr<Connection<TestMsgType>> client) override
    {
        return true;
    }

    void OnClientValidated(std::shared_ptr<Connection<TestMsgType>> client) override
    {
        Message<TestMsgType> validation_msg;
        validation_msg.header.id = TestMsgType::ClientValidated;
        MessageClient(client, validation_msg);
    }

    void OnMessage(std::shared_ptr<Connection<TestMsgType>> client, Message<TestMsgType>& msg)
    {

        switch (msg.header.id)
        {
            case TestMsgType::ClientValidated:
            {
                std::cerr << "Validation message should not be come to the server." << std::endl;
            }
            case TestMsgType::EchoA:
            {
                TestStructA inc_struct_a{};
                msg >> inc_struct_a;

                if(test_struct_a.data1 != inc_struct_a.data1)
                {
                    std::cout << "[A Stage]: int phase was not succesfull." << std::endl;
                }
                if(test_struct_a.data2 != inc_struct_a.data2)
                {
                    std::cout << "[A Stage]: short phase was not succesfull." << std::endl;
                }
                if(test_struct_a.data3 != inc_struct_a.data3)
                {
                    std::cout << "[A Stage]: float phase was not succesfull.";
                }

                msg << inc_struct_a;
                MessageClient(client,msg);

                break;
            }
            case TestMsgType::EchoB:
            {
                TestStructA inc_struct_a{};
                TestStructB inc_struct_b{};

                msg >> inc_struct_b >> inc_struct_a;

                if(test_struct_b.data1 != inc_struct_b.data1)
                {
                    std::cout << "[B Stage]: double phase was not succesfull." << std::endl;
                }

                if(test_struct_b.data2 != inc_struct_b.data2)
                {
                    std::cout << "[B Stage]: long phase was not succesfull." << std::endl;
                }
                if(std::string(test_struct_b.data3) != std::string(inc_struct_b.data3))
                    std::cout << "[B Stage]:  fixed char array phase was not succesfull." << std::endl;

                if(test_struct_b.struct_data.data1 != inc_struct_b.struct_data.data1)
                {
                    std::cout << "[B Stage]: inner struct-int phase was not succesfull" << std::endl;
                }
                if(test_struct_b.struct_data.data2 != inc_struct_b.struct_data.data2)
                {
                    std::cout << "[B Stage]: inner struct-short phase was not succesfull" << std::endl;
                }
                if(test_struct_b.struct_data.data3 != inc_struct_b.struct_data.data3)
                {
                    std::cout << "[B Stage]: inner struct-float phase was not succesfull" << std::endl;
                }

                if(test_struct_a.data1 != inc_struct_a.data1)
                {
                    std::cout << "[B Stage]: outer struct-int phase was not succesfull." << std::endl;
                }
                if(test_struct_a.data2 != inc_struct_a.data2)
                {
                    std::cout << "[B Stage]: outer struct-short phase was not succesfull." << std::endl;
                }
                if(test_struct_a.data3 !=  inc_struct_a.data3)
                {
                    std::cout << "[B Stage]: outer struct-float phase was not succesfull." << std::endl;
                }

                msg << inc_struct_a << inc_struct_b;
                MessageClient(client,msg);
                break;
            }
            case TestMsgType::FinishSTR:
            {
                std::string test_string_from_client("This is a test string from client.");
                std::string inc_string;
                msg >> inc_string;
                if(test_string_from_client != inc_string)
                {
                    std::cout << "[C Stage]: string phase was not succesfull" << std::endl;
                }

                Message<TestMsgType> message_str;
                message_str.header.id = TestMsgType::FinishSTR;
                message_str << std::string("This is a test string from server.");
                MessageClient(client, message_str);

                //After last message sent, we are waiting 5 second here to finish asio context its job.
                //Normally we let the server run forever.
                std::this_thread::sleep_for(5s);

                Stop();
                break;
            }
            default:
            {
                std::cerr << "Incoming message header id could not be recognized. " << std::endl;
                Stop();
            }

        }

    }


private:
    TestStructA test_struct_a{};
    TestStructB test_struct_b{};
    std::string test_finish_string;
    bool result = true;

};



int main(int argc, char** argv)
{
    FakeServer server(45555);
    server.Start();

    while (server.IsRunning())
    {
        server.Update(-1, true);
    }

    if(server.Result())
    {
        std::cout << "FakeServerTest was passed." << std::endl;
    }
    else
    {
        std::cout << "FakeServerTest was not passed." << std::endl;
    }

    return EXIT_SUCCESS;
}