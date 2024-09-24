#include "srpt_chunking.h"
#include "srpt_package.h"

SRPTChunk::SRPTChunk(const std::string& packageId, size_t sequenceNumber, const std::vector<uint8_t>& data)
    : packageId(packageId), sequenceNumber(sequenceNumber), data(data) {}

size_t SRPTChunk::getSize() const { return data.size(); }
size_t SRPTChunk::getSequenceNumber() const { return sequenceNumber; }
std::string SRPTChunk::getPackageId() const { return packageId; }
const std::vector<uint8_t>& SRPTChunk::getData() const { return data; }

SRPTChunking::SRPTChunking(size_t chunkSize) : chunkSize(chunkSize) {}

std::vector<SRPTChunk> SRPTChunking::createChunks(const SRPTPackage& package) {
    std::vector<SRPTChunk> chunks;
    const std::vector<uint8_t>& packageData = package.getData();
    size_t packageSize = packageData.size();
    size_t numFullChunks = packageSize / chunkSize;
    size_t remainingBytes = packageSize % chunkSize;

    for (size_t i = 0; i < numFullChunks; ++i) {
        std::vector<uint8_t> chunkData(packageData.begin() + i * chunkSize, 
                                       packageData.begin() + (i + 1) * chunkSize);
        chunks.emplace_back(package.getId(), i, chunkData);
    }

    if (remainingBytes > 0) {
        std::vector<uint8_t> chunkData(packageData.end() - remainingBytes, packageData.end());
        chunks.emplace_back(package.getId(), numFullChunks, chunkData);
    }

    return chunks;
}