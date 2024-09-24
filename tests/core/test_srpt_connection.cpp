#include <gtest/gtest.h>
#include "../../src/core/srpt_connection.h"

using namespace SRPT;

class SRPTConnectionTest : public ::testing::Test {
protected:
    SRPTConnection client;
    SRPTConnection server;

    void SetUp() override {
        // Any common setup can go here
    }
};

TEST_F(SRPTConnectionTest, InitialState) {
    EXPECT_EQ(client.getState(), SRPTConnectionState::CLOSED);
    EXPECT_EQ(server.getState(), SRPTConnectionState::CLOSED);
}

TEST_F(SRPTConnectionTest, ConnectionEstablishment) {
    // Client initiates connection
    EXPECT_TRUE(client.initiate());
    EXPECT_EQ(client.getState(), SRPTConnectionState::SYN_SENT);

    // Server receives SYN and responds
    EXPECT_TRUE(server.handleIncomingSYN());
    EXPECT_EQ(server.getState(), SRPTConnectionState::SYN_RECEIVED);

    // Client receives SYN-ACK and completes handshake
    EXPECT_TRUE(client.handleIncomingSYNACK());
    EXPECT_EQ(client.getState(), SRPTConnectionState::ESTABLISHED);

    // Server receives ACK and completes handshake
    EXPECT_TRUE(server.handleIncomingACK());
    EXPECT_EQ(server.getState(), SRPTConnectionState::ESTABLISHED);
}

TEST_F(SRPTConnectionTest, ConnectionTermination) {
    // Set up established connection
    client.setState(SRPTConnectionState::ESTABLISHED);
    server.setState(SRPTConnectionState::ESTABLISHED);

    // Client initiates termination
    EXPECT_TRUE(client.initiateClose());
    EXPECT_EQ(client.getState(), SRPTConnectionState::FIN_WAIT_1);

    // Server receives FIN and acknowledges
    EXPECT_TRUE(server.handleIncomingFIN());
    EXPECT_EQ(server.getState(), SRPTConnectionState::CLOSE_WAIT);

    // Client receives ACK of FIN
    EXPECT_TRUE(client.handleIncomingACK());
    EXPECT_EQ(client.getState(), SRPTConnectionState::FIN_WAIT_2);

    // Server closes its end
    EXPECT_TRUE(server.close());
    EXPECT_EQ(server.getState(), SRPTConnectionState::LAST_ACK);

    // Client receives FIN and acknowledges
    EXPECT_TRUE(client.handleIncomingFIN());
    EXPECT_EQ(client.getState(), SRPTConnectionState::TIME_WAIT);

    // Server receives final ACK
    EXPECT_TRUE(server.handleIncomingACK());
    EXPECT_EQ(server.getState(), SRPTConnectionState::CLOSED);

    // Client's TIME_WAIT expires
    client.simulateTimeWaitTimeout();
    EXPECT_EQ(client.getState(), SRPTConnectionState::CLOSED);
}

TEST_F(SRPTConnectionTest, SimultaneousClose) {
    client.setState(SRPTConnectionState::ESTABLISHED);
    server.setState(SRPTConnectionState::ESTABLISHED);

    // Both sides initiate close simultaneously
    EXPECT_TRUE(client.initiateClose());
    EXPECT_TRUE(server.initiateClose());
    EXPECT_EQ(client.getState(), SRPTConnectionState::FIN_WAIT_1);
    EXPECT_EQ(server.getState(), SRPTConnectionState::FIN_WAIT_1);

    // Both sides receive FIN
    EXPECT_TRUE(client.handleIncomingFIN());
    EXPECT_TRUE(server.handleIncomingFIN());
    EXPECT_EQ(client.getState(), SRPTConnectionState::CLOSING);
    EXPECT_EQ(server.getState(), SRPTConnectionState::CLOSING);

    // Both sides receive ACK
    EXPECT_TRUE(client.handleIncomingACK());
    EXPECT_TRUE(server.handleIncomingACK());
    EXPECT_EQ(client.getState(), SRPTConnectionState::TIME_WAIT);
    EXPECT_EQ(server.getState(), SRPTConnectionState::TIME_WAIT);

    // TIME_WAIT expires
    client.simulateTimeWaitTimeout();
    server.simulateTimeWaitTimeout();
    EXPECT_EQ(client.getState(), SRPTConnectionState::CLOSED);
    EXPECT_EQ(server.getState(), SRPTConnectionState::CLOSED);
}

TEST_F(SRPTConnectionTest, ConnectionTimeout) {
    // Client initiates connection
    EXPECT_TRUE(client.initiate());
    EXPECT_EQ(client.getState(), SRPTConnectionState::SYN_SENT);

    // Simulate timeout
    client.simulateTimeout();
    EXPECT_EQ(client.getState(), SRPTConnectionState::CLOSED);
}

