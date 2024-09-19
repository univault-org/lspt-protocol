#pragma once

#include <cstdint>
#include <vector>

namespace LSPT {

// Calculate CRC-32C for the given data
uint32_t calculateCRC32C(const std::vector<uint8_t>& data);

// Verify the CRC-32C of the given data
bool verifyCRC32C(const std::vector<uint8_t>& data, uint32_t expected_crc);

} // namespace LSPT