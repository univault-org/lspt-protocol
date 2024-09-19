#include <gtest/gtest.h>
#include "../../src/core/lspt_retransmission.h"
#include "../../src/core/lspt_packet.h"
#include <vector>
#include <cstdint>

TEST(LSPTRetransmissionTest, AcknowledgeReceivedPacket) {
    LSPT::RetransmissionManager manager;
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o'};
    LSPTPacket packet(1, 12345, 1, 10, payload);

    manager.packetSent(packet);
    manager.packetReceived(packet.getSequenceNumber());

    EXPECT_FALSE(manager.needsRetransmission(packet.getSequenceNumber()));
}

TEST(LSPTRetransmissionTest, DetectLostPacket) {
    LSPT::RetransmissionManager manager;
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o'};
    LSPTPacket packet1(1, 12345, 1, 10, payload);
    LSPTPacket packet2(1, 12345, 2, 10, payload);

    manager.packetSent(packet1);
    manager.packetSent(packet2);
    manager.packetReceived(packet2.getSequenceNumber());  // Simulate packet1 loss

    EXPECT_TRUE(manager.needsRetransmission(packet1.getSequenceNumber()));
}

TEST(LSPTRetransmissionTest, RetransmitLostPacket) {
    LSPT::RetransmissionManager manager;
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o'};
    LSPTPacket packet(1, 12345, 1, 10, payload);

    manager.packetSent(packet);
    manager.packetReceived(packet.getSequenceNumber());  // Simulate packet loss

    if (manager.needsRetransmission(packet.getSequenceNumber())) {
        manager.packetSent(packet);  // Retransmit packet
    }

    EXPECT_FALSE(manager.needsRetransmission(packet.getSequenceNumber()));
}

TEST(LSPTRetransmissionTest, HandleOutOfOrderPackets) {
    LSPT::RetransmissionManager manager;
    std::vector<uint8_t> payload = {'H', 'e', 'l', 'l', 'o'};
    LSPTPacket packet1(1, 12345, 1, 10, payload);
    LSPTPacket packet2(1, 12345, 2, 10, payload);

    manager.packetSent(packet1);
    manager.packetSent(packet2);
    manager.packetReceived(packet2.getSequenceNumber());  // Receive out of order

    EXPECT_TRUE(manager.needsRetransmission(packet1.getSequenceNumber()));
    EXPECT_FALSE(manager.needsRetransmission(packet2.getSequenceNumber()));
}