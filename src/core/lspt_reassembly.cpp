#include "lspt_reassembly.h"
#include <algorithm>

LSPTPackage LSPTReassembly::reassembleChunks(const std::vector<LSPTChunk>& chunks) {
    // Sort chunks by sequence number
    std::vector<LSPTChunk> sortedChunks = chunks;
    std::sort(sortedChunks.begin(), sortedChunks.end(), 
              [](const LSPTChunk& a, const LSPTChunk& b) {
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

    return LSPTPackage(reassembledData);
}