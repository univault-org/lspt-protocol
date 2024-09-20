#include <gtest/gtest.h>
#include "../../src/core/lspt_handshake.h"

class HandshakeTest : public ::testing::Test {
protected:
    LSPT::Handshake droneHandshake;
    LSPT::Handshake groundStationHandshake;
};

TEST_F(HandshakeTest, DroneInitiatedHandshake) {
    // Drone initiates handshake
    auto droneHello = droneHandshake.initiateHandshake(false);
    ASSERT_FALSE(droneHello.empty());
    EXPECT_EQ(droneHandshake.getState(), LSPT::HandshakeState::DroneHello);

    // Ground Station processes Drone's hello and responds
    auto groundStationResponse = groundStationHandshake.handleHandshakeMessage(droneHello);
    ASSERT_FALSE(groundStationResponse.empty());
    EXPECT_EQ(groundStationHandshake.getState(), LSPT::HandshakeState::Established);

    // Drone processes Ground Station's response
    auto droneFinished = droneHandshake.handleHandshakeMessage(groundStationResponse);
    EXPECT_TRUE(droneFinished.empty());  // No more messages needed
    EXPECT_EQ(droneHandshake.getState(), LSPT::HandshakeState::Established);

    // Verify shared secrets match
    EXPECT_FALSE(droneHandshake.getSharedSecret().empty());
    EXPECT_FALSE(groundStationHandshake.getSharedSecret().empty());
    EXPECT_EQ(droneHandshake.getSharedSecret(), groundStationHandshake.getSharedSecret());
}

TEST_F(HandshakeTest, GroundStationInitiatedHandshake) {
    // Ground Station initiates handshake
    auto groundStationHello = groundStationHandshake.initiateHandshake(true);
    ASSERT_FALSE(groundStationHello.empty());
    EXPECT_EQ(groundStationHandshake.getState(), LSPT::HandshakeState::GroundStationHello);

    // Drone processes Ground Station's hello and responds
    auto droneResponse = droneHandshake.handleHandshakeMessage(groundStationHello);
    ASSERT_FALSE(droneResponse.empty());
    EXPECT_EQ(droneHandshake.getState(), LSPT::HandshakeState::Established);

    // Ground Station processes Drone's response
    auto groundStationFinished = groundStationHandshake.handleHandshakeMessage(droneResponse);
    EXPECT_TRUE(groundStationFinished.empty());  // No more messages needed
    EXPECT_EQ(groundStationHandshake.getState(), LSPT::HandshakeState::Established);

    // Verify shared secrets match
    EXPECT_FALSE(droneHandshake.getSharedSecret().empty());
    EXPECT_FALSE(groundStationHandshake.getSharedSecret().empty());
    EXPECT_EQ(droneHandshake.getSharedSecret(), groundStationHandshake.getSharedSecret());
}

TEST_F(HandshakeTest, ExceptionOnUnexpectedMessage) {
    // Initiate handshake
    auto droneHello = droneHandshake.initiateHandshake(false);
    EXPECT_EQ(droneHandshake.getState(), LSPT::HandshakeState::DroneHello);

    // Attempt to initiate again should throw an exception
    EXPECT_THROW(droneHandshake.initiateHandshake(false), std::runtime_error);

    // Complete handshake
    auto groundStationResponse = groundStationHandshake.handleHandshakeMessage(droneHello);
    EXPECT_EQ(groundStationHandshake.getState(), LSPT::HandshakeState::Established);
    
    auto droneFinished = droneHandshake.handleHandshakeMessage(groundStationResponse);
    EXPECT_EQ(droneHandshake.getState(), LSPT::HandshakeState::Established);
    EXPECT_TRUE(droneFinished.empty());

    // Attempt to handle message after handshake is established should throw an exception
    EXPECT_THROW(droneHandshake.handleHandshakeMessage(std::vector<uint8_t>{0x00}), std::runtime_error);
}

TEST_F(HandshakeTest, ExceptionOnPrematureSharedSecretAccess) {
    // Attempt to access shared secret before handshake is complete should throw an exception
    EXPECT_THROW(droneHandshake.getSharedSecret(), std::runtime_error);
}