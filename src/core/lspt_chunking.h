#pragma once

#include <vector>
#include <string>
#include <cstdint>

// Forward declaration
class LSPTPackage;

class LSPTChunk {
public:
    LSPTChunk(const std::string& packageId, size_t sequenceNumber, const std::vector<uint8_t>& data);
    size_t getSize() const;
    size_t getSequenceNumber() const;
    std::string getPackageId() const;
    const std::vector<uint8_t>& getData() const;

private:
    std::string packageId;
    size_t sequenceNumber;
    std::vector<uint8_t> data;
};

class LSPTChunking {
public:
    explicit LSPTChunking(size_t chunkSize);
    std::vector<LSPTChunk> createChunks(const LSPTPackage& package);

private:
    size_t chunkSize;
};