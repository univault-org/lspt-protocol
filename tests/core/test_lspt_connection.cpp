#include <gtest/gtest.h>
#include "../../src/core/lspt_connection.h"
#include <chrono>
#include <thread>

using namespace LSPT;  

class LSPTConnectionTest : public ::testing::Test {
protected:
    LSPTConnection client;
    LSPTConnection server;

    void SetUp() override {
        // Any common setup can go here
    }
};

TEST_F(LSPTConnectionTest, InitialState) {
    EXPECT_EQ(client.getState(), LSPTConnectionState::CLOSED);
    EXPECT_EQ(server.getState(), LSPTConnectionState::CLOSED);
}

TEST_F(LSPTConnectionTest, ConnectionEstablishment) {
    // Client initiates connection
    EXPECT_TRUE(client.initiate());
    EXPECT_EQ(client.getState(), LSPTConnectionState::SYN_SENT);

    // Server receives SYN and responds
    EXPECT_TRUE(server.handleIncomingSYN());
    EXPECT_EQ(server.getState(), LSPTConnectionState::SYN_RECEIVED);

    // Client receives SYN-ACK and completes handshake
    EXPECT_TRUE(client.handleIncomingSYNACK());
    EXPECT_EQ(client.getState(), LSPTConnectionState::ESTABLISHED);

    // Server receives ACK and completes handshake
    EXPECT_TRUE(server.handleIncomingACK());
    EXPECT_EQ(server.getState(), LSPTConnectionState::ESTABLISHED);
}

TEST_F(LSPTConnectionTest, ConnectionTermination) {
    // Set up established connection
    client.setState(LSPTConnectionState::ESTABLISHED);
    server.setState(LSPTConnectionState::ESTABLISHED);

    // Client initiates termination
    EXPECT_TRUE(client.initiateClose());
    EXPECT_EQ(client.getState(), LSPTConnectionState::FIN_WAIT_1);

    // Server receives FIN and acknowledges
    EXPECT_TRUE(server.handleIncomingFIN());
    EXPECT_EQ(server.getState(), LSPTConnectionState::CLOSE_WAIT);

    // Client receives ACK of FIN
    EXPECT_TRUE(client.handleIncomingACK());
    EXPECT_EQ(client.getState(), LSPTConnectionState::FIN_WAIT_2);

    // Server closes its end
    EXPECT_TRUE(server.close());
    EXPECT_EQ(server.getState(), LSPTConnectionState::LAST_ACK);

    // Client receives FIN and acknowledges
    EXPECT_TRUE(client.handleIncomingFIN());
    EXPECT_EQ(client.getState(), LSPTConnectionState::TIME_WAIT);

    // Server receives final ACK
    EXPECT_TRUE(server.handleIncomingACK());
    EXPECT_EQ(server.getState(), LSPTConnectionState::CLOSED);

    // Client's TIME_WAIT expires
    client.simulateTimeWaitTimeout();
    EXPECT_EQ(client.getState(), LSPTConnectionState::CLOSED);
}

TEST_F(LSPTConnectionTest, SimultaneousClose) {
    client.setState(LSPTConnectionState::ESTABLISHED);
    server.setState(LSPTConnectionState::ESTABLISHED);

    // Both sides initiate close simultaneously
    EXPECT_TRUE(client.initiateClose());
    EXPECT_TRUE(server.initiateClose());
    EXPECT_EQ(client.getState(), LSPTConnectionState::FIN_WAIT_1);
    EXPECT_EQ(server.getState(), LSPTConnectionState::FIN_WAIT_1);

    // Both sides receive FIN
    EXPECT_TRUE(client.handleIncomingFIN());
    EXPECT_TRUE(server.handleIncomingFIN());
    EXPECT_EQ(client.getState(), LSPTConnectionState::CLOSING);
    EXPECT_EQ(server.getState(), LSPTConnectionState::CLOSING);

    // Both sides receive ACK
    EXPECT_TRUE(client.handleIncomingACK());
    EXPECT_TRUE(server.handleIncomingACK());
    EXPECT_EQ(client.getState(), LSPTConnectionState::TIME_WAIT);
    EXPECT_EQ(server.getState(), LSPTConnectionState::TIME_WAIT);

    // TIME_WAIT expires
    client.simulateTimeWaitTimeout();
    server.simulateTimeWaitTimeout();
    EXPECT_EQ(client.getState(), LSPTConnectionState::CLOSED);
    EXPECT_EQ(server.getState(), LSPTConnectionState::CLOSED);
}

