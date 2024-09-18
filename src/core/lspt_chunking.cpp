#include "lspt_chunking.h"
#include "lspt_package.h"

LSPTChunk::LSPTChunk(const std::string& packageId, size_t sequenceNumber, const std::vector<uint8_t>& data)
    : packageId(packageId), sequenceNumber(sequenceNumber), data(data) {}

size_t LSPTChunk::getSize() const { return data.size(); }
size_t LSPTChunk::getSequenceNumber() const { return sequenceNumber; }
std::string LSPTChunk::getPackageId() const { return packageId; }
const std::vector<uint8_t>& LSPTChunk::getData() const { return data; }

LSPTChunking::LSPTChunking(size_t chunkSize) : chunkSize(chunkSize) {}

std::vector<LSPTChunk> LSPTChunking::createChunks(const LSPTPackage& package) {
    std::vector<LSPTChunk> chunks;
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