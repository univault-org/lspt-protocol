// lspt-protocol/tests/satellite/test_satellite_api.cpp

#include <gtest/gtest.h>
#include "lspt_satellite.h"

class SatelliteAPITest : public ::testing::Test {
protected:
    void SetUp(LSPT::Satellite::Provider provider) {
        config.setProvider(provider);
        std::cout << "SetUp: Provider set to " << (config.getProvider() == LSPT::Satellite::Provider::STARLINK ? "STARLINK" : "IRIDIUM") << std::endl;
        
        ASSERT_EQ(config.getProvider(), provider);
        
        session = std::make_unique<LSPT::Satellite::SatelliteSession>(config);
    }

    void TearDown() override {
        // Clean up code here
    }

    void RunDataTransferTest() {
        std::cout << "DataTransferTest: Connecting to satellite" << std::endl;
        bool connected = session->Connect("SAT001");
        std::cout << "DataTransferTest: Connect result: " << (connected ? "Success" : "Failure") << std::endl;
        ASSERT_TRUE(connected);

        std::cout << "DataTransferTest: Creating satellite stream" << std::endl;
        auto stream = session->CreateSatelliteStream();
        std::cout << "DataTransferTest: Stream created: " << (stream != nullptr ? "Success" : "Failure") << std::endl;
        ASSERT_NE(stream, nullptr);

        LSPT::ByteVector testData = {1, 2, 3, 4, 5};
        std::cout << "DataTransferTest: Writing data: ";
        for (auto byte : testData) std::cout << static_cast<int>(byte) << " ";
        std::cout << std::endl;
        
        bool writeResult = stream->Write(testData);
        std::cout << "DataTransferTest: Write result: " << (writeResult ? "Success" : "Failure") << std::endl;
        EXPECT_TRUE(writeResult);

        LSPT::ByteVector receivedData;
        std::cout << "DataTransferTest: Reading data" << std::endl;
        bool readResult = stream->Read(receivedData);
        std::cout << "DataTransferTest: Read result: " << (readResult ? "Success" : "Failure") << std::endl;
        EXPECT_TRUE(readResult);
        
        std::cout << "DataTransferTest: Received data: ";
        for (auto byte : receivedData) std::cout << static_cast<int>(byte) << " ";
        std::cout << std::endl;
        
        std::cout << "DataTransferTest: Comparing sent and received data" << std::endl;
        bool dataEqual = (testData == receivedData);
        std::cout << "DataTransferTest: Data comparison result: " << (dataEqual ? "Match" : "Mismatch") << std::endl;
        EXPECT_EQ(testData, receivedData);

        std::cout << "DataTransferTest: Disconnecting" << std::endl;
        bool disconnected = session->Disconnect();
        std::cout << "DataTransferTest: Disconnect result: " << (disconnected ? "Success" : "Failure") << std::endl;
        EXPECT_TRUE(disconnected);
    }

    void RunProviderCommandTest() {
        std::cout << "ProviderCommandTest: Connecting to satellite" << std::endl;
        bool connected = session->Connect("SAT001");
        std::cout << "ProviderCommandTest: Connect result: " << (connected ? "Success" : "Failure") << std::endl;
        ASSERT_TRUE(connected);

        std::string command = "TEST_COMMAND";
        std::string response;
        std::cout << "ProviderCommandTest: Executing command: " << command << std::endl;
        bool commandExecuted = session->ExecuteProviderCommand(command, response);
        std::cout << "ProviderCommandTest: Command execution result: " << (commandExecuted ? "Success" : "Failure") << std::endl;
        EXPECT_TRUE(commandExecuted);

        std::cout << "ProviderCommandTest: Response received: " << response << std::endl;
        EXPECT_FALSE(response.empty());

        std::cout << "ProviderCommandTest: Disconnecting" << std::endl;
        bool disconnected = session->Disconnect();
        std::cout << "ProviderCommandTest: Disconnect result: " << (disconnected ? "Success" : "Failure") << std::endl;
        EXPECT_TRUE(disconnected);
    }

    LSPT::Satellite::SatelliteConfig config;
    std::unique_ptr<LSPT::Satellite::SatelliteSession> session;
};

// Starlink tests
TEST_F(SatelliteAPITest, StarlinkConfigurationTest) {
    SetUp(LSPT::Satellite::Provider::STARLINK);
    config.setProviderSpecificOption("beam_id", "B123");
    EXPECT_NO_THROW(LSPT::Satellite::SatelliteSession testSession(config));
}

TEST_F(SatelliteAPITest, StarlinkConnectionTest) {
    SetUp(LSPT::Satellite::Provider::STARLINK);
    EXPECT_TRUE(session->Connect("SAT001"));
    EXPECT_TRUE(session->Disconnect());
}

TEST_F(SatelliteAPITest, StarlinkDataTransferTest) {
    SetUp(LSPT::Satellite::Provider::STARLINK);
    RunDataTransferTest();
}

// TEST_F(SatelliteAPITest, StarlinkProviderCommandTest) {
//     SetUp(LSPT::Satellite::Provider::STARLINK);
//     RunProviderCommandTest();
// }

TEST_F(SatelliteAPITest, StarlinkConfigTest) {
    SetUp(LSPT::Satellite::Provider::STARLINK);
    LSPT::Satellite::SatelliteConfig testConfig;
    testConfig.setProvider(LSPT::Satellite::Provider::STARLINK);
    EXPECT_EQ(testConfig.getProvider(), LSPT::Satellite::Provider::STARLINK);
}

// Iridium tests
TEST_F(SatelliteAPITest, IridiumConfigurationTest) {
    SetUp(LSPT::Satellite::Provider::IRIDIUM);
    config.setProviderSpecificOption("beam_id", "B123");
    EXPECT_NO_THROW(LSPT::Satellite::SatelliteSession testSession(config));
}

TEST_F(SatelliteAPITest, IridiumConnectionTest) {
    SetUp(LSPT::Satellite::Provider::IRIDIUM);
    EXPECT_TRUE(session->Connect("SAT001"));
    EXPECT_TRUE(session->Disconnect());
}

TEST_F(SatelliteAPITest, IridiumDataTransferTest) {
    SetUp(LSPT::Satellite::Provider::IRIDIUM);
    RunDataTransferTest();
}

// TEST_F(SatelliteAPITest, IridiumProviderCommandTest) {
//     SetUp(LSPT::Satellite::Provider::IRIDIUM);
//     RunProviderCommandTest();
// }

TEST_F(SatelliteAPITest, IridiumConfigTest) {
    SetUp(LSPT::Satellite::Provider::IRIDIUM);
    LSPT::Satellite::SatelliteConfig testConfig;
    testConfig.setProvider(LSPT::Satellite::Provider::IRIDIUM);
    EXPECT_EQ(testConfig.getProvider(), LSPT::Satellite::Provider::IRIDIUM);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
