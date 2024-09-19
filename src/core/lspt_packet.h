#pragma once

#include <cstdint>
#include <vector>

struct LSPTPacketHeader {
    uint8_t flags;  // Contains version (4 bits) and packet type (4 bits)
    std::vector<uint8_t> packageId;  // Variable-length, up to 8 bytes
    std::vector<uint8_t> sequenceNumber;  // Variable-length, up to 4 bytes
    std::vector<uint8_t> totalPackets;  // Variable-length, up to 4 bytes
    uint16_t payloadSize;
    uint32_t crc; // CRC-32C
};

class LSPTPacket {
public:
    LSPTPacket(uint8_t packetType, uint64_t packageId, uint32_t sequenceNumber, 
               uint32_t totalPackets, const std::vector<uint8_t>& payload);
    
    static LSPTPacket fromBytes(const std::vector<uint8_t>& bytes);
    std::vector<uint8_t> toBytes() const;

    const LSPTPacketHeader& getHeader() const { return header; }
    const std::vector<uint8_t>& getPayload() const { return payload; }

    // New public methods to access decoded values
    uint8_t getPacketType() const { return header.flags & 0x0F; }
    uint64_t getPackageId() const;
    uint32_t getSequenceNumber() const;
    uint32_t getTotalPackets() const;

private:
    LSPTPacketHeader header;
    std::vector<uint8_t> payload;

    void calculateCRC(); 
    static uint32_t computeChecksum(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> encodeVariableLength(uint64_t value);
    static uint64_t decodeVariableLength(const std::vector<uint8_t>& bytes);
};