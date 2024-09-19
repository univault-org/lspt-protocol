#include "lspt_error_detection.h"

namespace LSPT {

// CRC-32C (Castagnoli) polynomial
constexpr uint32_t CRC32C_POLY = 0x82F63B78;

// CRC-32C lookup table
uint32_t crc32c_table[256];

// Initialize the CRC-32C lookup table
void initCRC32CTable() {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (uint32_t j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ ((crc & 1) * CRC32C_POLY);
        }
        crc32c_table[i] = crc;
    }
}

uint32_t calculateCRC32C(const std::vector<uint8_t>& data) {
    static bool table_initialized = false;
    if (!table_initialized) {
        initCRC32CTable();
        table_initialized = true;
    }

    uint32_t crc = 0xFFFFFFFF;
    for (uint8_t byte : data) {
        crc = (crc >> 8) ^ crc32c_table[(crc & 0xFF) ^ byte];
    }
    return crc ^ 0xFFFFFFFF;
}

bool verifyCRC32C(const std::vector<uint8_t>& data, uint32_t expected_crc) {
    uint32_t calculated_crc = calculateCRC32C(data);
    return calculated_crc == expected_crc;
}

} // namespace LSPT