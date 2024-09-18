#include <gtest/gtest.h>
#include "../../src/core/lspt_connection.h"

TEST(LSPTDemoTest, BasicTest)
{
    EXPECT_EQ(1, 1);
}

TEST(LSPTDemoTest, CreateLSPTConnection)
{
    LSPTConnection connection;
    EXPECT_NE(nullptr, &connection);
}

TEST(LSPTDemoTest, ConnectionInitialization)
{
    LSPTConnection connection;
    EXPECT_FALSE(connection.isConnected());
}


// #include <gtest/gtest.h>
// #include "../../src/core/lspt_connection.h"
// #include "../../src/core/lspt_package.h"
// #include "../../src/satellite/satellite_transmitter.h"
// #include "../../src/crypto/encryption.h"
// #include "../../src/utils/test_utils.h"
// #include <chrono>
// #include <stdexcept>

// class LSPTDemoTest : public ::testing::Test {
// protected:
//     LSPTDemo demo;

//     void SetUp() override {
//         // Any setup code
//     }

//     void TearDown() override {
//         // Any cleanup code
//     }
// };

// TEST_F(LSPTDemoTest, SendLargePackageSuccess) {
//     Package testPackage = createTestPackage(1024 * 1024); // 1MB package
//     EXPECT_NO_THROW(demo.send_large_package(testPackage));
//     // Add more specific assertions based on expected behavior
// }

// TEST_F(LSPTDemoTest, SendLargePackageFailure) {
//     Package invalidPackage;
//     EXPECT_THROW(demo.send_large_package(invalidPackage), std::invalid_argument);
// }

// TEST_F(LSPTDemoTest, ReceiveLargePackageSuccess) {
//     // Setup: Simulate a package being sent
//     Package expectedPackage = createTestPackage(1024 * 1024);
//     demo.send_large_package(expectedPackage);

//     Package receivedPackage = demo.receive_large_package();
//     EXPECT_EQ(receivedPackage, expectedPackage);
// }

// TEST_F(LSPTDemoTest, ReceiveLargePackageCorrupted) {
//     // Setup: Simulate a corrupted package being sent
//     Package corruptedPackage = createCorruptedTestPackage();
//     demo.send_large_package(corruptedPackage);

//     EXPECT_THROW(demo.receive_large_package(), std::runtime_error);
// }

// TEST_F(LSPTDemoTest, PreparePackageCorrectly) {
//     Package originalPackage = createTestPackage(1024 * 1024);
//     Package preparedPackage = demo.prepare_package(originalPackage);

//     EXPECT_NE(preparedPackage, originalPackage);
//     // Add more specific assertions about the prepared package
// }

// TEST_F(LSPTDemoTest, VerifyPackageSuccess) {
//     Package validPackage = createTestPackage(1024 * 1024);
//     EXPECT_NO_THROW(demo.verify_package(validPackage));
// }

// TEST_F(LSPTDemoTest, VerifyPackageFailure) {
//     Package invalidPackage = createInvalidTestPackage();
//     EXPECT_THROW(demo.verify_package(invalidPackage), std::runtime_error);
// }

// Add more tests for other methods like initiate_transmission, monitor_transmission, etc.