#include <gtest/gtest.h>
#include "../../src/core/srpt_error_detection.h"
#include <vector>
#include <cstdint>

TEST(SRPTErrorDetectionTest, CalculateCRC32C) {
    std::vector<uint8_t> data = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    uint32_t expected_crc = 0xE3069283; // Pre-calculated CRC-32C for "123456789"
    
    EXPECT_EQ(SRPT::calculateCRC32C(data), expected_crc);
}

TEST(SRPTErrorDetectionTest, DetectNoError) {
    std::vector<uint8_t> data = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    uint32_t crc = SRPT::calculateCRC32C(data);
    
    EXPECT_TRUE(SRPT::verifyCRC32C(data, crc));
}

TEST(SRPTErrorDetectionTest, DetectError) {
    std::vector<uint8_t> data = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    uint32_t crc = SRPT::calculateCRC32C(data);
    
    // Introduce an error
    data[5] = 'X';
    
    EXPECT_FALSE(SRPT::verifyCRC32C(data, crc));
}

TEST(SRPTErrorDetectionTest, HandleEmptyData) {
    std::vector<uint8_t> data;
    uint32_t crc = SRPT::calculateCRC32C(data);
    
    EXPECT_TRUE(SRPT::verifyCRC32C(data, crc));
}

TEST(SRPTErrorDetectionTest, ConsistentResults) {
    std::vector<uint8_t> data = {'T', 'e', 's', 't', ' ', 'D', 'a', 't', 'a'};
    
    uint32_t crc1 = SRPT::calculateCRC32C(data);
    uint32_t crc2 = SRPT::calculateCRC32C(data);
    
    EXPECT_EQ(crc1, crc2);
}