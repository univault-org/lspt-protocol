#include <gtest/gtest.h>
#include <stdexcept>
#include "../../src/core/srpt_packet.h"

TEST(SRPTPacketTest, CreatePacket) {
    std::vector<uint8_t> payload = {1, 2, 3, 4, 5};
    SRPTPacket packet(1, 12345, 1, 10, payload);

    EXPECT_EQ(packet.getPacketType(), 1);
    EXPECT_EQ(packet.getPackageId(), 12345);
    EXPECT_EQ(packet.getSequenceNumber(), 1);
    EXPECT_EQ(packet.getTotalPackets(), 10);
    EXPECT_EQ(packet.getPayload(), payload);
    EXPECT_EQ(packet.getHeader().payloadSize, 5);
}

TEST(SRPTPacketTest, SerializeAndDeserialize) {
    std::vector<uint8_t> payload = {1, 2, 3, 4, 5};
    SRPTPacket originalPacket(1, 12345, 1, 10, payload);

    std::vector<uint8_t> serialized = originalPacket.toBytes();
    SRPTPacket deserializedPacket = SRPTPacket::fromBytes(serialized);

    EXPECT_EQ(originalPacket.getPacketType(), deserializedPacket.getPacketType());
    EXPECT_EQ(originalPacket.getPackageId(), deserializedPacket.getPackageId());
    EXPECT_EQ(originalPacket.getSequenceNumber(), deserializedPacket.getSequenceNumber());
    EXPECT_EQ(originalPacket.getTotalPackets(), deserializedPacket.getTotalPackets());
    EXPECT_EQ(originalPacket.getHeader().payloadSize, deserializedPacket.getHeader().payloadSize);
    EXPECT_EQ(originalPacket.getPayload(), deserializedPacket.getPayload());
}

TEST(SRPTPacketTest, ChecksumValidation) {
    std::vector<uint8_t> payload = {1, 2, 3, 4, 5};
    SRPTPacket packet(1, 12345, 1, 10, payload);

    std::vector<uint8_t> serialized = packet.toBytes();
    serialized[0] ^= 0xFF;  // Corrupt the first byte

    EXPECT_THROW(SRPTPacket::fromBytes(serialized), std::runtime_error);
}

TEST(SRPTPacketTest, InvalidPacketSize) {
    std::vector<uint8_t> tooSmall(1, 0);  // A vector with just one byte
    EXPECT_THROW(SRPTPacket::fromBytes(tooSmall), std::runtime_error);
}

TEST(SRPTPacketTest, VariableLengthEncoding) {
    std::vector<uint8_t> payload = {1, 2, 3};
    SRPTPacket packet1(1, 127, 1, 10, payload);
    SRPTPacket packet2(1, 128, 1, 10, payload);
    SRPTPacket packet3(1, 16383, 1, 10, payload);
    SRPTPacket packet4(1, 16384, 1, 10, payload);

    EXPECT_EQ(packet1.getHeader().packageId.size(), 1);
    EXPECT_EQ(packet2.getHeader().packageId.size(), 2);
    EXPECT_EQ(packet3.getHeader().packageId.size(), 2);
    EXPECT_EQ(packet4.getHeader().packageId.size(), 3);
}

TEST(SRPTPacketTest, LargeValues) {
    std::vector<uint8_t> payload(65535, 1);  // Maximum payload size
    SRPTPacket packet(15, UINT64_MAX, UINT32_MAX, UINT32_MAX, payload);

    std::vector<uint8_t> serialized = packet.toBytes();
    SRPTPacket deserializedPacket = SRPTPacket::fromBytes(serialized);

    EXPECT_EQ(deserializedPacket.getPacketType(), 15);
    EXPECT_EQ(deserializedPacket.getPackageId(), UINT64_MAX);
    EXPECT_EQ(deserializedPacket.getSequenceNumber(), UINT32_MAX);
    EXPECT_EQ(deserializedPacket.getTotalPackets(), UINT32_MAX);
    EXPECT_EQ(deserializedPacket.getHeader().payloadSize, 65535);
    EXPECT_EQ(deserializedPacket.getPayload(), payload);
}