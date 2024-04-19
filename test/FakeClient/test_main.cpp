#include "gtest/gtest.h"

#define TEST_COMMON 1
#include "NL/Common.h"
#include "NL/ClientInterface.h"

TEST(NLClientTest, FakeClientTest)
{
    NL::ClientInterface<TestMsgType> ci;
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
