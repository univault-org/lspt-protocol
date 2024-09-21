#include <gtest/gtest.h>
#include "../../src/space/drone/drone_handshake.h"
#include "../../src/ground/ground_handshake.h"

class HandshakeIntegrationTest : public ::testing::Test {
protected:
    LSPT::Space::Drone::DroneHandshake droneHandler;
    LSPT::Ground::GroundHandshake groundHandler;
    LSPT::Common::ByteVector droneHello;
    LSPT::Common::ByteVector groundResponse;
    LSPT::Common::ByteVector droneFinished;
    LSPT::Common::ByteVector groundFinished;

    void SetUp() override {
        droneHello = droneHandler.initiateHandshake();
    }
};

TEST_F(HandshakeIntegrationTest, DroneInitiatesHandshake) {
    ASSERT_FALSE(droneHello.empty());
    EXPECT_EQ(droneHello[0], LSPT::Common::SPACE_IDENTIFIER);
    EXPECT_EQ(droneHandler.getState(), LSPT::HandshakeState::HelloSent);
    std::cout << "Drone state after initiation: " << static_cast<int>(droneHandler.getState()) << std::endl;
}

TEST_F(HandshakeIntegrationTest, GroundProcessesDroneHello) {
    groundResponse = groundHandler.handleHandshakeMessage(droneHello);
    ASSERT_FALSE(groundResponse.empty());
    EXPECT_EQ(groundResponse[0], LSPT::Common::GROUND_IDENTIFIER);
    EXPECT_EQ(groundHandler.getState(), LSPT::HandshakeState::HelloSent);
    std::cout << "Ground state after processing drone hello: " << static_cast<int>(groundHandler.getState()) << std::endl;
}

TEST_F(HandshakeIntegrationTest, DroneProcessesGroundResponse) {
    groundResponse = groundHandler.handleHandshakeMessage(droneHello);
    droneFinished = droneHandler.handleHandshakeMessage(groundResponse);
    EXPECT_EQ(droneHandler.getState(), LSPT::HandshakeState::Established);
    std::cout << "Drone state after processing ground response: " << static_cast<int>(droneHandler.getState()) << std::endl;
    std::cout << "Drone finished message size: " << droneFinished.size() << std::endl;
}

TEST_F(HandshakeIntegrationTest, GroundProcessesDroneFinished) {
    groundResponse = groundHandler.handleHandshakeMessage(droneHello);
    droneFinished = droneHandler.handleHandshakeMessage(groundResponse);
    groundFinished = groundHandler.handleHandshakeMessage(droneFinished);
    EXPECT_EQ(groundHandler.getState(), LSPT::HandshakeState::Established);
    std::cout << "Ground state after processing drone final message: " << static_cast<int>(groundHandler.getState()) << std::endl;
}

TEST_F(HandshakeIntegrationTest, BothSidesEstablished) {
    groundResponse = groundHandler.handleHandshakeMessage(droneHello);
    droneFinished = droneHandler.handleHandshakeMessage(groundResponse);
    groundFinished = groundHandler.handleHandshakeMessage(droneFinished);

    EXPECT_EQ(droneHandler.getState(), LSPT::HandshakeState::Established);
    EXPECT_EQ(groundHandler.getState(), LSPT::HandshakeState::Established);
}

TEST_F(HandshakeIntegrationTest, SharedSecretsMatch) {
    groundResponse = groundHandler.handleHandshakeMessage(droneHello);
    droneFinished = droneHandler.handleHandshakeMessage(groundResponse);
    groundFinished = groundHandler.handleHandshakeMessage(droneFinished);

    ASSERT_EQ(droneHandler.getState(), LSPT::HandshakeState::Established);
    ASSERT_EQ(groundHandler.getState(), LSPT::HandshakeState::Established);

    const auto& droneSecret = droneHandler.getSharedSecret();
    const auto& groundSecret = groundHandler.getSharedSecret();

    EXPECT_FALSE(droneSecret.empty());
    EXPECT_FALSE(groundSecret.empty());

    std::cout << "Drone shared secret size: " << droneSecret.size() << std::endl;
    std::cout << "Ground shared secret size: " << groundSecret.size() << std::endl;

    // Print the first few bytes of each shared secret
    auto printBytes = [](const std::string& label, const LSPT::Common::ByteVector& data, size_t count = 8) {
        std::cout << label << ": ";
        for (size_t i = 0; i < count && i < data.size(); ++i) {
            printf("%02X ", static_cast<unsigned char>(data[i]));
        }
        std::cout << std::endl;
    };

    printBytes("Drone shared secret", droneSecret);
    printBytes("Ground shared secret", groundSecret);

    EXPECT_EQ(droneSecret, groundSecret);
}