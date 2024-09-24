#include <gtest/gtest.h>
#include "mocks/iridium_mock_api.h"

class IridiumAPITest : public ::testing::Test {
protected:
    std::unique_ptr<SRPT::Satellite::Test::IridiumMockAPI> iridiumMock;

    void SetUp() override {
        iridiumMock = SRPT::Satellite::Test::createIridiumMockAPI();
    }
};

TEST_F(IridiumAPITest, InitiateSession) {
    auto status = iridiumMock->initiateSession("300234060109206");
    EXPECT_EQ(status.status, 0);
    EXPECT_EQ(status.description, "Session initiated successfully");
}

TEST_F(IridiumAPITest, SendMOMessage) {
    SRPT::Satellite::Test::IridiumMockAPI::MOMessage message{
        {1, 2, 3, 4, 5},  // payload
        1,                // momsn
        "300234060109206",// imei
        "2023-05-10T12:00:00Z" // timestamp
    };
    auto status = iridiumMock->sendMOMessage(message);
    EXPECT_EQ(status.status, 0);
    EXPECT_EQ(status.description, "Message sent successfully");
}

TEST_F(IridiumAPITest, CheckMTMessages) {
    std::vector<SRPT::Satellite::Test::IridiumMockAPI::MTMessage> messages;
    auto status = iridiumMock->checkMTMessages("300234060109206", messages);
    EXPECT_EQ(status.status, 0);
    EXPECT_EQ(status.description, "Messages retrieved successfully");
    EXPECT_EQ(messages.size(), 1);
    EXPECT_EQ(messages[0].payload, std::vector<uint8_t>({1, 2, 3}));
}

TEST_F(IridiumAPITest, GetSignalQuality) {
    int32_t quality = iridiumMock->getSignalQuality("300234060109206");
    EXPECT_EQ(quality, 5);
}

TEST_F(IridiumAPITest, GetIMEI) {
    std::string imei = iridiumMock->getIMEI();
    EXPECT_EQ(imei, "300234060109206");
}

TEST_F(IridiumAPITest, ClearQueues) {
    auto moStatus = iridiumMock->clearMOQueue();
    EXPECT_EQ(moStatus.status, 0);
    EXPECT_EQ(moStatus.description, "MO queue cleared successfully");

    auto mtStatus = iridiumMock->clearMTQueue();
    EXPECT_EQ(mtStatus.status, 0);
    EXPECT_EQ(mtStatus.description, "MT queue cleared successfully");
}