TEST_F(SRPTConnectionTest, InvalidStateTransitions) {
    // Attempt to initiate connection from non-CLOSED state
    client.setState(SRPTConnectionState::ESTABLISHED);
    EXPECT_FALSE(client.initiate());
    EXPECT_EQ(client.getState(), SRPTConnectionState::ESTABLISHED);

    // Attempt to close from SYN_SENT state
    client.setState(SRPTConnectionState::SYN_SENT);
    EXPECT_FALSE(client.close());
    EXPECT_EQ(client.getState(), SRPTConnectionState::SYN_SENT);
}

TEST_F(SRPTConnectionTest, UnexpectedPacketsInVariousStates) {
    // Test unexpected SYN in ESTABLISHED state
    client.setState(SRPTConnectionState::ESTABLISHED);
    EXPECT_FALSE(client.handleIncomingSYN());
    EXPECT_EQ(client.getState(), SRPTConnectionState::ESTABLISHED);

    // Test unexpected SYNACK in CLOSED state
    client.setState(SRPTConnectionState::CLOSED);
    EXPECT_FALSE(client.handleIncomingSYNACK());
    EXPECT_EQ(client.getState(), SRPTConnectionState::CLOSED);

    // Test unexpected FIN in SYN_SENT state
    client.setState(SRPTConnectionState::SYN_SENT);
    EXPECT_FALSE(client.handleIncomingFIN());
    EXPECT_EQ(client.getState(), SRPTConnectionState::SYN_SENT);
}

TEST_F(SRPTConnectionTest, ConnectionResetScenarios) {
    // Test reset in ESTABLISHED state
    client.setState(SRPTConnectionState::ESTABLISHED);
    EXPECT_TRUE(client.handleReset());
    EXPECT_EQ(client.getState(), SRPTConnectionState::CLOSED);

    // Test reset in SYN_RECEIVED state
    server.setState(SRPTConnectionState::SYN_RECEIVED);
    EXPECT_TRUE(server.handleReset());
    EXPECT_EQ(server.getState(), SRPTConnectionState::CLOSED);

    // Test reset in FIN_WAIT_1 state
    client.setState(SRPTConnectionState::FIN_WAIT_1);
    EXPECT_TRUE(client.handleReset());
    EXPECT_EQ(client.getState(), SRPTConnectionState::CLOSED);
}

TEST_F(SRPTConnectionTest, ConnectionTimeouts) {
    // Test timeout in SYN_SENT state
    client.setState(SRPTConnectionState::SYN_SENT);
    client.simulateTimeout();
    EXPECT_EQ(client.getState(), SRPTConnectionState::CLOSED);

    // Test timeout in FIN_WAIT_2 state
    client.setState(SRPTConnectionState::FIN_WAIT_2);
    client.simulateTimeout();
    EXPECT_EQ(client.getState(), SRPTConnectionState::CLOSED);

    // Test timeout in CLOSE_WAIT state
    server.setState(SRPTConnectionState::CLOSE_WAIT);
    server.simulateTimeout();
    EXPECT_EQ(server.getState(), SRPTConnectionState::CLOSED);
}

class SRPTConnectionReliabilityTest : public ::testing::Test {
protected:
    SRPTConnection connection;

    void SetUp() override {
        connection.setState(SRPTConnectionState::ESTABLISHED);
    }
};

TEST_F(SRPTConnectionReliabilityTest, PacketRetransmission) {
    // Send some packets
    EXPECT_TRUE(connection.sendPacket(1, {1, 2, 3}));
    EXPECT_TRUE(connection.sendPacket(2, {4, 5, 6}));
    EXPECT_TRUE(connection.sendPacket(3, {7, 8, 9}));

    EXPECT_EQ(connection.getUnacknowledgedPacketCount(), 3);

    // Acknowledge one packet
    EXPECT_TRUE(connection.acknowledgePacket(2));
    EXPECT_EQ(connection.getUnacknowledgedPacketCount(), 2);

    // Retransmit unacknowledged packets
    connection.retransmitUnacknowledgedPackets();
    EXPECT_EQ(connection.getUnacknowledgedPacketCount(), 2);

    // Acknowledge remaining packets
    EXPECT_TRUE(connection.acknowledgePacket(1));
    EXPECT_TRUE(connection.acknowledgePacket(3));
    EXPECT_EQ(connection.getUnacknowledgedPacketCount(), 0);
}

TEST_F(SRPTConnectionReliabilityTest, KeepAlive) {
    connection.setKeepAliveInterval(std::chrono::seconds(2));
    EXPECT_TRUE(connection.isConnectionAlive());

    // Simulate passage of time without activity
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_TRUE(connection.isConnectionAlive());

    // Send keep-alive
    connection.sendKeepAlive();
    EXPECT_TRUE(connection.isConnectionAlive());

    // Simulate more passage of time
    std::this_thread::sleep_for(std::chrono::seconds(3));
    EXPECT_FALSE(connection.isConnectionAlive());

    // Handle incoming keep-alive
    EXPECT_TRUE(connection.handleKeepAlive());
    EXPECT_TRUE(connection.isConnectionAlive());
}

