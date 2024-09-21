#include <gtest/gtest.h>
#include "../../src/ground/ground_handshake.h"
#include "../../src/space/drone/drone_handshake.h"      // Updated include
#include "../../src/space/satellite/satellite_handshake.h"  // Updated include

TEST(GroundHandshakeTest, GroundInitiatedHandshake) {
    LSPT::Ground::GroundHandshake groundHandler;
    LSPT::Space::Drone::DroneHandshake droneHandler;  // Updated to use DroneHandshake
    LSPT::Space::Satellite::SatelliteHandshake satelliteHandler;  // Updated to use SatelliteHandshake

    // Ground initiates handshake
    auto groundHello = groundHandler.initiateHandshake();
    ASSERT_FALSE(groundHello.empty());
    EXPECT_EQ(groundHello[0], 0x01);  // Ground identifier

    // Drone processes Ground's hello and responds
    auto droneResponse = droneHandler.handleHandshakeMessage(groundHello);
    ASSERT_FALSE(droneResponse.empty());
    EXPECT_EQ(droneResponse[0], 0x02);  // Drone identifier (example)

    // Ground processes Drone's response
    auto groundFinished = groundHandler.handleHandshakeMessage(droneResponse);
    EXPECT_TRUE(groundFinished.empty());  // No more messages needed

    // Verify shared secrets match
    EXPECT_EQ(groundHandler.getSharedSecret(), droneHandler.getSharedSecret());

    // Satellite processes Ground's hello and responds
    auto satelliteResponse = satelliteHandler.handleHandshakeMessage(groundHello);
    ASSERT_FALSE(satelliteResponse.empty());
    EXPECT_EQ(satelliteResponse[0], 0x03);  // Satellite identifier (example)

    // Ground processes Satellite's response
    groundFinished = groundHandler.handleHandshakeMessage(satelliteResponse);
    EXPECT_TRUE(groundFinished.empty());  // No more messages needed

    // Verify shared secrets match
    EXPECT_EQ(groundHandler.getSharedSecret(), satelliteHandler.getSharedSecret());
}