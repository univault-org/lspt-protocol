#include <gtest/gtest.h>
#include <stdexcept>
#include "../../src/core/lspt_handshake.h"
#include "mocks/mock_handshake.h"  // Include the mock class

class HandshakeTest : public ::testing::Test {
protected:
    MockHandshake handshake1;  // Use the mock class
    MockHandshake handshake2;  // Use the mock class
};

TEST(HandshakeTest, GenericHandshake) {
    MockHandshake handshake1, handshake2;
    
    // Initiate handshake for both objects
    auto response1 = handshake1.initiateHandshake();
    EXPECT_FALSE(response1.empty());
    
    auto response2 = handshake2.initiateHandshake();
    EXPECT_FALSE(response2.empty());
    
    // Handle each other's initial messages
    auto reply1 = handshake2.handleHandshakeMessage(response1);
    EXPECT_FALSE(reply1.empty());
    
    auto reply2 = handshake1.handleHandshakeMessage(response2);
    EXPECT_FALSE(reply2.empty());
    
    // Final message exchange to complete the handshake
    auto finalResponse1 = handshake1.handleHandshakeMessage(reply1);
    auto finalResponse2 = handshake2.handleHandshakeMessage(reply2);
    
    // These should be empty responses indicating completion
    EXPECT_TRUE(finalResponse1.empty());
    EXPECT_TRUE(finalResponse2.empty());

    std::cout << "Handshake1 state: " << static_cast<int>(handshake1.getState()) 
              << ", message count: " << handshake1.getMessageCount() << std::endl;
    std::cout << "Handshake2 state: " << static_cast<int>(handshake2.getState()) 
              << ", message count: " << handshake2.getMessageCount() << std::endl;

    EXPECT_EQ(handshake1.getState(), LSPT::HandshakeState::Established);
    EXPECT_EQ(handshake2.getState(), LSPT::HandshakeState::Established);
}

TEST(HandshakeTest, ExceptionOnUnexpectedMessage) {
    MockHandshake handshake1, handshake2;
    
    // First initiation should not throw
    EXPECT_NO_THROW(handshake1.initiateHandshake());
    
    // Second initiation should throw
    EXPECT_THROW(handshake1.initiateHandshake(), std::runtime_error);
    
    auto response = handshake2.initiateHandshake();
    auto reply = handshake1.handleHandshakeMessage(response);
    handshake2.handleHandshakeMessage(reply);
    
    // Ensure both handshakes are fully established
    handshake1.handleHandshakeMessage(LSPT::Common::ByteVector{1});
    handshake2.handleHandshakeMessage(LSPT::Common::ByteVector{1});
    
    std::cout << "Handshake1 state: " << static_cast<int>(handshake1.getState()) 
              << ", message count: " << handshake1.getMessageCount() << std::endl;
    std::cout << "Handshake2 state: " << static_cast<int>(handshake2.getState()) 
              << ", message count: " << handshake2.getMessageCount() << std::endl;
    
    EXPECT_EQ(handshake2.getState(), LSPT::HandshakeState::Established);
    EXPECT_EQ(handshake1.getState(), LSPT::HandshakeState::Established);
    
    // This should now throw an exception
    EXPECT_THROW(handshake1.handleHandshakeMessage(std::vector<uint8_t>{0x00}), std::runtime_error);
}

TEST(HandshakeTest, ExceptionOnPrematureSharedSecretAccess) {
    MockHandshake handshake1;
    EXPECT_THROW(handshake1.getSharedSecret(), std::runtime_error);
}