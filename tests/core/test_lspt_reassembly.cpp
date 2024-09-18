#include <gtest/gtest.h>
#include "../../src/core/lspt_package.h"
#include "../../src/core/lspt_chunking.h"
#include "../../src/core/lspt_reassembly.h"
#include <algorithm>
#include <random>

TEST(LSPTReassemblyTest, ReassembleChunks) {
    // Create a package
    std::vector<uint8_t> originalData(1024 * 1024, 'A');  // 1 MB of 'A's
    LSPTPackage originalPackage(originalData);

    // Create chunks
    LSPTChunking chunking(256 * 1024);  // 256 KB chunks
    auto chunks = chunking.createChunks(originalPackage);

    // Reassemble chunks
    LSPTReassembly reassembly;
    auto reassembledPackage = reassembly.reassembleChunks(chunks);

    // Check if reassembled package matches the original
    EXPECT_EQ(originalPackage.getSize(), reassembledPackage.getSize());
    EXPECT_EQ(originalPackage.getData(), reassembledPackage.getData());
}

TEST(LSPTReassemblyTest, ReassembleOutOfOrderChunks) {
    // Create a package
    std::vector<uint8_t> originalData(1024 * 1024, 'A');  // 1 MB of 'A's
    LSPTPackage originalPackage(originalData);

    // Create chunks
    LSPTChunking chunking(256 * 1024);  // 256 KB chunks
    auto chunks = chunking.createChunks(originalPackage);

    // Shuffle chunks
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(chunks.begin(), chunks.end(), g);

    // Reassemble chunks
    LSPTReassembly reassembly;
    auto reassembledPackage = reassembly.reassembleChunks(chunks);

    // Check if reassembled package matches the original
    EXPECT_EQ(originalPackage.getSize(), reassembledPackage.getSize());
    EXPECT_EQ(originalPackage.getData(), reassembledPackage.getData());
}