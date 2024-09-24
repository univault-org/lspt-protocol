#include "srpt_reassembly.h"
#include <algorithm>

SRPTPackage SRPTReassembly::reassembleChunks(const std::vector<SRPTChunk>& chunks) {
    // Sort chunks by sequence number
    std::vector<SRPTChunk> sortedChunks = chunks;
    std::sort(sortedChunks.begin(), sortedChunks.end(), 
              [](const SRPTChunk& a, const SRPTChunk& b) {
                  return a.getSequenceNumber() < b.getSequenceNumber();
              });

    // Calculate total size
    size_t totalSize = 0;
    for (const auto& chunk : sortedChunks) {
        totalSize += chunk.getSize();
    }

    // Reassemble data
    std::vector<uint8_t> reassembledData;
    reassembledData.reserve(totalSize);
    for (const auto& chunk : sortedChunks) {
        reassembledData.insert(reassembledData.end(), 
                               chunk.getData().begin(), 
                               chunk.getData().end());
    }

    return SRPTPackage(reassembledData);
}