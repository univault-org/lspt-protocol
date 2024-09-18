#include <gtest/gtest.h>
#include "../../src/core/lspt_package.h"
#include "../../src/core/lspt_chunking.h"

TEST(LSPTChunkingTest, CreateChunks) {
    LSPTPackage package(1024 * 1024);  // 1 MB package
    LSPTChunking chunking(256 * 1024);  // 256 KB chunks
    auto chunks = chunking.createChunks(package);
    EXPECT_EQ(4, chunks.size());
}

TEST(LSPTChunkingTest, ChunkSize) {
    LSPTPackage package(1024 * 1024);  // 1 MB package
    LSPTChunking chunking(256 * 1024);  // 256 KB chunks
    auto chunks = chunking.createChunks(package);
    for (const auto& chunk : chunks) {
        EXPECT_EQ(256 * 1024, chunk.getSize());
    }
}

TEST(LSPTChunkingTest, LastChunkSize) {
    LSPTPackage package(1024 * 1024 + 100);  // 1 MB + 100 bytes package
    LSPTChunking chunking(256 * 1024);  // 256 KB chunks
    auto chunks = chunking.createChunks(package);
    EXPECT_EQ(256 * 1024, chunks[0].getSize());
    EXPECT_EQ(256 * 1024, chunks[1].getSize());
    EXPECT_EQ(256 * 1024, chunks[2].getSize());
    EXPECT_EQ(256 * 1024, chunks[3].getSize());
    EXPECT_EQ(100, chunks[4].getSize());
}

TEST(LSPTChunkingTest, ChunkIdentification) {
    LSPTPackage package(1024 * 1024);  // 1 MB package
    LSPTChunking chunking(256 * 1024);  // 256 KB chunks
    auto chunks = chunking.createChunks(package);
    for (size_t i = 0; i < chunks.size(); ++i) {
        EXPECT_EQ(i, chunks[i].getSequenceNumber());
        EXPECT_EQ(package.getId(), chunks[i].getPackageId());
    }
}