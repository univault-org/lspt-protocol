#include <gtest/gtest.h>
#include "../../src/core/srpt_retransmission.h"
#include "../../src/core/srpt_packet.h"
#include <vector>
#include <cstdint>

TEST(SRPTRetransmissionTest, AcknowledgeReceivedPacket) {
    SRPT::RetransmissionManager manager;
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o'};
    SRPTPacket packet(1, 12345, 1, 10, payload);

    manager.packetSent(packet);
    manager.packetReceived(packet.getSequenceNumber());

    EXPECT_FALSE(manager.needsRetransmission(packet.getSequenceNumber()));
}

TEST(SRPTRetransmissionTest, DetectLostPacket) {
    SRPT::RetransmissionManager manager;
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o'};
    SRPTPacket packet1(1, 12345, 1, 10, payload);
    SRPTPacket packet2(1, 12345, 2, 10, payload);

    manager.packetSent(packet1);
    manager.packetSent(packet2);
    manager.packetReceived(packet2.getSequenceNumber());  // Simulate packet1 loss

    EXPECT_TRUE(manager.needsRetransmission(packet1.getSequenceNumber()));
}

TEST(SRPTRetransmissionTest, RetransmitLostPacket) {
    SRPT::RetransmissionManager manager;
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o'};
    SRPTPacket packet(1, 12345, 1, 10, payload);

    manager.packetSent(packet);
    manager.packetReceived(packet.getSequenceNumber());  // Simulate packet loss

    if (manager.needsRetransmission(packet.getSequenceNumber())) {
        manager.packetSent(packet);  // Retransmit packet
    }

    EXPECT_FALSE(manager.needsRetransmission(packet.getSequenceNumber()));
}

TEST(SRPTRetransmissionTest, HandleOutOfOrderPackets) {
    SRPT::RetransmissionManager manager;
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o'};
    SRPTPacket packet1(1, 12345, 1, 10, payload);
    SRPTPacket packet2(1, 12345, 2, 10, payload);

    manager.packetSent(packet1);
    manager.packetSent(packet2);
    manager.packetReceived(packet2.getSequenceNumber());  // Receive out of order

    EXPECT_TRUE(manager.needsRetransmission(packet1.getSequenceNumber()));
    EXPECT_FALSE(manager.needsRetransmission(packet2.getSequenceNumber()));
}