TEST_F(SRPTConnectionReliabilityTest, LongLivedConnection) {
    connection.setKeepAliveInterval(std::chrono::seconds(2));
    EXPECT_TRUE(connection.isConnectionAlive());

    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        connection.sendKeepAlive();
        EXPECT_TRUE(connection.isConnectionAlive());
    }

    // Simulate network interruption
    std::this_thread::sleep_for(std::chrono::seconds(3));
    EXPECT_FALSE(connection.isConnectionAlive());

    // Restore connection
    EXPECT_TRUE(connection.handleKeepAlive());
    EXPECT_TRUE(connection.isConnectionAlive());
}

class SRPTFlowControlTest : public ::testing::Test {
protected:
    SRPT::SRPTConnection connection;

    void SetUp() override {
        connection.setState(SRPT::SRPTConnectionState::ESTABLISHED);
    }
};

TEST_F(SRPTFlowControlTest, InitialWindowSize) {
    connection.setReceiveWindowSize(65536);
    EXPECT_EQ(connection.getReceiveWindowSize(), 65536);
    EXPECT_EQ(connection.getAvailableWindowSize(), 65536);
}

TEST_F(SRPTFlowControlTest, UpdateAvailableWindowSize) {
    connection.setReceiveWindowSize(65536);
    connection.updateAvailableWindowSize(32768);
    EXPECT_EQ(connection.getAvailableWindowSize(), 32768);
}

TEST_F(SRPTFlowControlTest, CanSendData) {
    connection.setReceiveWindowSize(1000);
    EXPECT_TRUE(connection.canSendData(500));
    EXPECT_TRUE(connection.canSendData(1000));
    EXPECT_FALSE(connection.canSendData(1001));
}

TEST_F(SRPTFlowControlTest, SendDataReducesWindow) {
    connection.setReceiveWindowSize(1000);
    std::vector<uint8_t> data(500, 0);
    EXPECT_TRUE(connection.sendData(data));
    EXPECT_EQ(connection.getAvailableWindowSize(), 500);
}

TEST_F(SRPTFlowControlTest, ReceiveDataUpdatesWindow) {
    connection.setReceiveWindowSize(1000);
    std::vector<uint8_t> data(500, 0);
    connection.receiveData(data);
    // Assuming all data is processed immediately in the test
    EXPECT_EQ(connection.getAvailableWindowSize(), 1000);
}

TEST_F(SRPTFlowControlTest, WindowUpdateAfterProcessing) {
    connection.setReceiveWindowSize(1000);
    std::vector<uint8_t> data(500, 0);
    connection.receiveData(data);
    // Simulate processing of 250 bytes
    connection.updateAvailableWindowSize(750);
    EXPECT_EQ(connection.getAvailableWindowSize(), 750);
}

TEST_F(SRPTFlowControlTest, PreventSendingWhenWindowFull) {
    connection.setReceiveWindowSize(1000);
    std::vector<uint8_t> data(1000, 0);
    EXPECT_TRUE(connection.sendData(data));
    EXPECT_FALSE(connection.canSendData(1));
    std::vector<uint8_t> moreData(1, 0);
    EXPECT_FALSE(connection.sendData(moreData));
}

TEST_F(SRPTFlowControlTest, SendDataUpdatesLastActivity) {
    connection.setReceiveWindowSize(1000); // Ensure there's enough window size
    auto before = connection.getLastActivityTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::vector<uint8_t> data(100, 0);
    ASSERT_TRUE(connection.sendData(data));
    EXPECT_GT(connection.getLastActivityTime(), before);
}

TEST_F(SRPTFlowControlTest, ReceiveDataUpdatesLastActivity) {
    auto before = connection.getLastActivityTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::vector<uint8_t> data(100, 0);
    connection.receiveData(data);
    EXPECT_GT(connection.getLastActivityTime(), before);
}

TEST_F(SRPTFlowControlTest, ReceiveDataInNonEstablishedState) {
    connection.setState(SRPT::SRPTConnectionState::CLOSED);
    uint32_t initialWindow = connection.getAvailableWindowSize();
    std::vector<uint8_t> data(100, 0);
    connection.receiveData(data);
    EXPECT_EQ(connection.getAvailableWindowSize(), initialWindow);
}

TEST_F(SRPTFlowControlTest, SendWindowUpdate) {
    connection.setReceiveWindowSize(1000);
    connection.sendWindowUpdate(500);
    EXPECT_EQ(connection.getAvailableWindowSize(), 500);
}