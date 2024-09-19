#include "lspt_packet.h"
#include "lspt_error_detection.h"
#include <cstring>
#include <stdexcept>

constexpr uint8_t LSPT_CURRENT_VERSION = 1;

LSPTPacket::LSPTPacket(uint8_t packetType, uint64_t packageId, uint32_t sequenceNumber, 
                       uint32_t totalPackets, const std::vector<uint8_t>& payload)
    : payload(payload) {
    header.flags = (LSPT_CURRENT_VERSION << 4) | (packetType & 0x0F);
    header.packageId = encodeVariableLength(packageId);
    header.sequenceNumber = encodeVariableLength(sequenceNumber);
    header.totalPackets = encodeVariableLength(totalPackets);
    header.payloadSize = static_cast<uint16_t>(payload.size());
    calculateCRC();
}

void LSPTPacket::calculateCRC() {
    std::vector<uint8_t> dataToCheck;
    dataToCheck.push_back(header.flags);
    dataToCheck.insert(dataToCheck.end(), header.packageId.begin(), header.packageId.end());
    dataToCheck.insert(dataToCheck.end(), header.sequenceNumber.begin(), header.sequenceNumber.end());
    dataToCheck.insert(dataToCheck.end(), header.totalPackets.begin(), header.totalPackets.end());
    dataToCheck.push_back(header.payloadSize & 0xFF);
    dataToCheck.push_back((header.payloadSize >> 8) & 0xFF);
    dataToCheck.insert(dataToCheck.end(), payload.begin(), payload.end());
    
    header.crc = LSPT::calculateCRC32C(dataToCheck);
}

std::vector<uint8_t> LSPTPacket::encodeVariableLength(uint64_t value) {
    std::vector<uint8_t> encoded;
    do {
        uint8_t byte = value & 0x7F;
        value >>= 7;
        if (value > 0) {
            byte |= 0x80;
        }
        encoded.push_back(byte);
    } while (value > 0);
    return encoded;
}

uint64_t LSPTPacket::decodeVariableLength(const std::vector<uint8_t>& bytes) {
    uint64_t value = 0;
    int shift = 0;
    for (uint8_t byte : bytes) {
        value |= static_cast<uint64_t>(byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) {
            break;
        }
        shift += 7;
    }
    return value;
}

LSPTPacket LSPTPacket::fromBytes(const std::vector<uint8_t>& bytes) {
    if (bytes.size() < 8) {  // Minimum header size (flags + payloadSize + crc)
        throw std::runtime_error("Insufficient data for LSPT packet header");
    }

    size_t offset = 0;
    LSPTPacketHeader header;
    header.flags = bytes[offset++];

    auto readVariableLength = [&bytes, &offset]() {
        std::vector<uint8_t> value;
        while (offset < bytes.size() && (bytes[offset] & 0x80)) {
            value.push_back(bytes[offset++]);
        }
        if (offset < bytes.size()) {
            value.push_back(bytes[offset++]);
        }
        return value;
    };

    header.packageId = readVariableLength();
    header.sequenceNumber = readVariableLength();
    header.totalPackets = readVariableLength();

    if (bytes.size() - offset < 6) {  // payloadSize + crc
        throw std::runtime_error("Insufficient data for LSPT packet header");
    }

    header.payloadSize = (static_cast<uint16_t>(bytes[offset + 1]) << 8) | bytes[offset];
    offset += 2;

    std::memcpy(&header.crc, &bytes[offset], sizeof(uint32_t));
    offset += sizeof(uint32_t);

    if (bytes.size() != offset + header.payloadSize) {
        throw std::runtime_error("Packet size mismatch");
    }

    std::vector<uint8_t> payload(bytes.begin() + offset, bytes.end());

    uint8_t packetType = header.flags & 0x0F;
    uint64_t packageId = decodeVariableLength(header.packageId);
    uint32_t sequenceNumber = static_cast<uint32_t>(decodeVariableLength(header.sequenceNumber));
    uint32_t totalPackets = static_cast<uint32_t>(decodeVariableLength(header.totalPackets));

    LSPTPacket packet(packetType, packageId, sequenceNumber, totalPackets, payload);

    // Verify the CRC
    uint32_t receivedCRC = header.crc;
    packet.calculateCRC();
    if (receivedCRC != packet.header.crc) {
        throw std::runtime_error("CRC mismatch");
    }

    return packet;
}

std::vector<uint8_t> LSPTPacket::toBytes() const {
    std::vector<uint8_t> bytes;
    bytes.push_back(header.flags);
    bytes.insert(bytes.end(), header.packageId.begin(), header.packageId.end());
    bytes.insert(bytes.end(), header.sequenceNumber.begin(), header.sequenceNumber.end());
    bytes.insert(bytes.end(), header.totalPackets.begin(), header.totalPackets.end());
    bytes.push_back(header.payloadSize & 0xFF);
    bytes.push_back((header.payloadSize >> 8) & 0xFF);
    bytes.insert(bytes.end(), reinterpret_cast<const uint8_t*>(&header.crc),
                 reinterpret_cast<const uint8_t*>(&header.crc) + sizeof(uint32_t));
    bytes.insert(bytes.end(), payload.begin(), payload.end());
    return bytes;
}

uint64_t LSPTPacket::getPackageId() const {
    return decodeVariableLength(header.packageId);
}

uint32_t LSPTPacket::getSequenceNumber() const {
    return static_cast<uint32_t>(decodeVariableLength(header.sequenceNumber));
}

uint32_t LSPTPacket::getTotalPackets() const {
    return static_cast<uint32_t>(decodeVariableLength(header.totalPackets));
}