TEST_F(LSPTConnectionTest, ConnectionTimeout) {
    // Client initiates connection
    EXPECT_TRUE(client.initiate());
    EXPECT_EQ(client.getState(), LSPTConnectionState::SYN_SENT);

    // Simulate timeout
    client.simulateTimeout();
    EXPECT_EQ(client.getState(), LSPTConnectionState::CLOSED);
}

TEST_F(LSPTConnectionTest, InvalidStateTransitions) {
    // Attempt to initiate connection from non-CLOSED state
    client.setState(LSPTConnectionState::ESTABLISHED);
    EXPECT_FALSE(client.initiate());
    EXPECT_EQ(client.getState(), LSPTConnectionState::ESTABLISHED);

    // Attempt to close from SYN_SENT state
    client.setState(LSPTConnectionState::SYN_SENT);
    EXPECT_FALSE(client.close());
    EXPECT_EQ(client.getState(), LSPTConnectionState::SYN_SENT);
}

TEST_F(LSPTConnectionTest, UnexpectedPacketsInVariousStates) {
    // Test unexpected SYN in ESTABLISHED state
    client.setState(LSPTConnectionState::ESTABLISHED);
    EXPECT_FALSE(client.handleIncomingSYN());
    EXPECT_EQ(client.getState(), LSPTConnectionState::ESTABLISHED);

    // Test unexpected SYNACK in CLOSED state
    client.setState(LSPTConnectionState::CLOSED);
    EXPECT_FALSE(client.handleIncomingSYNACK());
    EXPECT_EQ(client.getState(), LSPTConnectionState::CLOSED);

    // Test unexpected FIN in SYN_SENT state
    client.setState(LSPTConnectionState::SYN_SENT);
    EXPECT_FALSE(client.handleIncomingFIN());
    EXPECT_EQ(client.getState(), LSPTConnectionState::SYN_SENT);
}

TEST_F(LSPTConnectionTest, ConnectionResetScenarios) {
    // Test reset in ESTABLISHED state
    client.setState(LSPTConnectionState::ESTABLISHED);
    EXPECT_TRUE(client.handleReset());
    EXPECT_EQ(client.getState(), LSPTConnectionState::CLOSED);

    // Test reset in SYN_RECEIVED state
    server.setState(LSPTConnectionState::SYN_RECEIVED);
    EXPECT_TRUE(server.handleReset());
    EXPECT_EQ(server.getState(), LSPTConnectionState::CLOSED);

    // Test reset in FIN_WAIT_1 state
    client.setState(LSPTConnectionState::FIN_WAIT_1);
    EXPECT_TRUE(client.handleReset());
    EXPECT_EQ(client.getState(), LSPTConnectionState::CLOSED);
}

TEST_F(LSPTConnectionTest, ConnectionTimeouts) {
    // Test timeout in SYN_SENT state
    client.setState(LSPTConnectionState::SYN_SENT);
    client.simulateTimeout();
    EXPECT_EQ(client.getState(), LSPTConnectionState::CLOSED);

    // Test timeout in FIN_WAIT_2 state
    client.setState(LSPTConnectionState::FIN_WAIT_2);
    client.simulateTimeout();
    EXPECT_EQ(client.getState(), LSPTConnectionState::CLOSED);

    // Test timeout in CLOSE_WAIT state
    server.setState(LSPTConnectionState::CLOSE_WAIT);
    server.simulateTimeout();
    EXPECT_EQ(server.getState(), LSPTConnectionState::CLOSED);
}

class LSPTConnectionReliabilityTest : public ::testing::Test {
protected:
    LSPTConnection connection;

    void SetUp() override {
        connection.setState(LSPTConnectionState::ESTABLISHED);
    }
};

TEST_F(LSPTConnectionReliabilityTest, PacketRetransmission) {
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

TEST_F(LSPTConnectionReliabilityTest, KeepAlive) {
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

TEST_F(LSPTConnectionReliabilityTest, LongLivedConnection) {
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