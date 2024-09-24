#pragma once

#include <vector>
#include <string>
#include <cstdint>

// Forward declaration
class SRPTPackage;

class SRPTChunk {
public:
    SRPTChunk(const std::string& packageId, size_t sequenceNumber, const std::vector<uint8_t>& data);
    size_t getSize() const;
    size_t getSequenceNumber() const;
    std::string getPackageId() const;
    const std::vector<uint8_t>& getData() const;

private:
    std::string packageId;
    size_t sequenceNumber;
    std::vector<uint8_t> data;
};

class SRPTChunking {
public:
    explicit SRPTChunking(size_t chunkSize);
    std::vector<SRPTChunk> createChunks(const SRPTPackage& package);

private:
    size_t chunkSize;
};