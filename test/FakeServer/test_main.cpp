#include "gtest/gtest.h"

#define TEST_COMMON 1
#include "NL/Common.h"
#include "NL/ServerInterface.h"

class FakeServer : NL::ServerInterface<TestMsgType>
{

};


TEST(NLClientTest, FakeClientTest)
{

}

int main(int argc, char** argv)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}