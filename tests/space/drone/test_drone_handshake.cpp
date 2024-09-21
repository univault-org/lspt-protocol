#include <gtest/gtest.h>
#include "../../../src/space/drone/drone_handshake.h"
#include "../../../src/ground/ground_handshake.h"

TEST(DroneHandshakeTest, DroneInitiatedHandshake) {
    LSPT::Space::Drone::DroneHandshake droneHandler;
    LSPT::Ground::GroundHandshake groundHandler;

    // Drone initiates handshake
    auto droneHello = droneHandler.initiateHandshake();
    ASSERT_FALSE(droneHello.empty());
    EXPECT_EQ(droneHello[0], LSPT::Common::SPACE_IDENTIFIER);

    // Ground processes Drone's hello and responds
    auto groundResponse = groundHandler.handleHandshakeMessage(droneHello);
    ASSERT_FALSE(groundResponse.empty());
    EXPECT_EQ(groundResponse[0], LSPT::Common::GROUND_IDENTIFIER);

    // Drone processes Ground's response
    auto droneFinished = droneHandler.handleHandshakeMessage(groundResponse);
    EXPECT_TRUE(droneFinished.empty());  // No more messages needed

    // Verify shared secrets match
    EXPECT_EQ(droneHandler.getSharedSecret(), groundHandler.getSharedSecret());
}